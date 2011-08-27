#include "../../d2hackit/includes/D2Client.h"
#include "AutoStocker.h"
#include <fstream>
#include <string>

#define CONFIG_FILE ".\\plugin\\autostockerMisc.ini"

// Must match order of TransmuteStages
char *stockerSetNames[] =
{
	"gem",
	"runea",
	"runeb",
	"decal",
	"multistocker",
	"tomea",
	"tomeb",
	"crystal",
	"reroll",
};

AutoStocker::AutoStocker()
{
	currentState = STATE_UNINITIALIZED;
}

/// <summary>
/// Initalizes autostocker
/// </summary>
/// <param name="useChat">Determines if output messages should be sent as chat messages.</param>
bool AutoStocker::Init(bool useChat)
{
	this->useChat = useChat;

	if(!ReadConfig(".\\plugin\\autostocker.ini"))
	{
		return false;
	}

	if(!ReadAffixConfig(".\\plugin\\goodPrefix_as.txt", goodPrefix))
	{
		return false;
	}

	if(!ReadAffixConfig(".\\plugin\\goodSuffix_as.txt", goodSuffix))
	{
		return false;
	}

	MaxUnidentifiedSCharmLevel = GetPrivateProfileInt("Autostocker", "MaxUnidentifiedSCharmLvl", 0, CONFIG_FILE);
	MaxUnidentifiedLCharmLevel = GetPrivateProfileInt("Autostocker", "MaxUnidentifiedLCharmLvl", 0, CONFIG_FILE);
	MaxUnidentifiedGCharmLevel = GetPrivateProfileInt("Autostocker", "MaxUnidentifiedGCharmLvl", 0, CONFIG_FILE);
	IsTransmutingUnidentifiedSmallCharms = (GetPrivateProfileInt("Autostocker", "NonIDSmallCharms", 0, CONFIG_FILE) == 1);
	IsTransmutingUnidentifiedLargeCharms = (GetPrivateProfileInt("Autostocker", "NonIDLargeCharms", 0, CONFIG_FILE) == 1);
	IsTransmutingUnidentifiedGrandCharms = (GetPrivateProfileInt("Autostocker", "NonIDGrandCharms", 0, CONFIG_FILE) == 1);

	return true;
}

/// <summary>
/// Starts the autostocker with custom settings
/// </summary>
/// <param name="transmuteSet">Determines if set items should be cubed.</param>
/// <param name="transmuteRare">Determines if rare items should be cubed.</param>
/// <param name="transmuteUnique">Determines if unique items should be cubed.</param>
/// <param name="useChat">Determines if output messages should be sent as chat messages.</param>
/// <returns>true if successful, false if failed.</returns>
bool AutoStocker::StartRares(bool transmuteSet, bool transmuteRare, bool transmuteUnique, bool useChat)
{
	if(!Init(useChat))
	{
		return false;
	}

	this->transmuteSet = transmuteSet;
	this->transmuteRare = transmuteRare;
	this->transmuteUnique = transmuteUnique;

	return BeginAutostocking();
}

/// <summary>
/// Starts the autostocker with default settings (only normal/magic items)
/// </summary>
/// <param name="useChat">Determines if output messages should be sent as chat messages.</param>
/// <returns>true if successful, false if failed.</returns>
bool AutoStocker::Start(bool useChat)
{
	if(!Init(useChat))
	{
		return false;
	}

	this->transmuteSet = false;
	this->transmuteRare = false;
	this->transmuteUnique = false;

	return BeginAutostocking();
}

/// <summary>
/// Begins the autostocker process
/// </summary>
/// <returns>true if successful, false if failed.</returns>
bool AutoStocker::BeginAutostocking()
{
	std::vector<ITEM> itemsInInventory;

	// Clear previous settings
	itemsInInventory.clear();
	itemsToTransmute.clear();
	itemsToTransmute.resize(TRANSMUTE_END);
	restockers.clear();
	restockers.resize(TRANSMUTE_END, 0);
	restockerPositions.clear();
	restockerPositions.resize(TRANSMUTE_END);
	currentItem = -1;
	currentStocker = -1;

	// Notify the user about the current settings
	server->GameStringf("ÿc:Autostockerÿc0: Transmuting: ÿc3Magic%s%s%s", transmuteSet?" ÿc2Sets":"", transmuteRare?" ÿc9Rares":"", transmuteUnique?" ÿc4Uniques":"");

	// Open the player's cube
	if(!OpenCube())
	{
		Abort();
		return false;
	}

	// Make sure the cube is empty
	if(!IsCubeEmpty())
	{
		if(useChat)
		{
			me->Say("ÿc:Autostockerÿc0: Please empty your cube");
		}
		server->GameStringf("ÿc:Autostockerÿc0: Please empty your cube");

		Abort();
		return false;
	}

	// Read all items from the player's inventory
	me->EnumStorageItems(STORAGE_INVENTORY, enumItemProc, (LPARAM)&itemsInInventory);

	// Find the stockers that we will be using, if no stockers exist then we quit
	if(!FindStockers(itemsInInventory))
	{
		return false;
	}

	// Find the items in the player's inventory that need to be cubed
	FindItemsToTransmute(itemsInInventory);

	// Begin processing the stockers and their items
	ProcessNextStocker();

	return true;
}

/// <summary>
/// Called at a constant rate by d2hackit's timer
/// </summary>
void AutoStocker::OnTick()
{
	switch(currentState)
	{
		case STATE_COMPLETE:
		{
			currentState = STATE_UNINITIALIZED;

			if(useChat)
			{
				me->Say("ÿc:Autostockerÿc0: Autostocker Ended");
			}

			server->GameStringf("ÿc:Autostockerÿc0: Autostocker Ended");

			break;
		}
	}
}

/// <summary>
/// Called whenever an item is picked up to the cursor from the player's inventory
/// </summary>
/// <param name="item">Item that was picked up.</param>
void AutoStocker::OnItemFromInventory(const ITEM &item)
{
	// Only care about this event when we're picking up the stocker or item to be cubed
	if(currentState != STATE_PICKUPSTOCKER && currentState != STATE_PICKUPITEM)
	{
		return;
	}

	// User might of picked up an item while autostocker was running, abort before we attempt
	// to pickup another item and have the server kick us
	if(item.dwItemID != itemWaitingOn)
	{
		server->GameStringf("I'm not waiting on this item!");
		Abort();
		return;
	}

	// Make sure cube UI is still open
	if(!CheckCubeUI())
	{
		return;
	}

	if(currentState == STATE_PICKUPSTOCKER)
	{
		// We're going to move the stocker item from cursor to cube
		currentState = STATE_STOCKERTOCUBE;
	}
	else if(currentState == STATE_PICKUPITEM)
	{
		// We're going to move the non-stocker item to the cube
		currentState = STATE_ITEMTOCUBE;
	}

	if(!me->DropItemToStorage(STORAGE_CUBE, item.dwItemID))
	{
		server->GameStringf("DropItemToStorage failed");
	}
}

/// <summary>
/// Called whenever an item is pickedup to the cursor from the cube
/// </summary>
/// <param name="item">Item picked up from the cube.</param>
void AutoStocker::OnItemFromCube(const ITEM &item)
{
	// We only care about this event when we're picking up the stocker from the cube
	if(currentState != STATE_STOCKERFROMCUBE)
	{
		return;
	}

	// Make sure the item picked up from the cube is the stocker, otherwise there's a problem
	if(item.dwItemID == restockers[currentStocker])
	{
		currentState = STATE_STOCKERTOINVENTORY;

		if(!me->DropItemToStorageEx(STORAGE_INVENTORY, restockerPositions[currentStocker], item.dwItemID))
		{
			if(useChat)
			{
				me->Say("ÿc:Autostockerÿc0: Failed drop stocker back to inventory");
			}
			server->GameStringf("ÿc:Autostockerÿc0: Failed drop stocker back to inventory");

			Abort();
		}
	}
	else
	{
		// User might of picked up item from the cube while process was running, if so we need to abort
		//  to avoid being kicked by the server
		if(useChat)
		{
			me->Say("ÿc:Autostockerÿc0: Unknown item picked up");
		}
		server->GameStringf("ÿc:Autostockerÿc0: Unknown item picked up: [%X] %s", item.dwItemID, item.szItemCode);

		Abort();
		return;
	}
}

/// <summary>
/// Called whenever an item is moved to the player's inventory
/// </summary>
/// <param name="item">The item that was moved to the player's inventory.</param>
void AutoStocker::OnItemToInventory(const ITEM &item)
{
	// We only care about this event when we're moving our stocker back to the player's inventory
	if(currentState != STATE_STOCKERTOINVENTORY)
	{
		return;
	}

	// Make sure the item that was moved was the stocker, otherwise ignore it
	if(item.dwItemID != restockers[currentStocker])
	{
		server->GameStringf("ÿc:Autostockerÿc0: Item to inventory not stocker");
		return;
	}

	// Process the next stocker now that the cube is empty
	ProcessNextStocker();
}

/// <summary>
/// Called whenever an item is moved to the cube
/// </summary>
/// <param name="item">The item that was moved to the cube.</param>
void AutoStocker::OnItemToCube(const ITEM &item)
{
	if(currentState == STATE_STOCKERTOCUBE)
	{
		// Stocker was added to the cube, start processing items belonging to this stocker
		ProcessNextItem();
	}
	else if(currentState == STATE_ITEMTOCUBE)
	{
		currentState = STATE_TRANSMUTE;

		// Make sure user still has the cube UI open
		if(!CheckCubeUI())
		{
			return;
		}

		// Stocker and item to be stocked are in cube, transmute them and wait for next
		//  OnItemToCube event (see below)
		me->Transmute();
	}
	else if(currentState == STATE_TRANSMUTE)
	{
		// Only the stocker should be returned from the transmute process
		if(!GetStockerTypeByCode(item.szItemCode, NULL))
		{
			// Plugin and game are not always in sync, game might not know this item exists yet
			server->GameStringf("ÿc:Autostockerÿc0: Warning! Tried to assign non stocker to stocker");
		}

		// Every time a stocker is transmuted it gets a new itemId, updated our collection of stockers
		//  with the new id
		restockers[currentStocker] = item.dwItemID;

		// Process the next item for this stocker
		ProcessNextItem();
	}
}

/// <summary>
/// Starts proecessing the next stocker that have items needing to be cubed
/// </summary>
void AutoStocker::ProcessNextStocker()
{
	POINT location = {0, 0};
	currentStocker++;

	// Open the player's cube
	if(!OpenCube())
	{
		Abort();
		return;
	}

	// Find the next stocker that has items to transmute
	while(currentStocker < restockers.size() && (restockers[currentStocker] == 0 || itemsToTransmute[currentStocker].size() == 0))
	{
		++currentStocker;
	}

	// All stockers have been processed, autostocker complete
	if(currentStocker >= restockers.size())
	{
		Abort();
		return;
	}

	// Remember position of stocker so we can put it back when we're done with it
	if(!me->FindItemPosition(STORAGE_INVENTORY, restockers[currentStocker], &location))
	{
		server->GameStringf("ÿc:Autostockerÿc0: Failed to find position of next stocker");
	}

	currentItem = -1;
	restockerPositions[currentStocker] = location;
	itemWaitingOn = restockers[currentStocker];

	currentState = STATE_PICKUPSTOCKER;
	if(!me->PickStorageItemToCursor(restockers[currentStocker]))
	{
		if(useChat)
		{
			me->Say("ÿc:Autostockerÿc0: Failed to pick up new stocker");
		}

		server->GameStringf("ÿc:Autostockerÿc0: Failed to pick up new stocker");

		Abort();
		return;
	}
}

/// <summary>
/// Processes the next item to be cubed
/// </summary>
void AutoStocker::ProcessNextItem()
{
	if(!CheckCubeUI())
	{
		return;
	}

	// CurrentItem starts at -1 so the first time this is called 'currentITem' will be incremented to the first element
	currentItem++;

	if(currentItem < itemsToTransmute[currentStocker].size())
	{
		// Remember the item we're picking up for curecking during the OnItemFromInventory event
		itemWaitingOn = itemsToTransmute[currentStocker][currentItem];
		currentState = STATE_PICKUPITEM;

		if(!me->PickStorageItemToCursor(itemsToTransmute[currentStocker][currentItem]))
		{
			server->GameStringf("ÿc:Autostockerÿc0: Failed to pickup item %d/%d", currentItem+1, itemsToTransmute[currentStocker].size());
			ProcessNextItem();
			return;
		}
	}
	else
	{
		// No more items to transmute for current stocker, remove stocker from cube
		currentState = STATE_STOCKERFROMCUBE;

		if(!me->PickStorageItemToCursor(restockers[currentStocker]))
		{
			if(useChat)
			{
				me->Say("ÿc:Autostockerÿc0: Failed to pickup stocker item");
			}

			server->GameStringf("ÿc:Autostockerÿc0: Failed to pickup stocker item %d", currentStocker);

			Abort();
			return;
		}		
	}
}

/// <summary>
/// Attempts to open the player's cube
/// </summary>
/// <returns>true if successful, false if failed.</returns>
bool AutoStocker::OpenCube()
{
	if(!me->OpenCube())
	{
		if(useChat)
		{
			me->Say("ÿc:Autostockerÿc0: You must open your cube before using");
		}

		server->GameStringf("ÿc:Autostockerÿc0: You must open your cube before using");

		Abort();
		return false;
	}

	return true;
}

/// <summary>
/// Determines if the cube is currently open and aborts if it's not open
/// </summary>
/// <returns>true if cube is open, false if cube is not open and process has been aborted.</returns>
bool AutoStocker::CheckCubeUI()
{
	if(!me->IsUIOpened(UI_CUBE))
	{
		server->GameStringf("ÿc:Autostockerÿc0: Cube UI closed, aborting");
		Abort();
	}

	return true;
}

/// <summary>
/// Aborts the autostocker process if it's currently running
/// </summary>
void AutoStocker::Abort()
{
	if(currentState != STATE_COMPLETE && currentState != STATE_UNINITIALIZED)
	{
		currentState = STATE_COMPLETE;
	}
}

/// <summary>
/// Determines if the cube is currently empty
/// </summary>
/// <returns>true if cube is empty, false if items are currently in the cube.</returns>
bool AutoStocker::IsCubeEmpty()
{
	int itemCount = 0;
	me->EnumStorageItems(STORAGE_CUBE, enumItemCountProc, (LPARAM)&itemCount);

	if(itemCount != 0)
	{
		return false;
	}

	return true;
}

/// <summary>
/// Goes through list of items in player's inventory and determines which ones should be stocked and the
//  type of stocker item should be cubed with
/// </summary>
/// <param name="itemsInInventory">List of items in player's inventory.</param>
void AutoStocker::FindItemsToTransmute(const std::vector<ITEM> &itemsInInventory)
{
	for(unsigned int i = 0; i < itemsInInventory.size(); ++i)
	{
		if(IsGemCanItem(itemsInInventory[i].szItemCode))
		{
			itemsToTransmute[TRANSMUTE_GEM].push_back(itemsInInventory[i].dwItemID);
		}
		else if(IsRuneStockerAItem(itemsInInventory[i].szItemCode))
		{
			itemsToTransmute[TRANSMUTE_RUNEA].push_back(itemsInInventory[i].dwItemID);
		}
		else if(IsRuneStockerBItem(itemsInInventory[i].szItemCode))
		{
			itemsToTransmute[TRANSMUTE_RUNEB].push_back(itemsInInventory[i].dwItemID);
		}
		else if(IsMultiStockerItem(itemsInInventory[i].szItemCode))
		{
			itemsToTransmute[TRANSMUTE_MULTISTOCKER].push_back(itemsInInventory[i].dwItemID);
		}
		else if(IsDecalStockerItem(itemsInInventory[i].szItemCode))
		{
			itemsToTransmute[TRANSMUTE_DECAL].push_back(itemsInInventory[i].dwItemID);
		}
		else if(IsTomeAItem(itemsInInventory[i].szItemCode))
		{
			itemsToTransmute[TRANSMUTE_TOMEA].push_back(itemsInInventory[i].dwItemID);
		}
		else if(IsTomeBItem(itemsInInventory[i].szItemCode))
		{
			itemsToTransmute[TRANSMUTE_TOMEB].push_back(itemsInInventory[i].dwItemID);
		}
		else if(IsCrystalItem(itemsInInventory[i].szItemCode))
		{
			itemsToTransmute[TRANSMUTE_CRYSTAL].push_back(itemsInInventory[i].dwItemID);
		}
		else if(IsRerollItem(itemsInInventory[i]))
		{
			itemsToTransmute[TRANSMUTE_REROLL].push_back(itemsInInventory[i].dwItemID);
		}
	}
}

/// <summary>
/// Determines the type of stocker the item with specified ID is
/// </summary>
/// <param name="itemId">ItemID of item being checked.</param>
/// <param name="stockerType">[out] Type of stocker this item is.</param>
/// <returns>true if item is a stocker, false if not a stocker.</returns>
bool AutoStocker::GetStockerTypeByID(DWORD itemId, int *stockerType)
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
bool AutoStocker::GetStockerTypeByCode(const char *itemCode, int *stockerType)
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


/// <summary>
/// Obtains a list of stockers found in the player's inventory
/// </summary>
/// <param name="itemsInInventory">Where to store list of found stockers.</param>
/// <returns>true...?</returns>
bool AutoStocker::FindStockers(const std::vector<ITEM> &itemsInInventory)
{
	int stockerType = 0;

	for(unsigned int i = 0; i < itemsInInventory.size(); ++i)
	{
		if(GetStockerTypeByCode(itemsInInventory[i].szItemCode, &stockerType))
		{
			restockers[stockerType] = itemsInInventory[i].dwItemID;
		}
	}

	return true;
}

/// <summary>
/// Reads the specified config file specifying what items are to be cubed with
/// </summary>
/// <param name="configPath">Path of config file.</param>
/// <returns>true if successful, false if failed.</returns>
bool AutoStocker::ReadConfig(std::string configPath)
{
	std::string readLineBuff;
	std::string currentSectionName;
	int currentStockerSet = 0;
	int i = 0;

	// Clear any previous config settings
	for(i = 0; i < TRANSMUTE_END; i++)
	{
		stockerItems[i].clear();
	}

	std::ifstream inFile(configPath.c_str());
	if(!inFile)
	{
		server->GameStringf("ÿc:AutoStockerÿc0: Failed to open config\n");
		return false;
	}

	while(inFile.good())
	{
		std::getline(inFile, readLineBuff);
		if(readLineBuff[0] == '[')
		{
			currentSectionName = readLineBuff.substr(1, readLineBuff.size()-2);

			for(i = 0; i < TRANSMUTE_END; i++)
			{
				if(currentSectionName.compare(stockerSetNames[i]) == 0)
				{
					currentStockerSet = i;
					break;
				}
			}
			if(i == TRANSMUTE_END)
			{
				server->GameStringf("ÿc:AutoStockerÿc0: Invalid section in config -> %s", currentSectionName);
				inFile.close();
				return false;
			}
		}
		else
		{
			if(readLineBuff.length() < 3)
			{
				continue;
			}

			stockerItems[currentStockerSet].insert(readLineBuff.substr(0, 3));
		}
	}

	inFile.close();

	return true;
}

/// <summary>
/// Reads list of affix ids from file into specified map
/// </summary>
/// <param name="configPath">Path of config file.</param>
/// <param name="readTo">Map to read good affix values into.</param>
/// <returns>true if successful, false if failed.</returns>
bool AutoStocker::ReadAffixConfig(std::string configPath, stdext::hash_set<int> &readTo)
{
	std::string readLineBuff;
	int readNum = 0;

	readTo.clear();

	std::ifstream inFile(configPath.c_str());
	if(!inFile)
	{
		server->GameStringf("ÿc:AutoStockerÿc0: Failed to open file %s\n", configPath.c_str());
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

/// <summary>
/// Determines if this item has a good affix and should be kept
/// </summary>
/// <param name="item">The item to check.</param>
/// <returns>true if the item has good affix and should be kept, false if it's junk.</returns>
bool AutoStocker::CheckItemAffix(const ITEM &item)
{
	int goodPrefixCount = 0;
	int goodSuffixCount = 0;

	if(item.iQuality == ITEM_QUALITY_RARE || item.iQuality == ITEM_QUALITY_MAGIC || item.iQuality == ITEM_QUALITY_CRAFT)
	{
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
	}

	return goodPrefixCount >= 1 || goodSuffixCount >= 1;
}

/// <summary>
/// Determines if specified item should be stored in the gem can
/// </summary>
/// <param name="item">Item being checked.</param>
/// <returns>true if item should be cubed a stocker.</returns>
bool AutoStocker::IsGemCanItem(LPCSTR itemCode)
{
	return stockerItems[TRANSMUTE_GEM].count(itemCode) > 0;
}

/// <summary>
/// Determines if specified item should be stored in the multi stocker
/// </summary>
/// <param name="item">Item being checked.</param>
/// <returns>true if item should be cubed a stocker.</returns>
bool AutoStocker::IsMultiStockerItem(LPCSTR itemCode)
{
	return stockerItems[TRANSMUTE_MULTISTOCKER].count(itemCode) > 0;
}

/// <summary>
/// Determines if specified item should be stored in the low rune stocker
/// </summary>
/// <param name="item">Item being checked.</param>
/// <returns>true if item should be cubed a stocker.</returns>
bool AutoStocker::IsRuneStockerAItem(LPCSTR itemCode)
{
	return stockerItems[TRANSMUTE_RUNEA].count(itemCode) > 0;
}

/// <summary>
/// Determines if specified item should be stored in the high rune stocker
/// </summary>
/// <param name="item">Item being checked.</param>
/// <returns>true if item should be cubed a stocker.</returns>
bool AutoStocker::IsRuneStockerBItem(LPCSTR itemCode)
{
	return stockerItems[TRANSMUTE_RUNEB].count(itemCode) > 0;

}

/// <summary>
/// Determines if specified item should be stored in the decal stocker
/// </summary>
/// <param name="item">Item being checked.</param>
/// <returns>true if item should be cubed a stocker.</returns>
bool AutoStocker::IsDecalStockerItem(LPCSTR itemCode)
{
	return stockerItems[TRANSMUTE_DECAL].count(itemCode) > 0;
}

/// <summary>
/// Determines if specified item should be stored in the low level ancient tome stocker
/// </summary>
/// <param name="item">Item being checked.</param>
/// <returns>true if item should be cubed a stocker.</returns>
bool AutoStocker::IsTomeAItem(LPCSTR itemCode)
{
	return stockerItems[TRANSMUTE_TOMEA].count(itemCode) > 0;
}

/// <summary>
/// Determines if specified item should be stored in the high level ancient tome stocker
/// </summary>
/// <param name="item">Item being checked.</param>
/// <returns>true if item should be cubed a stocker.</returns>
bool AutoStocker::IsTomeBItem(LPCSTR itemCode)
{
	return stockerItems[TRANSMUTE_TOMEB].count(itemCode) > 0;
}

/// <summary>
/// Determines if specified item should be stored in the crystal can
/// </summary>
/// <param name="item">Item being checked.</param>
/// <returns>true if item should be cubed with a stocker.</returns>
bool AutoStocker::IsCrystalItem(LPCSTR itemCode)
{
	return stockerItems[TRANSMUTE_CRYSTAL].count(itemCode) > 0;
}

/// <summary>
/// Determines if specified item should be stored in the rerolling orb
/// </summary>
/// <param name="item">Item being checked.</param>
/// <returns>true if item should be cubed a stocker.</returns>
bool AutoStocker::IsRerollItem(const ITEM &item)
{
	if(!item.iIdentified)
	{
		// Transmute unidentified magic charms based on settings from autostockerMisc.ini
		if(item.iQuality == ITEM_LEVEL_MAGIC)
		{
			if(IsTransmutingUnidentifiedSmallCharms &&
				item.iLevel <= MaxUnidentifiedSCharmLevel && 
				strcmp(item.szItemCode, "cx1") == 0)
			{
				return true;
			}
			else if(IsTransmutingUnidentifiedLargeCharms &&
				item.iLevel <= MaxUnidentifiedLCharmLevel && 
				strcmp(item.szItemCode, "cx2") == 0)
			{
				return true;
			}
			else if(IsTransmutingUnidentifiedGrandCharms &&
				item.iLevel <= MaxUnidentifiedGCharmLevel && 
				strcmp(item.szItemCode, "cx3") == 0)
			{
				return true;
			}
		}

		// Don't transmute any other unidentified charms or jewels
		if(strcmp(item.szItemCode, "cx1") == 0 ||
			strcmp(item.szItemCode, "cx2") == 0 ||
			strcmp(item.szItemCode, "cx3") == 0 ||
			strcmp(item.szItemCode, "jew") == 0)
		{
			return false;
		}
	}

	if(item.iQuality == ITEM_LEVEL_MAGIC ||
		(item.iQuality == ITEM_LEVEL_SET && transmuteSet) ||
		(item.iQuality == ITEM_LEVEL_RARE && transmuteRare) ||
		(item.iQuality == ITEM_LEVEL_UNIQUE && transmuteUnique))
	{
		if(!CheckItemAffix(item))
		{
			return stockerItems[TRANSMUTE_REROLL].count(item.szItemCode) > 0;
		}
	}

	return false;
}

/// <summary>
/// Stores a list of all the items found in the players inventory
/// </summary>
/// <param name="item">Item found in the player's inventory.</param>
/// <param name="lParam">Pointer to list of items found so far.</param>
/// <returns>true if it succeeds, false if it fails.</returns>
BOOL CALLBACK enumItemProc(LPCITEM item, LPARAM lParam)
{
	((std::vector<ITEM>*)lParam)->push_back(*item);

	return TRUE;
}

/// <summary>
/// Counts the number of items in the cube, while displaying the item description.
/// </summary>
/// <param name="item">Item found in the cube.</param>
/// <param name="lParam">Pointer to item count.</param>
/// <returns>true if it succeeds, false if it fails.</returns>
BOOL CALLBACK enumItemCountProc(LPCITEM item, LPARAM lParam)
{
	(*(int *)lParam)++;

	server->GameStringf("Cube item: %s", server->GetItemName(item->szItemCode));
	return TRUE;
}