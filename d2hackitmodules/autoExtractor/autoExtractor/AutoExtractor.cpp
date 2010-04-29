#include "../../d2hackit/includes/D2Client.h"
#include "AutoExtractor.h"
#include <string>
#include <fstream>

#include "../../d2hackit/includes/itemPrefix.h"
#include "../../d2hackit/includes/itemSuffix.h"

AutoExtractor::AutoExtractor()
{
	currentState = STATE_UNINITIALIZED;
}

bool AutoExtractor::Init(bool useChat)
{
	this->useChat = useChat;

	if(!ReadConfig(".\\plugin\\goodPrefix_ae.txt", goodPrefix))
		return false;

	if(!ReadConfig(".\\plugin\\goodSuffix_ae.txt", goodSuffix))
		return false;

	return true;
}

bool AutoExtractor::ReadConfig(std::string configPath, stdext::hash_set<int> &readTo)
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

bool AutoExtractor::Start(bool useChat)
{
	if(!Init(useChat))
	{
		return false;
	}

	return StartExtraction();
}

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

	extractors.clear();
	me->EnumStorageItems(STORAGE_CUBE, enumFindExtractors, (LPARAM)&extractors);

	extractedItemID = 0;
	currentState = STATE_STARTEXTRACTION;

	itemsExpectedToCube = extractors.size()+1;

	return true;
}

bool AutoExtractor::CheckCubeUI()
{
	if(!me->IsUIOpened(UI_CUBE))
	{
		if(useChat)
			me->Say("ÿc:AutoExtractorÿc0: Cube UI closed, aborting");

		server->GameStringf("ÿc:AutoExtractorÿc0: Cube UI closed, aborting");

		currentState = STATE_COMPLETE;
		return false;
	}

	return true;
}

void AutoExtractor::OnTick()
{
	switch(currentState)
	{
		case STATE_TRANSMUTE:
		{
			if(!CheckCubeUI())
				return;

			if(ticksTillTransmuteTimeout-- <= 0)
			{
				if(useChat)
					me->Say("ÿc:AutoExtractorÿc0: Timed out waiting for transmute results");

				server->GameStringf("ÿc:AutoExtractorÿc0: Timed out waiting for transmute results");
				currentState = STATE_COMPLETE;
			}

			break;
		}
		case STATE_STARTEXTRACTION:
		{
			currentState = STATE_TRANSMUTE;
			ticksTillTransmuteTimeout = 30;

			if(!CheckCubeUI())
				return;

			me->Transmute();
			break;
		}
		case STATE_EXTRACTIONCOMPLETE:
		{
			currentState = STATE_WAITINGFORNEXTSTATE;
			SIZE oneByOne = {1, 1};
			POINT emptySpot = {0, 0};
			bool emptySpotsLeft = true;

			for(int i = 0; i < 3; i++)
			{
				if(!me->FindFirstStorageSpaceEx(STORAGE_INVENTORY, oneByOne, &emptySpot, emptySpot))
				{
					emptySpotsLeft = false;

					if(useChat)
						me->Say("ÿc:AutoExtractorÿc0: No more room");
					else
						server->GameStringf("ÿc:AutoExtractorÿc0: No more room");

					currentState = STATE_COMPLETE;
					return;
				}

				emptySpot.x++;
				if(emptySpot.x >= INV_COL)
				{
					emptySpot.x = 0;
					emptySpot.y++;
				}
			}

			if(emptySpotsLeft)
			{
				currentState = STATE_STARTEXTRACTION;
			}

			break;
		}
		case STATE_TRANSMUTE_COMPLETE:
		{
			currentState = STATE_PICKUPEXTRACTEDITEM;

			if(!CheckCubeUI())
				return;

			if(!me->PickStorageItemToCursor(extractedItemID))
			{
				if(useChat)
					me->Say("ÿc:AutoExtractorÿc0: Failed drop pickup extracted item");
				
				server->GameStringf("ÿc:AutoExtractorÿc0: Failed drop pickup extracted item");

				currentState = STATE_COMPLETE;
			}

			break;
		}
		case STATE_COMPLETE:
		{
			currentState = STATE_UNINITIALIZED;

			if(useChat)
				me->Say("ÿc:AutoExtractorÿc0: AutoExtractor Ended");
			else
				server->GameStringf("ÿc:AutoExtractorÿc0: AutoExtractor Ended");

			break;
		}
	}
}

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

void AutoExtractor::OnItemFromCube(DWORD itemID)
{
	if(currentState != STATE_PICKUPEXTRACTEDITEM)
		return;

	if(me->GetCursorItem() == extractedItemID)
	{
		currentState = STATE_EXTRACTEDTOINVENTORY;
		if(!me->DropCursorItemToStorage(STORAGE_INVENTORY))
		{
			if(useChat)
				me->Say("ÿc:AutoExtractorÿc0: Failed drop extracted item back to inventory");
			
			server->GameStringf("ÿc:AutoExtractorÿc0: Failed drop extracted item back to inventory");

			currentState = STATE_COMPLETE;
		}
	}
	else
	{
		char itemCode[4];
		server->GetItemCode(itemID, itemCode, sizeof(itemCode)/sizeof(itemCode[0]));

		if(useChat)
			me->Say("ÿc:AutoExtractorÿc0: Unknown item picked up");

		server->GameStringf("ÿc:AutoExtractorÿc0: Unknown item picked up: [%X] %s", itemID, itemCode);

		currentState = STATE_COMPLETE;
		if(!me->DropCursorItemToStorage(STORAGE_INVENTORY))
		{
			if(useChat)
				me->Say("ÿc:AutoExtractorÿc0: Failed drop unknown back to inventory");

			server->GameStringf("ÿc:AutoExtractorÿc0: Failed drop unknown back to inventory");
		}
	}
}

void AutoExtractor::OnItemToInventory(const ITEM &item)
{
	if(currentState != STATE_EXTRACTEDTOINVENTORY)
		return;

	if(item.dwItemID != extractedItemID)
	{
		if(useChat)
			me->Say("ÿc:AutoExtractorÿc0: Item to inventory not extracted item");

		server->GameStringf("ÿc:AutoExtractorÿc0: Item to inventory not extracted item");
	}

	currentState = STATE_EXTRACTIONCOMPLETE;
}

void AutoExtractor::OnItemToCube(const ITEM &item)
{
	if(currentState == STATE_TRANSMUTE)
	{
		bool itemIsNotExtractor = true;

		// Ignore our extractors, they get new ids every transmute
		for(int i = 0; i < (int)extractors.size(); i++)
		{
			if(_stricmp(extractors[i].c_str(), item.szItemCode) == 0)
			{
				itemIsNotExtractor = false;
				itemsExpectedToCube--;
				break;
			}
		}

		if(itemIsNotExtractor)
		{
			itemsExpectedToCube--;
			extractedItemID = item.dwItemID;
			CheckExtractedItem(item);
		}

		if(itemsExpectedToCube == 0)
		{
			currentState = STATE_TRANSMUTE_COMPLETE;
			itemsExpectedToCube = extractors.size()+1;
		}
	}
}

void AutoExtractor::Abort()
{
	if(currentState != STATE_COMPLETE && currentState != STATE_UNINITIALIZED)
		currentState = STATE_COMPLETE;
}

BOOL CALLBACK enumFindExtractors(LPCITEM item, LPARAM lParam)
{
	((std::vector<std::string>*)lParam)->push_back(item->szItemCode);

	return TRUE;
}