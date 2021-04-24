#include "../../Includes/D2Client.h"
#include "AutoOre.h"

AutoOre::AutoOre()
{
	currentState = State::Uninitialized;
	this->autoExtractorLoaded = false;
	this->autoStockerLoaded = false;
	this->emptyCubeLoaded = false;
}

void AutoOre::SetState(State newState)
{
	this->currentState = newState;
}
void AutoOre::Start()
{
	this->itemsToDrop.clear();
	this->oreIds.clear();
	this->currentItemToDrop = 0;
	this->currentOreId = 0;
	SetState(State::Uninitialized);

	server->GameStringf("ÿc:ÿc5AutoOreÿc0: Starting");

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

	StartAsAe();
}

void AutoOre::StartAsAe()
{
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

	me->EnumStorageItems(STORAGE_INVENTORY, enumFindOre, (LPARAM)this);
	if (this->oreIds.empty())
	{
		server->GameStringf("ÿc5AutoOreÿc0: No ores found");
		Abort();
		return;
	}

	if (!this->itemsToDrop.empty())
	{
		this->PickupNextItemToDrop();
		return;
	}

	this->PickupNextOre();
}

void AutoOre::PickupNextItemToDrop()
{
	if (this->itemsToDrop.empty())
	{
		server->GameStringf("ÿc:ÿc5AutoOreÿc0: No more items to drop");
		this->StartAsAe();
		return;
	}

	this->currentItemToDrop = this->itemsToDrop.back();
	this->itemsToDrop.pop_back();

	SetState(State::PickupNextItemToDrop);
	me->PickStorageItemToCursor(this->currentItemToDrop);
}

void AutoOre::PickupNextOre()
{
	if (this->oreIds.empty())
	{
		server->GameStringf("ÿc:ÿc5AutoOreÿc0: No more ore to extract");
		Abort();
		return;
	}

	this->currentOreId = this->oreIds.back();
	this->oreIds.pop_back();

	SetState(State::PickupNextOre);
	me->PickStorageItemToCursor(this->currentOreId);
}

void AutoOre::OnItemPickedUpFromInventory(DWORD itemId)
{
	if (this->currentState == State::PickupNextItemToDrop)
	{
		if (itemId != this->currentItemToDrop)
		{
			return;
		}

		this->DropNextItemToDrop();
	}
	else if (this->currentState == State::PickupNextOre)
	{
		if (itemId != this->currentOreId)
		{
			return;
		}

		this->DropNextOreToCube();
	}
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

void AutoOre::OnItemDroppedToCube(DWORD itemId)
{
	if (this->currentState == State::DropNextOreToCube)
	{
		if (itemId != this->currentOreId)
		{
			return;
		}

		this->RunAutoExtractor();
	}
}

void AutoOre::DropNextItemToDrop()
{
	if (this->currentItemToDrop == 0)
	{
		server->GameStringf("ÿc:ÿc5AutoOreÿc0: Bad state. Dropping an invalid item ID");
		return;
	}

	SetState(State::DropNextItemToDrop);
	me->DropCursorItemToGround();
}

void AutoOre::DropNextOreToCube()
{
	if (this->currentOreId == 0)
	{
		server->GameStringf("ÿc:ÿc5AutoOreÿc0: Bad state. Dropping an invalid ore ID to cube");
		return;
	}

	SetState(State::DropNextOreToCube);
	me->DropCursorItemToStorage(STORAGE_CUBE);
}

void AutoOre::RunAutoExtractor()
{
	SetState(State::RunAutoExtractor);
	server->GameCommandLine("ae start chat");
}

bool AutoOre::OnAutoExtractorMessage(const std::string_view &message)
{
	if (currentState != State::RunAutoExtractor)
	{
		return false;
	}

	if (message == "AutoExtractor Ended")
	{
		this->RunAutoStocker();
	}
	return true;
}

void AutoOre::RunAutoStocker()
{
	SetState(State::RunAutoStocker);
	server->GameCommandLine("as start chat");
}

bool AutoOre::OnAutoStockerMessage(const std::string_view &message)
{
	if (currentState != State::RunAutoStocker)
	{
		return false;
	}

	if (message == "Autostocker Ended")
	{
		StartAsAe();
	}
	return true;
}

void AutoOre::ProcessInventoryItem(const ITEM &item)
{
	if (strcmp(item.szItemCode, "ore") == 0)
	{
		this->oreIds.push_back(item.dwItemID);
	}
	else if (strcmp(item.szItemCode, "fkn") == 0)
	{
		this->itemsToDrop.push_back(item.dwItemID);
	}
}

void AutoOre::Abort()
{
	if (currentState != State::Uninitialized)
	{
		SetState(State::Uninitialized);
		server->GameStringf("ÿc5AutoOreÿc0: Ended");
	}
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
