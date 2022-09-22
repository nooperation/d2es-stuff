#include <fstream>
#include <functional>
#include <algorithm>
#include "ItemWatcher.h"
#include "../../Includes/D2Client.h"

#define PICK_SETTINGS_PATH ".\\plugin\\pick.ini"

int goldPicksThisFrame = 0;

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
	isPickingItems = true;
	goldSpeed = 1;
	isMute = false;
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
	bool needToResortWatchedItems = false;

	if(!destroyedItemsSinceLastCheck.empty())
	{
		for (auto watchedItemIter = watchedItems.begin(); watchedItemIter != watchedItems.end();)
		{
			if (destroyedItemsSinceLastCheck.find(watchedItemIter->id) != destroyedItemsSinceLastCheck.end())
			{
				watchedItemIter = watchedItems.erase(watchedItemIter);
				needToResortWatchedItems = true;
			}
			else
			{
				++watchedItemIter;
			}
		}
	}

	// No items will be added to destroyedItemsSinceLastCheck because Tick() and OnPacket() functions are only fired from the
	//  same thread (Proc_OnGameTimerTick() server20.cpp)
	destroyedItemsSinceLastCheck.clear();

	if(std::chrono::system_clock::now() < nextPickAttemptTime)
	{
		return;
	}

	if(me->GetOpenedUI() != 0)
	{
		return;
	}

	if(watchedItems.empty())
	{
		return;
	}

	SortWatchedItems();

	int numPickAttemptsThisFrame = 0;
	for (auto itemIter = watchedItems.begin(); itemIter != watchedItems.end(); )
	{
		GAMEUNIT itemUnit;
		itemUnit.dwUnitType = UNIT_TYPE_ITEM;
		itemUnit.dwUnitID = itemIter->id;
		if(!server->VerifyUnit(&itemUnit))
		{
			itemIter = watchedItems.erase(itemIter);
			continue;
		}

		if(itemIter->distanceToItem > radius || numPickAttemptsThisFrame > 5)
		{
			break;
		}

		if(itemIter->isGold)
		{
			numPickAttemptsThisFrame++;
			me->PickGroundItem(itemIter->id, this->isWalkToGold);
		}
		else if(itemIter->isIdScroll)
		{
			TomeInfo info{ 0, 0, 0 };
			me->EnumStorageItems(STORAGE_INVENTORY, searchForTomesItemProc, (LPARAM)&info);

			const auto numCharges = me->GetSpellCharges(D2S_TOMEOFIDENTIFY);
			const auto maxCharges = info.totalTomesOfIdentify * 40;

			if(maxCharges - numCharges < 2)
			{
				itemIter = watchedItems.erase(itemIter);
				continue;
			}

			numPickAttemptsThisFrame++;
			me->PickGroundItem(itemIter->id, this->isWalkToItems);
		}
		else if(itemIter->isTpScroll)
		{
			TomeInfo info{ 0, 0, 0 };
			me->EnumStorageItems(STORAGE_INVENTORY, searchForTomesItemProc, (LPARAM)&info);

			const auto numCharges = me->GetSpellCharges(D2S_TOMEOFTOWNPORTAL);
			const auto maxCharges = info.totalTomesOfTownPortal * 40;

			if(maxCharges - numCharges < 2)
			{
				itemIter = watchedItems.erase(itemIter);
				continue;
			}

			numPickAttemptsThisFrame++;
			me->PickGroundItem(itemIter->id, this->isWalkToItems);
		}
		else if(itemIter->keyCount > 0)
		{
			TomeInfo info{ 0, 0, 0 };
			me->EnumStorageItems(STORAGE_INVENTORY, searchForTomesItemProc, (LPARAM)&info);
			if((info.totalKeys + itemIter->keyCount) > 20)
			{
				itemIter = watchedItems.erase(itemIter);
				continue;
			}

			numPickAttemptsThisFrame++;
			me->PickGroundItem(itemIter->id, this->isWalkToItems);
		}
		else if(me->FindFirstStorageSpace(STORAGE_INVENTORY, itemIter->itemSize, NULL))
		{
			numPickAttemptsThisFrame++;
			me->PickGroundItem(itemIter->id, this->isWalkToItems);
		}
		else
		{
			server->GameStringf("ÿc1Pickÿc0: ÿc:Not enough room for item, skipping");
			itemIter = watchedItems.erase(itemIter);
			continue;
		}

		if(radius > 5 && itemIter->distanceToItem > 5)
		{
			// We're likely walking somewhere, give it 200ms unless something new drops
		   // nextPickAttemptTime = std::chrono::system_clock::now() + std::chrono::milliseconds(200);
			return;
		}

		++itemIter;
	}

	nextPickAttemptTime = std::chrono::system_clock::now() + std::chrono::milliseconds(10);
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

void ItemWatcher::Mute()
{
	isMute = !isMute;

	if (!isMute)
	{
		server->GameInfof("Pick: Item announcements ÿc2enabledÿc0");
	}
	else
	{
		server->GameInfof("Pick: Item announcements ÿc1disabledÿc0");
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

void ItemWatcher::SetGoldSpeed(int goldSpeed)
{
	server->GameInfof("Pick: Setting gold pickup speed to %d", goldSpeed);
	this->goldSpeed = goldSpeed;
}

void ItemWatcher::SetMinGold(int amount)
{
	server->GameInfof("Pick: Setting minimum gold amount to %d", amount);
	minGold = amount;
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

	if (!loadItemMap(".\\plugin\\pickAnnounce_uniques.txt", uniquesToAnnounce))
	{
		return false;
	}

	if (!loadItemMap(".\\plugin\\pickAnnounce_sets.txt", setsToAnnounce))
	{
		return false;
	}

	isAnnouncingAllCrafted = GetPrivateProfileInt("Pick.Announce", "AllCrafted", 1, PICK_SETTINGS_PATH) == TRUE;
	isAnnouncingAllSets = GetPrivateProfileInt("Pick.Announce", "AllSets", 1, PICK_SETTINGS_PATH) == TRUE;
	isAnnouncingAllUniques = GetPrivateProfileInt("Pick.Announce", "AllUniques", 1, PICK_SETTINGS_PATH) == TRUE;
	isOverridingUniqueItemNames = GetPrivateProfileInt("Pick.Announce", "OverrideUniqueItemName", 0, PICK_SETTINGS_PATH) == TRUE;
	isOverridingSetItemNames = GetPrivateProfileInt("Pick.Announce", "OverrideSetItemName", 0, PICK_SETTINGS_PATH) == TRUE;

	radius = GetPrivateProfileInt("Pick", "Radius", 4, PICK_SETTINGS_PATH);
	minGold = GetPrivateProfileInt("Pick", "MinGold", 50, PICK_SETTINGS_PATH);
	townPickup = GetPrivateProfileInt("Pick", "Town", 1, PICK_SETTINGS_PATH) == TRUE;
	isPickingItems = GetPrivateProfileInt("Pick", "Items", 1, PICK_SETTINGS_PATH) == TRUE;
	goldSpeed = GetPrivateProfileInt("Pick", "GoldSpeed", 1, PICK_SETTINGS_PATH);
	isWalkToGold = GetPrivateProfileInt("Pick", "WalkToGold", 1, PICK_SETTINGS_PATH) == TRUE;
	isWalkToItems = GetPrivateProfileInt("Pick", "WalkToItems", 1, PICK_SETTINGS_PATH) == TRUE;

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

	goldPicksThisFrame = 0;
	CheckWatchedItems();
}

void ItemWatcher::OnItemDestroy(DWORD itemId)
{
	if(!isInGame)
	{
		return;
	}

	destroyedItemsSinceLastCheck.insert(itemId);
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
			SortWatchedItems();

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
				SortWatchedItems();
			}
		}
	}

	watchedItems.push_back(itemData);
	AnnounceItem(item);
}

float ItemWatcher::Distance(float x1, float x2, float y1, float y2) const
{
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

void ItemWatcher::SortWatchedItems()
{
	auto myPosition = me->GetPosition();

	for (auto& item : watchedItems)
	{
		item.distanceToItem = Distance(myPosition.x, item.x, myPosition.y, item.y);
	}

	std::sort(watchedItems.begin(), watchedItems.end(), [](const WatchedItemData& left, const WatchedItemData& right) {
		return left.distanceToItem < right.distanceToItem;
		});
}

void ItemWatcher::AnnounceItem(const ITEM& item)
{
	if(isMute)
	{
		return;
	}

	std::string outputMessage;
	outputMessage.reserve(128);


	bool overrideAnnouncment = false;
	const auto itemCode = std::string(item.szItemCode);
	std::string itemName;

	if(item.iEthereal)
	{
		outputMessage += "ÿc0[ÿc5Ethÿc0]";
	}
	if(item.iSocketed)
	{
		outputMessage += "ÿc0[ÿc;socÿc0]";
	}

	switch(item.iQuality)
	{
		case ITEM_LEVEL_CRAFT:
			overrideAnnouncment = isAnnouncingAllCrafted;
			outputMessage += "ÿc1Craft ";
			break;
		case ITEM_LEVEL_UNIQUE:
		{
			overrideAnnouncment = isAnnouncingAllUniques;

			auto uniqueOverride = uniquesToAnnounce.find(itemCode);
			if (uniqueOverride != uniquesToAnnounce.end())
			{
				overrideAnnouncment = true;
				if (isOverridingUniqueItemNames)
				{
					itemName = uniqueOverride->second;
				}
			}

			if (overrideAnnouncment)
			{
				outputMessage += "ÿc4Unique ";
			}
			break;
		}
		case ITEM_LEVEL_SET:
		{
			overrideAnnouncment = isAnnouncingAllSets;

			auto setOverride = setsToAnnounce.find(itemCode);
			if (setOverride != setsToAnnounce.end())
			{
				overrideAnnouncment = true;
				if (isOverridingSetItemNames)
				{
					itemName = setOverride->second;
				}
			}

			if (overrideAnnouncment)
			{
				outputMessage += "ÿc2Set ";
			}
			break;
		}
		case ITEM_LEVEL_RARE:
			outputMessage += "ÿc9Rare ";
			break;
		case ITEM_LEVEL_MAGIC:
			outputMessage += "ÿc3Magic ";
			break;
		case ITEM_LEVEL_INFERIOR:
			outputMessage += "ÿc5Inferior ";
			break;
		case ITEM_LEVEL_SUPERIOR:
			outputMessage += "ÿc5Superior ";
			break;
	}

	if(!overrideAnnouncment && !IsOkToAnnounce(itemCode.c_str()))
	{
		return;
	}

	tagMapPos myPos = me->GetPosition();
	const char *directionString =  GetDirectionFrom(myPos.x, myPos.y, item.wPositionX, item.wPositionY);

	if (itemName.empty())
	{
		outputMessage += GetItemDesc(item);
	}
	else
	{
		outputMessage += itemName;
	}

	const auto distance = me->GetDistanceFrom(item.wPositionX, item.wPositionY);

	outputMessage += " ( ilvl = " + std::to_string(item.iLevel) + " ) ÿc0- " + directionString + " " + std::to_string(distance);
	server->GamePrintString(outputMessage.c_str());
}