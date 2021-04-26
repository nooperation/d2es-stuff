#pragma once

#include <vector>
#include <unordered_map>
#include <windows.h>
#include "../../Includes/D2Client.h"

enum class State
{
	Uninitialized = 0,
	PickupOrganFromInventory,
	DropOrganToCube,
	TransmuteTargetAndOrgan
};

class AutoTinker
{
public:
	AutoTinker();
	bool Start();

	// Sequence:
	//   / Find organs
	//   | Pickup organs
	//   | Drop organs to cube
	//   | Transmute
	//   \ Wait for single item to cube then repeat step 1
	
	void PickupOrganFromInventory();
	void DropOrganToCube();

	void TransmuteTargetAndOrgan();

	void OnItemFromInventory(const ITEM &item);
	void OnItemToCube(const ITEM &item);
	void Abort();

	void ProcessInventoryItem(const ITEM *item);

private:
	void FindRecipe();
	bool ReadRecipes(const std::string &fileName);

	bool IsOrgan(const std::string_view &itemCode) const;

	void SetState(State newState);
	bool IsCubeEmpty();
	bool OpenCube();
	bool CheckCubeUI();

	State currentState;

	std::unordered_map<std::string, int> requiredOrganCounts;
	std::unordered_map<std::string, std::vector<DWORD>> organsIdsByType;

	std::vector<DWORD> itemsBeingMovedToCube;
	DWORD itemWaitingOn;
};

BOOL CALLBACK enumItemProc(LPCITEM item, LPARAM lParam);
BOOL CALLBACK enumItemCountProc(LPCITEM item, LPARAM lParam);
