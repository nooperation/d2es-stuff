#include "AutoBuy.h"
#include <fstream>
#include <string>
#include <sstream>
#include "../../Core/npc.h"
#include "../../Core/npc.cpp" // TODO: Let's not do this

#define CONFIG_PATH ".\\plugin\\AutoBuy.ini"

BOOL CALLBACK FindStuffToRefillCallback(LPCITEM item, LPARAM lParam);

int32_t* D2Client_pIsNpcDialogOpen_6FBB5CF9 = nullptr; // 115CF9

AutoBuy::AutoBuy()
{
	memset(&merchantNpc, 0, sizeof(GAMEUNIT));
	merchantName = "";

	this->isAutomaticallyRefillTP = GetPrivateProfileInt("AutoBuy", "AutomaticallyRefillTP", 1, CONFIG_PATH);
	this->refillTpAtCharge = GetPrivateProfileInt("AutoBuy", "RefillTpAtCharge", 35, CONFIG_PATH);

	auto d2ClientModule = GetModuleHandle("D2Client");

	D2Client_pIsNpcDialogOpen_6FBB5CF9 = (int32_t*)((char*)d2ClientModule + 0x115CF9);
}

#pragma pack(push, 1)
struct D2GS_NPC_BUY
{
	uint8_t PacketId;
	uint32_t UnitId;
	uint32_t ItemId;
	uint32_t BuyType;
	uint32_t Cost;
};
#pragma pack(pop)

bool AutoBuy::BuyItemInQuantity(DWORD dwItemID)
{
	if (!me->IsUIOpened(UI_NPCSHOP))
	{
		server->GameErrorf("ÿc:AutoBuyÿc0: You must first open the trade interface with a merchant");
		return false;
	}

	const auto vendorId = server->GetInteractedNPCUniqueID();
	if (vendorId == 0)
	{
		server->GameErrorf("ÿc:AutoBuyÿc0: No merchant found");
		return false;
	}

	const auto vendorClassId = server->GetInteractedNPCClassID();
	if (GetNpcTradeMenuID(vendorClassId) == -1)
	{
		server->GameErrorf("ÿc:AutoBuyÿc0: Currently selected npc is not a merchant");
		return false;
	}

	GAMEUNIT unit;
	unit.dwUnitID = vendorId;
	unit.dwUnitType = UNIT_TYPE_MONSTER;
	if (!server->VerifyUnit(&unit))
	{
		server->GameErrorf("ÿc:AutoBuyÿc0: could not verify merchant");
		return FALSE;
	}

	D2GS_NPC_BUY packet;
	packet.PacketId = 0x32;
	packet.UnitId = vendorId;
	packet.ItemId = dwItemID;
	packet.BuyType = 0x80000000;
	packet.Cost = 0;


	currentState = State::PurchaseWaitforitem;
	return server->GameSendPacketToServer((uint8_t*)&packet, sizeof(packet));
}

bool AutoBuy::Start(int quantity, const std::string& itemCode, bool isAutomaticMode)
{
	if (currentState != State::Uninitialized)
	{
		this->Stop();
		return false;
	}

	this->isAutomaticMode = isAutomaticMode;
	this->numTPTomesToRefill = 0;
	this->itemQuantityToBuy = isAutomaticMode ? 999 : max(0, quantity);
	this->itemCodeToBuy = itemCode;
	this->hasAlreadyRestockedTps = false;

	if (!me->IsUIOpened(UI_NPCSHOP))
	{
		server->GameStringf("Attempting to buy %d %s", quantity, itemCode.c_str());

		merchantNpc.dwUnitType = UNIT_TYPE_MONSTER;
		merchantNpc.dwUnitID = FindMerchant();

		if (merchantNpc.dwUnitID == 0)
		{
			server->GamePrintString("ÿc:AutoBuyÿc0: Failed to find merchant");
			currentState = State::Uninitialized;
			return false;
		}

		currentState = State::NpcListingitems;
		if (!me->StartNpcSession(&merchantNpc, NPC_TRADE))
		{
			server->GamePrintString("ÿc:AutoBuyÿc0: Failed to start npc session");
			currentState = State::Uninitialized;
			return false;
		}
	}
	else
	{
		currentState = State::CloseMerchantUiAndRestart;
		me->CloseAllUIs();
		return true;
	}

	return true;
}

void AutoBuy::Stop()
{
	if (currentState != State::Uninitialized)
	{
		this->itemQuantityToBuy = 0;
		this->itemCodeToBuy = "";
		this->numTPTomesToRefill = 0;
		this->hasAlreadyRestockedTps = false;
		merchantItems.clear();

		me->CleanJobs();
		server->GameStringf("ÿc:AutoBuyÿc0: Done");
		currentState = State::Uninitialized;
	}
}

void AutoBuy::BuyOurStuff()
{
	if (isAutomaticMode)
	{
		itemQuantityToBuy = me->GetNumberOfFreeStorageSlots(STORAGE_INVENTORY);
	}

	if (itemQuantityToBuy > 0)
	{
		auto customItemIdIter = merchantItems.find(itemCodeToBuy);
		if (customItemIdIter != merchantItems.end())
		{
			for (auto i = 0; i < itemQuantityToBuy; i++)
			{
				BuyItemInQuantity(customItemIdIter->second);
			}
		}
	}

	if (!isAutomaticMode)
	{
		this->Stop();
	}
}

void AutoBuy::RestockScrolls()
{
	if (isAutomaticallyRefillTP)
	{
		this->numTPTomesToRefill = 0;
		me->EnumStorageItems(STORAGE_INVENTORY, FindStuffToRefillCallback, (LPARAM)this);

		auto tpScrollIdIter = merchantItems.find("tsc");
		if (tpScrollIdIter != merchantItems.end())
		{
			if(this->numTPTomesToRefill > 0)
			{
				BuyItemInQuantity(tpScrollIdIter->second);
			}
		}
	}
}

// Called when NPC is listing items up for gamble, before NPC_SESSION message
void AutoBuy::OnNpcItemList(const ITEM& merchantItem)
{
	if (!me->IsUIOpened(UI_NPCSHOP))
	{
		return;
	}

	const auto itemCode = std::string(merchantItem.szItemCode);

	merchantItems[itemCode] = merchantItem.dwItemID;
	if (itemCode == "tsc" && !hasAlreadyRestockedTps)
	{
		hasAlreadyRestockedTps = true;
		RestockScrolls();
	}
}

void AutoBuy::OnNPCShopScreenOpened()
{
	merchantItems.clear();
	this->hasAlreadyRestockedTps = false;
}

void AutoBuy::ProcessInventoryItem(const ITEM* item)
{
	if (item == nullptr)
	{
		return;
	}

	if (strcmp(item->szItemCode, "tbk") == 0)
	{
		// TODO: d2hackit doesn't update item->iQuantity, never trust it...
		GAMEUNIT unit;
		unit.dwUnitID = item->dwItemID;
		unit.dwUnitType = UNIT_TYPE_ITEM;

		auto charges = server->GetUnitStat(&unit, STAT_AMMOQUANTITY);
		if (charges < refillTpAtCharge)
		{
			this->numTPTomesToRefill++;
		}
	}

	return;
}

BOOL CALLBACK FindStuffToRefillCallback(LPCITEM item, LPARAM lParam)
{
	auto instance = (AutoBuy *)lParam;
	instance->ProcessInventoryItem(item);

	return TRUE;
}

void AutoBuy::OnNpcSession(int success)
{
	if (currentState != State::NpcListingitems)
	{
		return;
	}

	if (!success)
	{
		me->RedrawClient(FALSE);
		me->MoveToUnit(&merchantNpc, TRUE);
		server->GamePrintInfo("ÿc:AutoBuyÿc0: NPC request failed");
		currentState = State::Uninitialized; // try again

		if (isAutomaticMode)
		{
			currentState = State::Uninitialized;
			Start(itemQuantityToBuy, itemCodeToBuy, isAutomaticMode);
		}

		return;
	}

	currentState = State::PurchaseNextitem;
	BuyOurStuff();
}

void AutoBuy::OnItemToStorageFromStore(ITEM& item)
{
	if (currentState != State::PurchaseWaitforitem)
	{
		return;
	}

	if (!me->IsInTown() || !me->IsUIOpened(UI_NPCSHOP) || D2Client_pIsNpcDialogOpen_6FBB5CF9 == nullptr || *D2Client_pIsNpcDialogOpen_6FBB5CF9 == 0)
	{
		this->Stop();
		return;
	}
	
	if (isAutomaticMode && !WillItemFit(item.dwItemID))
	{
		currentState = State::CloseMerchantUiAndRunAutostock;
		me->CloseAllUIs();
	}
}

void AutoBuy::OnUIClosed()
{
	if (currentState == State::CloseMerchantUiAndRestart)
	{
		currentState = State::Uninitialized;
		Start(itemQuantityToBuy, itemCodeToBuy, isAutomaticMode);
		return;
	}
	else if (currentState == State::CloseMerchantUiAndRunAutostock)
	{
		if (!me->IsInTown())
		{
			this->Stop();
			return;
		}

		currentState = State::RunAutostocker;
		server->GameStringf("ÿc:AutoBuyÿc0: Running autostocker...");
		server->GameCommandLine("as start chat");
		return;
	}
}

bool AutoBuy::OnAutostockerMessage(const std::string_view& message)
{
	if (currentState != State::RunAutostocker || !me->IsInTown())
	{
		return false;
	}

	if (message == "Autostocker Ended")
	{
		me->CloseAllUIs();

		if (isAutomaticMode)
		{
			currentState = State::Uninitialized;
			Start(itemQuantityToBuy, itemCodeToBuy, isAutomaticMode);
		}
	}

	return true;
}

bool AutoBuy::WillItemFit(DWORD dwItemId)
{
	char itemCode[4];

	if (!server->GetItemCode(dwItemId, itemCode, 4))
	{
		server->GameErrorf("WillItemFit: Failed to get item code (ID = %04X), attempting retry", dwItemId);
		Sleep(1000);

		if (!server->GetItemCode(dwItemId, itemCode, 4))
		{
			server->GameErrorf("WillItemFit: Failed to get item code (ID = %04X)", dwItemId);
			return false;
		}
	}

	return me->FindFirstStorageSpace(STORAGE_INVENTORY, server->GetItemSize(itemCode), NULL) == true;
}

DWORD AutoBuy::FindMerchant()
{
	GAMEUNIT gameUnit;

	if (merchantName.length() == 0)
	{
		const char* merchantNames[] =
		{
			"charsi",
			"gheed",
			"akara",
			"elzix",
			"lysander",
			"fara",
			"drognan",
			"alkor",
			"asheara",
			"ormus",
			"hratli",
			"jamella",
			"halbu",
			"larzuk",
			"malah",
			"anya",
		};

		DWORD closestNpcID = 0;
		DWORD closestDistance = 0;

		for (int i = 0; i < sizeof(merchantNames) / sizeof(merchantNames[0]); i++)
		{
			if (server->FindUnitByName(merchantNames[i], UNIT_TYPE_MONSTER, &gameUnit))
			{
				MAPPOS npcPos = server->GetUnitPosition(&gameUnit);
				DWORD distance = me->GetDistanceFrom(npcPos.x, npcPos.y);

				if (closestNpcID == 0 || closestDistance > distance)
				{
					closestNpcID = gameUnit.dwUnitID;
					closestDistance = distance;
				}
			}
		}

		return closestNpcID;
	}
	else
	{
		if (server->FindUnitByName(merchantName.c_str(), UNIT_TYPE_MONSTER, &gameUnit))
		{
			return gameUnit.dwUnitID;
		}
	}

	return 0;
}
