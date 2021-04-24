#include "../../Includes/D2Client.h"
#include "AutostockAutoextract.h"


AutostockAutoextract::AutostockAutoextract()
{
	currentState = STATE_UNINITIALIZED;
}

/// <summary>
/// Initializes autostockAutoextract
/// </summary>
/// <param name="useChat">Determines if output messages should be sent as chat messages.</param>
/// <returns>true if successful, false if failed.</returns>
bool AutostockAutoextract::Init(bool useChat)
{
	this->useChat = useChat;
	autoExtractorLoaded = false;
	autoStockerLoaded = false;
	emptyCubeLoaded = false;

	return true;
}

/// <summary>
/// Starts asae with custom settings for autostocker
/// </summary>
/// <param name="transmuteSet">Determines if set items should be cubed.</param>
/// <param name="transmuteRare">Determines if rare items should be cubed.</param>
/// <param name="transmuteUnique">Determines if unique items should be cubed.</param>
/// <param name="useChat">Determines if output messages should be sent as chat messages.</param>
/// <returns>true if successful, false if failed.</returns>
bool AutostockAutoextract::Start(bool transmuteSet, bool transmuteRare, bool transmuteUnique, bool useChat)
{
	if(!Init(useChat))
	{
		return false;
	}

	this->transmuteSet = transmuteSet;
	this->transmuteRare = transmuteRare;
	this->transmuteUnique = transmuteUnique;

	server->GameStringf("ÿc:Autostockerÿc0: Transmuting: ÿc3Magic%s%s%s", transmuteSet?" ÿc2Sets":"", transmuteRare?" ÿc9Rares":"", transmuteUnique?" ÿc4Uniques":"");

	return StartAsAe();
}

/// <summary>
/// Starts the actual ASAE process
/// </summary>
/// <returns>true if successful, false if failed.</returns>
bool AutostockAutoextract::StartAsAe()
{
	if(!me->OpenCube())
	{
		if(useChat)
		{
			me->Say("ÿc5AutostockAutoextractÿc0: Cube not opened");
		}
		server->GameStringf("ÿc5EmptyCubeÿc0: Cube not opened");

		Abort();
		return false;
	}

	// Make sure we have both the extractor (e.g: key) and resocker (e.g: rerolling orb) in the
	//   cube before we start.
	memset(&extractorStuff, 0, sizeof(ExtractorStuff));
	me->EnumStorageItems(STORAGE_CUBE, enumFindCubeItems, (LPARAM)&extractorStuff);
	if (strlen(extractorStuff.extractorCode) == 0 || strlen(extractorStuff.restockerCode) == 0)
	{
		if (strcmp(extractorStuff.extractorCode, "ore") != 0)
		{
			server->GameStringf("ÿc5AutostockAutoextractÿc0: Place extractors in cube before starting");
			Abort();
			return false;
		}
	}

	// Load required modules
	if(!autoExtractorLoaded)
	{
		autoExtractorLoaded = true;
		server->GameCommandLine("load ae");
	}
	if(!autoStockerLoaded)
	{
		autoStockerLoaded = true;
		server->GameCommandLine("load as");
	}
	if(!emptyCubeLoaded)
	{
		emptyCubeLoaded = true;
		server->GameCommandLine("load emptycube");
	}

	// We must run autoextractor first and wait for it to tell us it's complete
	currentState = STATE_RUNAUTOEXTRACTOR;
	server->GameCommandLine("ae start chat");

	return true;
}

/// <summary>
/// Called when the autoextractor finishes extracting the items. Extractor and restocker
///   will remain in the cube when it's complete
/// </summary>
/// <returns>true if this event was handled, false if ignored.</returns>
bool AutostockAutoextract::OnAutoExtractorEnded()
{
	// We only care about this event when we just started the autoextractor
	if(currentState != STATE_RUNAUTOEXTRACTOR)
	{
		return false;
	}

	// Find the extractor and restocker (e.g: key + rerolling orb) ids before we clean the cube so we can
	//   put them back in later on
	me->EnumStorageItems(STORAGE_CUBE, enumFindCubeItems, (LPARAM)&extractorStuff);

	// Clear the cube so we can run the autostocker next
	currentState = STATE_RUNEMPTYCUBE;
	server->GameCommandf("emptycube start chat");

	return true;
}

/// <summary>
/// Called when emptyCube has finished cleaning out the cube. Starts the autostocker process.
/// </summary>
/// <returns>true if this event was handled, false if ignored.</returns>
bool AutostockAutoextract::OnEmptyCubeEnded()
{
	// We only care about this event when we have called emptyCube
	if(currentState != STATE_RUNEMPTYCUBE)
	{
		return false;
	}

	// Now that the cube is empty, we can start the autostocker
	currentState = STATE_RUNAUTOSTOCKER;
	server->GameCommandf("as start_rares chat%s%s%s",  transmuteSet?" Sets":"", transmuteRare?" Rares":"", transmuteUnique?" Uniques":"");

	return true;
}

/// <summary>
/// Called when the autoextractor finishes extracting the items. Extractor and restocker
///   will remain in the cube when it's complete
/// </summary>
/// <returns>true if this event was handled, false if ignored.</returns>
bool AutostockAutoextract::OnAutostockerEnded()
{
	// We only care about this event when we start the autostocker
	if(currentState != STATE_RUNAUTOSTOCKER)
	{
		return false;
	}

	MoveExtractorAndStockerToCube();

	return true;
}

/// <summary>
/// Moves the extractor (e.g: key) and stocker (e.g: rerolling orb) to the cube. If both items
///   have already been moved to the cube, then the ASAE process is started all over again
/// </summary>
void AutostockAutoextract::MoveExtractorAndStockerToCube()
{
	DWORD itemToPickup = 0;

	if(!extractorStuff.movedExtractor)
	{
		// Pickup the extractor (e.g: key) first if it hasn't been moved to the cube yet
		itemToPickup = extractorStuff.extractorID;
		extractorStuff.movedExtractor = true;
	}
	else if(!extractorStuff.movedRestocker)
	{
		// Pickup the stocker (e.g: rerolling orb) second if it hasn't been moved to the cube yet
		itemToPickup = extractorStuff.restockerID;
		extractorStuff.movedRestocker = true;
	}
	else
	{
		// We moved both the extractor and stocker to the cube, we should start the ASAE process all over again
		StartAsAe();
		return;
	}

	// Pickup the extractor or stocker so we can move it to the cube
	currentState = STATE_PICKUPEXTRACTORSTUFF;
	if(!me->PickStorageItemToCursor(itemToPickup))
	{
		server->GameStringf("Failed to pickup extractor (e.g: key) or restocker (e.g: rerolling orb)");
		Abort();
		return;
	}
}

/// <summary>
/// Aborts the ASAE process if it's currently running
/// </summary>
void AutostockAutoextract::Abort()
{
	if(currentState != STATE_COMPLETE && currentState != STATE_UNINITIALIZED)
	{
		currentState = STATE_COMPLETE;
	}
}

/// <summary>
/// Called at a constant rate by d2hackit's timer
/// </summary>
void AutostockAutoextract::OnTick()
{
	switch(currentState)
	{
		case STATE_COMPLETE:
		{
			currentState = STATE_UNINITIALIZED;

			if(useChat)
			{
				me->Say("ÿc5AutostockAutoextractÿc0: AutostockAutoextract Ended");
			}
			else
			{
				server->GameStringf("ÿc5AutostockAutoextractÿc0: AutostockAutoextract Ended");
			}

			break;
		}
	}
}

/// <summary>
/// Called whenever an item is picked up from the inventory
/// </summary>
/// <param name="item">The item picked up from player's inventory.</param>
void AutostockAutoextract::OnItemFromInventory(const ITEM &item)
{
	// We only care about items picked up from the player's inventory when we're moving
	//   the extractor and stocker to the cube
	if(currentState != STATE_PICKUPEXTRACTORSTUFF)
	{
		return;
	}

	// Drop the extractor (e.g: key) or reroller (e.g: rerolling orb) to the cube
	currentState = STATE_EXTRACTORSTUFFTOCUBE;
	if(!me->DropItemToStorage(STORAGE_CUBE, item.dwItemID))
	{
		server->GameStringf("ÿc5AutostockAutoextractÿc0: Failed to drop extractorstuff to cube");
		Abort();
		return;
	}
}

/// <summary>
/// Called whenever an item is moved to the player's inventory
/// </summary>
/// <param name="item">The item moved to the player's inventory.</param>
void AutostockAutoextract::OnItemFromCube(const ITEM &item)
{
	// We only care about items going to the inventory when autostocker is
	//  moving the extractor back to the inventory
	if(currentState != STATE_RUNAUTOSTOCKER)
	{
		return;
	}

	// The stocker item ID changes each time it's transmuted so we need
	//   to double check that the item placed in our inventory by autostocker
	//   was indeed a restocker and store the new item ID of that stocker for
	//   future use
	if(GetStockerTypeByCode(item.szItemCode, NULL))
	{
		extractorStuff.restockerID = item.dwItemID;
	}
}

/// <summary>
/// Called whenever an item is moved to the cube
/// </summary>
/// <param name="item">The item moved to the cube.</param>
void AutostockAutoextract::OnItemToCube(const ITEM &item)
{
	// We only care about items going to the cube when we're moving the extractor 
	//   or stocker to the cube
	if(currentState != STATE_EXTRACTORSTUFFTOCUBE)
	{
		return;
	}

	// Move the stocker to the cube next, or start ASAE all over again since both
	//   the extractor and restocker are in the cube
	MoveExtractorAndStockerToCube();
}

/// <summary>
/// Searches the cube for the extractor (e.g: key) and stocker (e.g: rerolling orb). ID's of these
///   two items are stored for future use.
/// </summary>
/// <param name="item">Item found in the player's cube.</param>
/// <param name="lParam">Pointer to ExtractorStuff structure where extractor and stocker item IDs will be stored.</param>
/// <returns>Always true.</returns>
BOOL CALLBACK enumFindCubeItems(LPCITEM item, LPARAM lParam)
{
	ExtractorStuff *extractorStuff = (ExtractorStuff *)lParam;

	if(GetStockerTypeByCode(item->szItemCode, NULL))
	{
		strcpy_s(extractorStuff->restockerCode, 4, item->szItemCode);
		extractorStuff->restockerID = item->dwItemID;
	}
	else
	{
		strcpy_s(extractorStuff->extractorCode, 4, item->szItemCode);
		extractorStuff->extractorID = item->dwItemID;
	}

	return TRUE;
}

/// <summary>
/// Determines the type of stocker the item with specified ID is
/// </summary>
/// <param name="itemId">ItemID of item being checked.</param>
/// <param name="stockerType">[out] Type of stocker this item is.</param>
/// <returns>true if item is a stocker, false if not a stocker.</returns>
bool GetStockerTypeByID(DWORD itemId, int *stockerType)
{
	char itemCode[4];

	if(!server->GetItemCodeEx(itemId, itemCode, sizeof(itemCode)/sizeof(itemCode[0]), 10, 50))
	{
		server->GameStringf("ÿc5AutostockAutoextractÿc0: Failed to get item code");
		return false;
	}

	return GetStockerTypeByCode(itemCode, stockerType);
}

/// <summary>
/// Determines the type of stocker the item with specified item code
/// TODO: this is horrible -> Store all stockers in a nice, easy to access list
/// TODO: This is duplicate code from autostocker!
/// </summary>
/// <param name="itemCode">Itemcode with 4 elements.</param>
/// <param name="stockerType">[out] Type of stocker this item is.</param>
/// <returns>true if item is a stocker, false if not a stocker.</returns>
bool GetStockerTypeByCode(const char *itemCode, int *stockerType)
{
	int itemCodeNum = 0;

	if(itemCode[0] == 'k' && itemCode[2] == '0')
	{
		if(itemCode[1] == 'v' || itemCode[1] == 'y' ||
			itemCode[1] == 'b' || itemCode[1] == 'g' ||
			itemCode[1] == 'r' || itemCode[1] == 'w' ||
			itemCode[1] == 's' || itemCode[1] == 'k')
		{
			// GemCan
			if(stockerType)
				*stockerType = TRANSMUTE_GEM;
			return true;
		}
	}
	else if(itemCode[0] == 't' && isdigit(itemCode[1]) && isdigit(itemCode[2]))
	{
		itemCodeNum = atoi(itemCode + 1);

		if(itemCodeNum >= 01 && itemCodeNum <= 30)
		{
			// Rerolling orb = t01-t30
			if(stockerType)
				*stockerType = TRANSMUTE_REROLL;
			return true;
		}
		else if(itemCodeNum >= 51 && itemCodeNum <= 75)
		{
			// Multistocker = t51-t75
			if(stockerType)
				*stockerType = TRANSMUTE_MULTISTOCKER;
			return true;
		}
	}
	else if(itemCode[0] == 's' && isdigit(itemCode[1]) && isdigit(itemCode[2]))
	{
		itemCodeNum = atoi(itemCode + 1);

		if(itemCodeNum >= 01 && itemCodeNum <= 21)
		{
			// s01 - s21 Rune stocker a
			if(stockerType)
				*stockerType = TRANSMUTE_RUNEA;
			return true;
		}
		else if(itemCodeNum >= 22 && itemCodeNum <= 50)
		{
			//s22 - s50 Rune stocker b
			if(stockerType)
				*stockerType = TRANSMUTE_RUNEB;
			return true;
		}
		else if(itemCodeNum >= 51 && itemCodeNum <= 83)
		{
			//s51 - s83 Decal stocker
			if(stockerType)
				*stockerType = TRANSMUTE_DECAL;
			return true;
		}
	}
	else if(itemCode[0] == 'w' && isdigit(itemCode[1]) && isdigit(itemCode[2]))
	{
		itemCodeNum = atoi(itemCode + 1);
		
		if(itemCodeNum >= 31 && itemCodeNum <= 55)
		{
			//w31 Tome a
			if(stockerType)
				*stockerType = TRANSMUTE_TOMEA;
			return true;
		}
		else if(itemCodeNum >= 56 && itemCodeNum <= 80)
		{
			//w56 Tome b
			if(stockerType)
				*stockerType = TRANSMUTE_TOMEB;
			return true;
		}
	}
	else if(itemCode[0] == 'x' && itemCode[1] == 'u')
	{
		if(isdigit(itemCode[2]) || itemCode[2] == 'x' || itemCode[2] == 'y')
		{
			if(stockerType)
				*stockerType = TRANSMUTE_CRYSTAL;
			return true;
		}
	}

	return false;
}
