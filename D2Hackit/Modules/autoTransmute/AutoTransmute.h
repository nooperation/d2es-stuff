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
	TransmuteTargetAndOrgan,
	RunEmptyCube
};

struct Recipe
{
	std::string name;
	int outputCount;
	bool emptyCube;
	std::unordered_map<std::string, int> Ingredients;
};

class AutoTransmute
{
public:
	AutoTransmute();
	bool Start(const std::string &recipeName);

	// Sequence:
	//   .at start hand
	//   Find 3x chipped frozen soul + 3x chipped shadow quartz
	//   /  / Pickup ingredient 1..N
	//   |  \ Drop ingredient 1..N to cube
	//   | Transmute
	//   | || EmptyCube (if requested)
	//   | || Wait for EmptyCube to finish (if requested)
	//   \ Wait for X items to cube then repeat step 2

	void PickupOrganFromInventory();
	void DropOrganToCube();

	void TransmuteTargetAndOrgan();

	void OnItemFromInventory(const ITEM &item);
	void OnItemToCube(const ITEM &item);

	void RunEmptyCube();
	bool OnEmptyCubeMessage(const std::string_view &message);

	void Abort();

	void ProcessInventoryItem(const ITEM *item);

private:
	void FindRecipe();
	
	std::pair<std::string, int> ReadRecipeConfig(std::stringstream &ss);
	bool ReadRecipeLine(std::string line, Recipe &recipe);
	bool ReadRecipes(const std::string &fileName, std::unordered_map<std::string, Recipe> &recipes);

	bool IsOrgan(const std::string_view &itemCode) const;

	void SetState(State newState);
	bool OpenCube();
	bool CheckCubeUI();

	State currentState;
	bool emptyCubeLoaded;

	std::unordered_map<std::string, Recipe> recipes;
	std::unordered_map<std::string, std::vector<DWORD>> organsIdsByType;
	Recipe currentRecipe;

	std::vector<DWORD> itemsBeingMovedToCube;
	DWORD itemWaitingOn;
	int expectedItemsToCube;
};

BOOL CALLBACK enumItemProc(LPCITEM item, LPARAM lParam);
