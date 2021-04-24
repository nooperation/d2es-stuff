#pragma once

#include <vector>
#include <windows.h>
#include "../../Includes/D2Client.h"

enum class State
{
	Uninitialized = 0,
	PickupAncientDecipherer,
	MoveAncientDeciphererToCube,
	PickupAncientScroll,
	MoveAncientScrollToCube,
	Transmute,
	PickupIdentifiedScrollFromCube,
	DropIdentifiedScrollToInventory
};

class AutoAncientScroll
{
public:
	AutoAncientScroll();
	bool Start();
	void OnItemFromInventory(const ITEM &item);
	void OnItemToCube(const ITEM &item);
	void OnItemToInventory(const ITEM &item);
	void OnItemFromCube(const ITEM &item);
	void Abort();

	void ProcessInventoryItem(const ITEM *item);


private:
	void IdentifyNextAncientScroll();

	bool IsCubeEmpty();
	void PickupNextAncientDecipherer();
	void PickupNextAncientScroll();
	bool OpenCube();
	bool CheckCubeUI();

	State currentState;

	std::vector<DWORD> ancientDeciphererIds;
	std::vector<DWORD> ancientScrollIds;

	DWORD itemWaitingOn;
	POINT lastScrollPosition;
};

BOOL CALLBACK enumItemProc(LPCITEM item, LPARAM lParam);
BOOL CALLBACK enumItemCountProc(LPCITEM item, LPARAM lParam);
