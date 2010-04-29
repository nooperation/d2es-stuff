#include "gambler.h"
#include <fstream>

Gambler::Gambler()
{
	requestedGoldSplitBy = 1;
	isSellingGambledItems = false;
	isRequestingGold = false;
	transmuteSet = false;
	transmuteRare = false;
	transmuteUnique = false;
	transmuteEnabled = false;
	autostockStartDelay = 10; // ~1s autostock delay before start
	ticksTillAutostock = autostockStartDelay;

	currentState = STATE_UNINITIALIZED;
}

bool Gambler::Init(std::vector<std::string> itemCodes)
{
	itemsToGamble.clear();

	for(int i = 0; i < (int)itemCodes.size(); i++)
	{
		itemsToGamble.insert(itemCodes[i]);
	}

	gamblingNpc.dwUnitType = UNIT_TYPE_MONSTER;
	gamblingNpc.dwUnitID = FindGamblingNpc();

	if(gamblingNpc.dwUnitID == 0)
	{
		server->GamePrintString("ÿc3Gambleÿc0: Failed to find gambling npc");
		return false;
	}

	Reset(false);
	return true;
}

void Gambler::StopGambling()
{
	Reset(true);
}

void Gambler::Reset(bool haltGambling)
{
	if(haltGambling)
	{
		if(currentState != STATE_READYTORUN && currentState != STATE_HALTED && currentState != STATE_UNINITIALIZED)
		{
			int totalGambled = stats.magic + stats.rares + stats.sets + stats.uniques;

			if(totalGambled > 0)
			{
				server->GameStringf("ÿc3Gambleÿc0: ÿc3Magic: %d (%d%%)  ÿc9Rare: %d (%d%%)  ÿc2Set: %d (%d%%)  ÿc4Unique: %d (%d%%)ÿc0",
					stats.magic, (int)(100 * (stats.magic / (float)totalGambled)),
					stats.rares, (int)(100 * (stats.rares / (float)totalGambled)),
					stats.sets, (int)(100 * (stats.sets / (float)totalGambled)),
					stats.uniques, (int)(100 * (stats.uniques / (float)totalGambled)));
			}

			memset(&stats, 0, sizeof(GambleStats));

			server->GamePrintInfo("ÿc3Gambleÿc0: Gamble ended");
		}

		currentState = STATE_HALTED;

		me->CleanJobs();
		me->EndNpcSession();
		
		while(!itemsToSell.empty())
			itemsToSell.pop();
	}
	else
	{
		currentState = STATE_READYTORUN;
	}

	ticksTillAutostock = autostockStartDelay;
	ticksTillNextPurchase = 0;

	while(!gambleQueue.empty())
		gambleQueue.pop();
}

bool Gambler::StartGambling()
{
	if(currentState != STATE_READYTORUN && currentState != STATE_AUTOSTOCK_ENDED)
		return false;

	for(stdext::hash_set<std::string>::iterator i = itemsToGamble.begin(); i != itemsToGamble.end(); ++i)
	{
		if(!WillItemFit((*i).c_str()))
		{
			if(itemsToSell.empty())
			{
				server->GameStringf("ÿc3Gambleÿc0: Not enough room");
				if(transmuteEnabled && currentState != STATE_AUTOSTOCK_ENDED)
				{
					me->EndNpcSession();
					currentState = STATE_AUTOSTOCK_START;
				}
				else
				{
					StopGambling();
				}
				return false;
			}
			else
			{
				//server->GameStringf("Try to sell again...");
				currentState = STATE_GAMBLE_SELLITEM;
				return false;
			}
		}
	}

	currentState = STATE_NPC_LISTINGITEMS;
	if(!me->StartNpcSession(&gamblingNpc, NPC_GAMBLE))
	{
		server->GamePrintInfo("ÿc3Gambleÿc0: Failed to start gambling");
		me->RedrawClient(false);
		Reset(false);
		return false;
	}

	return true;
}

// Called when NPC is listing items up for gamble, before NPC_SESSION message
void Gambler::OnNpcGambleItemList(ITEM &gambleItem)
{
	if(currentState != STATE_NPC_LISTINGITEMS)
		return;

	if(itemsToGamble.count(gambleItem.szItemCode) > 0)
	{
		gambleQueue.push(gambleItem.dwItemID);
		ticksTillGambleItemTimeout = 50; // 5 second timeout
	}
}

void Gambler::OnNotEnoughMoney()
{
	if(currentState != STATE_GAMBLE_WAITFORITEM)
		return;

	server->GameStringf("ÿc3Gambleÿc0: Not enough money");

	currentState = STATE_GOLD_NEEDMORE;
}

void Gambler::OnItemSold()
{
	if(currentState != STATE_GAMBLE_SOLDITEM)
		return;

	currentState = STATE_GAMBLE_SELLITEM;
}

void Gambler::OnTick()
{
	switch(currentState)
	{
		case STATE_GAMBLE_NEXTITEM:
		{
			if(--ticksTillNextPurchase <= 0)
			{
				GambleQueuedItems();
				ticksTillNextPurchase = 2;
			}
			break;
		}
		case STATE_GAMBLE_WAITFORITEM:
		{
			if(ticksTillGambleItemTimeout-- == 0)
			{
				server->GamePrintInfo("ÿc3Gambleÿc0: Timed out buying items");
				Reset(false);
			}
			break;
		}
		case STATE_NPC_DONELISTINGITEMS:
		{
			currentState = STATE_GAMBLE_NEXTITEM;
			break;
		}
		case STATE_READYTORUN:
		{
			StartGambling();
			break;
		}
		case STATE_GAMBLE_SELLITEM:
		{
			if(itemsToSell.empty())
			{
				//server->GameStringf("ÿc3Gambleÿc0: Inventory full, nothing to sell");
				Reset(false);
			}
			else
			{
				SellQueuedItems();
			}

			break;
		}
		case STATE_GAMBLE_SOLDITEM:
		{
			currentState = STATE_GAMBLE_SELLITEM;
			break;
		}
		case STATE_GAMBLE_DONESELLING:
		{
			Reset(false);
			break;
		}
		case STATE_GOLD_NEEDMORE:
		{
			if(isRequestingGold)
			{
				me->CleanJobs();
				me->EndNpcSession();
				currentState = STATE_GOLD_WAIT;

				char goldRequest[32];

				sprintf_s(goldRequest, sizeof(goldRequest)/sizeof(goldRequest[0]), "%d", (me->GetInventoryGoldLimit() - me->GetStat(STAT_GOLD))/requestedGoldSplitBy);

				server->GameStringf("Looking for gold...");
				me->Say(goldRequest);				
			}
			else
			{
				if(isSellingGambledItems && !itemsToSell.empty())
				{
					currentState = STATE_GAMBLE_SELLITEM;
				}
				else
				{
					StopGambling();
				}
			}

			break;
		}
		case STATE_GOLD_REFILLED:
		{
			Reset(false);
			break;
		}
		case STATE_GAMBLE_DONE:
		{
			Reset(false);
			break;
		}
		case STATE_AUTOSTOCK_START:
		{
			if(ticksTillAutostock-- > 0)
				break;

			std::string asCommand;
			
			ticksTillNextPurchase = 0;

			while(!gambleQueue.empty())
				gambleQueue.pop();

			asCommand = server->GetCommandCharacter();
			asCommand += "as start_rares chat";

			if(transmuteSet)
				asCommand += " sets";
			if(transmuteRare)
				asCommand += " rares";
			if(transmuteUnique)
				asCommand += " uniques";
			
			me->Say(asCommand.c_str());

			currentState = STATE_AUTOSTOCK_RUNNING;
			ticksTillAutostock = autostockStartDelay;

			break;
		}
		case STATE_AUTOSTOCK_ENDED:
		{
			StartGambling();
			break;
		}
	}
}

void Gambler::SellQueuedItems()
{
	if(currentState != STATE_GAMBLE_SELLITEM)
		return;

	if(itemsToSell.empty())
	{
		currentState = STATE_GAMBLE_DONESELLING;
		return;
	}
	
	DWORD currentItemId = itemsToSell.front();
	itemsToSell.pop();

	if(!server->VerifyUnit(&gamblingNpc))
	{
		server->GamePrintInfo("ÿc3Gambleÿc0: Could not verify npc unit");
		StopGambling();
		return;
	}

	if(!me->SellItem(currentItemId))
	{
		server->GameStringf("ÿc3Gambleÿc0: Unable to sell item");
		return;
	}	

	currentState = STATE_GAMBLE_SELLITEM;
}

bool Gambler::WillItemFit(DWORD dwItemId)
{
	char itemCode[4];

	if(!server->GetItemCode(dwItemId, itemCode, 4))
	{
		server->GameErrorf("WillItemFit: Failed to get item code");
	}
	else
	{
		return me->FindFirstStorageSpace(STORAGE_INVENTORY, server->GetItemSize(itemCode), NULL);
	}

	return false;
}

bool Gambler::WillItemFit(const char *itemCode)
{
	return me->FindFirstStorageSpace(STORAGE_INVENTORY, server->GetItemSize(itemCode), NULL);
}

// only when [next item to gamble]
void Gambler::GambleQueuedItems()
{
	if(currentState != STATE_GAMBLE_NEXTITEM)
		return;

	if(gambleQueue.empty())
	{
		currentState = STATE_GAMBLE_DONE;
		return;
	}
	
	DWORD currentItemId = gambleQueue.front();
	gambleQueue.pop();

	if(!WillItemFit(currentItemId))
	{
		Reset(false);
		return;
	}

	if(!server->VerifyUnit(&gamblingNpc))
	{
		server->GamePrintInfo("ÿc3Gambleÿc0: Could not verify npc unit");
		StopGambling();
		return;
	}

	currentState = STATE_GAMBLE_WAITFORITEM;
	if(!me->Gamble(currentItemId))
	{
		server->GamePrintInfo("ÿc3Gambleÿc0: Unable to buy item");
		Reset(false);
		return;
	}
}


// [Wait for item to inventory]
// Called when a gambled item is placed in our inventory
void Gambler::OnItemToStorage(ITEM &gambleItem)
{
	if(currentState != STATE_GAMBLE_WAITFORITEM)
		return;

	std::string itemName = server->GetItemName(gambleItem.szItemCode);

	if(gambleItem.iQuality == ITEM_LEVEL_UNIQUE)
	{
		server->GameStringf("Gamble: ÿc4Unique %s", itemName.c_str());
		stats.uniques++;
	}
	else if(gambleItem.iQuality == ITEM_LEVEL_SET)
	{
		server->GameStringf("Gamble: ÿc2Set %s", itemName.c_str());
		stats.sets++;
	}
	else if(gambleItem.iQuality == ITEM_LEVEL_RARE)
	{
		server->GameStringf("Gamble: ÿc9Rare %s", itemName.c_str());
		stats.rares++;

		if(isSellingGambledItems)
		{
			itemsToSell.push(gambleItem.dwItemID);
		}
	}
	else if(gambleItem.iQuality == ITEM_LEVEL_MAGIC)
	{
		server->GameStringf("Gamble: ÿc3Magic %s", itemName.c_str());
		stats.magic++;

		if(isSellingGambledItems)
		{
			itemsToSell.push(gambleItem.dwItemID);
		}
	}
	else
	{
		server->GameStringf("Gamble: ÿc2Unknown[%02X] %s", gambleItem.iQuality, itemName.c_str());
	}

	currentState = STATE_GAMBLE_NEXTITEM;
}

// Called when NPC is done giving us the list of items to gamble
void Gambler::OnNpcSession(int success)
{
	if(currentState != STATE_NPC_LISTINGITEMS)
		return;

	if(!success)
	{
		me->RedrawClient(FALSE);
		me->MoveToUnit(&gamblingNpc, TRUE);
		server->GamePrintInfo("ÿc3Gambleÿc0: NPC request failed");
		Reset(false);
		return;
	}

	currentState = STATE_NPC_DONELISTINGITEMS;
}

void Gambler::OnGoldPickup()
{
	if(currentState != STATE_GOLD_WAIT)
		return;

	currentState = STATE_GOLD_REFILLED;
}

bool Gambler::OnAutostockerEnded()
{
	if(currentState != STATE_AUTOSTOCK_RUNNING)
		return false;

	currentState = STATE_AUTOSTOCK_ENDED;

	return true;
}

void Gambler::ToggleRequestGold(int splitBy)
{
	isRequestingGold = !isRequestingGold;
	requestedGoldSplitBy = splitBy;

	if(isRequestingGold)
		server->GameStringf("ÿc3Gambleÿc0: Gold requesting ÿc2enabledÿc0, split by %d", splitBy);
	else
		server->GamePrintInfo("ÿc3Gambleÿc0: Gold requesting ÿc1disabledÿc0");
}

void Gambler::ToggleGambleSell()
{
	isSellingGambledItems = !isSellingGambledItems;

	if(isSellingGambledItems)
	{
		server->GamePrintInfo("ÿc3Gambleÿc0: Selling gambled magic items ÿc2enabledÿc0");
	}
	else
	{
		while(!itemsToSell.empty())
		{
			itemsToSell.pop();
		}

		server->GamePrintInfo("ÿc3Gambleÿc0: Selling gambled magic items ÿc1disabledÿc0");
	}
}

void Gambler::ToggleAutostock(bool transmuteSet, bool transmuteRare, bool transmuteUnique)
{
	transmuteEnabled = !transmuteEnabled;

	if(transmuteEnabled)
	{
		this->transmuteSet = transmuteSet;
		this->transmuteRare = transmuteRare;
		this->transmuteUnique = transmuteUnique;
		server->GameStringf("ÿc3Gambleÿc0: Autostocking ÿc3Magic%s%s%s", transmuteSet?" ÿc2Sets":"", transmuteRare?" ÿc9Rares":"", transmuteUnique?" ÿc4Uniques":"");
	}
	else
	{
		this->transmuteSet = false;
		this->transmuteRare = false;
		this->transmuteUnique = false;
		server->GamePrintInfo("ÿc3Gambleÿc0: Autostock ÿc1disabledÿc0");
	}
}

void Gambler::SetAutostockStartDelay(int ticks)
{
	server->GameStringf("ÿc3Gambleÿc0: Delay before starting autostocker is now %dms", ticks*100);

	autostockStartDelay = ticks;
	ticksTillAutostock = ticks;
}

DWORD Gambler::FindGamblingNpc()
{
	GAMEUNIT gameUnit;
	const char *gamblingNpcNames[] = 
	{
		"Gheed",
		"Elzix",
		"Alkor",
		"Jamella",
		"Nihlathak",
		"Anya"
	};

	for(int i = 0; i < sizeof(gamblingNpcNames)/sizeof(gamblingNpcNames[0]); i++)
	{
		if(server->FindUnitByName(gamblingNpcNames[i], UNIT_TYPE_MONSTER, &gameUnit))
		{
			return gameUnit.dwUnitID;
		}
	}

	return 0;
}
