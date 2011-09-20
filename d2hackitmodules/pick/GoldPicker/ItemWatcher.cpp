#include <fstream>
#include <functional>
#include "ItemWatcher.h"
#include "../../d2hackit/includes/D2Client.h"

bool WatchedItemData::operator==(const WatchedItemData &rhs)
{
	return this->id == rhs.id && 
		this->x == rhs.x && 
		this->y == rhs.y;
}

ItemWatcher::ItemWatcher()
{
	radius = 4;
	minGold = 50;
	townPickup = true;
	showEtherealSocketed = false;
	showEthereal = false;
	isPickingItems = true;

	iddqdUniques.insert("yul");
	iddqdUniques.insert("0cr"); 
	iddqdUniques.insert("0gi");
	iddqdUniques.insert("ci3");
	iddqdUniques.insert("yft");
	iddqdUniques.insert("amf");
	iddqdUniques.insert("yrb");
}


void ItemWatcher::Cleanup()
{
	watchedItems.clear();
	destroyedItemsSinceLastCheck.clear();
}

void ItemWatcher::CheckWatchedItems()
{
	GAMEUNIT itemUnit;
	itemUnit.dwUnitType = UNIT_TYPE_ITEM;

	if(destroyedItemsSinceLastCheck.size() > 0)
	{
		std::list<WatchedItemData> itemsToStopWatching;

		for(std::list<WatchedItemData>::iterator i = watchedItems.begin(); i != watchedItems.end(); i++)
		{
			for(std::vector<DWORD>::iterator j = destroyedItemsSinceLastCheck.begin(); j != destroyedItemsSinceLastCheck.end(); ++j)
			{
				if(i->id == *j)
				{
					itemsToStopWatching.push_back((*i));
				}
			}
		}

		for each(auto item in itemsToStopWatching)
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

	for(std::list<WatchedItemData>::iterator i = watchedItems.begin(); i != watchedItems.end(); )
	{
		itemUnit.dwUnitID = i->id;
		if(server->VerifyUnit(&itemUnit))
		{
			tagMapPos myPos = me->GetPosition();

			if(server->GetDistance(myPos.x, myPos.y, i->x, i->y) <= radius && me->GetMode() != MODE_CAST)
			{
				//server->GameStringf("Picking %X... [%d]", i->id, watchedItems.size());
				if(i->isGold)
				{
					me->PickGroundItem(i->id, TRUE);
					return;
				}
				else
				{
					if(me->FindFirstStorageSpace(STORAGE_INVENTORY, i->itemSize, NULL))
					{
						//server->GameStringf("Pick item %02X", i->id);
						me->PickGroundItem(i->id, TRUE);
						return;
						//watchedItems.erase(i++); // TODO: needs to check items on pickup to see if it worked
					}
					else
					{
						server->GameStringf("ÿc1Pickÿc0: ÿc:Not enough room for item, skipping");
						watchedItems.erase(i++);
					}
				}
				return;
			}
			else
			{
				++i;
			}
		}
		else
		{
			//server->GameStringf("Erasing %X... [%d]", i->id, watchedItems.size());
			watchedItems.erase(i++);
		}
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

bool ItemWatcher::loadItemMap(std::string fileName, stdext::hash_map<std::string, std::string> &itemMap)
{
	std::ifstream inFile(fileName.c_str());

	std::string itemName;
	std::string itemDesc;
	std::string readBuff;

	if(!inFile)
	{
		return false;
	}

	itemMap.clear();

	
	while(inFile.good())
	{
		std::getline(inFile, readBuff);

		if(readBuff.length() <= 0)
		{
			continue;
		}

		itemName = readBuff.substr(0, 3);
		itemDesc = readBuff.substr(4);

		if(itemName.length() < 3)
		{
			continue;
		}

		if(itemMap.count(itemName) == 0)
			itemMap[itemName] = itemDesc;
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

bool ItemWatcher::IsOkToPick(const char *itemCode)
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

	if(item.iIdentified && strcmp(item.szItemCode, "ore") == 0)
	{
		return;
	}

	if((townPickup || !me->IsInTown()) && item.iAction!= ITEM_ACTION_DROP)
	{
		if(strcmp(item.szItemCode, "gld") == 0 && item.dwGoldAmount >= minGold)
		{
			itemData.isGold = true;
			watchedItems.push_back(itemData);
			return;
		}
		else if(isPickingItems)
		{
			if(IsOkToPick(item.szItemCode))
			{
				itemData.itemSize = server->GetItemSize(item.szItemCode);
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
			if(iddqdUniques.find(item.szItemCode) != iddqdUniques.end())
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

	if(!overrideAnnouncment && !IsOkToAnnounce(item.szItemCode))
	{
		return;
	}

	tagMapPos myPos = me->GetPosition();
	const char *directionString =  GetDirectionFrom(myPos.x, myPos.y, item.wPositionX, item.wPositionY);

	strcat_s(outputMessage, GetItemDesc(item));
	sprintf_s(outputMessage, "%s ( ilvl = %d ) ÿc0- %s %d", outputMessage, item.iLevel, directionString, me->GetDistanceFrom(item.wPositionX, item.wPositionY));
	server->GamePrintString(outputMessage);
}