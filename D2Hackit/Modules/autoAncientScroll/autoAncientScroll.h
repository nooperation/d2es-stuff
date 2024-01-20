#pragma once

#include <vector>
#include <windows.h>
#include "../../Includes/D2Client.h"

enum class State
{
	Uninitialized = 0,
	Initializing,
	PickupMultistocker,
	DropMultistockerToCube,
	PickupNextAncientScroll,
	DropNextAncientScroll,
	TransmuteScrollAndStocker,
	PickupIdentifiedScroll,
	DropIdentifiedScrollToInventory,
	PickupMultistockerFromCube,
	DropMultistockerToInventory,
	RunAutoStocker
};

class AutoAncientScroll
{
public:
	AutoAncientScroll();
	bool Start(bool useChat);

	// Sequence:
	//   Pickup multistocker
	//   Drop multistocker to cube
	//   / Pickup Ancient Scroll
	//   | Drop Ancient scroll to cube
	//   | Transmute
	//   | Move identified scroll to inventory, ideally not the multistocker position
	//   \ Repeat until no more scrolls
	//   Pickup multistocker from cube
	//   Drop multistocker to inventory (ideally the original position)
	

	void PickupMultistocker();
	void DropMultistockerToCube();

	void PickupNextAncientScroll();
	void DropNextAncientScroll();

	void TransmuteScrollAndStocker();

	void PickupIdentifiedScroll(DWORD itemId);
	void DropIdentifiedScrollToInventory();

	void DropMultistockerToInventory();
	void PickupMultistockerFromCube(DWORD itemId);

	void RunAutoStocker();
	bool OnAutoStockerMessage(const std::string_view &message);

	void OnItemFromInventory(const ITEM &item);
	void OnItemToCube(const ITEM &item);
	void OnItemToInventory(const ITEM &item);
	void OnItemFromCube(const ITEM &item);
	void Abort();

	void ProcessInventoryItem(const ITEM *item);

private:
	bool IsMultistocker(const std::string_view &itemCode) const;
	bool IsAncientScroll(const std::string_view &itemCode) const;
	bool IsIdentifiedAncientScroll(const std::string_view &itemCode) const;

	void SetState(State newState);
	bool IsCubeEmpty();
	bool OpenCube();
	bool CheckCubeUI();

	bool autoStockerLoaded;
	State currentState;
	bool useChat;

	std::vector<DWORD> ancientScrollIds;

	DWORD multistockerId;

	DWORD itemWaitingOn;
	POINT lastScrollPosition;
	POINT lastMultistockerPosition;

	int numItemsRemainingToAppearInCube;
	DWORD identifiedScrollId;
};

BOOL CALLBACK enumItemProc(LPCITEM item, LPARAM lParam);
BOOL CALLBACK enumItemCountProc(LPCITEM item, LPARAM lParam);
