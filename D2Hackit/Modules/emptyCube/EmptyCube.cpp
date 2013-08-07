#include "../../Includes/D2Client.h"
#include "EmptyCube.h"

#include "../../Includes/itemPrefix.h"
#include "../../Includes/itemSuffix.h"

EmptyCube::EmptyCube()
{
	currentState = STATE_UNINITIALIZED;
}

/// <summary>
/// Initializes emptycube
/// </summary>
/// <param name="useChat">Determines if output messages should be sent as chat messages.</param>
/// <returns>true if successful, false if failed.</returns>
bool EmptyCube::Init(bool useChat)
{
	this->useChat = useChat;

	return true;
}

/// <summary>
/// Starts exmptyCube
/// </summary>
/// <param name="useChat">Determines if output messages should be sent as chat messages.</param>
/// <returns>true if successful, false if failed.</returns>
bool EmptyCube::Start(bool useChat)
{
	if(!Init(useChat))
	{
		return false;
	}

	return StartCleaning();
}

/// <summary>
/// Starts the actual process of EmptyCube
/// </summary>
/// <returns>true if successful, false if failed.</returns>
bool EmptyCube::StartCleaning()
{
	if(!me->OpenCube())
	{
		if(useChat)
		{
			me->Say("ÿc5EmptyCubeÿc0: Cube not opened");
		}
		server->GameStringf("ÿc5EmptyCubeÿc0: Cube not opened");

		Abort();
		return false;
	}

	// Get a list of items in the cube
	itemsInCube.clear();
	me->EnumStorageItems(STORAGE_CUBE, enumFindCubeItems, (LPARAM)&itemsInCube);

	// Cube is empty, no more work needs to be done
	if(itemsInCube.size() == 0)
	{
		Abort();
		return true;
	}

	currentItemToMove = -1;
	ProcessNextCubeItem();

	return true;
}

/// <summary>
/// Moves the next item from the player's inventory to the cube
/// </summary>
void EmptyCube::ProcessNextCubeItem()
{
	// Initial 'next' item is -1, so it's the first element after incrementing
	currentItemToMove++;

	// Check to see if we have any more items to process
	if(currentItemToMove >= (int)itemsInCube.size())
	{
		Abort();
		return;
	}

	// Pickup the item that needs to be moved from the cube to player's inventory
	currentState = STATE_PICKUPCUBEITEM;
	if(!me->PickStorageItemToCursor(itemsInCube[currentItemToMove]))
	{
		if(useChat)
		{
			me->Say("ÿc5CleanCubeÿc0: Failed to pickup item from cube");
		}
		server->GameStringf("ÿc5CleanCubeÿc0: Failed to pickup item from cube");

		Abort();
		return;
	}
}

/// <summary>
/// Aborts the EmptyCube process if it's currently running
/// </summary>
void EmptyCube::Abort()
{
	if(currentState != STATE_COMPLETE && currentState != STATE_UNINITIALIZED)
	{
		currentState = STATE_COMPLETE;
	}
}

/// <summary>
/// Called at a constant rate by d2hackit's timer
/// </summary>
void EmptyCube::OnTick()
{
	switch(currentState)
	{
		case STATE_COMPLETE:
		{
			currentState = STATE_UNINITIALIZED;

			if(useChat)
			{
				me->Say("ÿc5EmptyCubeÿc0: EmptyCube Ended");
			}
			else
			{
				server->GameStringf("ÿc5EmptyCubeÿc0: EmptyCube Ended");
			}

			break;
		}
	}
}

/// <summary>
/// Called whenever an item is picked up from the cube
/// </summary>
/// <param name="item">The item picked up.</param>
void EmptyCube::OnItemFromCube(DWORD itemID)
{
	// We only care about items from the cube when we're moving items from the cube
	//  to player's inventory
	if(currentState != STATE_PICKUPCUBEITEM)
	{
		return;
	}

	// Drop the item that was picked up from the cube to the player's inventory
	currentState = STATE_CUBEITEMTOINVENTORY;
	if(!me->DropItemToStorage(STORAGE_INVENTORY, itemID))
	{
		if(useChat)
		{
			me->Say("ÿc5EmptyCubeÿc0: Failed drop item to inventory");
		}
		server->GameStringf("ÿc5EmptyCubeÿc0: Failed drop item to inventory");

		Abort();
	}
}

/// <summary>
/// Called whenever an item is moved to the player's inventory
/// </summary>
/// <param name="item">The item moved to the player's inventory.</param>
void EmptyCube::OnItemToInventory(const ITEM &item)
{
	// We only care about this event when we're dropping the item from the cube to
	//   the player's inventory
	if(currentState != STATE_CUBEITEMTOINVENTORY)
	{
		return;
	}

	// Make sure the item that was moved to the inventory was actually from the cube,
	//   otherwise ignore it
	if(item.dwItemID != itemsInCube[currentItemToMove])
	{
		if(useChat)
		{
			me->Say("ÿc5EmptyCubeÿc0: Item to inventory not extracted item");
		}

		server->GameStringf("ÿc5EmptyCubeÿc0: Item to inventory not extracted item");
		return;
	}

	// Move the next item from the cube to inventory
	ProcessNextCubeItem();
}

/// <summary>
/// Stores each item ID found in the cube to the list if itemIds (lParam) 
/// </summary>
/// <param name="item">Item found in the player's cube.</param>
/// <param name="lParam">Pointer to list of itemIds for storing cube item IDs in.</param>
/// <returns>Always true.</returns>
BOOL CALLBACK enumFindCubeItems(LPCITEM item, LPARAM lParam)
{
	((std::vector<DWORD>*)lParam)->push_back(item->dwItemID);

	return TRUE;
}