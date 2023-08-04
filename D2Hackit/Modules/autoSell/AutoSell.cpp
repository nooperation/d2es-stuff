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

bool AutoSell::Start(bool silentStart)
{
	if (currentState != State::Uninitialized)
	{
		this->Stop();
	}

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

	me->EnumStorageItems(STORAGE_INVENTORY, FindStuffToSellCallback, (LPARAM)this);
	if (this->itemsToSell.empty())
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

void AutoSell::SellQueuedItems()
{
	if (itemsToSell.empty())
	{
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
		return;
	}

	// We only want to sell empty ores, ores that have 0 for xtal_10-xtal_11
	if (strcmp(item->szItemCode, "ore") == 0)
	{
		GAMEUNIT unit;
		unit.dwUnitID = item->dwItemID;
		unit.dwUnitType = UNIT_TYPE_ITEM;

		for (size_t i = 470; i <= 481; i++)
		{
			auto statValue = server->GetUnitStat(&unit, i);
			if (statValue != 0)
			{
				return;
			}
		}
	}

	this->itemsToSell.push(ItemToSell(item->dwItemID, item->szItemCode));
}

BOOL CALLBACK FindStuffToSellCallback(LPCITEM item, LPARAM lParam)
{
	auto instance = (AutoSell *)lParam;
	instance->ProcessInventoryItem(item);

	return TRUE;
}
