#include "gambler.h"
#include <fstream>

#if 0
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

/// <summary>
/// Initializes the gambling process
/// </summary>
/// <param name="itemCodes">List of item codes we're gambling for</param>
/// <returns>true on success, false on failure (no gambling npc)</returns>
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

	ResetQueues();
	while(!itemsToSell.empty())
	{
		itemsToSell.pop();
	}

	return true;
}

/// <summary>
/// Stops the entire gambling process.
/// </summary>
void Gambler::StopGambling()
{
	if(currentState == STATE_UNINITIALIZED)
		return;

	DEBUG_TRACE();

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
		
	ResetQueues();
	while(!itemsToSell.empty())
	{
		itemsToSell.pop();
	}

	memset(&stats, 0, sizeof(GambleStats));
	currentState = STATE_UNINITIALIZED;
}

/// <summary>
/// Resets the queuest of items we are trying to buy from the current npc session
/// </summary>
void Gambler::ResetQueues()
{
	DEBUG_TRACE();

	while(!gambleQueue.empty())
	{
		gambleQueue.pop();
	}
}

/// <summary>
/// Attempts to make more inventory room by either selling or transmuting unwanted gambled items.
/// </summary>
void Gambler::AttemptToMakeRoom()
{
	DEBUG_TRACE();

	if(!itemsToSell.empty())
	{
		SellQueuedItems();
		return;
	}

	server->GameStringf("ÿc3Gambleÿc0: Not enough room");
	if(transmuteEnabled && currentState != STATE_AUTOSTOCK_ENDED)
	{
		if(me->GetOpenedUI())
		{
			me->EndNpcSession();
			currentState = STATE_UI_CLOSING_FOR_AUTOSTOCKER;
			return;
		}

		StartAutostocker();
		return;
	}

	StopGambling();
}

/// <summary>
/// Begins another iteration of gambling. Inventory space remaining is checked prior to gambling and attempts
///   are made to free some room if possible. Gambling ends if inventory is completely full.
/// </summary>
bool Gambler::StartGambling()
{
	DEBUG_TRACE();

	for(stdext::hash_set<std::string>::iterator i = itemsToGamble.begin(); i != itemsToGamble.end(); ++i)
	{
		if(!WillItemFit((*i).c_str()))
		{
			AttemptToMakeRoom();
			return false;
		}
	}

	currentState = STATE_NPC_LISTINGITEMS;
	if(!me->StartNpcSession(&gamblingNpc, NPC_GAMBLE))
	{
		server->GamePrintInfo("ÿc3Gambleÿc0: Failed to start gambling");
		me->RedrawClient(false);
		StartGambling();
		return false;
	}

	return true;
}

/// <summary>
/// Attempts to request more gold by either asking our money bot or selling previously purchesd items.
///   Gambling is ended if the previous two options are not available.
/// </summary>
void Gambler::RequestMoreGold()
{
	DEBUG_TRACE();

	if(isRequestingGold)
	{
		me->CleanJobs();
		me->EndNpcSession();
		ResetQueues();
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

/// <summary>
/// Starts the autostocker. Completion of the autostocker is handled in the
///   OnAutostockerEnded event handler.
/// </summary>
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

/// <summary>
/// Attempts to sell a previously gambled item. Successful selling of the item will
///   change to the STATE_GAMBLE_SOLDITEM state, which will be handled in the OnItemSold
///   event handler.
/// </summary>
void Gambler::SellQueuedItems()
{
	DEBUG_TRACE();

	if(itemsToSell.empty())
	{
		StartGambling();
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

/// <summary>
/// Locates the nearest gambling NPC and returns its unique ID
/// </summary>
/// <returns>ID of the closest gambling NPC or 0 on failure</returns>
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

/// <summary>
/// Toggles requesting of the maximum amount of gold when gold runs out
/// </summary>
/// <param name="splitBy">Number of players listening to our request. Total amount requested is divided by this amount</param>
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

/// <summary>
/// Toggles the selling gambled items when inventory becomes full during gambling
/// </summary>
/// <param name="sellSet">Set items will be sold</param>
/// <param name="sellRare">Rare items will be sold</param>
/// <param name="sellUnique">Unique items will be sold</param>
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

/// <summary>
/// Toggles the usage of AutoStocker when inventory becomes full during gambling
/// </summary>
/// <param name="transmuteSet">Set items will be transmuted</param>
/// <param name="transmuteRare">Rare items will be transmuted</param>
/// <param name="transmuteUnique">Unique items will be transmuted</param>
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

/// <summary>
/// Check to see if an exsiting item can fit in our inventory
/// </summary>
/// <param name="dwItemId">Unique ID of the item being checked</param>
/// <returns>True if item can fit, false if it won't fit</returns>
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

/// <summary>
/// Check to see if an item of specified item code can fit in our inventory
/// </summary>
/// <param name="itemCode">Code of item being checked</param>
/// <returns>True if item can fit, false if it won't fit</returns>
bool Gambler::WillItemFit(const char *itemCode)
{
	DEBUG_TRACE();

	return me->FindFirstStorageSpace(STORAGE_INVENTORY, server->GetItemSize(itemCode), NULL) == TRUE;
}

/// <summary>
/// Attempt to buy all of the items we're interested in. If we're full we will attempt to sell any
///   previously purchased items (if enabled)
/// </summary>
void Gambler::GambleQueuedItems()
{
	DEBUG_TRACE();

	if(gambleQueue.empty())
	{
		StartGambling();
		return;
	}
	
	// Don't pop the item from the queue, we might be able to attempt another gamble after
	//  selling items
	DWORD currentItemId = gambleQueue.front();
	if(!WillItemFit(currentItemId))
	{
		SellQueuedItems();
		return;
	}
	gambleQueue.pop();

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
		ResetQueues();
		StartGambling();
		return;
	}
}

/// <summary>
/// NPC is listing their inventory
/// </summary>
/// <param name="gambleItem">Item that can be purchased</param>
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

/// <summary>
/// We don't have enough money to buy the item(s), request more gold
/// </summary>
void Gambler::OnNotEnoughMoney()
{
	if(currentState != STATE_GAMBLE_WAITFORITEM)
		return;

	DEBUG_TRACE();
	server->GameStringf("ÿc3Gambleÿc0: Not enough money");
	RequestMoreGold();
}

/// <summary>
/// Item was sold to the vendor. Event occurs after game has processed it.
/// </summary>
void Gambler::OnItemSold()
{
	if(currentState != STATE_GAMBLE_SOLDITEM)
		return;

	DEBUG_TRACE();
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
				ResetQueues();
				StartGambling();
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
		StartGambling();
		return;
	}

	GambleQueuedItems();
}

/// <summary>
/// We have picked up more gold and are ready to gamble more
/// </summary>
void Gambler::OnGoldPickup()
{
	if(currentState != STATE_GOLD_WAIT)
		return;

	DEBUG_TRACE();
	StartGambling();
}

/// <summary>
/// Autostocker has completed, we will continue gambling if we requested autostocker to run
///   previously
/// </summary>
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