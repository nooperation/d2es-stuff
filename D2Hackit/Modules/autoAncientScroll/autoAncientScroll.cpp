#include "../../Includes/D2Client.h"
#include "AutoAncientScroll.h"
#include <string>

AutoAncientScroll::AutoAncientScroll()
{
	currentState = State::Uninitialized;
	this->autoExtractorLoaded = false;
	this->autoStockerLoaded = false;
	this->transmuteLoaded = false;
	this->emptyCubeLoaded = false;
}

void AutoAncientScroll::SetState(State newState)
{
	if (false)
	{
		const char * const stateNames[] = {
			"Uninitialized",
			"PickupMultistocker",
			"DropMultistockerToCube",
			"RunTransmute",
			"PickupKey",
			"DropKey",
			"RunAutoExtractor",
			"RunEmptyCube",
			"PickupNextAncientDecipherer",
			"DropNextAncientDecipherer",
			"PickupNextAncientScroll",
			"DropNextAncientScroll",
			"TransmuteScrollAndDecipherer",
			"PickupIdentifiedScroll",
			"DropIdentifiedScroll",
			"RunAutoStocker"
		};
		server->GameStringf("ÿc:AutoAncientScrollÿc0: State %s -> %s", stateNames[(int)this->currentState], stateNames[(int)newState]);
	}

	this->currentState = newState;
}

bool AutoAncientScroll::Start()
{
	// Clear previous settings
	ancientDeciphererIds.clear();
	ancientScrollIds.clear();
	lastScrollPosition.x = 0;
	lastScrollPosition.y = 0;
	keyId = 0;
	multistockerId = 0;
	itemWaitingOn = 0;

	// Notify the user about the current settings
	server->GameStringf("ÿc:AutoAncientScrollÿc0: Starting");

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
	if (!transmuteLoaded)
	{
		transmuteLoaded = true;
		server->GameCommandLine("load t");
	}	
	if (!emptyCubeLoaded)
	{
		emptyCubeLoaded = true;
		server->GameCommandLine("load emptycube");
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
	this->ancientDeciphererIds.clear();
	this->ancientScrollIds.clear();
	me->EnumStorageItems(STORAGE_INVENTORY, enumItemProc, (LPARAM)this);

	const auto numberOfFreeSlots = me->GetNumberOfFreeStorageSlots(STORAGE_INVENTORY);
	if (numberOfFreeSlots == 0)
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: Your inventory is full");
		Abort();
		return false;
	}

	if (this->keyId == 0)
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: You must have a key");
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

	this->SetState(State::PickupMultistocker);
	this->itemWaitingOn = this->multistockerId;
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

void AutoAncientScroll::RunTransmute()
{
	SetState(State::RunTransmute);
	server->GameCommandLine("t start dec chat");
}

bool AutoAncientScroll::OnTransmuteMessage(const std::string_view &message)
{
	if (this->currentState != State::RunTransmute)
	{
		return false;
	}

	if (message == "Transmute Ended")
	{
		PickupKey();
	}

	return true;
}

void AutoAncientScroll::PickupKey()
{
	SetState(State::PickupKey);
	this->itemWaitingOn = this->keyId;
	if(!me->PickStorageItemToCursor(keyId))
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: Failed to pick up key");

		Abort();
		return;
	}
}

void AutoAncientScroll::DropKey()
{
	if (me->GetCursorItem() != this->itemWaitingOn)
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: DropKey - Unknown cursor item. Expected the key");
		return;
	}

	SetState(State::DropKey);
	if(!me->DropCursorItemToStorage(STORAGE_CUBE))
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: Failed to drop the key");

		Abort();
		return;
	}
}

void AutoAncientScroll::RunAutoExtractor()
{
	auto numDeciphersToExtract = this->ancientScrollIds.size() - this->ancientDeciphererIds.size();
	if (numDeciphersToExtract < 0)
	{
		numDeciphersToExtract = 0;
	}

	const auto numberOfFreeSlots = me->GetNumberOfFreeStorageSlots(STORAGE_INVENTORY);
	if (numDeciphersToExtract > numberOfFreeSlots)
	{
		numDeciphersToExtract = numberOfFreeSlots;
	}

	SetState(State::RunAutoExtractor);
	std::string command = "ae start " + std::to_string(numDeciphersToExtract) + " chat";
	server->GameCommandLine((char *)command.c_str());
}

bool AutoAncientScroll::OnAutoExtractorMessage(const std::string_view &message)
{
	if (this->currentState != State::RunAutoExtractor)
	{
		return false;
	}

	if (message == "AutoExtractor Ended")
	{
		this->SetState(State::RunEmptyCube);
		server->GameCommandLine("emptycube start chat");
	}

	return true;
}

bool AutoAncientScroll::OnEmptyCubeMessage(const std::string_view &message)
{
	if (this->currentState != State::RunEmptyCube)
	{
		return false;
	}

	if (message == "EmptyCube Ended")
	{
		// Find the ancient decipherers now that we've extraced some
		this->ancientDeciphererIds.clear();
		this->ancientScrollIds.clear();
		me->EnumStorageItems(STORAGE_INVENTORY, enumItemProc, (LPARAM)this);
		this->PickupNextAncientDecipherer();
	}
	else if (message == "Failed drop item to inventory")
	{
		this->Abort();
	}

	return true;
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

	SetState(State::PickupNextAncientDecipherer);
	if (!me->PickStorageItemToCursor(itemWaitingOn))
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: Failed to pick up ancient decipherer");

		Abort();
		return;
	}
}

void AutoAncientScroll::DropNextAncientDecipherer()
{
	if (me->GetCursorItem() != this->itemWaitingOn)
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: DropKey - Unknown cursor item. Expected the ancient decipherer");
		return;
	}

	SetState(State::DropNextAncientDecipherer);
	if(!me->DropCursorItemToStorage(STORAGE_CUBE))
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: Failed to drop the ancient decipherer");

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

void AutoAncientScroll::TransmuteScrollAndDecipherer()
{
	// TODO: Check cube items for scroll+decipherer
	SetState(State::TransmuteScrollAndDecipherer);
	if(!me->Transmute())
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: Failed to transmute");

		Abort();
		return;
	}
}

void AutoAncientScroll::PickupIdentifiedScroll(DWORD identifiedScrollId)
{
	SetState(State::PickupIdentifiedScroll);
	this->itemWaitingOn = identifiedScrollId;
	if(!me->PickStorageItemToCursor(identifiedScrollId))
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: Failed to drop the identified scroll");

		Abort();
		return;
	}
}

void AutoAncientScroll::DropIdentifiedScroll()
{
	if (me->GetCursorItem() != this->itemWaitingOn)
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: DropKey - Unknown cursor item. Expected the identified ancient scroll");
		return;
	}

	SetState(State::DropIdentifiedScroll);
	if(!me->DropCursorItemToStorageEx(STORAGE_INVENTORY, this->lastScrollPosition))
	{
		server->GameStringf("ÿc:AutoAncientScrollÿc0: Failed to drop the identified scroll");

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
	else if (this->currentState == State::PickupKey)
	{
		if (itemCode != "key")
		{
			server->GameStringf("ÿc:AutoAncientScrollÿc0: I was expecting to pickup a key, but got a %s instead", itemCode.data());
			return;
		}

		this->DropKey();
		return;
	}
	else if (this->currentState == State::PickupNextAncientDecipherer)
	{
		if (itemCode != "ddd")
		{
			server->GameStringf("ÿc:AutoAncientScrollÿc0: I was expecting to pickup an ancient decipherer, but got a %s instead", itemCode.data());
			return;
		}

		this->DropNextAncientDecipherer();
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

		this->DropIdentifiedScroll();
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
	if (currentState == State::DropIdentifiedScroll)
	{
		if(!this->IsIdentifiedAncientScroll(itemCode))
		{
			server->GameStringf("ÿc:AutoAncientScrollÿc0: I was expecting to drop an identified ancient scroll, but got a %s instead", itemCode.data());
			return;
		}

		if (this->ancientDeciphererIds.empty() || this->ancientScrollIds.empty())
		{
			this->RunAutoStocker();
		}
		else
		{
			this->PickupNextAncientDecipherer();
		}

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

		this->RunTransmute();
		return;
	}
	else if (currentState == State::DropKey)
	{
		if (itemCode != "key")
		{
			server->GameStringf("ÿc:AutoAncientScrollÿc0: I was expecting to drop a key, but got a %s instead", itemCode.data());
			return;
		}

		this->RunAutoExtractor();
		return;
	}
	else if (currentState == State::DropNextAncientDecipherer)
	{
		if (itemCode != "ddd")
		{
			server->GameStringf("ÿc:AutoAncientScrollÿc0: I was expecting to drop an ancient decipherer, but got a %s instead", itemCode.data());
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

		this->TransmuteScrollAndDecipherer();
		return;
	}
	else if (currentState == State::TransmuteScrollAndDecipherer)
	{
		if (!this->IsIdentifiedAncientScroll(itemCode))
		{
			server->GameStringf("ÿc:AutoAncientScrollÿc0: I was expecting an identified ancient scroll, but got a %s instead", itemCode.data());
			return;
		}

		this->PickupIdentifiedScroll(item.dwItemID);
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

	if (itemCode == "ddd")
	{
		this->ancientDeciphererIds.push_back(item->dwItemID);
	}
	else if (IsAncientScroll(itemCode))
	{
		this->ancientScrollIds.push_back(item->dwItemID);
	}
	else if (IsMultistocker(itemCode))
	{
		this->multistockerId = item->dwItemID;
	}
	else if (itemCode == "key")
	{
		this->keyId = item->dwItemID;
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