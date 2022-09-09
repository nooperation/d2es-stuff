#pragma once

#include <vector>
#include <unordered_map>
#include <windows.h>
#include "../../Includes/D2Client.h"

enum class State
{
	Uninitialized = 0,
	PickupItemToTransmuteFromInventory,
	DropOrganToCube,
	TransmuteTargetAndOrgan,
	RunEmptyCube,
	PickupItemFromCubeToDrop,
	DropItemToGround,
	PickupItemFromCubeToDelete,
};

enum class OutputAction {
	Invalid,
	DropToInventory,
	DropToGround,
	Delete,
};

struct Recipe
{
	std::string inputCode;
	std::string outputCode;
	OutputAction unexpectedOutputAction;
	OutputAction uniqueOutputAction;
	OutputAction rareOutputAction;
	OutputAction setOutputAction;
	OutputAction magicOutputAction;
	OutputAction normalOutputAction;
	int numOutputs;
};

class AutoChit
{
public:
	AutoChit();
	bool Start();

	void PickupItemToTransmuteFromInventory();
	void DropItemToTransmuteToCube();

	void TransmuteTarget();

	void OnItemFromInventory(const ITEM &item);
	void OnItemFromCube(const ITEM &item);
	void OnItemToCube(const ITEM &item);
	void OnItemDropped(DWORD itemId);

	void RunEmptyCube();
	bool OnEmptyCubeMessage(const std::string_view &message);

	void Abort();

	bool ProcessInventoryItem(const ITEM *item);

private:
	void FindRecipe();
	
	std::pair<std::string, std::string> ReadRecipeConfig(std::stringstream &ss);
	bool ReadRecipeLine(std::string line, Recipe &recipe);
	bool ReadRecipes(const std::string &fileName, std::unordered_map<std::string, Recipe> &recipes);

	bool IsOrgan(const std::string_view &itemCode) const;

	void HandlePostTransmuteAction(OutputAction action, const ITEM &item);

	void SetState(State newState);
	bool OpenCube();
	bool CheckCubeUI();

	State currentState;
	bool emptyCubeLoaded;

	std::unordered_map<std::string, Recipe> recipes;
	Recipe currentRecipe;

	DWORD itemIdToTransmute;
	DWORD itemIdToDrop;
	int expectedItemsToCube;
};

BOOL CALLBACK enumItemProc(LPCITEM item, LPARAM lParam);
