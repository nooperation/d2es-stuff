#include "gambler.h"
#include <fstream>

#if 1
	#define DEBUG_TRACE() {server->GameStringf("%s (%s:%d)", __FUNCTION__, __FILE__, __LINE__);}
#else
	#define DEBUG_TRACE() {}
#endif

Gambler::Gambler()
{
	requestedGoldSplitBy = 1;
	isSellingGambledItems = false;
	isRequestingGold = false;
	transmuteSet = false;
	transmuteRare = false;
	transmuteUnique = false;
	transmuteEnabled = false;

	sellRare = false;
	sellSet = false;
	sellUnique = false;

	currentState = STATE_UNINITIALIZED;
}

bool Gambler::Init(std::vector<std::string> itemCodes)
{
	DEBUG_TRACE();
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

	while(!gambleQueue.empty())
	{
		gambleQueue.pop();
	}
	while(!itemsToSell.empty())
	{
		itemsToSell.pop();
	}

	return true;
}

void Gambler::StopGambling()
{
	if(currentState == STATE_UNINITIALIZED)
		return;

	DEBUG_TRACE();
	Reset(true);
}

void Gambler::Reset(bool haltGambling)
{
	DEBUG_TRACE();

	while(!gambleQueue.empty())
	{
		gambleQueue.pop();
	}

	if(haltGambling)
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

		server->GamePrintInfo("ÿc3Gambleÿc0: Gamble ended");
		me->CleanJobs();
		me->EndNpcSession();
		
		while(!itemsToSell.empty())
		{
			itemsToSell.pop();
		}

		memset(&stats, 0, sizeof(GambleStats));
		currentState = STATE_UNINITIALIZED;
	}
	else
	{
		StartGambling();
	}
}

bool Gambler::StartGambling()
{
	DEBUG_TRACE();

	for(stdext::hash_set<std::string>::iterator i = itemsToGamble.begin(); i != itemsToGamble.end(); ++i)
	{
		if(!WillItemFit((*i).c_str()))
		{
			if(itemsToSell.empty())
			{
				server->GameStringf("ÿc3Gambleÿc0: Not enough room");
				if(transmuteEnabled && currentState != STATE_AUTOSTOCK_ENDED)
				{
					if(me->GetOpenedUI())
					{
						me->EndNpcSession();
						currentState = STATE_UI_CLOSING_FOR_AUTOSTOCKER;
					}
					else
					{
						StartAutostocker();
					}
					//me->EndNpcSession();
					//StartAutostocker();
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
				SellQueuedItems();
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

void Gambler::RequestMoreGold()
{
	DEBUG_TRACE();
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
			SellQueuedItems();
		}
		else
		{
			StopGambling();
		}
	}
}

void Gambler::StartAutostocker()
{
	DEBUG_TRACE();
	currentState = STATE_AUTOSTOCK_RUNNING;

	std::string asCommand;

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
}

void Gambler::SellQueuedItems()
{
	DEBUG_TRACE();
	if(itemsToSell.empty())
	{
		StartGambling();
		//Reset(false);
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

	currentState = STATE_GAMBLE_SOLDITEM;
	if(!me->SellItem(currentItemId))
	{
		server->GameStringf("ÿc3Gambleÿc0: Unable to sell item");
		return;
	}
}

DWORD Gambler::FindGamblingNpc()
{
	DEBUG_TRACE();
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

void Gambler::ToggleRequestGold(int splitBy)
{
	DEBUG_TRACE();
	isRequestingGold = !isRequestingGold;
	requestedGoldSplitBy = splitBy;

	if(isRequestingGold)
		server->GameStringf("ÿc3Gambleÿc0: Gold requesting ÿc2enabledÿc0, split by %d", splitBy);
	else
		server->GamePrintInfo("ÿc3Gambleÿc0: Gold requesting ÿc1disabledÿc0");
}

void Gambler::ToggleGambleSell(bool sellSet, bool sellRare, bool sellUnique)
{
	DEBUG_TRACE();
	isSellingGambledItems = !isSellingGambledItems;

	this->sellRare = sellRare;
	this->sellSet = sellSet;
	this->sellUnique = sellUnique;

	if(isSellingGambledItems)
	{
		server->GameStringf("ÿc3Gambleÿc0: Selling ÿc3Magic%s%s%s", sellSet?" ÿc2Sets":"", sellRare?" ÿc9Rares":"", sellUnique?" ÿc4Uniques":"");
	}
	else
	{
		while(!itemsToSell.empty())
		{
			itemsToSell.pop();
		}

		server->GamePrintInfo("ÿc3Gambleÿc0: Selling gambled items ÿc1disabledÿc0");
	}
}

void Gambler::ToggleAutostock(bool transmuteSet, bool transmuteRare, bool transmuteUnique)
{
	DEBUG_TRACE();
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

bool Gambler::WillItemFit(DWORD dwItemId)
{
	DEBUG_TRACE();
	char itemCode[4];

	if(!server->GetItemCode(dwItemId, itemCode, 4))
	{
		server->GameErrorf("WillItemFit: Failed to get item code");
	}
	else
	{
		return me->FindFirstStorageSpace(STORAGE_INVENTORY, server->GetItemSize(itemCode), NULL) == TRUE;
	}

	return false;
}

bool Gambler::WillItemFit(const char *itemCode)
{
	DEBUG_TRACE();
	return me->FindFirstStorageSpace(STORAGE_INVENTORY, server->GetItemSize(itemCode), NULL) == TRUE;
}

// only when [next item to gamble]
void Gambler::GambleQueuedItems()
{
	DEBUG_TRACE();
	if(gambleQueue.empty())
	{
		Reset(false);
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


// Called when NPC is listing items up for gamble, before NPC_SESSION message
void Gambler::OnNpcGambleItemList(ITEM &gambleItem)
{
	if(currentState != STATE_NPC_LISTINGITEMS)
		return;

	//DEBUG_TRACE();
	if(itemsToGamble.count(gambleItem.szItemCode) > 0)
	{
		gambleQueue.push(gambleItem.dwItemID);
		ticksTillGambleItemTimeout = 30 * (1000 / server->GetTickRate()); // 30 second timeout
	}
}

void Gambler::OnNotEnoughMoney()
{
	if(currentState != STATE_GAMBLE_WAITFORITEM)
		return;

	DEBUG_TRACE();
	server->GameStringf("ÿc3Gambleÿc0: Not enough money");
	RequestMoreGold();
}

/// <summary>
/// Item was sold to the vendor
/// </summary>
/// <param name="">.</param>
void Gambler::OnItemSold()
{
	if(currentState != STATE_GAMBLE_SOLDITEM)
		return;

	DEBUG_TRACE();
	server->GameStringf("Sold item, %d left in gamble queue", gambleQueue.size());
	GambleQueuedItems();
}

/// <summary>
/// Process a single tick. Handles timeouts/etc
/// </summary>
void Gambler::OnTick()
{
	switch(currentState)
	{
		case STATE_GAMBLE_WAITFORITEM:
		{
			if(ticksTillGambleItemTimeout-- == 0)
			{
				server->GamePrintInfo("ÿc3Gambleÿc0: Timed out buying items");
				Reset(false);
			}
			break;
		}
	}
}

/// <summary>
/// An item was placed in the player's inventory. If we're currently gambling then the item
///   is processed, otherwise it's ignored.
/// </summary>
/// <param name="gambleItem">Item that was placed in our inventory.</param>
void Gambler::OnItemToStorage(ITEM &gambleItem)
{
	if(currentState != STATE_GAMBLE_WAITFORITEM)
		return;

	DEBUG_TRACE();
	std::string itemName = server->GetItemName(gambleItem.szItemCode);

	if(gambleItem.iQuality == ITEM_LEVEL_UNIQUE)
	{
		if(isSellingGambledItems && sellUnique)
		{
			itemsToSell.push(gambleItem.dwItemID);
		}

		server->GameStringf("Gamble: ÿc4Unique %s", itemName.c_str());
		stats.uniques++;
	}
	else if(gambleItem.iQuality == ITEM_LEVEL_SET)
	{
		if(isSellingGambledItems && sellSet)
		{
			itemsToSell.push(gambleItem.dwItemID);
		}

		server->GameStringf("Gamble: ÿc2Set %s", itemName.c_str());
		stats.sets++;
	}
	else if(gambleItem.iQuality == ITEM_LEVEL_RARE)
	{
		server->GameStringf("Gamble: ÿc9Rare %s", itemName.c_str());
		stats.rares++;

		if(isSellingGambledItems && sellRare)
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
}

/// <summary>
/// *Should* be called whenever the npc has completed listing all of their items. Possible to miss
///   some since d2hackit waits 1000ms after every item from the merchant (resetting to 1000ms every
///   time a new item packet comes in) before this message is sent
/// </summary>
/// <param name="success">True if session was successfully established with NPC.</param>
void Gambler::OnNpcSession(int success)
{
	if(currentState != STATE_NPC_LISTINGITEMS)
		return;

	DEBUG_TRACE();
	if(!success)
	{
		me->RedrawClient(FALSE);
		me->MoveToUnit(&gamblingNpc, TRUE);
		server->GamePrintInfo("ÿc3Gambleÿc0: NPC request failed");
		Reset(false);
		return;
	}

	GambleQueuedItems();
}

/// <summary>
/// 
/// </summary>
/// <param name="">.</param>
void Gambler::OnGoldPickup()
{
	if(currentState != STATE_GOLD_WAIT)
		return;

	DEBUG_TRACE();
	Reset(false);
}

/// <summary>
/// Autostocker has completed, we will continue gambling if we requested autostocker to run
///   previously
/// </summary>
/// <param name="">.</param>
bool Gambler::OnAutostockerEnded()
{
	if(currentState != STATE_AUTOSTOCK_RUNNING)
		return false;

	DEBUG_TRACE();
	StartGambling();
	return true;
}

/// <summary>
/// The 'item identified' swish sound has been scheduled to play, this is the last identifiable packet that is 
///   sent done before we can continue gambling the rest of our items. If we gamble again before this packet then
///   we might get kicked
/// </summary>
void Gambler::OnItemIdentifiedSounded()
{
	if(currentState != STATE_GAMBLE_WAITFORITEM)
		return;

	DEBUG_TRACE();
	GambleQueuedItems();
}

/// <summary>
/// All UIs have been closed and we're able to start the autostocker now if we're waiting on it
/// </summary>
void Gambler::OnUIClosed()
{
	if(currentState != STATE_UI_CLOSING_FOR_AUTOSTOCKER)
		return;

	DEBUG_TRACE();
	StartAutostocker();
}