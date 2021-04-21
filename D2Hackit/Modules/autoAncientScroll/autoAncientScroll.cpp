#include "../../Includes/D2Client.h"
#include "AutoAncientScroll.h"
#include <string>

AutoAncientScroll::AutoAncientScroll()
{
	currentState = State::Uninitialized;
}

bool AutoAncientScroll::Start()
{
	// Clear previous settings
	ancientDeciphererIds.clear();
	ancientScrollIds.clear();
	lastScrollPosition.x = 0;
	lastScrollPosition.y = 0;

	// Notify the user about the current settings
	server->GameStringf("ÿc:AutoAncientScrollÿc0: Starting");

	// Open the player's cube
	if (!OpenCube())
	{
		Abort();
		return false;
	}

	// Make sure the cube is empty
	if (!IsCubeEmpty())
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: Please empty your cube");

		Abort();
		return false;
	}

	// Populate ancientDeciphererIds and ancientScrollIds with items from the players inventory
	me->EnumStorageItems(STORAGE_INVENTORY, enumItemProc, (LPARAM)this);
	IdentifyNextAncientScroll();

	return true;
}

void AutoAncientScroll::IdentifyNextAncientScroll()
{
	if (ancientScrollIds.empty())
	{
		if (currentState == State::MoveAncientScrollToCube)
		{
			// TODO: .as start
		}
		Abort();
		return;
	}

	if (ancientDeciphererIds.empty())
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: No ancient decipherers found");
		Abort();
		return;
	}

	PickupNextAncientDecipherer();
}

/// <summary>
/// Called whenever an item is pickedup to the cursor from the player's inventory
/// </summary>
/// <param name="item">Item picked up from the player's inventory.</param>
void AutoAncientScroll::OnItemFromInventory(const ITEM &item)
{
	// Only care about this event when we're picking up the stocker or item to be cubed
	if (currentState != State::PickupAncientDecipherer && currentState != State::PickupAncientScroll)
	{
		return;
	}

	// User might of picked up an item while autostocker was running, abort before we attempt
	// to pickup another item and have the server kick us
	if (item.dwItemID != itemWaitingOn)
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: I'm not waiting on this item!");
		Abort();
		return;
	}

	// Make sure cube UI is still open
	if (!CheckCubeUI())
	{
		return;
	}

	if (currentState == State::PickupAncientDecipherer)
	{
		currentState = State::MoveAncientDeciphererToCube;
	}
	else if (currentState == State::PickupAncientScroll)
	{
		currentState = State::MoveAncientScrollToCube;
	}

	if (!me->DropItemToStorage(STORAGE_CUBE, item.dwItemID))
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: DropItemToStorage failed");
	}
}

/// <summary>
/// Called whenever an item is pickedup to the cursor from the cube
/// </summary>
/// <param name="item">Item picked up from the cube.</param>
void AutoAncientScroll::OnItemFromCube(const ITEM &item)
{
	// We only care about this event when we're picking up the stocker from the cube
	if (currentState != State::PickupIdentifiedScrollFromCube)
	{
		return;
	}

	// Make sure the item picked up from the cube is the stocker, otherwise there's a problem
	if (item.dwItemID == itemWaitingOn)
	{
		currentState = State::DropIdentifiedScrollToInventory;

		if (!me->DropItemToStorageEx(STORAGE_INVENTORY, lastScrollPosition, item.dwItemID))
		{
			server->GameStringf("ÿc:AutoAncientScrollÿc0: Failed drop identified ancient scroll back to inventory");

			Abort();
		}
	}
	else
	{
		// User might of picked up item from the cube while process was running, if so we need to abort
		//  to avoid being kicked by the server
		server->GameStringf("ÿc:AutoAncientScrollÿc0: Unknown item picked up: [%X] %s", item.dwItemID, item.szItemCode);

		Abort();
		return;
	}
}

/// <summary>
/// Called whenever an item is moved to the player's inventory
/// </summary>
/// <param name="item">The item that was moved to the player's inventory.</param>
void AutoAncientScroll::OnItemToInventory(const ITEM &item)
{
	// We only care about this event when we're moving our stocker back to the player's inventory
	if (currentState != State::DropIdentifiedScrollToInventory)
	{
		return;
	}

	// Make sure the item that was moved was the stocker, otherwise ignore it
	if (item.dwItemID != itemWaitingOn)
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: Item to inventory not stocker");
		return;
	}

	this->IdentifyNextAncientScroll();
}

/// <summary>
/// Called whenever an item is moved to the cube
/// </summary>
/// <param name="item">The item that was moved to the cube.</param>
void AutoAncientScroll::OnItemToCube(const ITEM &item)
{
	if (currentState == State::MoveAncientDeciphererToCube)
	{
		PickupNextAncientScroll();
	}
	else if (currentState == State::MoveAncientScrollToCube)
	{
		currentState = State::Transmute;

		// Make sure user still has the cube UI open
		if (!CheckCubeUI())
		{
			return;
		}
		me->Transmute();
	}
	else if (currentState == State::Transmute)
	{
		itemWaitingOn = item.dwItemID;

		currentState = State::PickupIdentifiedScrollFromCube;
		if (!me->PickStorageItemToCursor(itemWaitingOn))
		{
			server->GameStringf("ÿc:AutoAncientScrollÿc0: Failed to pick up item from cube");

			Abort();
			return;
		}
	}
}

void AutoAncientScroll::PickupNextAncientDecipherer()
{
	if (this->ancientDeciphererIds.empty())
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: No more ancient decipherers");
		Abort();
		return;
	}

	itemWaitingOn = ancientDeciphererIds.back();
	ancientDeciphererIds.pop_back();

	currentState = State::PickupAncientDecipherer;
	if (!me->PickStorageItemToCursor(itemWaitingOn))
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: Failed to pick up ancient decipherer");

		Abort();
		return;
	}
}

void AutoAncientScroll::PickupNextAncientScroll()
{
	if (this->ancientScrollIds.empty())
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: No more ancient scrolls");
		Abort();
		return;
	}

	itemWaitingOn = ancientScrollIds.back();
	ancientScrollIds.pop_back();

	if (!me->FindItemPosition(STORAGE_INVENTORY, itemWaitingOn, &lastScrollPosition))
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: Failed to find position of next ancient scroll");
	}

	currentState = State::PickupAncientScroll;
	if (!me->PickStorageItemToCursor(itemWaitingOn))
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: Failed to pick up ancient scroll");

		Abort();
		return;
	}
}

bool AutoAncientScroll::OpenCube()
{
	if (!me->OpenCube())
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: You must open your cube before using");

		Abort();
		return false;
	}

	return true;
}

bool AutoAncientScroll::CheckCubeUI()
{
	if (!me->IsUIOpened(UI_CUBE))
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: Cube UI closed, aborting");
		Abort();
	}

	return true;
}

void AutoAncientScroll::Abort()
{
	if (currentState != State::Uninitialized)
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: Done");
		currentState = State::Uninitialized;
	}
}

bool AutoAncientScroll::IsCubeEmpty()
{
	int itemCount = 0;
	me->EnumStorageItems(STORAGE_CUBE, enumItemCountProc, (LPARAM)&itemCount);

	if (itemCount != 0)
	{
		return false;
	}

	return true;
}

void AutoAncientScroll::ProcessInventoryItem(const ITEM *item)
{
	const auto itemCode0 = item->szItemCode[0];
	const auto itemCode1 = item->szItemCode[1];
	const auto itemCode2 = item->szItemCode[2];

	if (itemCode0 == 'd' && itemCode1 == 'd' && itemCode2 == 'd')
	{
		ancientDeciphererIds.push_back(item->dwItemID);
	}
	else if (itemCode0 >= '0' && itemCode0 <= '5' && itemCode1 >= '0' && itemCode1 <= '9' && itemCode2 == 'b')
	{
		ancientScrollIds.push_back(item->dwItemID);
	}
}

BOOL CALLBACK enumItemProc(LPCITEM item, LPARAM lParam)
{
	auto autoStocker = (AutoAncientScroll *)lParam;
	autoStocker->ProcessInventoryItem(item);

	return TRUE;
}

BOOL CALLBACK enumItemCountProc(LPCITEM item, LPARAM lParam)
{
	auto pItemCount = (int *)lParam;
	(*pItemCount)++;

	server->GameStringf("Cube item: %s", server->GetItemName(item->szItemCode));
	return TRUE;
}