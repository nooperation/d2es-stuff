#include "../../Includes/D2Client.h"
#include "AutoReroll.h"
#include <fstream>
#include <string>

#include "../../Includes/itemPrefix.h"
#include "../../Includes/itemSuffix.h"

std::unordered_set<std::string> gemItemCodes;
std::unordered_set<std::string> gemCanItemCodes;
std::unordered_set<std::string> canOpenerItemCodes;

AutoReroll::AutoReroll()
{
	currentState = STATE_UNINITIALIZED;

	gemItemCodes.clear();
	gemItemCodes.insert("gcv"); // �c;Chipped Amethyst
	gemItemCodes.insert("gfv"); // �c;Flawed Amethyst
	gemItemCodes.insert("gsv"); // �c;Amethyst
	gemItemCodes.insert("gzv"); // �c;Flawless Amethyst
	gemItemCodes.insert("gpv"); // �c;Blemished Amethyst
	gemItemCodes.insert("gcy"); // �c9Chipped Topaz
	gemItemCodes.insert("gfy"); // �c9Flawed Topaz
	gemItemCodes.insert("gsy"); // �c9Topaz
	gemItemCodes.insert("gly"); // �c9Flawless Topaz
	gemItemCodes.insert("gpy"); // �c9Blemished Topaz
	gemItemCodes.insert("gcb"); // �c3Chipped Sapphire
	gemItemCodes.insert("gfb"); // �c3Flawed Sapphire
	gemItemCodes.insert("gsb"); // �c3Sapphire
	gemItemCodes.insert("glb"); // �c3Flawless Sapphire
	gemItemCodes.insert("gpb"); // �c3Blemished Sapphire
	gemItemCodes.insert("gcg"); // �c2Chipped Emerald
	gemItemCodes.insert("gfg"); // �c2Flawed Emerald
	gemItemCodes.insert("gsg"); // �c2Emerald
	gemItemCodes.insert("glg"); // �c2Flawless Emerald
	gemItemCodes.insert("gpg"); // �c2Blemished Emerald
	gemItemCodes.insert("gcr"); // �c1Chipped Ruby
	gemItemCodes.insert("gfr"); // �c1Flawed Ruby
	gemItemCodes.insert("gsr"); // �c1Ruby
	gemItemCodes.insert("glr"); // �c1Flawless Ruby
	gemItemCodes.insert("gpr"); // �c1Blemished Ruby
	gemItemCodes.insert("gcw"); // �c/Chipped Diamond
	gemItemCodes.insert("gfw"); // �c/Flawed Diamond
	gemItemCodes.insert("gsw"); // �c/Diamond
	gemItemCodes.insert("glw"); // �c/Flawless Diamond
	gemItemCodes.insert("gpw"); // �c/Blemished Diamond
	gemItemCodes.insert("skc"); // �c0Chipped Skull
	gemItemCodes.insert("skf"); // �c0Flawed Skull
	gemItemCodes.insert("sku"); // �c0Skull
	gemItemCodes.insert("skl"); // �c0Flawless Skull
	gemItemCodes.insert("skz"); // �c0Blemished Skull
	gemItemCodes.insert("gvb"); // �c;Perfect Amethyst
	gemItemCodes.insert("gyb"); // �c9Perfect Topaz
	gemItemCodes.insert("gbb"); // �c3Perfect Sapphire
	gemItemCodes.insert("ggb"); // �c2Perfect Emerald
	gemItemCodes.insert("grb"); // �c1Perfect Ruby
	gemItemCodes.insert("gwb"); // �c/Perfect Diamond
	gemItemCodes.insert("skb"); // �c0Perfect Skull
	gemItemCodes.insert("gck"); // �c5Chipped Obsidian
	gemItemCodes.insert("gfk"); // �c5Flawed Obsidian
	gemItemCodes.insert("gsk"); // �c5Obsidian
	gemItemCodes.insert("gzk"); // �c5Flawless Obsidian
	gemItemCodes.insert("gpk"); // �c5Blemished Obsidian
	gemItemCodes.insert("gbk"); // �c5Perfect Obsidian

	gemCanItemCodes.clear();
	gemCanItemCodes.insert("kv0"); // Gem Can
	gemCanItemCodes.insert("ky0"); // Gem Can
	gemCanItemCodes.insert("kb0"); // Gem Can
	gemCanItemCodes.insert("kg0"); // Gem Can
	gemCanItemCodes.insert("kr0"); // Gem Can
	gemCanItemCodes.insert("kw0"); // Gem Can
	gemCanItemCodes.insert("ks0"); // Gem Can
	gemCanItemCodes.insert("kk0"); // Gem Can

	canOpenerItemCodes.clear();
	canOpenerItemCodes.insert("ko0"); // Opener
	canOpenerItemCodes.insert("ko1"); // Opener
	canOpenerItemCodes.insert("ko2"); // Opener
	canOpenerItemCodes.insert("ko3"); // Opener
	canOpenerItemCodes.insert("ko4"); // Opener
	canOpenerItemCodes.insert("ko5"); // Opener
}

/// <summary>
/// Initalizes AutoReroll
/// </summary>
/// <param name="useChat">Determines if output messages should be sent as chat messages.</param>
/// <returns>true if successful, false if failed.</returns>
bool AutoReroll::Init(bool useChat)
{
	this->useChat = useChat;

	if(!ReadAffixConfig(".\\plugin\\goodPrefix_ar.txt", goodPrefix))
	{
		return false;
	}

	if(!ReadAffixConfig(".\\plugin\\goodSuffix_ar.txt", goodSuffix))
	{
		return false;
	}

	minPrefix = GetPrivateProfileInt("AutoReroll", "PrefixCount", 2, CONFIG_PATH);
	minSuffix = GetPrivateProfileInt("AutoReroll", "SuffixCount", 0, CONFIG_PATH);
	numGemsToUse = GetPrivateProfileInt("AutoReroll", "GemCount", 2, CONFIG_PATH);

	server->GameStringf("Min prefix: %d Min suffix: %d", minPrefix, minSuffix);

	itemToRerollID = 0;
	rerollItemNeedsToGoBackToCube = false;
	loadedEmptyCube = false;

	return true;
}

/// <summary>
/// Reads list of affix ids from file into specified map
/// </summary>
/// <param name="configPath">Path of config file.</param>
/// <param name="readTo">Map to read good affix values into.</param>
/// <returns>true if successful, false if failed.</returns>
bool AutoReroll::ReadAffixConfig(const std::string &configPath, std::unordered_set<int> &readTo)
{
	std::string readLineBuff;
	int readNum = 0;

	readTo.clear();

	std::ifstream inFile(configPath.c_str());
	if(!inFile)
	{
		server->GameStringf("�c:AutoReroll�c0: Failed to open file %s\n", configPath.c_str());
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
/// Starts AutoReroll
/// </summary>
/// <param name="numGems">Number of gems to cube item with.</param>
/// <param name="useChat">Determines if output messages should be sent as chat messages.</param>
/// <returns>true if successful, false if failed.</returns>
bool AutoReroll::Start(int numGems, bool useChat)
{
	if(!Init(useChat))
	{
		return false;
	}

	if(numGems > 0)
	{
		server->GameStringf("�c:AutoReroll�c0: Using %d gems", numGems);
		numGemsToUse = numGems;
	}

	server->GameCommandLine("load ae");
	server->GameCommandLine("load emptycube");

	return StartRerollingItemInCube();
}

/// <summary>
/// Starts the rerolling process. The cube must only contain the item to be rerolled.
/// </summary>
/// <returns>True on success, false on failure</returns>
bool AutoReroll::StartRerollingItemInCube()
{
	// Make sure the user has the cube UI open
	if(!me->OpenCube())
	{
		if(useChat)
		{
			me->Say("�c:AutoReroll�c0: Cube not opened");
		}
		server->GamePrintString("�c:AutoReroll�c0: Cube not opened");
		
		Abort();
		return false;
	}

	// Make sure that there's only one item in the cube
	int itemCount = 0;
	me->EnumStorageItems(STORAGE_CUBE, enumItemCountProc, (LPARAM)&itemCount);
	if(itemCount != 1)
	{
		if(useChat)
		{
			me->Say("�c:AutoReroll�c0: Place only one rare weapon/armor to reroll in cube before starting");
		}
		
		server->GamePrintString("�c:AutoReroll�c0: Place only one rare weapon/armor to reroll in cube before starting");
		
		Abort();
		return false;
	}

	// Get the item ID of the only item in the player's cube, this is the item we will reroll
	me->EnumStorageItems(STORAGE_CUBE, enumFindItemToReroll, (LPARAM)&itemToRerollID);

	// Get a list of item IDs for the gems in the player's inventory
	currentGemIndex = 0;
	gemsInInventory.clear();
	me->EnumStorageItems(STORAGE_INVENTORY, enumFindGems, (LPARAM)&gemsInInventory);

	// We don't have enough gems to use, need to get more from the gemcan
	if((int)gemsInInventory.size() < numGemsToUse)
	{
		ExtractMoreGems();
		return true;
	}

	MoveNextGemToCube();

	return true;
}

// Empty cube -> 
void AutoReroll::ExtractMoreGems()
{
	// We're out of gems so we need to move the item to be rerolled to the player's inventory
	//   then start extracting more gems
	currentState = STATE_RUNNINGEMPTYCUBE;
	server->GameCommandLine("emptycube start chat");
	loadedEmptyCube = true;
	return;
}

/// <summary>
/// Picks up the next extractor (e.g: gem) and moves it to the cube
/// </summary>
void AutoReroll::MoveNextGemToCube()
{
	// I've never had the currentGemIndex index go out of bounds, but just checking just
	//   to make sure it doesn't
	if(currentGemIndex >= (int)gemsInInventory.size())
	{
		if(useChat)
		{
			me->Say("�c:AutoReroll�c0: currentGemIndex > gemsInInventory.size()");
		}
		server->GamePrintString("�c:AutoReroll�c0: currentGemIndex > gemsInInventory.size()");

		Abort();
		return;
	}

	// Pickup the current extractor (e.g: gem) and begin moving it to the cube
	currentState = STATE_PICKUPGEM;
	if(!me->PickStorageItemToCursor(gemsInInventory[currentGemIndex]))
	{
		if(useChat)
		{
			me->Say("�c:AutoReroll�c0: Failed to pickup gem");
		}

		server->GamePrintString("�c:AutoReroll�c0: Failed to pickup gem");
		
		Abort();
		return;
	}
}

/// <summary>
/// Starts moving the gemcan and can opener to the cube
/// </summary>
void AutoReroll::MoveGemCanAndOpenerToCube()
{
	DWORD itemToPickup = 0;

	if(!gemCanAndOpener.GemCanMoved)
	{
		// Gem can hasn't been moved yet, we will move it now
		gemCanAndOpener.GemCanMoved = true;
		itemToPickup = gemCanAndOpener.GemCanID;
	}
	else if(!gemCanAndOpener.CanOpenerMoved)
	{
		// Gem can is already in cube, but we haven't moved the can opener, we will move it now
		gemCanAndOpener.CanOpenerMoved = true;
		itemToPickup = gemCanAndOpener.CanOpenerID;
	}

	// We already moved both the gemcan and canopener, this shouldn't happen
	// TODO: Check if this actually happens
	if(itemToPickup == 0)
	{
		return;
	}

	// Pickup the gemcan or canopenr so it can be moved to the cube
	currentState = STATE_PICKUPGEMCANSTUFF;
	if(!me->PickStorageItemToCursor(itemToPickup))
	{
		if(useChat)
		{
			me->Say("�c:AutoReroll�c0: Failed drop pickup can opener");
		}
		server->GamePrintString("�c:AutoReroll�c0: Failed drop pickup  can opener");

		Abort();
		return;
	}
}

/// <summary>
/// Checks specified item to see if it can be considered good based on its affixes. If the item
///   is good, then the AutoReroll process must stop.
/// </summary>
/// <param name="item">Item to check.</param>
/// <returns>true if item is good and process needs to stop, false if it's junk.</returns>
bool AutoReroll::CheckRerolledItem(const ITEM &item)
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
				server->GameStringf("�c;Prefix�c0 %s: %s", Prefix[item.wPrefix[i]], PrefixDetails[item.wPrefix[i]]);
			}
			if(goodSuffix.count(item.wSuffix[i]) > 0)
			{
				goodSuffixCount++;
				server->GameStringf("�c:Suffix�c0 %s: %s", Suffix[item.wSuffix[i]], SuffixDetails[item.wSuffix[i]]);
			}
		}
	}

	return goodPrefixCount >= minPrefix && goodSuffixCount >= minSuffix;
}

/// <summary>
/// Aborts the AutoReroll process if it's currently running
/// </summary>
void AutoReroll::Abort()
{
	if(currentState != STATE_COMPLETE && currentState != STATE_UNINITIALIZED)
	{
		currentState = STATE_COMPLETE;
	}
}

/// <summary>
/// Called at a constant rate by d2hackit's timer
/// </summary>
void AutoReroll::OnTick()
{
	switch(currentState)
	{
		case STATE_COMPLETE:
		{
			currentState = STATE_UNINITIALIZED;

			if(useChat)
			{
				me->Say("�c:AutoReroll�c0: AutoReroll Ended");
			}
			server->GamePrintString("�c:AutoReroll�c0: AutoReroll Ended");

			break;
		}
	}
}

/// <summary>
/// Called whenever an item is picked up to the cursor from the player's inventory
/// </summary>
/// <param name="item">Item that was picked up.</param>
void AutoReroll::OnItemFromInventory(DWORD itemID)
{
	switch(currentState)
	{
		case STATE_PICKUPGEMCANSTUFF:
		{
			// We picked up the gem can or can opener
			currentState = STATE_GEMCANSTUFFTOCUBE;
			break;
		}
		case STATE_PICKUPGEM:
		{
			// We picked up a gem
			currentState = STATE_GEMTOCUBE;
			break;
		}
		case STATE_PICKUPITEMTOREROLLED:
		{
			// We picked up the item to be rerolled
			currentState = STATE_ITEMTOREROLLTOCUBE;
			break;
		}
		default:
		{
			// Don't care about any other items being picked up from the inventory in our current
			//  state
			return;
		}
	}

	// Drop the item we picked up to the cube
	if(!me->DropItemToStorage(STORAGE_CUBE, itemID))
	{
		if(useChat)
		{
			me->Say("�c:AutoReroll�c0: Failed drop inventory item to cube");
		}
		server->GameStringf("�c:AutoReroll�c0: Failed drop inventory item to cube");

		Abort();
		return;
	}
}

/// <summary>
/// Called whenever an item is placed in the cube
/// </summary>
/// <param name="item">Item placed in the cube.</param>
void AutoReroll::OnItemToCube(const ITEM &item)
{
	switch(currentState)
	{
		case STATE_TRANSMUTE:
		{
			// Only the item to be rerolled will be sent to the cube after being cubed with the gems
			//  we need to remember the ID of this item for future use
			itemToRerollID = item.dwItemID;

			// Check to see if this is a good item. If it's not, then just restart the rerolling process again
			if(!CheckRerolledItem(item))
			{
				StartRerollingItemInCube();
				return;
			}

			if(useChat)
			{
				me->Say("�c:AutoReroll�c0: Rolled good item");
			}
			server->GamePrintString("�c:AutoReroll�c0: Rolled good item");

			Abort();
			return;
		}
		case STATE_GEMTOCUBE:
		{
			// Item moved to the cube was a gem, If we have enough gems to reroll the item
			//   then it's time to transmute. Otherwise we need to add the next extractor (e.g: next gem)
			currentGemIndex++;
			if(currentGemIndex >= numGemsToUse)
			{
				currentState = STATE_TRANSMUTE;
				me->Transmute();
			}
			else
			{
				MoveNextGemToCube();
			}
			break;
		}
		case STATE_ITEMTOREROLLTOCUBE: // The item we're rerolling
		{
			// We just moved the item we're rerolling to the cube. Start adding the required gems
			//MoveNextGemToCube();
			StartRerollingItemInCube();
			break;
		}
		case STATE_GEMCANSTUFFTOCUBE: // Gem can and can opener
		{
			// Item placed in the cube was the gem can or can opener. If both of them have been
			//   moved to the cube then we can start the autoextractor process to get the gems.
			//   Otherwise we need to move the other gemcan or can opener
			if(gemCanAndOpener.CanOpenerMoved && gemCanAndOpener.GemCanMoved)
			{
				currentState = STATE_RUNNINGAUTOEXTRACTOR;
				server->GameCommandLine("ae start chat");
			}
			else
			{
				MoveGemCanAndOpenerToCube();
			}
			break;
		}
	}
}

/// <summary>
/// Called whenever AutoExtractor has finished
/// </summary>
/// <returns>true if we processed this event, false if we ignored it.</returns>
bool AutoReroll::OnAutoExtractorMessage(const std::string_view &message)
{
	// Make sure we actually started the AutoExtractor
	if(currentState != STATE_RUNNINGAUTOEXTRACTOR)
	{
		return false;
	}

	if (message == "AutoExtractor Ended")
	{
		// Our inventory should be full of gems now or our gem can is empty and a few gems are in
		//  our inventory. Get Get a list of item IDs for the gems in the player's inventory just
		//  to make sure we haven't run out of gems
		std::vector<DWORD> tempGemsInInventory;
		me->EnumStorageItems(STORAGE_INVENTORY, enumFindGems, (LPARAM)&tempGemsInInventory);
		if ((int)tempGemsInInventory.size() < numGemsToUse)
		{
			if (useChat)
			{
				me->Say("�c:AutoReroll�c0: No more gems");
			}
			server->GamePrintString("�c:AutoReroll�c0: No more gems");

			Abort();
			return true;
		}

		// Gem can and can opener are still in the cube, we need to empty the cube now
		rerollItemNeedsToGoBackToCube = true;
		currentState = STATE_RUNNINGEMPTYCUBE;
		server->GameCommandLine("emptycube start chat");
	}

	return true;
}

/// <summary>
/// Called whenever EmptyCube has finished
/// </summary>
/// <returns>true if we processed this event, false if we ignored it.</returns>
bool AutoReroll::OnEmptyCubeMessage(const std::string_view &message)
{
	// Make sure we actually started the EmptyCube process
	if(currentState != STATE_RUNNINGEMPTYCUBE)
	{
		return false;
	}

	if (message != "EmptyCube Ended")
	{
		return true;
	}

	// Make sure the cube is actually empty
	int itemCount = 0;
	me->EnumStorageItems(STORAGE_CUBE, enumItemCountProc, (LPARAM)&itemCount);
	if(itemCount != 0)
	{
		server->GameStringf("�c:AutoReroll�c0: Unable to clean cube");
		Abort();
		return true;
	}

	// We just finished running autoextractor, cube is empty and gems are in our inventory.
	//  Now we need to move the item to be rerolled back into the cube so the whole rerolling
	//  process can restart
	if(rerollItemNeedsToGoBackToCube)
	{
		rerollItemNeedsToGoBackToCube = false;
		currentState = STATE_PICKUPITEMTOREROLLED;
		if(!me->PickStorageItemToCursor(itemToRerollID))
		{
			if(useChat)
			{
				me->Say("�c:AutoReroll�c0: Unable to pickup previously extracted item from inventory");
			}
			server->GamePrintString("�c:AutoReroll�c0: Unable to pickup previously extracted item from inventory");

			Abort();
			return false;
		}

		return true;
	}

	// We just ran out of gems and moved the item to be rerolled back into our inventory. Now
	//   we need to put the gemcan and can opener into the cube so we can extract more gems.
	//   Find the IDs of the can opener and gem can (they change every time it's transmuted)
	memset(&gemCanAndOpener, 0, sizeof(GemCanStuff));
	me->EnumStorageItems(STORAGE_INVENTORY, enumFindGemCanStuff, (LPARAM)&gemCanAndOpener);
	if(gemCanAndOpener.CanOpenerID == 0 || gemCanAndOpener.GemCanID == 0)
	{
		if(useChat)
		{
			me->Say("�c:AutoReroll�c0: Unable to find gem can or gem can opener");
		}
		server->GameStringf("�c:AutoReroll�c0: Unable to find %s%s", (gemCanAndOpener.CanOpenerID==0)?" Can Opener":"", (gemCanAndOpener.GemCanID==0)?" Gem Gan":"");

		Abort();
		return true;
	}

	// Move the gem can or can opener to cube. Both gem can and can opener will be moved to the
	//  cube during MoveGemCanAndOpenerToCube. After they're both in there autoextractor will begin
	MoveGemCanAndOpenerToCube();

	return true;
}

/// <summary>
/// Counts the number of items in the cube.
/// </summary>
/// <param name="item">Item found in the cube.</param>
/// <param name="lParam">Pointer to item count.</param>
/// <returns>true.</returns>
BOOL CALLBACK enumItemCountProc(LPCITEM item, LPARAM lParam)
{
	(*(int *)lParam)++;
	return TRUE;
}

/// <summary>
/// Looks in the cube for the item that will be rerolled. Cube should only contain that one item.
/// </summary>
/// <param name="item">Item found in the cube.</param>
/// <param name="lParam">Pointer to item id.</param>
/// <returns>true.</returns>
BOOL CALLBACK enumFindItemToReroll(LPCITEM item, LPARAM lParam)
{
	(*(DWORD *)lParam) = item->dwItemID;
	return TRUE;
}

/// <summary>
/// Finds our gem can and can opener, stores the IDs of these items in the GemCanStuff structure (lParam)
/// </summary>
/// <param name="item">Item found in the cube.</param>
/// <param name="lParam">Pointer to GemCanStuff structure to store item IDs.</param>
/// <returns>true.</returns>
BOOL CALLBACK enumFindGemCanStuff(LPCITEM item, LPARAM lParam)
{
	GemCanStuff *canStuff = (GemCanStuff *)lParam;

	// Store ID of this item as our can opener if it's a can opener
	if(canOpenerItemCodes.count(item->szItemCode) > 0)
	{
		canStuff->GemCanID = item->dwItemID;
		return canStuff->CanOpenerID == 0;
	}

	// Store ID of this item as our gem can if it's a gem can
	if(gemCanItemCodes.count(item->szItemCode) > 0)
	{
		canStuff->CanOpenerID = item->dwItemID;
		return canStuff->GemCanID == 0;
	}
	return TRUE;
}

/// <summary>
/// Searches the player's inventory for gems and stores their ID's in a list (lParam)
/// </summary>
/// <param name="item">Item found in the cube.</param>
/// <param name="lParam">Pointer to list of item IDs.</param>
/// <returns>true.</returns>
BOOL CALLBACK enumFindGems(LPCITEM item, LPARAM lParam)
{
	if(gemItemCodes.count(item->szItemCode) > 0)
	{
		((std::vector<DWORD>*)lParam)->push_back(item->dwItemID);
	}

	return TRUE;
}