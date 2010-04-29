#include "../../d2hackit/includes/D2Client.h"
#include "AutostockAutoextract.h"


AutostockAutoextract::AutostockAutoextract()
{
	currentState = STATE_UNINITIALIZED;
}

bool AutostockAutoextract::Init(bool useChat)
{
	this->useChat = useChat;
	autoExtractorLoaded = false;
	autoStockerLoaded = false;
	emptyCubeLoaded = false;

	return true;
}

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

bool AutostockAutoextract::StartAsAe()
{
	if(!me->OpenCube())
	{
		if(useChat)
			me->Say("ÿc5AutostockAutoextractÿc0: Cube not opened");

		server->GameStringf("ÿc5EmptyCubeÿc0: Cube not opened");
		currentState = STATE_COMPLETE;

		return false;
	}

	memset(&extractorStuff, 0, sizeof(ExtractorStuff));
	me->EnumStorageItems(STORAGE_CUBE, enumFindCubeItems, (LPARAM)&extractorStuff);

	if(strlen(extractorStuff.extractorCode) == 0 || strlen(extractorStuff.restockerCode) == 0)
	{
		server->GameStringf("ÿc5AutostockAutoextractÿc0: Place extractors in cube before starting");
		currentState = STATE_COMPLETE;
		return false;
	}

	currentState = STATE_RUNAUTOEXTRACTOR;
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

	server->GameCommandLine("ae start chat");

	return true;
}

void AutostockAutoextract::OnTick()
{
	switch(currentState)
	{
		case STATE_NEXTEXTRACTORSTUFF:
		{
			currentState = STATE_WAITINGFORNEXTSTATE;
			DWORD itemToPickup = 0;

			if(!extractorStuff.movedExtractor)
			{
				itemToPickup = extractorStuff.extractorID;
				extractorStuff.movedExtractor = true;
			}
			else if(!extractorStuff.movedRestocker)
			{
				itemToPickup = extractorStuff.restockerID;
				extractorStuff.movedRestocker = true;
			}

			if(itemToPickup == 0)
			{
				StartAsAe();
				return;
			}
			else
			{
				currentState = STATE_PICKUPEXTRACTORSTUFF;
				if(!me->PickStorageItemToCursor(itemToPickup))
				{
					server->GameStringf("Failed to pickup extractor stuff");
					currentState = STATE_COMPLETE;
					return;
				}
			}

			break;
		}
		case STATE_COMPLETE:
		{
			currentState = STATE_UNINITIALIZED;

			if(useChat)
				me->Say("ÿc5AutostockAutoextractÿc0: AutostockAutoextract Ended");
			else
				server->GameStringf("ÿc5AutostockAutoextractÿc0: AutostockAutoextract Ended");

			break;
		}
	}
}

void AutostockAutoextract::Abort()
{
	if(currentState != STATE_COMPLETE && currentState != STATE_UNINITIALIZED)
		currentState = STATE_COMPLETE;
}

void AutostockAutoextract::OnItemFromInventory(ITEM &item)
{
	if(currentState != STATE_PICKUPEXTRACTORSTUFF)
		return;

	currentState = STATE_EXTRACTORSTUFFTOCUBE;
	if(!me->DropCursorItemToStorage(STORAGE_CUBE))
	{
		server->GameStringf("Failed to drop extractorstuff to cube");
		currentState = STATE_COMPLETE;
		return;
	}
}

void AutostockAutoextract::OnItemToInventory(ITEM &item)
{
	if(currentState != STATE_RUNAUTOSTOCKER)
		return;

	// Need to get new restocker ID, last item placed in inventory
	//   from autostocker is the restocker.
	if(GetStockerType(item.dwItemID, NULL))
	{
		extractorStuff.restockerID = item.dwItemID;
	}
}
void AutostockAutoextract::OnItemToCube(ITEM &item)
{
	if(currentState != STATE_EXTRACTORSTUFFTOCUBE)
		return;

	currentState = STATE_NEXTEXTRACTORSTUFF;
}

bool AutostockAutoextract::OnAutostockerEnded()
{
	if(currentState != STATE_RUNAUTOSTOCKER)
		return false;

	currentState = STATE_NEXTEXTRACTORSTUFF;

	return true;
}

// Start -> Run AutoExtractor -> Clean Cube -> Run AutoStocker
bool AutostockAutoextract::OnAutoExtractorEnded()
{
	if(currentState != STATE_RUNAUTOEXTRACTOR)
		return false;

	// Find the extractorStuff ids before we clean the cube so we can
	//   put them back in later on
	me->EnumStorageItems(STORAGE_CUBE, enumFindCubeItems, (LPARAM)&extractorStuff);

	currentState = STATE_RUNEMPTYCUBE;
	server->GameCommandf("emptycube start chat");

	return true;
}

bool AutostockAutoextract::OnEmptyCubeEnded()
{
	if(currentState != STATE_RUNEMPTYCUBE)
		return false;

	currentState = STATE_RUNAUTOSTOCKER;
	server->GameCommandf("as start_rares chat%s%s%s",  transmuteSet?" Sets":"", transmuteRare?" Rares":"", transmuteUnique?" Uniques":"");

	return true;
}

BOOL CALLBACK enumFindCubeItems(LPCITEM item, LPARAM lParam)
{
	ExtractorStuff *extractorStuff = (ExtractorStuff *)lParam;

	if(GetStockerType(item->dwItemID, NULL))
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

bool GetStockerType(DWORD itemId, int *stockerType)
{
	char itemCode[4];
	int itemCodeNum = 0;

	server->GetItemCode(itemId, itemCode, sizeof(itemCode)/sizeof(itemCode[0]));
	
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
