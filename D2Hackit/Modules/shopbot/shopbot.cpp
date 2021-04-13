#include "shopbot.h"
#include <fstream>
#include <string>

#define CONFIG_PATH ".\\plugin\\shopbot.ini"

ShopBot::ShopBot()
{
	currentState = STATE_UNINITIALIZED;
	
	memset(&merchantNpc, 0, sizeof(GAMEUNIT));
	currentTeleportIndex = 1;
	merchantName = "";
}

bool ShopBot::LoadItemMap(const std::string &fileName, std::unordered_map<std::string, std::string> &itemMap)
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

bool ShopBot::ReadConfig(const std::string &configPath, std::unordered_set<int> &readTo)
{
	std::string readLineBuff;
	int readNum = 0;

	readTo.clear();

	std::ifstream inFile(configPath.c_str());
	if(!inFile)
	{
		server->GameStringf("ÿc:Shopbotÿc0: Failed to open file %s\n", configPath.c_str());
		return false;
	}

	while(inFile.good())
	{
		std::getline(inFile, readLineBuff);

		readNum = atoi(readLineBuff.c_str());
		if(readNum > 0)
		{
			readTo.insert(readNum);
		}
	}

	inFile.close();

	return true;
}

bool ShopBot::Start(const std::vector<MAPPOS> &customPath, const std::string &merchant)
{
	memset(&merchantNpc, 0, sizeof(GAMEUNIT));
	merchantName = merchant;
	teleportPath = customPath;
	currentTeleportIndex = 1;

	if(!ReadConfig(".\\plugin\\goodPrefix_shopbot.txt", goodPrefix))
		return false;

	if(!ReadConfig(".\\plugin\\goodSuffix_shopbot.txt", goodSuffix))
		return false;

	if(!LoadItemMap(".\\plugin\\shopbot_items.txt", targetItems))
		return false;


	minPrefix = GetPrivateProfileInt("ShopBot", "PrefixCount", 1, CONFIG_PATH);
	minSuffix = GetPrivateProfileInt("ShopBot", "SuffixCount", 0, CONFIG_PATH);

	server->GameStringf("ÿc:Shopbotÿc0: Min prefix: %d Min suffix: %d", minPrefix, minSuffix);

	currentState = STATE_NEXTTELEPORT;
	return true;
}

void ShopBot::Stop()
{
	if(currentState != STATE_COMPLETE && currentState != STATE_UNINITIALIZED)
	{
		me->CleanJobs();

		currentState = STATE_COMPLETE;
	}
}

DWORD ShopBot::FindMerchant()
{
	GAMEUNIT gameUnit;
	
	if(merchantName.length() == 0)
	{
		const char *merchantNames[] = 
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

		for(int i = 0; i < sizeof(merchantNames)/sizeof(merchantNames[0]); i++)
		{
			if(server->FindUnitByName(merchantNames[i], UNIT_TYPE_MONSTER, &gameUnit))
			{
				MAPPOS npcPos = server->GetUnitPosition(&gameUnit);
				DWORD distance = me->GetDistanceFrom(npcPos.x, npcPos.y);

				if(closestNpcID == 0 || closestDistance > distance)
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
		if(server->FindUnitByName(merchantName.c_str(), UNIT_TYPE_MONSTER, &gameUnit))
		{
			return gameUnit.dwUnitID;
		}
	}

	return 0;
}

bool ShopBot::WillItemFit(DWORD dwItemId)
{
	char itemCode[4];

	if(!server->GetItemCode(dwItemId, itemCode, 4))
	{
		server->GameErrorf("WillItemFit: Failed to get item code (ID = %04X), attempting retry", dwItemId);
		Sleep(1000);

		if(!server->GetItemCode(dwItemId, itemCode, 4))
		{
			server->GameErrorf("WillItemFit: Failed to get item code (ID = %04X)", dwItemId);
			return false;
		}
	}

	return me->FindFirstStorageSpace(STORAGE_INVENTORY, server->GetItemSize(itemCode), NULL)  == true;
}

void ShopBot::PurchaseQueuedItems()
{
	if(currentState != STATE_PURCHASE_NEXTITEM)
		return;

	if(gambleQueue.empty())
	{
		currentState = STATE_PURCHASE_DONE;
		return;
	}
	
	DWORD currentItemId = gambleQueue.front();
	gambleQueue.pop();

	if(!WillItemFit(currentItemId))
	{
		server->GameStringf("ÿc:Shopbotÿc0: skipping item");
		currentState = STATE_PURCHASE_NEXTITEM;


		//currentState = STATE_COMPLETE;
		return;
	}

	if(!server->VerifyUnit(&merchantNpc))
	{
		server->GamePrintInfo("ÿc:Shopbotÿc0: Could not verify npc unit");
		currentState = STATE_COMPLETE;
		return;
	}

	currentState = STATE_PURCHASE_WAITFORITEM;
	if(!me->BuyItem(currentItemId))
	{
		server->GamePrintInfo("ÿc:Shopbotÿc0: Unable to buy item");
		return;
	}
}

void ShopBot::OnTick()
{
	switch(currentState)
	{
		case STATE_PURCHASE_NEXTITEM:
		{
			PurchaseQueuedItems();
			break;
		}
		case STATE_PURCHASE_DONE:
		{
			me->CloseAllUIs();

			currentTeleportIndex++;
			currentState = STATE_NEXTTELEPORT;

			break;
		}
		case STATE_NPC_DONELISTINGITEMS:
		{
			currentState = STATE_PURCHASE_NEXTITEM;
			break;
		}
		case STATE_NEXTTELEPORT:
		{
			MAPPOS dest = teleportPath[currentTeleportIndex];

			currentState = STATE_TELEPORTING;
			me->CastOnMap(D2S_TELEPORT, dest.x, dest.y, TRUE);

			break;
		}
		case STATE_TELEPORTCOMPLETE:
		{
			if (currentTeleportIndex == 0)
			{
				// Wait for 2x cost of teleport before continuing...
				const auto myMana = me->GetStat(STAT_MANA);
				if (myMana < 20)
				{
					return;
				}
			}

			currentState = STATE_WAITINGFORNEXTSTATE;

			if(currentTeleportIndex == 0)
			{
				merchantNpc.dwUnitType = UNIT_TYPE_MONSTER;
				merchantNpc.dwUnitID = FindMerchant();

				if(merchantNpc.dwUnitID == 0)
				{
					server->GamePrintString("ÿc:Shopbotÿc0: Failed to find merchant");
					currentState = STATE_COMPLETE;
					return;
				}

				currentState = STATE_NPC_LISTINGITEMS;
				if(!me->StartNpcSession(&merchantNpc, NPC_TRADE))
				{
					server->GamePrintString("ÿc:Shopbotÿc0: Failed to start npc session");
					currentState = STATE_COMPLETE;
					return;
				}

				backToTown = false;
			}
			else
			{
				if(currentTeleportIndex+1 >= teleportPath.size())
				{
					backToTown = true;
				}

				if(backToTown)
				{
					currentTeleportIndex--;
				}
				else
				{
					currentTeleportIndex++;
				}

				currentState = STATE_NEXTTELEPORT;
			}

			break;
		}
		case STATE_COMPLETE:
		{
			server->GameStringf("ÿc:Shopbotÿc0: complete");
			currentState = STATE_UNINITIALIZED;
			return;
		}
	}
}

void ShopBot::OnNotEnoughMoney()
{
	if(currentState != STATE_PURCHASE_WAITFORITEM)
		return;

	server->GameStringf("ÿc:Shopbotÿc0: Not enough money");

	currentState = STATE_COMPLETE;
}

bool ShopBot::IsItemGood(const ITEM &item)
{
	int goodPrefixCount = 0;
	int goodSuffixCount = 0;


	for(int i = 0; i < ITEM_MAX_PREFIX; i++)
	{
		if(goodPrefix.count(item.wPrefix[i]) > 0)
		{
			goodPrefixCount++;
			//server->GameStringf("ÿc;Prefixÿc0 %s: %s", Prefix[item.wPrefix[i]], PrefixDetails[item.wPrefix[i]]);
		}
		if(goodSuffix.count(item.wSuffix[i]) > 0)
		{
			goodSuffixCount++;
			//server->GameStringf("ÿc:Suffixÿc0 %s: %s", Suffix[item.wSuffix[i]], SuffixDetails[item.wSuffix[i]]);
		}
	}

	return goodPrefixCount >= minPrefix && goodSuffixCount >= minSuffix;
}

// Called when NPC is listing items up for gamble, before NPC_SESSION message
void ShopBot::OnNpcItemList(ITEM &merchantItem)
{
	if(currentState != STATE_NPC_LISTINGITEMS)
		return;

	if((targetItems.empty() && server->IsArmor(merchantItem.szItemCode)) || targetItems.count(merchantItem.szItemCode) > 0)
	{
		if(IsItemGood(merchantItem))
		{
			if(targetItems.empty())
			{
				server->GameStringf("ÿc:Shopbotÿc0: Good item found");
			}
			else
			{
				server->GameStringf("ÿc:Shopbotÿc0: %s found", targetItems[merchantItem.szItemCode].c_str());
			}

			gambleQueue.push(merchantItem.dwItemID);
		}
	}	
}

// Called when a gambled item is placed in our inventory
void ShopBot::OnItemToStorage(ITEM &merchantItem)
{
	if(currentState != STATE_PURCHASE_WAITFORITEM)
		return;

	currentState = STATE_PURCHASE_NEXTITEM;
}

void ShopBot::OnNpcSession(int success)
{
	if(currentState != STATE_NPC_LISTINGITEMS)
		return;

	if(!success)
	{
		me->RedrawClient(FALSE);
		me->MoveToUnit(&merchantNpc, TRUE);
		server->GamePrintInfo("ÿc:Shopbotÿc0: NPC request failed");
		currentState = STATE_TELEPORTCOMPLETE; // try again
		return;
	}

	currentState = STATE_NPC_DONELISTINGITEMS;
}

void ShopBot::OnMapBlink()
{
	if(currentState == STATE_TELEPORTING)
	{
		currentState = STATE_TELEPORTCOMPLETE;			
	}	
}