#include "AutoSell.h"
#include <fstream>
#include <string>
#include <sstream>
#include "../../Core/npc.h"
#include "../../Core/npc.cpp" // TODO: Let's not do this

#define CONFIG_PATH ".\\plugin\\AutoSell.ini"

BOOL CALLBACK FindStuffToSellCallback(LPCITEM item, LPARAM lParam);

AutoSell::AutoSell()
{
	this->currentState = State::Uninitialized;

	this->isFullyAutomatic = GetPrivateProfileInt("AutoSell", "IsAutomatic", false, CONFIG_PATH);
	this->isAnnouncingSoldItems = GetPrivateProfileInt("AutoSell", "IsAnnouncingSoldItems", true, CONFIG_PATH);
	this->merchantTpScrollId = 0;
	this->numTPTomesToRefill = 0;
	this->startingGold = 0;
}

bool AutoSell::LoadItemMap(const std::string& fileName, std::unordered_map<std::string, std::string>& itemMap)
{
	std::ifstream inFile(fileName);
	if (!inFile)
	{
		return false;
	}

	itemMap.clear();

	std::string readBuff;
	while (std::getline(inFile, readBuff))
	{
		if (readBuff.length() <= 4 || readBuff.at(3) != ' ')
		{
			continue;
		}

		const auto itemName = readBuff.substr(0, 3);
		const auto itemDesc = readBuff.substr(4);

		itemMap.insert({ itemName, itemDesc });
	}

	return true;
}

bool AutoSell::SellItem(DWORD dwItemID) const
{
	if (!me->IsUIOpened(UI_NPCSHOP))
	{
		server->GameErrorf("ÿc:AutoSellÿc0: You must first open the trade interface with a merchant");
		return false;
	}

	const auto vendorId = server->GetInteractedNPCUniqueID();
	if (vendorId == 0)
	{
		server->GameErrorf("ÿc:AutoSellÿc0: No merchant found");
		return false;
	}

	const auto vendorClassId = server->GetInteractedNPCClassID();
	if (GetNpcTradeMenuID(vendorClassId) == -1)
	{
		server->GameErrorf("ÿc:AutoSellÿc0: Currently selected npc is not a merchant");
		return false;
	}

	GAMEUNIT unit;
	unit.dwUnitID = vendorId;
	unit.dwUnitType = UNIT_TYPE_MONSTER;
	if (!server->VerifyUnit(&unit))
	{
		server->GameErrorf("ÿc:AutoSellÿc0: could not verify merchant");
		return FALSE;
	}

	unit.dwUnitID = dwItemID;
	unit.dwUnitType = UNIT_TYPE_ITEM;
	if (!server->VerifyUnit(&unit))
	{
		server->GameErrorf("ÿc:AutoSellÿc0: could not verify item");
		return FALSE;
	}

	BYTE aPacket[17] = { 0 };
	aPacket[0] = 0x33;
	::memcpy(&aPacket[1], &vendorId, 4);
	::memcpy(&aPacket[5], &dwItemID, 4);
	aPacket[9] = 0x00;
	aPacket[13] = 0x01;
	return server->GameSendPacketToServer(aPacket, 17);
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

bool AutoSell::BuyItemInQuantity(DWORD dwItemID) const
{
	if (!me->IsUIOpened(UI_NPCSHOP))
	{
		server->GameErrorf("ÿc:AutoSellÿc0: You must first open the trade interface with a merchant");
		return false;
	}

	const auto vendorId = server->GetInteractedNPCUniqueID();
	if (vendorId == 0)
	{
		server->GameErrorf("ÿc:AutoSellÿc0: No merchant found");
		return false;
	}

	const auto vendorClassId = server->GetInteractedNPCClassID();
	if (GetNpcTradeMenuID(vendorClassId) == -1)
	{
		server->GameErrorf("ÿc:AutoSellÿc0: Currently selected npc is not a merchant");
		return false;
	}

	GAMEUNIT unit;
	unit.dwUnitID = vendorId;
	unit.dwUnitType = UNIT_TYPE_MONSTER;
	if (!server->VerifyUnit(&unit))
	{
		server->GameErrorf("ÿc:AutoSellÿc0: could not verify merchant");
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

bool AutoSell::Start(bool silentStart)
{
	if (currentState != State::Uninitialized)
	{
		this->Stop();
	}

	numTPTomesToRefill = 0;
	while (!itemsToSell.empty())
	{
		itemsToSell.pop();
	}

	if (!LoadItemMap(".\\plugin\\AutoSell_items.txt", targetItems))
	{
		if (!silentStart)
		{
			server->GameStringf("ÿc:AutoSellÿc0: Failed to read from .\\plugin\\AutoSell_items.txt");
		}
		return false;
	}

	if (targetItems.empty())
	{
		if (!silentStart)
		{
			server->GameStringf("ÿc:AutoSellÿc0: No items found in .\\plugin\\AutoSell_items.txt");
		}
		return false;
	}

	this->numTPTomesToRefill = 0;
	me->EnumStorageItems(STORAGE_INVENTORY, FindStuffToSellCallback, (LPARAM)this);
	if (this->itemsToSell.empty() && this->numTPTomesToRefill == 0)
	{
		if (!silentStart)
		{
			server->GameStringf("ÿc:AutoSellÿc0: Found nothing to sell");
		}
		this->Stop();
		return true;
	}

	this->startingGold = me->GetStat(STAT_GOLD);
	SellQueuedItems();

	return true;
}

void AutoSell::Stop()
{
	if (currentState != State::Uninitialized)
	{
		const auto currentGold = me->GetStat(STAT_GOLD);
		const auto goldEarned = (currentGold - startingGold);

		std::stringstream ss;
		ss.imbue(std::locale(""));
		ss << std::fixed << goldEarned;
		
		me->CleanJobs();
		server->GameStringf("ÿc:AutoSellÿc0: Total: ÿc4%sÿc0", ss.str().c_str());
		currentState = State::Uninitialized;
	}
}

void AutoSell::RestockScrolls()
{
	if (numTPTomesToRefill == 0)
	{
		this->Stop();
		return;
	}

	if (this->merchantTpScrollId == 0)
	{
		return;
	}

	for (auto i = 0; i < this->numTPTomesToRefill; i++)
	{
		BuyItemInQuantity(this->merchantTpScrollId);
	}

	this->numTPTomesToRefill = 0;
	this->Stop();
}


// Called when NPC is listing items up for gamble, before NPC_SESSION message
void AutoSell::OnNpcItemList(const ITEM& merchantItem)
{
	if (strcmp(merchantItem.szItemCode, "tsc") != 0)
	{
		return;
	}

	// scroll of town portal is flagged as an npc item for some reason. we'll trigger here no matter what if it drops?
	if (!me->IsUIOpened(UI_NPCSHOP))
	{
		return;
	}

	this->merchantTpScrollId = merchantItem.dwItemID;
	if (this->currentState == State::Uninitialized)
	{
		Start(true);
	}
}

void AutoSell::SellQueuedItems()
{
	if (itemsToSell.empty())
	{
		RestockScrolls();
		return;
	}

	currentState = State::SellingItem;
	const auto itemToSell = itemsToSell.front();
	itemsToSell.pop();

	const auto itemName = server->GetItemName(itemToSell.ItemCode.c_str());
	if (isAnnouncingSoldItems)
	{
		server->GameStringf("ÿc:AutoSellÿc0: Selling '%s'", itemName);
	}

	currentState = State::WaitForItemToSell;
	if (!SellItem(itemToSell.ItemId))
	{
		server->GameErrorf("ÿc:AutoSellÿc0: Unable to sell %s", itemName);
		this->Stop();
		return;
	}
}

void AutoSell::OnItemSold()
{
	if (currentState != State::WaitForItemToSell)
	{
		return;
	}

	SellQueuedItems();
}

void AutoSell::OnNPCShopScreenOpened()
{
	this->merchantTpScrollId = 0;
	if (!this->isFullyAutomatic)
	{
		return;
	}

	Start(true);
}

void AutoSell::ProcessInventoryItem(const ITEM* item)
{
	if (item == nullptr)
	{
		return;
	}

	if (this->targetItems.find(item->szItemCode) == this->targetItems.end())
	{
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

	this->itemsToSell.push(ItemToSell(item->dwItemID, item->szItemCode));
}

BOOL CALLBACK FindStuffToSellCallback(LPCITEM item, LPARAM lParam)
{
	auto instance = (AutoSell *)lParam;
	instance->ProcessInventoryItem(item);

	return TRUE;
}
