#include "../../Includes/D2Client.h"
#include "AutoOre.h"

#define CONFIG_FILE ".\\plugin\\ore.ini"

BOOL CALLBACK enumGetAllCubeItems(LPCITEM item, LPARAM lParam);

AutoOre::AutoOre()
{
	currentState = State::Uninitialized;
	this->autoExtractorLoaded = false;
	this->autoStockerLoaded = false;
	this->emptyCubeLoaded = false;
	this->expectedItemToHand = 0;
	this->numExpectedTransmuteResults = 0;
	this->numTransmuteResults = 0;
	this->currentItemToDrop = 0;
	this->currentOreId = 0;
}

void AutoOre::SetState(State newState)
{
	if (false)
	{
		const char *const stateNames[] = {
			"Uninitialized",
			"Initializing",
			"PickupNextItemToDrop",
			"DropNextItemToDrop",
			"PickupNextOre",
			"DropNextOreToCube",
			"Transmute",
			"WaitForTransmuteResults",
			"RunEmptyCube",
			"RunAutoStocker",
		};
		server->GameStringf("ÿc5AutoOreÿc0: State %s -> %s", stateNames[(int)this->currentState], stateNames[(int)newState]);
	}

	this->currentState = newState;
}

void AutoOre::Start(bool useChat)
{
	this->useChat = useChat;
	SetState(State::Initializing);

	if (this->useChat)
		me->Say("ÿc5AutoOreÿc0: Starting");
	else
		server->GameStringf("ÿc5AutoOreÿc0: Starting");

	// Load required modules
	if (!autoExtractorLoaded)
	{
		autoExtractorLoaded = true;
		server->GameCommandLine("load ae");
	}
	if (!autoStockerLoaded)
	{
		autoStockerLoaded = true;
		server->GameCommandLine("load as");
	}
	if (!emptyCubeLoaded)
	{
		emptyCubeLoaded = true;
		server->GameCommandLine("load emptycube");
	}

	this->oreIds.clear();
	me->EnumStorageItems(STORAGE_INVENTORY, enumFindOre, (LPARAM)this);
	if (this->oreIds.empty())
	{
		Abort();
		return;
	}

	StartAutoOre();
}

void AutoOre::StartAutoOre()
{
	this->itemsToDrop.clear();
	this->currentItemToDrop = 0;
	this->currentOreId = 0;
	this->expectedItemToHand = 0;
	this->numExpectedTransmuteResults = 0;
	this->numTransmuteResults = 0;

	if (!me->OpenCube())
	{
		server->GameStringf("ÿc5AutoOreÿc0: Cube not opened");

		Abort();
		return;
	}

	auto cubeItemCount = 0;
	me->EnumStorageItems(STORAGE_CUBE, enumFindCubeItems, (LPARAM)&cubeItemCount);
	if (cubeItemCount != 0)
	{
		server->GameStringf("ÿc5AutoOreÿc0: Please empty your cube");
		Abort();
		return;
	}

	this->PickupNextOre();
}

void AutoOre::PickupNextOre()
{
	if (this->oreIds.empty())
	{
		server->GameStringf("ÿc5AutoOreÿc0: No more ore to extract");
		Abort();
		return;
	}

	this->currentOreId = this->oreIds.back();
	this->oreIds.pop_back();

	SetState(State::PickupNextOre);
	me->PickStorageItemToCursor(this->currentOreId);
}


void AutoOre::PickupNextItemToDrop()
{
	if (this->itemsToDrop.empty())
	{
		this->RunAutoStocker();
		return;
	}

	this->currentItemToDrop = this->itemsToDrop.back();
	this->itemsToDrop.pop_back();

	SetState(State::PickupNextItemToDrop);
	me->PickStorageItemToCursor(this->currentItemToDrop);
}

void AutoOre::DropNextItemToDrop()
{
	if (this->currentItemToDrop == 0)
	{
		server->GameStringf("ÿc5AutoOreÿc0: Bad state. Dropping an invalid item ID");
		return;
	}

	SetState(State::DropNextItemToDrop);
	me->DropCursorItemToGround();
}

void AutoOre::OnItemDropped(DWORD itemId)
{
	if (this->currentState != State::DropNextItemToDrop)
	{
		return;
	}

	if (itemId != this->currentItemToDrop)
	{
		return;
	}

	this->PickupNextItemToDrop();
}

void AutoOre::OnItemPickedUpFromInventory(DWORD itemId)
{
	if (this->currentState == State::PickupNextOre)
	{
		if (itemId != this->currentOreId)
		{
			return;
		}

		this->DropNextOreToCube();
	}
}

void AutoOre::OnItemPickedUpFromCube(DWORD itemId)
{
	if (this->currentState == State::PickupNextItemToDrop)
	{
		if (itemId != this->currentItemToDrop)
		{
			server->GameStringf("ÿc5AutoOreÿc0: Expected item id %d to hand, but got item id %d (State = PickupNextItemToDrop)", expectedItemToHand, itemId);
			Abort();
			return;
		}

		this->DropNextItemToDrop();
	}
}

void AutoOre::OnItemDroppedToCube(const ITEM &itemDroppedToCube)
{
	if (this->currentState == State::DropNextOreToCube)
	{
		if (itemDroppedToCube.dwItemID != this->currentOreId)
		{
			return;
		}

		// NOTE: We cannot run extractor yet. If we do then it will also see this item 'to cube' and
		//       will think it was part of its extraction process
		SetState(State::Transmute);
	}
	else if (this->currentState == State::WaitForTransmuteResults)
	{
		numTransmuteResults++;
		if (numTransmuteResults != numExpectedTransmuteResults)
		{
			return;
		}

		SetState(State::RunEmptyCube);
		server->GameCommandLine("emptycube start chat");
	}

}

void AutoOre::DropNextOreToCube()
{
	if (this->currentOreId == 0)
	{
		server->GameStringf("ÿc5AutoOreÿc0: Bad state. Dropping an invalid ore ID to cube");
		return;
	}

	SetState(State::DropNextOreToCube);
	me->DropCursorItemToStorage(STORAGE_CUBE);
}

void AutoOre::OnTick()
{
	if (currentState == State::Transmute)
	{
		// HACK: AE must be delayed a full tick so the autoextract module does not process the item we
		//       just put in the cube as a transmute result
		SetState(State::WaitForTransmuteResults);
		numTransmuteResults = 0;
		numExpectedTransmuteResults = 2;
		me->Transmute();
	}
}

bool AutoOre::OnEmptyCubeMessage(const std::string_view &message)
{
	if (currentState != State::RunEmptyCube)
	{
		return false;
	}

	if (message == "EmptyCube Ended")
	{
		this->RunAutoStocker();
	}

	return true;
}

void AutoOre::RunAutoStocker()
{
	if (this->useChat)
	{
		// Hack: AutoStocker calls .ore start chat, then ore calls .autostocker start chat, and horrible things happen. No more autostocker when in chat mode
		StartAutoOre();
	}
	else
	{
		SetState(State::RunAutoStocker);
		server->GameCommandLine("as start chat");
	}
}

bool AutoOre::OnAutoStockerMessage(const std::string_view &message)
{
	if (currentState != State::RunAutoStocker)
	{
		return false;
	}

	if (message == "Autostocker Ended")
	{
		StartAutoOre();
	}
	return true;
}

void AutoOre::ProcessInventoryItem(const ITEM &item)
{
	if (strcmp(item.szItemCode, "ore") == 0)
	{
		GAMEUNIT unit;
		unit.dwUnitID = item.dwItemID;
		unit.dwUnitType = UNIT_TYPE_ITEM;

		// We only care about ores that have at least one gem to extract (xtal_00-xtal_11)
		for (size_t i = 470; i <= 481; i++)
		{
			auto statValue = server->GetUnitStat(&unit, i);
			if (statValue != 0)
			{
				this->oreIds.push_back(item.dwItemID);
				return;
			}
		}
	}
}

void AutoOre::Abort()
{
	if (currentState != State::Uninitialized)
	{
		SetState(State::Uninitialized);
		if (this->useChat)
			me->Say("ÿc5AutoOreÿc0: Ended");
		else
			server->GameStringf("ÿc5AutoOreÿc0: Ended");
	}
}

BOOL CALLBACK enumGetAllCubeItems(LPCITEM item, LPARAM lParam)
{
	auto cubeItemList = (std::vector<ITEM> *)lParam;
	if (cubeItemList == nullptr)
	{
		return FALSE;
	}

	cubeItemList->push_back(*item);
	return TRUE;
}

BOOL CALLBACK enumFindCubeItems(LPCITEM item, LPARAM lParam)
{
	auto cubeItemCount = (int *)lParam;
	if (cubeItemCount != nullptr)
	{
		(*cubeItemCount)++;
	}

	return TRUE;
}

BOOL CALLBACK enumFindOre(LPCITEM item, LPARAM lParam)
{
	auto autoOre = (AutoOre *)lParam;
	if (autoOre != nullptr && item != nullptr)
	{
		autoOre->ProcessInventoryItem(*item);
	}

	return TRUE;
}
