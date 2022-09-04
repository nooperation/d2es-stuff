#include <string>
#include <fstream>
#include "AutoExtractor.h"
#include "../../Includes/D2Client.h"
#include "../../Includes/itemPrefix.h"
#include "../../Includes/itemSuffix.h"

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
bool AutoExtractor::ReadAffixFile(const std::string &configPath, std::unordered_set<int> &readTo)
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
	currentState = STATE_START;

	if(!me->OpenCube())
	{
		if(useChat)
			me->Say("ÿc:AutoExtractorÿc0: Cube not opened");
		else
			server->GameStringf("ÿc:AutoExtractorÿc0: Cube not opened");

		Abort();
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
	if (extractors.empty())
	{
		if (useChat)
			me->Say("ÿc:AutoExtractorÿc0: Nothing found to extract");
		else 
			server->GameStringf("ÿc:AutoExtractorÿc0: Nothing found to extract");

		Abort();
		return true;
	}

	// Reset count of items expected after transmute to the number of extractors + 1 for the output
	//   e.g: key + rerolling orb + rare ring
	while (!extractedItemIDs.empty())
	{
		extractedItemIDs.pop();
	}

	auto expectedOutputSize = GetExpectedOutputSize();
	if (expectedOutputSize.cx == 1 && expectedOutputSize.cy == 1)
	{
		itemsExpectedToCube = GetNumberOfExpectedOutputs();
		if (itemsExpectedToCube > me->GetNumberOfFreeStorageSlots(STORAGE_INVENTORY))
		{
			if (useChat)
				me->Say("ÿc:AutoExtractorÿc0: No more room");
			else
				server->GameStringf("ÿc:AutoExtractorÿc0: No more room");

			Abort();
			return true;
		}
	}
	else
	{
		POINT buffer;

		// Make sure our oddly shaped output will fit in our inventory
		if (!me->FindFirstStorageSpace(STORAGE_INVENTORY, expectedOutputSize, &buffer))
		{
			if (useChat)
				me->Say("ÿc:AutoExtractorÿc0: No more room");
			else
				server->GameStringf("ÿc:AutoExtractorÿc0: No more room");

			Abort();
			return true;
		}

		// Also make sure our extractors will too
		itemsExpectedToCube = GetNumberOfExpectedOutputs() - 1; // exclude our oddly shaped item
		if (itemsExpectedToCube > me->GetNumberOfFreeStorageSlots(STORAGE_INVENTORY))
		{
			if (useChat)
				me->Say("ÿc:AutoExtractorÿc0: No more room");
			else
				server->GameStringf("ÿc:AutoExtractorÿc0: No more room");

			Abort();
			return true;
		}
	}

	// Make sure we still have the cube open, slightly lessens the chance of being kicked if user closes
	//  it during the extraction process
	if (!CheckCubeUI())
	{
		Abort();
		return false;
	}

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
		else 
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
		if(_stricmp(extractors[i].itemCode.c_str(), item.szItemCode) == 0)
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
	if(item.dwItemID != extractedItemIDs.front())
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
	if(item.dwItemID != extractedItemIDs.front())
	{
		if(useChat)
		{
			me->Say("ÿc:AutoExtractorÿc0: Item to inventory not extracted item");
		}

		server->GameStringf("ÿc:AutoExtractorÿc0: Item to inventory not extracted item, ignoring");
		return;
	}

	//// TODO: this assumes all extracted items are 1x1
	//auto freeSpotsRemaining = me->GetNumberOfFreeStorageSlots(STORAGE_INVENTORY);
	//if (GetNumberOfExpectredOutputs() > freeSpotsRemaining)
	//{
	//	if (useChat)
	//	{
	//		me->Say("ÿc:AutoExtractorÿc0: No more room");
	//	}
	//	else
	//	{
	//		server->GameStringf("ÿc:AutoExtractorÿc0: No more room");
	//	}

	//	Abort();
	//	return;
	//}

	extractedItemIDs.pop();
	if (!extractedItemIDs.empty())
	{
		// We still have expected items. Pickup the next extracted item.
		PickupNextExtractedItem();
	}
	else
	{
		StartExtraction();
	}
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

	itemsExpectedToCube--;

	// Check this item against each of the items we're using to extract items (e.g: key + rerolling orb). If
	//  this item is not an extractor, then it's an extracted item (e.g: rare ring). Check the extracted item for
	//  good affix
	if(!IsItemAnExtractor(item))
	{
		extractedItemIDs.push(item.dwItemID);
		CheckExtractedItem(item);
	}

	if(itemsExpectedToCube == 0)
	{
		// reset the expected item count back to the number of extractors we use plus 1 for the output
		//   (e.g: key + rerollingOrb + rare ring = 3 items expected)
		itemsExpectedToCube = GetNumberOfExpectedOutputs();
		
		if(!CheckCubeUI())
		{
			return;
		}

		PickupNextExtractedItem();
	}
}

void AutoExtractor::PickupNextExtractedItem()
{
	currentState = STATE_PICKUPEXTRACTEDITEM;

	if (extractedItemIDs.empty())
	{
		// TODO: Figure out why this is happening... Possible feedback from other modules
		StartExtraction();
		return;
	}

	auto extractedItemId = extractedItemIDs.front();

	if (!me->PickStorageItemToCursor(extractedItemId))
	{
		if (useChat)
		{
			me->Say("ÿc:AutoExtractorÿc0: Failed drop pickup extracted item");
		}

		server->GameStringf("ÿc:AutoExtractorÿc0: Failed drop pickup extracted item");
		Abort();
	}
}

SIZE AutoExtractor::GetExpectedOutputSize() const
{
	SIZE expectedSize = { 1,1 };

	for (auto &item : this->extractors)
	{
		if (
			item.itemCode == "t16" || // magic large charm -> magic large charm
			item.itemCode == "t17" || // rare large charm -> magic large charm
			item.itemCode == "t18"    // rare large charm -> rare large charm
		)
		{
			expectedSize.cy = 2;
			return expectedSize;
		}
		else if (
			item.itemCode == "t19" || // magic grand charm -> magic grand charm
			item.itemCode == "t20" || // magic grand charm -> rare grand charm
			item.itemCode == "t21" || // rare grand charm -> rare grand charm
			item.itemCode == "t24" || // rare->rare arrow
			item.itemCode == "t25"    // rare->unique arrow
		)
		{
			expectedSize.cy = 3;
			return expectedSize;
		}
	}

	return expectedSize;
}

int AutoExtractor::GetNumberOfExpectedOutputs() const
{
	// Ore has a special case for determining number of outputs. See 'enumFindExtractors'
	if (this->extractors.size() == 1 && this->extractors[0].itemCode == "ore")
	{
		auto &ore = this->extractors[0];
		if (ore.oreCounts.size() == 0)
		{
			// empty ore -> note + note
			return 2;
		}

		//// No longer needed for modified eastern sun since ore -> ore + bigGem. No more small gems.
		//for (auto &item : this->extractors[0].oreCounts)
		//{
		//	if (item.value > 1)
		//	{
		//		// ore -> ore + gem + gem
		//		return 3;
		//	}
		//}

		// ore -> ore + gem
		return 2;
	}
	
	// #extractors -> #extractors + output
	return this->extractors.size() + 1;
}

/// <summary>
/// Copies the item codes of all the enumerated items (item) to a list of strings (lParam)
/// </summary>
/// <param name="item">The current item.</param>
/// <param name="lParam">a std::vector<string>* used to store all found item codes<param>
BOOL CALLBACK enumFindExtractors(LPCITEM item, LPARAM lParam)
{
	auto extractors = (std::vector<ExtractorInfo> *)lParam;

	ExtractorInfo info;
	info.itemCode = item->szItemCode;

	if (info.itemCode == "ore")
	{
		for (auto i = 0; i < item->iNumProperties; ++i)
		{
			info.oreCounts.push_back(item->aProperties[i]);
		}
	}

	extractors->push_back(info);

	return TRUE;
}

