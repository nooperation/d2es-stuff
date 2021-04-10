#include <fstream>
#include <functional>
#include "ItemWatcher.h"
#include "../../Includes/D2Client.h"

bool operator==(const WatchedItemData &lhs, const WatchedItemData &rhs)
{
	return lhs.id == rhs.id &&
		lhs.x == rhs.x &&
		lhs.y == rhs.y;
}

ItemWatcher::ItemWatcher()
{
	radius = 4;
	minGold = 50;
	townPickup = true;
	showEtherealSocketed = false;
	showEthereal = false;
	isPickingItems = true;

	iddqdUniques.insert("0gi");
	iddqdUniques.insert("yul");
	iddqdUniques.insert("ci3");
	iddqdUniques.insert("yft");
	iddqdUniques.insert("0ts");
	iddqdUniques.insert("yrb");
	iddqdUniques.insert("0cr");
	iddqdUniques.insert("ytp");
}


void ItemWatcher::Cleanup()
{
	watchedItems.clear();
	destroyedItemsSinceLastCheck.clear();
}

struct TomeInfo {
	int totalTomesOfTownPortal;
	int totalTomesOfIdentify;
	int totalKeys;
};

BOOL CALLBACK searchForTomesItemProc(LPCITEM item, LPARAM lParam)
{
	auto tomeInfo = (TomeInfo *)lParam;

	if (strcmp(item->szItemCode, "tbk") == 0)
	{
		tomeInfo->totalTomesOfTownPortal++;
	}
	else if (strcmp(item->szItemCode, "ibk") == 0)
	{
		tomeInfo->totalTomesOfIdentify++;
	}
	else if (strcmp(item->szItemCode, "key") == 0)
	{
		tomeInfo->totalKeys += item->iQuantity;
	}

	return TRUE;
}

void ItemWatcher::CheckWatchedItems()
{
	if(destroyedItemsSinceLastCheck.size() > 0)
	{
		std::list<WatchedItemData> itemsToStopWatching;

		for(auto &watchedItem: watchedItems)
		{
			for(auto &destroyedItemId: destroyedItemsSinceLastCheck)
			{
				if(watchedItem.id == destroyedItemId)
				{
					itemsToStopWatching.push_back(watchedItem);
				}
			}
		}

		for (auto &item: itemsToStopWatching)
		{
			watchedItems.remove(item);
		}
	}

	if(me->GetOpenedUI() != 0)
	{
		return;
	}

	// No items will be added to destroyedItemsSinceLastCheck because Tick() and OnPacket() functions are only fired from the
	//  same thread (Proc_OnGameTimerTick() server20.cpp)
	destroyedItemsSinceLastCheck.clear();

	for(auto itemIter = watchedItems.begin(); itemIter != watchedItems.end(); )
	{
		GAMEUNIT itemUnit;
		itemUnit.dwUnitType = UNIT_TYPE_ITEM;
		itemUnit.dwUnitID = itemIter->id;
		if(!server->VerifyUnit(&itemUnit))
		{
			itemIter = watchedItems.erase(itemIter);
			continue;
		}

		const auto myPos = me->GetPosition();
		if (server->GetDistance(myPos.x, myPos.y, itemIter->x, itemIter->y) > radius || me->GetMode() == MODE_CAST)
		{
			++itemIter;
			continue;
		}

		//server->GameStringf("Picking %X... [%d]", i->id, watchedItems.size());
		if(itemIter->isGold)
		{
			me->PickGroundItem(itemIter->id, TRUE);
			return;
		}

		if (itemIter->isIdScroll)
		{
			TomeInfo info{ 0, 0, 0 };
			me->EnumStorageItems(STORAGE_INVENTORY, searchForTomesItemProc, (LPARAM)&info);

			const auto numCharges = me->GetSpellCharges(D2S_TOMEOFIDENTIFY);
			const auto maxCharges = info.totalTomesOfIdentify * 40;

			if (maxCharges - numCharges < 2)
			{
				itemIter = watchedItems.erase(itemIter);
				continue;
			}

			me->PickGroundItem(itemIter->id, TRUE);
			return;
		}

		if (itemIter->isTpScroll)
		{
			TomeInfo info{ 0, 0, 0 };
			me->EnumStorageItems(STORAGE_INVENTORY, searchForTomesItemProc, (LPARAM)&info);

			const auto numCharges = me->GetSpellCharges(D2S_TOMEOFTOWNPORTAL);
			const auto maxCharges = info.totalTomesOfTownPortal * 40;

			if (maxCharges - numCharges < 2)
			{
				itemIter = watchedItems.erase(itemIter);
				continue;
			}

			me->PickGroundItem(itemIter->id, TRUE);
			return;
		}

		if (itemIter->keyCount > 0)
		{
			TomeInfo info{ 0, 0, 0};
			me->EnumStorageItems(STORAGE_INVENTORY, searchForTomesItemProc, (LPARAM)&info);
			if ((info.totalKeys + itemIter->keyCount) > 20)
			{
				itemIter = watchedItems.erase(itemIter);
				continue;
			}

			me->PickGroundItem(itemIter->id, TRUE);
			return;
		}

		if(me->FindFirstStorageSpace(STORAGE_INVENTORY, itemIter->itemSize, NULL))
		{
			me->PickGroundItem(itemIter->id, TRUE);
			return;
		}

		server->GameStringf("ÿc1Pickÿc0: ÿc:Not enough room for item, skipping");
		itemIter = watchedItems.erase(itemIter);
		return;
	}
}

////////////////////////////////////////////
//
//               INTERFACE
//
/////////////////////////////////////////////
void ItemWatcher::SetTownPickup(bool enabled)
{
	townPickup = enabled;

	if(enabled)
	{
		server->GamePrintInfo("Town pickup enabled");
	}
	else
	{
		server->GamePrintInfo("Town pickup disabled");
	}
}

void ItemWatcher::TogglePickItems()
{
	isPickingItems = !isPickingItems;

	if(isPickingItems)
	{
		server->GameInfof("Pick: Now picking items");
	}
	else
	{
		server->GameInfof("Pick: No longer picking items");
	}
}

void ItemWatcher::SetRadius(int radius)
{
	server->GameInfof("Pick: Setting radius to %d", radius);
	this->radius = radius;
}

void ItemWatcher::SetMinGold(int amount)
{
	server->GameInfof("Pick: Setting minimum gold amount to %d", amount);
	minGold = amount;
}

void ItemWatcher::ShowEthereal(bool show)
{
	showEthereal = show;

	if(showEthereal)
	{
		server->GamePrintInfo("Showing ethereal items");
	}
	else
	{
		server->GamePrintInfo("Ignoring ethereal items");
	}
}

void ItemWatcher::ShowEthSoc(bool show)
{
	showEtherealSocketed = show;

	if(showEtherealSocketed)
	{
		server->GamePrintInfo("Showing ethereal socketed items");
	}
	else
	{
		server->GamePrintInfo("Ignoring ethereal socketed items");
	}
}

////////////////////////////////////////////
//
//               HELPER FUNCTIONS
//
/////////////////////////////////////////////


bool ItemWatcher::LoadItems()
{
	if(!loadItemMap(".\\plugin\\pickItems.txt", itemsToPick))
	{
		return false;
	}

	if(!loadItemMap(".\\plugin\\pickAnnounce.txt", itemsToAnnounce))
	{
		return false;
	}

	return true;
}

bool ItemWatcher::loadItemMap(const std::string &fileName, std::unordered_map<std::string, std::string> &itemMap)
{
	std::ifstream inFile(fileName.c_str());

	if(!inFile)
	{
		return false;
	}

	itemMap.clear();
	
	while(inFile.good())
	{
		std::string readBuff;
		std::getline(inFile, readBuff);

		if(readBuff.length() <= 0)
		{
			continue;
		}

		auto itemName = readBuff.substr(0, 3);
		auto itemDesc = readBuff.substr(4);

		if(itemName.length() < 3)
		{
			continue;
		}

		if (itemMap.count(itemName) == 0)
		{
			itemMap[itemName] = itemDesc;
		}
	}

	inFile.close();
	return true;
}

const char *ItemWatcher::GetItemDesc(const ITEM &item)
{
	if(item.iQuality == ITEM_LEVEL_SET)
	{
		return server->GetItemSetName(item.szItemCode);
	}
	else if(itemsToAnnounce.count(item.szItemCode) != 0)
	{
		// Use the string from pickannounce.txt instead of whatever d2hackit is defaulting to to let
		//  the user customize the item name/colour
		return itemsToAnnounce[item.szItemCode].c_str();
	}
	else
	{
		return server->GetItemName(item.szItemCode);
	}
}

const char *ItemWatcher::GetDirectionFrom(WORD sourceX, WORD sourceY, WORD targetX, WORD targetY)
{
	int x = targetX - sourceX;
	int y = targetY - sourceY;
	const int fault = 13;

	if(x > fault)
	{
		if(y > fault)
			return "South";
		if(y < -fault)
			return "East";
		else
			return "South-East";
	}
	else if(x < -fault)
	{
		if(y > fault)
			return "West";
		if(y < -fault)
			return "North";
		else
			return "North-West";
	}
	else
	{
		if(y > fault)
			return "South-west";
		if(y < -fault)
			return "North-east";
		else
			return "Here";
	}

	return "Hay";
}

bool ItemWatcher::IsOkToPick(const std::string &itemCode)
{
	return itemsToPick.count(itemCode) > 0;
}

bool ItemWatcher::IsOkToAnnounce(const char *itemCode)
{
	return itemsToAnnounce.count(itemCode) > 0;
}

////////////////////////////////////////////
//
//               EVENTS
//
/////////////////////////////////////////////

void ItemWatcher::OnGameLeave()
{
	isInGame = false;
	Cleanup();
}

void ItemWatcher::OnGameJoin()
{
	isInGame = true;
}

void ItemWatcher::OnTick()
{
	if(!isInGame)
	{
		return;
	}

	CheckWatchedItems();
}

void ItemWatcher::OnItemDestroy(DWORD itemId)
{
	if(!isInGame)
	{
		return;
	}

	destroyedItemsSinceLastCheck.push_back(itemId);
}

void ItemWatcher::OnItemAction(const ITEM &item)
{
	if(!isInGame)
	{
		return;
	}

	switch(item.iAction)
	{
		case ITEM_ACTION_NEW_GROUND:
		case ITEM_ACTION_OLD_GROUND:
		case ITEM_ACTION_DROP:
		{
			OnItemFind(item);
		}
		break;
	}
}

void ItemWatcher::OnItemFind(const ITEM &item)
{
	if(!isInGame)
	{
		return;
	}
	
	WatchedItemData itemData;
	itemData.id = item.dwItemID;
	itemData.x = item.wPositionX;
	itemData.y = item.wPositionY;
	itemData.isGold = false;
	itemData.isTpScroll = false;
	itemData.isIdScroll = false;

	const auto itemCode = std::string(item.szItemCode);

	if(item.iIdentified && itemCode == "ore")
	{
		return;
	}

	if((townPickup || !me->IsInTown()) && item.iAction!= ITEM_ACTION_DROP)
	{
		if(itemCode == "gld" && item.dwGoldAmount >= minGold)
		{
			itemData.isGold = true;
			watchedItems.push_back(itemData);
			return;
		}
		else if(isPickingItems)
		{
			if(IsOkToPick(itemCode))
			{
				itemData.isTpScroll = itemCode == "tsc";
				itemData.isIdScroll = itemCode == "isc";
				itemData.keyCount = itemCode == "key" ? item.iQuantity : 0;

				itemData.itemSize = server->GetItemSize(itemCode.c_str());
				watchedItems.push_front(itemData);
			}
		}
	}

	AnnounceItem(item);
}

void ItemWatcher::AnnounceItem(const ITEM &item)
{
	char outputMessage[512] = {0};
	bool overrideAnnouncment = false;
	const auto itemCode = std::string(item.szItemCode);

	if(item.iEthereal)
	{
		strcat_s(outputMessage, "ÿc0[ÿc5Ethÿc0]");
	}
	if(item.iSocketed)
	{
		sprintf_s(outputMessage, "%sÿc0[ÿc;socÿc0]", outputMessage);
	}

	switch(item.iQuality)
	{
		case ITEM_LEVEL_CRAFT:	
			overrideAnnouncment = true;
			strcat_s(outputMessage, "ÿc1Craft ");
			break;
		case ITEM_LEVEL_UNIQUE:
			overrideAnnouncment = true;
			if(iddqdUniques.find(itemCode) != iddqdUniques.end())
			{
				strcat_s(outputMessage, "ÿc1IDDQD Unique ");
			}
			else
			{
				strcat_s(outputMessage, "ÿc4Unique ");
			}
			break;
		case ITEM_LEVEL_SET:
			overrideAnnouncment = true;
			strcat_s(outputMessage, "ÿc2Set ");
			break;
		case ITEM_LEVEL_RARE:
			strcat_s(outputMessage, "ÿc9Rare ");
			break;
		case ITEM_LEVEL_MAGIC:
			strcat_s(outputMessage, "ÿc3Magic ");
			break;
		case ITEM_LEVEL_INFERIOR:
			strcat_s(outputMessage, "ÿc5Inferior ");
			break;
		case ITEM_LEVEL_SUPERIOR:
			strcat_s(outputMessage, "ÿc5Superior ");
			break;
	}

	if(!overrideAnnouncment && !IsOkToAnnounce(itemCode.c_str()))
	{
		return;
	}

	tagMapPos myPos = me->GetPosition();
	const char *directionString =  GetDirectionFrom(myPos.x, myPos.y, item.wPositionX, item.wPositionY);

	strcat_s(outputMessage, GetItemDesc(item));
	sprintf_s(outputMessage, "%s ( ilvl = %d ) ÿc0- %s %d", outputMessage, item.iLevel, directionString, me->GetDistanceFrom(item.wPositionX, item.wPositionY));
	server->GamePrintString(outputMessage);
}