#include "../../d2hackit/includes/D2Client.h"
#include "AutoExtractor.h"
#include <string>
#include <fstream>

#include "../../d2hackit/includes/itemPrefix.h"
#include "../../d2hackit/includes/itemSuffix.h"

AutoExtractor::AutoExtractor()
{
	currentState = STATE_UNINITIALIZED;
	ticksTillTransmuteTimeout = 0;
}

/// <summary>
/// Initializes autoextractor
/// </summary>
/// <param name="useChat">Determins if output should be output as chat messages.</param>
/// <returns>true if successful, false if failed.</returns>
bool AutoExtractor::Init(bool useChat)
{
	ticksTillTransmuteTimeout = 0;
	this->useChat = useChat;

	if(!ReadAffixFile(".\\plugin\\goodPrefix_ae.txt", goodPrefix))
		return false;

	if(!ReadAffixFile(".\\plugin\\goodSuffix_ae.txt", goodSuffix))
		return false;

	return true;
}

/// <summary>
/// Reads list of affix ids from file into specified map
/// </summary>
/// <param name="configPath">Path of config file.</param>
/// <param name="readTo">Map to read good affix values into.</param>
/// <returns>true if successful, false if failed.</returns>
bool AutoExtractor::ReadAffixFile(std::string configPath, stdext::hash_set<int> &readTo)
{
	std::string readLineBuff;
	int readNum = 0;

	readTo.clear();

	std::ifstream inFile(configPath.c_str());
	if(!inFile)
	{
		server->GameStringf("ÿc:AutoRerollÿc0: Failed to open file %s\n", configPath.c_str());
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
/// Starts autoextractor
/// </summary>
/// <returns>true if success.</returns>
bool AutoExtractor::Start(int itemCount, bool useChat)
{
	numberOfRunsRemaining = itemCount;

	if(!Init(useChat))
	{
		return false;
	}

	return StartExtraction();
}

/// <summary>
/// Starts the extraction process
/// </summary>
/// <returns>true if success.</returns>
bool AutoExtractor::StartExtraction()
{
	if(!me->OpenCube())
	{
		if(useChat)
			me->Say("ÿc:AutoExtractorÿc0: Cube not opened");
		else
			server->GameStringf("ÿc:AutoExtractorÿc0: Cube not opened");

		return false;
	}

	if(numberOfRunsRemaining-- <= 0)
	{
		Abort();
		return true;
	}

	// Only the items we use as extractors are presently in the cube. Store the item codes of all
	//  items in the cube in 'extractors' list.
	extractors.clear();
	me->EnumStorageItems(STORAGE_CUBE, enumFindExtractors, (LPARAM)&extractors);

	// Reset count of items expected after transmute to the number of extractors + 1 for the output
	//   e.g: key + rerolling orb + rare ring
	extractedItemID = 0;
	itemsExpectedToCube = extractors.size()+1;

	// Make sure we still have the cube open, slightly lessens the chance of being kicked if user closes
	//  it during the extraction process
	if(!CheckCubeUI())
		return false;

	// 3000ms timeout
	ticksTillTransmuteTimeout = 3 * (1000 / server->GetTickRate());
	currentState = STATE_TRANSMUTE;
	me->Transmute();

	return true;
}

/// <summary>
/// Determines if the cube is currently open and aborts if it's not open
/// </summary>
/// <returns>true if cube is open, false if cube is not open and process has been aborted.</returns>
bool AutoExtractor::CheckCubeUI()
{
	if(!me->IsUIOpened(UI_CUBE))
	{
		if(useChat)
			me->Say("ÿc:AutoExtractorÿc0: Cube UI closed, aborting");

		server->GameStringf("ÿc:AutoExtractorÿc0: Cube UI closed, aborting");

		Abort();
		return false;
	}

	return true;
}

/// <summary>
/// Determines if this item has a good affix. If so, a message is displayed and true is returned.
/// </summary>
/// <param name="item">The item to check.</param>
/// <returns>true if the item has good affix, false if it's junk.</returns>
bool AutoExtractor::CheckExtractedItem(const ITEM &item)
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
				server->GameStringf("ÿc;Prefixÿc0 %s: %s", Prefix[item.wPrefix[i]], PrefixDetails[item.wPrefix[i]]);
			}
			if(goodSuffix.count(item.wSuffix[i]) > 0)
			{
				goodSuffixCount++;
				server->GameStringf("ÿc:Suffixÿc0 %s: %s", Suffix[item.wSuffix[i]], SuffixDetails[item.wSuffix[i]]);
			}
		}
	}

	return goodPrefixCount >= 1 || goodSuffixCount >= 1;
}

/// <summary>
/// Determines if the specified item is an extractor (e.g: key, rerolling orb)
/// </summary>
/// <param name="item">The item to check.</param>
/// <returns>true if it's an extractor (e.g: key, rerolling orb), false if not.</returns>
bool AutoExtractor::IsItemAnExtractor(const ITEM &item)
{
	for(int i = 0; i < (int)extractors.size(); i++)
	{
		if(_stricmp(extractors[i].c_str(), item.szItemCode) == 0)
		{
			return true;
		}
	}

	return false;
}

/// <summary>
/// Aborts the process if it's currently running
/// </summary>
void AutoExtractor::Abort()
{
	if(currentState != STATE_COMPLETE && currentState != STATE_UNINITIALIZED)
	{
		currentState = STATE_COMPLETE;
	}
}

/// <summary>
/// Called at a constant rate by d2hackit's clock
/// </summary>
void AutoExtractor::OnTick()
{
	switch(currentState)
	{
		case STATE_COMPLETE:
		{
			currentState = STATE_UNINITIALIZED;

			if(useChat)
			{
				me->Say("ÿc:AutoExtractorÿc0: AutoExtractor Ended");
			}
			else
			{
				server->GameStringf("ÿc:AutoExtractorÿc0: AutoExtractor Ended");
			}

			break;
		}
		case STATE_TRANSMUTE:
		{
			if(!CheckCubeUI())
			{
				return;
			}
		
			if(ticksTillTransmuteTimeout-- <= 0)
			{
				if(useChat)
					me->Say("ÿc:AutoExtractorÿc0: Timed out waiting for transmute results");
		
				server->GameStringf("ÿc:AutoExtractorÿc0: Timed out waiting for transmute results");
				Abort();
				return;
			}
		
			break;
		}
	}
}

/// <summary>
/// Called whenever an item is moved from the cube to the cursor
/// </summary>
/// <param name="item">The item that moved to the player's cursor.</param>
void AutoExtractor::OnItemFromCube(const ITEM &item)
{
	// Only interested in this event when we're picking up the extracted item from the cube
	if(currentState != STATE_PICKUPEXTRACTEDITEM)
		return;
	
	// Make sure the item that moved to our hand from the cube is the output from the extraction
	//  process (e.g: rare ring). If it's not, then we need to abort because there's going to be
	//  a request to pickup the extracted item from the cube while we have this item in our hand,
	//  the server will kick us.
	if(item.dwItemID != extractedItemID)
	{
		if(useChat)
		{
			me->Say("ÿc:AutoExtractorÿc0: Unknown item picked up");
		}
		server->GameStringf("ÿc:AutoExtractorÿc0: Unknown item picked up: [%X] %s", item.dwItemID, item.szItemCode);

		Abort();
		return;
	}

	// Drop the extracted item to our inventory
	currentState = STATE_EXTRACTEDTOINVENTORY;
	if(!me->DropItemToStorage(STORAGE_INVENTORY, item.dwItemID))
	{
		if(useChat)
		{
			me->Say("ÿc:AutoExtractorÿc0: Failed drop extracted item back to inventory");
		}
		server->GameStringf("ÿc:AutoExtractorÿc0: Failed drop extracted item back to inventory");

		Abort();
	}
}

/// <summary>
/// Called whenever an item is moved to the player's inventory
/// </summary>
/// <param name="item">The item that moved to the player's inventory.</param>
void AutoExtractor::OnItemToInventory(const ITEM &item)
{
	// We're only interested in this event when we're moving the extracted item to our inventory
	if(currentState != STATE_EXTRACTEDTOINVENTORY)
		return;

	// Make sure the item that was moved to the inventory is the same one that was extracted, otherwise
	//   ignore it
	if(item.dwItemID != extractedItemID)
	{
		if(useChat)
		{
			me->Say("ÿc:AutoExtractorÿc0: Item to inventory not extracted item");
		}

		server->GameStringf("ÿc:AutoExtractorÿc0: Item to inventory not extracted item, ignoring");
		return;
	}

	// All of the extracted items are 1x1, so make sure we have room for it
	SIZE itemSize = server->GetItemSize(item.szItemCode);;
	POINT emptySpot = {0, 0};

	bool foundSpot = false;

	// We check 3 times to make sure there's room for the output + extractors (e.g: rare ring, key, rerolling orb).
	//  when calling FindFirstStorageSpaceEx, it checks starting right after the previously found spot
	for(int i = 0; i < 11*9; i++)
	{
		if(me->FindFirstStorageSpaceEx(STORAGE_INVENTORY, itemSize, &emptySpot, emptySpot))
		{
			foundSpot = true;
			break;
		}

		// Increment the current inventory spot so next time we check for an empty spot it will be right
		//   after the previous one.
		emptySpot.x++;
		if(emptySpot.x >= INV_COL)
		{
			emptySpot.x = 0;
			emptySpot.y++;
		}
	}

	if(!foundSpot)
	{
		if(useChat)
		{
			me->Say("ÿc:AutoExtractorÿc0: No more room");
		}
		else
		{
			server->GameStringf("ÿc:AutoExtractorÿc0: No more room");
		}

		Abort();
		return;
	}

	StartExtraction();
}

/// <summary>
/// Called whenever an item is moved to the cube
/// </summary>
/// <param name="item">The item that moved to the cube.</param>
void AutoExtractor::OnItemToCube(const ITEM &item)
{
	// We only care about items to the cube if we just completed the transmute step
	if(currentState != STATE_TRANSMUTE)
	{
		return;
	}

	bool itemIsNotExtractor = true;
	itemsExpectedToCube--;

	// Check this item against each of the items we're using to extract items (e.g: key + rerolling orb). If
	//  this item is not an extractor, then it's an extracted item (e.g: rare ring). Check the extracted item for
	//  good affix
	if(!IsItemAnExtractor(item))
	{
		extractedItemID = item.dwItemID;
		CheckExtractedItem(item);
	}

	if(itemsExpectedToCube == 0)
	{
		// reset the expected item count back to the number of extractors we use plus 1 for the output
		//   (e.g: key + rerollingOrb + rare ring = 3 items expected)
		itemsExpectedToCube = extractors.size()+1;
		
		if(!CheckCubeUI())
		{
			return;
		}

		currentState = STATE_PICKUPEXTRACTEDITEM;
		if(!me->PickStorageItemToCursor(extractedItemID))
		{
			if(useChat)
				me->Say("ÿc:AutoExtractorÿc0: Failed drop pickup extracted item");

			server->GameStringf("ÿc:AutoExtractorÿc0: Failed drop pickup extracted item");
			Abort();
		}
	}
}

/// <summary>
/// Copies the item codes of all the enumerated items (item) to a list of strings (lParam)
/// </summary>
/// <param name="item">The current item.</param>
/// <param name="lParam">a std::vector<string>* used to store all found item codes<param>
BOOL CALLBACK enumFindExtractors(LPCITEM item, LPARAM lParam)
{
	((std::vector<std::string>*)lParam)->push_back(item->szItemCode);

	return TRUE;
}