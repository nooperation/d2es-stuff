#include "../../Includes/D2Client.h"
#include "AutoAncientScroll.h"
#include <string>

AutoAncientScroll::AutoAncientScroll()
{
	currentState = State::Uninitialized;
	this->autoStockerLoaded = false;
}

void AutoAncientScroll::SetState(State newState)
{
	if (false)
	{
		const char * const stateNames[] = {
			"Uninitialized ",
			"Initializing",
			"PickupMultistocker",
			"DropMultistockerToCube",
			"PickupNextAncientScroll",
			"DropNextAncientScroll",
			"TransmuteScrollAndStocker",
			"PickupIdentifiedScroll",
			"DropIdentifiedScrollToInventory",
			"PickupMultistockerFromCube",
			"DropMultistockerToInventory",
			"RunAutoStocker",
		};
		server->GameStringf("ÿc:AutoAncientScrollÿc0: State %s -> %s", stateNames[(int)this->currentState], stateNames[(int)newState]);
	}

	this->currentState = newState;
}

bool AutoAncientScroll::Start(bool useChat)
{
	SetState(State::Initializing);
	// Clear previous settings
	ancientScrollIds.clear();
	lastScrollPosition.x = 0;
	lastScrollPosition.y = 0;
	lastMultistockerPosition.x = 0;
	lastMultistockerPosition.y = 0;
	multistockerId = 0;
	itemWaitingOn = 0;
	numItemsRemainingToAppearInCube = 0;
	this->useChat = useChat;

	// Notify the user about the current settings
	if (useChat)
		me->Say("ÿc:AutoAncientScrollÿc0: Starting");
	else
		server->GameStringf("ÿc:AutoAncientScrollÿc0: Starting");

	// Load required modules
	if (!autoStockerLoaded)
	{
		autoStockerLoaded = true;
		server->GameCommandLine("load as");
	}	

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
	this->ancientScrollIds.clear();
	me->EnumStorageItems(STORAGE_INVENTORY, enumItemProc, (LPARAM)this);

	if (ancientScrollIds.empty())
	{
		Abort();
		return true;
	}

	const auto numberOfFreeSlots = me->GetNumberOfFreeStorageSlots(STORAGE_INVENTORY);
	if (numberOfFreeSlots == 0)
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: Your inventory is full");
		Abort();
		return false;
	}

	if (this->multistockerId == 0)
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: You must have a multistocker with ancient decipherer charges");
		Abort();
		return false;
	}

	PickupMultistocker();

	return true;
}

void AutoAncientScroll::PickupMultistocker()
{
	if (this->multistockerId == 0)
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: PickupMultistocker - Invalid multistocker ID");
		return;
	}

	this->itemWaitingOn = this->multistockerId;

	if (!me->FindItemPosition(STORAGE_INVENTORY, itemWaitingOn, &lastMultistockerPosition))
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: Failed to find position of next ancient scroll");
	}

	this->SetState(State::PickupMultistocker);
	if(!me->PickStorageItemToCursor(this->multistockerId))
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: Failed to pick up the multistocker");

		Abort();
		return;
	}
}

void AutoAncientScroll::DropMultistockerToCube()
{
	if (me->GetCursorItem() != this->itemWaitingOn)
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: DropMultistockerToCube - Unknown cursor item. Expected the multistocker");
		return;
	}

	this->SetState(State::DropMultistockerToCube);
	if(!me->DropCursorItemToStorage(STORAGE_CUBE))
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: Failed to drop the multistocker");

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

	SetState(State::PickupNextAncientScroll);
	if (!me->PickStorageItemToCursor(itemWaitingOn))
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: Failed to pick up ancient scroll");

		Abort();
		return;
	}
}

void AutoAncientScroll::DropNextAncientScroll()
{
	if (me->GetCursorItem() != this->itemWaitingOn)
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: DropKey - Unknown cursor item. Expected the ancient scroll");
		return;
	}

	SetState(State::DropNextAncientScroll);
	if(!me->DropCursorItemToStorage(STORAGE_CUBE))
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: Failed to drop the ancient scroll");

		Abort();
		return;
	}
}

void AutoAncientScroll::TransmuteScrollAndStocker()
{
	numItemsRemainingToAppearInCube = 2;
	SetState(State::TransmuteScrollAndStocker);
	if(!me->Transmute())
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: Failed to transmute");

		Abort();
		return;
	}
}

void AutoAncientScroll::PickupIdentifiedScroll(DWORD itemId)
{
	SetState(State::PickupIdentifiedScroll);
	this->itemWaitingOn = itemId;
	if(!me->PickStorageItemToCursor(itemId))
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: Failed to drop the identified scroll");

		Abort();
		return;
	}
}

void AutoAncientScroll::DropIdentifiedScrollToInventory()
{
	if (me->GetCursorItem() != this->itemWaitingOn)
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: DropIdentifiedScrollToInventory - Unknown cursor item. Expected the identified ancient scroll");
		return;
	}

	SetState(State::DropIdentifiedScrollToInventory);
	if(!me->DropCursorItemToStorageEx(STORAGE_INVENTORY, this->lastScrollPosition))
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: Failed to drop the identified scroll");

		Abort();
		return;
	}
}

void AutoAncientScroll::PickupMultistockerFromCube(DWORD itemId)
{
	SetState(State::PickupMultistockerFromCube);
	this->itemWaitingOn = itemId;
	if (!me->PickStorageItemToCursor(itemId))
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: Failed to drop the identified scroll");

		Abort();
		return;
	}
}

void AutoAncientScroll::DropMultistockerToInventory()
{
	if (me->GetCursorItem() != this->itemWaitingOn)
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: DropMultistockerToInventory - Unknown cursor item. Expected the multistocker");
		return;
	}

	SetState(State::DropMultistockerToInventory);
	if (!me->DropCursorItemToStorageEx(STORAGE_INVENTORY, lastMultistockerPosition))
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: Failed to drop the multistocker");

		Abort();
		return;
	}
}

void AutoAncientScroll::RunAutoStocker()
{
	SetState(State::RunAutoStocker);
	server->GameCommandLine("as start chat");
}

bool AutoAncientScroll::OnAutoStockerMessage(const std::string_view &message)
{
	if (currentState != State::RunAutoStocker)
	{
		return false;
	}

	if (message == "Autostocker Ended")
	{
		Abort();
	}

	return true;
}


/// <summary>
/// Called whenever an item is pickedup to the cursor from the player's inventory
/// </summary>
/// <param name="item">Item picked up from the player's inventory.</param>
void AutoAncientScroll::OnItemFromInventory(const ITEM &item)
{
	if (this->currentState == State::Uninitialized)
	{
		return;
	}

	const auto itemCode = std::string_view(item.szItemCode);

	if (this->currentState == State::PickupMultistocker)
	{
		if (!this->IsMultistocker(itemCode))
		{
			server->GameStringf("ÿc:AutoAncientScrollÿc0: I was expecting to pickup a multistocker, but got a %s instead", itemCode.data());
			return;
		}

		this->DropMultistockerToCube();
		return;
	}	
	else if (this->currentState == State::PickupNextAncientScroll)
	{
		if (!this->IsAncientScroll(itemCode))
		{
			server->GameStringf("ÿc:AutoAncientScrollÿc0: I was expecting to pickup an ancient scroll, but got a %s instead", itemCode.data());
			return;
		}

		this->DropNextAncientScroll();
		return;
	}
}

/// <summary>
/// Called whenever an item is pickedup to the cursor from the cube
/// </summary>
/// <param name="item">Item picked up from the cube.</param>
void AutoAncientScroll::OnItemFromCube(const ITEM &item)
{
	if (this->currentState == State::Uninitialized)
	{
		return;
	}

	const auto itemCode = std::string_view(item.szItemCode);

	if (this->currentState == State::PickupIdentifiedScroll)
	{
		if (!this->IsIdentifiedAncientScroll(itemCode))
		{
			server->GameStringf("ÿc:AutoAncientScrollÿc0: I was expecting to pickup an identified ancient scroll, but got a %s instead", itemCode.data());
			return;
		}

		this->DropIdentifiedScrollToInventory();
		return;
	}
	if (this->currentState == State::PickupMultistockerFromCube)
	{
		if (!this->IsMultistocker(itemCode))
		{
			server->GameStringf("ÿc:AutoAncientScrollÿc0: I was expecting to pickup multistocker from cube, but got a %s instead", itemCode.data());
			return;
		}

		this->DropMultistockerToInventory();
		return;
	}
}

/// <summary>
/// Called whenever an item is moved to the player's inventory
/// </summary>
/// <param name="item">The item that was moved to the player's inventory.</param>
void AutoAncientScroll::OnItemToInventory(const ITEM &item)
{
	if (this->currentState == State::Uninitialized)
	{
		return;
	}

	const auto itemCode = std::string_view(item.szItemCode);

	// We only care about this event when we're moving our stocker back to the player's inventory
	if (currentState == State::DropIdentifiedScrollToInventory)
	{
		if(!this->IsIdentifiedAncientScroll(itemCode))
		{
			server->GameStringf("ÿc:AutoAncientScrollÿc0: I was expecting to drop an identified ancient scroll, but got a %s instead", itemCode.data());
			return;
		}

		if (this->ancientScrollIds.empty())
		{
			this->PickupMultistockerFromCube(multistockerId);
		}
		else 
		{
			PickupNextAncientScroll();
		}

		return;
	}
	if (currentState == State::DropMultistockerToInventory)
	{
		if (item.dwItemID != this->itemWaitingOn)
		{
			server->GameStringf("ÿc:AutoAncientScrollÿc0: I was expecting to drop our multistocker, but got a %s instead", itemCode.data());
			return;
		}

		this->RunAutoStocker();
		return;
	}
}

/// <summary>
/// Called whenever an item is moved to the cube
/// </summary>
/// <param name="item">The item that was moved to the cube.</param>
void AutoAncientScroll::OnItemToCube(const ITEM &item)
{
	if (this->currentState == State::Uninitialized)
	{
		return;
	}

	const auto itemCode = std::string_view(item.szItemCode);

	if (this->currentState == State::DropMultistockerToCube)
	{
		if (!this->IsMultistocker(itemCode))
		{
			server->GameStringf("ÿc:AutoAncientScrollÿc0: I was expecting to drop a multistocker, but got a %s instead", itemCode.data());
			return;
		}

		this->PickupNextAncientScroll();
		return;
	}
	else if (currentState == State::DropNextAncientScroll)
	{
		if (!this->IsAncientScroll(itemCode))
		{
			server->GameStringf("ÿc:AutoAncientScrollÿc0: I was expecting to drop an ancient scroll, but got a %s instead", itemCode.data());
			return;
		}

		this->TransmuteScrollAndStocker();
		return;
	}
	else if (currentState == State::TransmuteScrollAndStocker)
	{
		if (this->IsMultistocker(itemCode))
		{
			multistockerId = item.dwItemID;
			--numItemsRemainingToAppearInCube;
		}
		else if (this->IsIdentifiedAncientScroll(itemCode))
		{
			identifiedScrollId = item.dwItemID;
			--numItemsRemainingToAppearInCube;
		}
		else 
		{
			server->GameStringf("ÿc:AutoAncientScrollÿc0: I was expecting an identified ancient scroll or stocker, but got a %s instead", itemCode.data());
			return;
		}

		if (numItemsRemainingToAppearInCube == 0) 
		{
			this->PickupIdentifiedScroll(identifiedScrollId);
		}

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
		if (useChat)
		{
			me->Say("ÿc:AutoAncientScrollÿc0: Ended");
		}
		else
		{
			server->GameStringf("ÿc:AutoAncientScrollÿc0: Ended");
		}

		SetState(State::Uninitialized);
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

bool AutoAncientScroll::IsMultistocker(const std::string_view &itemCode) const
{
	const auto itemCode0 = itemCode[0];
	const auto itemCode1 = itemCode[1];
	const auto itemCode2 = itemCode[2];

	if (itemCode0 == 't' && isdigit(itemCode1) && isdigit(itemCode2))
	{
		const auto itemCodeNum = atoi(&itemCode[1]);

		if (itemCodeNum >= 51 && itemCodeNum <= 75)
		{
			return true;
		}
	}

	return false;
}

bool AutoAncientScroll::IsAncientScroll(const std::string_view &itemCode) const
{
	const auto itemCode0 = itemCode[0];
	const auto itemCode1 = itemCode[1];
	const auto itemCode2 = itemCode[2];

	if (itemCode0 >= '0' && itemCode0 <= '5' && itemCode1 >= '0' && itemCode1 <= '9' && itemCode2 == 'b')
	{
		return true;
	}

	return false;
}

bool AutoAncientScroll::IsIdentifiedAncientScroll(const std::string_view &itemCode) const
{
	const auto itemCode0 = itemCode[0];
	const auto itemCode1 = itemCode[1];
	const auto itemCode2 = itemCode[2];

	if (itemCode0 >= '0' && itemCode0 <= '5' && itemCode1 >= '0' && itemCode1 <= '9' && itemCode2 == 'a')
	{
		return true;
	}

	return false;
}

void AutoAncientScroll::ProcessInventoryItem(const ITEM *item)
{
	const auto itemCode = std::string_view(item->szItemCode);

	if (IsAncientScroll(itemCode))
	{
		this->ancientScrollIds.push_back(item->dwItemID);
	}
	else if (IsMultistocker(itemCode))
	{
		this->multistockerId = item->dwItemID;
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