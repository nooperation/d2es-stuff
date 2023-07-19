#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <windows.h>
#include "../../Includes/D2Client.h"

enum class AutoJewelState
{
	Uninitialized = 0,
	PickupRingFromInventory,
	DropRingToCube,
	TransmuteRingAndStocker,
	PickupJewelFromCube,
	DropJewelToInventory,
};

class AutoJewel
{
public:
	AutoJewel();
	bool Start();

	// Sequence:
	//   .aj start
	//   Find unique ring in inventory
	//   /  / Pickup ring 1..N
	//   |  \ Drop ring 1..N to cube
	//   | Transmute
	//   \ Wait for 1 item to cube then repeat step 2

	void PickupRingFromInventory();
	void DropRingToCube();
	void TransmuteRingAndStocker();

	void OnItemFromInventory(const ITEM &item);
	void OnItemFromCube(const ITEM &item);
	void OnItemToInventory(const ITEM &item);
	void OnItemToCube(const ITEM &item);

	void Abort();

	void ProcessInventoryItem(const ITEM *item);
	bool ProcessCubeItem(const ITEM* item);

private:
	void FindRecipe();
	bool IsRing(const std::string_view &itemCode) const;
	bool IsOkayToDestroy(const ITEM &item) const;

	void SetState(AutoJewelState newState);
	bool OpenCube();
	bool CheckCubeUI();

	AutoJewelState currentState = AutoJewelState::Uninitialized;
	DWORD itemWaitingOn = 0;
	DWORD numRingsInCube = 0;
	DWORD numTransmuteResultsWaitingFor = 0;
	DWORD ancientScrollId = 0;
	DWORD jewelIdToMoveFromCubeToInventory = 0;
	std::vector<DWORD> targetRingsToTransmute;

	std::unordered_set<std::string> kRingItemCodes;
	std::unordered_set<int> kUniqueIdsWeCanDestroy;

};

BOOL CALLBACK enumItemProc(LPCITEM item, LPARAM lParam);
BOOL CALLBACK enumItemProcCube(LPCITEM item, LPARAM lParam);
