#include "AutoBuy.h"
#include <fstream>
#include <string>
#include <sstream>
#include "../../Core/npc.h"
#include "../../Core/npc.cpp" // TODO: Let's not do this

#define CONFIG_PATH ".\\plugin\\AutoBuy.ini"

BOOL CALLBACK FindStuffToRefillCallback(LPCITEM item, LPARAM lParam);

AutoBuy::AutoBuy()
{
	this->isAutomaticallyRefillTP = GetPrivateProfileInt("AutoBuy", "AutomaticallyRefillTP", 1, CONFIG_PATH);
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

bool AutoBuy::BuyItemInQuantity(DWORD dwItemID) const
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

	return server->GameSendPacketToServer((uint8_t*)&packet, sizeof(packet));
}

bool AutoBuy::Start(int quantity, const std::string& itemCode)
{
	if (currentState != State::Uninitialized)
	{
		this->Stop();
	}

	server->GameStringf("Attempting to buy %d %s", quantity, itemCode.c_str());

	this->numTPTomesToRefill = 0;
	this->itemQuantityToBuy = max(0, quantity);
	this->itemCodeToBuy = itemCode;
	this->hasAlreadyRestockedTps = false;

	BuyOurStuff();

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

	this->Stop();
}

void AutoBuy::RestockScrolls()
{
	if (isAutomaticallyRefillTP)
	{
		me->EnumStorageItems(STORAGE_INVENTORY, FindStuffToRefillCallback, (LPARAM)this);

		auto tpScrollIdIter = merchantItems.find("tsc");
		if (tpScrollIdIter != merchantItems.end())
		{
			for (auto i = 0; i < this->numTPTomesToRefill; i++)
			{
				BuyItemInQuantity(tpScrollIdIter->second);
			}
		}

		hasAlreadyRestockedTps = true;
	}
}

// Called when NPC is listing items up for gamble, before NPC_SESSION message
void AutoBuy::OnNpcItemList(const ITEM& merchantItem)
{
	const auto itemCode = std::string(merchantItem.szItemCode);

	merchantItems[itemCode] = merchantItem.dwItemID;
	if (itemCode == "tsc" && !hasAlreadyRestockedTps)
	{
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
		if (charges < 40) 
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
