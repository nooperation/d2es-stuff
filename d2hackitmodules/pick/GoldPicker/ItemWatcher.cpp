#include <fstream>
#include <functional>
#include "ItemWatcher.h"
#include "../../d2hackit/includes/D2Client.h"


ItemWatcher::ItemWatcher()
{
	radius = 4;
	minGold = 50;
	townPickup = true;
	showEtherealSocketed = false;
	showEthereal = false;
	isPickingItems = true;
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

void ItemWatcher::OnItemAction(ITEM &item)
{
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

void ItemWatcher::OnItemDestroy(DWORD itemId)
{
	destroyedItemsSinceLastCheck.push_back(itemId);
}

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

bool ItemWatcher::IsTownPickup()
{
	return townPickup;
}

void ItemWatcher::CheckWatchedItems()
{
	GAMEUNIT itemUnit;
	itemUnit.dwUnitType = UNIT_TYPE_ITEM;

	if(destroyedItemsSinceLastCheck.size() > 0)
	{
		for(std::list<WatchedItemData>::iterator i = watchedItems.begin(); i != watchedItems.end(); i++)
		{
			for(std::vector<DWORD>::iterator j = destroyedItemsSinceLastCheck.begin(); j != destroyedItemsSinceLastCheck.end(); ++j)
			{
				if(i->id == *j)
				{
					//server->GameStringf("Erasing %X... [%d] (item destroyed)", i->id, watchedItems.size());
					watchedItems.erase(i);
					return;
				}
			}
		}
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

			if(server->GetDistance(myPos.x, myPos.y, i->x, i->y) <= radius)
			{
				//server->GameStringf("Picking %X... [%d]", i->id, watchedItems.size());
				if(i->isGold)
				{
					me->PickGroundItem(i->id, TRUE);
				}
				else
				{
					if(me->FindFirstStorageSpace(STORAGE_INVENTORY, i->itemSize, NULL))
					{
						//server->GameStringf("Pick item %02X", i->id);
						me->PickGroundItem(i->id, TRUE);
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

bool ItemWatcher::loadItems()
{
	if(!loadItemMap(".\\plugin\\pickItems.txt", itemsToPick))
	{
		return false;
	}

	return true;
}

const char *ItemWatcher::GetItemDesc(ITEM &item)
{
	if(item.iQuality == ITEM_LEVEL_SET)
	{
		return server->GetItemSetName(item.szItemCode);
	}
	else
	{
		return server->GetItemName(item.szItemCode);
	}
}

bool ItemWatcher::IsOkToPick(char *itemCode)
{
	if(itemsToPick.count(itemCode) > 0)
		return true;

	return false;
}

void ItemWatcher::OnItemFind(ITEM &item)
{
	char outputMessage[512];
	tagMapPos myPos;
	WatchedItemData itemData;

	memset(&outputMessage, 0, sizeof(outputMessage));

	itemData.id = item.dwItemID;
	itemData.x = item.wPositionX;
	itemData.y = item.wPositionY;
	itemData.isGold = false;

	if((townPickup || !me->IsInTown()) && item.iAction!= ITEM_ACTION_DROP)
	{
		if(strcmp(item.szItemCode, "gld") == 0 && item.dwGoldAmount >= minGold)
		{
			itemData.isGold = true;
			watchedItems.push_front(itemData);
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
			strcat_s(outputMessage, "ÿc1Craft ");
			break;
		case ITEM_LEVEL_UNIQUE:
			strcat_s(outputMessage, "ÿc4Unique ");
			break;
		case ITEM_LEVEL_SET:	
			strcat_s(outputMessage, "ÿc2Set ");
			break;
		case ITEM_LEVEL_RARE:
			strcat_s(outputMessage, "ÿc9Rare ");
			// keep going to next case
		default:
			if(!IsValidItem(item))
			{
				return;
			}
	}

	strcat_s(outputMessage, GetItemDesc(item));
	
	myPos = me->GetPosition();
	
	sprintf_s(outputMessage, "%s ( ilvl = %d ) ÿc0- %s %d", outputMessage, item.iLevel, GetDirectionFrom(myPos.x, myPos.y, item.wPositionX, item.wPositionY), me->GetDistanceFrom(item.wPositionX, item.wPositionY));
	server->GamePrintString(outputMessage);
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

bool ItemWatcher::IsValidItem(ITEM &item)
{
	if(IsGoodItemCode(item.szItemCode))
	{
		return true;
	}

	if(IsGoodItemOther(item))
	{
		return true;
	}

	return false;
}

bool ItemWatcher::IsGoodItemOther(ITEM &item)
{
	if(item.iQuality == ITEM_LEVEL_RARE)
	{
		if(IsCharm(item.szItemCode))
		{
			return true;
		}
		if(IsRingAmulet(item.szItemCode))
		{
			return true;
		}
	}

	if(showEtherealSocketed && item.iSocketed && item.iEthereal)
	{
		return true;
	}
	else if(showEthereal && item.iEthereal)
	{
		return true;
	}

	return false;
}

bool ItemWatcher::IsGoodItemCode(char *itemCode)
{
	if(strlen(itemCode) < 3)
	{
		return false;
	}

	if(IsRareSpecial(itemCode) || IsRuneDecalScroll(itemCode))
	{
		return true;
	}

	if(strcmp(itemCode, "jew") == 0)
	{
		return true;
	}

	return false;
}

char *ItemWatcher::GetDirectionFrom(WORD sourceX, WORD sourceY, WORD targetX, WORD targetY)
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

bool ItemWatcher::IsCharm(char *itemCode)
{
	if(itemCode[0] == 'c' && (itemCode[1] == 'x' || itemCode[1] == 'm') && isdigit(itemCode[2]))
	{
		return true;
	}

	if((itemCode[0] == 'm' || itemCode[0] == 'n') && itemCode[1] == 'c' && isdigit(itemCode[2]))
	{
		// Charms and such
		return true;
	}

	return false;
}

bool ItemWatcher::IsRareSpecial(char *itemCode)
{
	// Unique boss parts
	if(itemCode[0] == '0' && itemCode[1] == '0')
	{
		if(itemCode[2] == 'g' || itemCode[2] == 't' || itemCode[2] == 'r' || itemCode[2] == 'h')
		{
			return true;
		}
	}

	// D-Stone, cookbook, ancient decipherer, etc
	if(itemCode[0] >= 'd' && itemCode[0] < 'z' && itemCode[0] == itemCode[1] && itemCode[1] == itemCode[2])
	{
		return true;
	}

	// Fake note, Maple leaf, Forging hammer, Holy symbol, socket donut
	if(strcmp(itemCode, "fkn") == 0 ||
		strcmp(itemCode, "map") == 0 ||
		strcmp(itemCode, "hh2") == 0 ||
		strcmp(itemCode, "hly") == 0 ||
		strcmp(itemCode, "sdo") == 0)
	{
		return true;
	}

	// special potions
	if(itemCode[0] == 'p' && itemCode[1] == 'o' && isdigit(itemCode[2]))
	{
		return true;
	}

	// Aura Stones, unique stones
	if(itemCode[0] == 'a' && itemCode[1] == 'n' && (itemCode[2] >= '0' && itemCode[2] <= '9'))
	{
		return true;
	}

	return false;
}

bool ItemWatcher::IsRuneDecalScroll(char *itemCode)
{
	// Runes
	if(itemCode[0] == 'r' && (itemCode[1] >= '0' && itemCode[1] <= '9'))
	{
		return true;
	}

	// Decals, unidentified ancient scrolls
	if(isdigit(itemCode[0]) && isdigit(itemCode[1]) && (itemCode[2] == 'b' || itemCode[2] == 'l' ))
	{
		return true;
	}

	return false;
}

bool ItemWatcher::IsRingAmulet(char *itemCode)
{
	if(strcmp(itemCode, "amu") == 0 ||
		strcmp(itemCode, "rin") == 0 ||
		strcmp(itemCode, "zrn") == 0 ||
		strcmp(itemCode, "srn") == 0 ||
		strcmp(itemCode, "nrn") == 0 ||
		strcmp(itemCode, "prn") == 0 ||
		strcmp(itemCode, "brg") == 0 ||
		strcmp(itemCode, "drn") == 0 ||
		strcmp(itemCode, "arn") == 0 ||
		strcmp(itemCode, "zam") == 0 ||
		strcmp(itemCode, "sam") == 0 ||
		strcmp(itemCode, "nam") == 0 ||
		strcmp(itemCode, "pam") == 0 ||
		strcmp(itemCode, "bam") == 0 ||
		strcmp(itemCode, "dam") == 0 ||
		strcmp(itemCode, "aam") == 0)

	{
		return true;
	}

	return false;
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