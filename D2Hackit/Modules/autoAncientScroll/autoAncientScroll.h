#pragma once

#include <vector>
#include <windows.h>
#include "../../Includes/D2Client.h"

enum class State
{
	Uninitialized = 0,
	PickupMultistocker,
	DropMultistockerToCube,
	RunTransmute,
	PickupKey,
	DropKey,
	RunAutoExtractor,
	RunEmptyCube,
	PickupNextAncientDecipherer,
	DropNextAncientDecipherer,
	PickupNextAncientScroll,
	DropNextAncientScroll,
	TransmuteScrollAndDecipherer,
	PickupIdentifiedScroll,
	DropIdentifiedScroll,
	RunAutoStocker
};

class AutoAncientScroll
{
public:
	AutoAncientScroll();
	bool Start();

	// Sequence:
	//   Pickup multistocker
	//   Drop multistocker to cube
	//   Transmute multistocker to ancient decipherer ".t start dec"
	//   Pickup key
	//   Drop key in cube
	//   Extract N ancient decipherers ".ae start N"
	//   Empty cube ".emptycube start"
	//   / Pickup Ancient Scroll
	//   | Drop Ancient scroll to cube
	//   | Move identified scroll to inventory (".ae start" maybe ?)
	//   \ Repeat until no more scrolls
	

	void PickupMultistocker();
	void DropMultistockerToCube();

	void RunTransmute();
	bool OnTransmuteMessage(const std::string_view &message);

	void PickupKey();
	void DropKey();

	void RunAutoExtractor();
	bool OnAutoExtractorMessage(const std::string_view &message);

	bool OnEmptyCubeMessage(const std::string_view &message);

	void PickupNextAncientDecipherer();
	void DropNextAncientDecipherer();

	void PickupNextAncientScroll();
	void DropNextAncientScroll();

	void TransmuteScrollAndDecipherer();

	void PickupIdentifiedScroll(DWORD identifiedScrollId);
	void DropIdentifiedScroll();

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

	bool autoExtractorLoaded;
	bool autoStockerLoaded;
	bool transmuteLoaded;
	bool emptyCubeLoaded;
	State currentState;

	std::vector<DWORD> ancientDeciphererIds;
	std::vector<DWORD> ancientScrollIds;
	DWORD multistockerId;
	DWORD keyId;

	DWORD itemWaitingOn;
	POINT lastScrollPosition;
};

BOOL CALLBACK enumItemProc(LPCITEM item, LPARAM lParam);
BOOL CALLBACK enumItemCountProc(LPCITEM item, LPARAM lParam);
