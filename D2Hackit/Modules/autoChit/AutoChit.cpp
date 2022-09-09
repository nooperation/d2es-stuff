#include "../../Includes/D2Client.h"
#include "AutoChit.h"
#include <string>
#include <sstream>
#include <fstream>

AutoChit::AutoChit()
{
	currentState = State::Uninitialized;
	emptyCubeLoaded = false;
}

void AutoChit::SetState(State newState)
{
	if (false)
	{
		const char *const stateNames[] = {
			"Uninitialized",
			"PickupItemToTransmuteFromInventory",
			"DropOrganToCube",
			"TransmuteTargetAndOrgan",
			"RunEmptyCube"
		};
		server->GameStringf("ÿc:AutoChitÿc0: State %s -> %s", stateNames[(int)this->currentState], stateNames[(int)newState]);
	}

	this->currentState = newState;
}


std::pair<std::string, std::string> AutoChit::ReadRecipeConfig(std::stringstream &ss)
{
	std::string name;
	std::string value;

	if (ss.eof())
	{
		return std::make_pair("END", "");
	}

	std::getline(ss, name, '=');
	if (name == "")
	{
		ss.seekg(2, std::ios_base::cur);
		return std::make_pair("END", "");
	}

	ss >> value;
	if (ss.peek() == ' ') {
		ss.seekg(1, std::ios_base::cur);
	}

	return std::make_pair(name, value);
}

OutputAction ParseOutputAction(std::string action)
{
	if (action == "save") 
	{
		return OutputAction::DropToInventory;
	}
	if (action == "drop") 
	{
		return OutputAction::DropToGround;
	}
	if (action == "delete") 
	{
		return OutputAction::Delete;
	}
	
	return OutputAction::Invalid;
}

bool AutoChit::ReadRecipeLine(std::string line, Recipe &recipe)
{
	std::string inputsText;

	std::stringstream ss(line);

	int numInputs = 0;
	int numOutputs = 0;
	bool emptyCube = false;

	recipe.inputCode = "";
	recipe.outputCode = "";
	recipe.unexpectedOutputAction = OutputAction::Invalid;
	recipe.uniqueOutputAction = OutputAction::Invalid;
	recipe.rareOutputAction = OutputAction::Invalid;
	recipe.setOutputAction = OutputAction::Invalid;
	recipe.magicOutputAction = OutputAction::Invalid;
	recipe.normalOutputAction = OutputAction::Invalid;
	recipe.numOutputs = 1;

	while (true)
	{
		const auto [name, value] = ReadRecipeConfig(ss);

		if (name == "EOF")
		{
			server->GameStringf("ÿc:AutoChitÿc0: Failed to parse config line '%s'\n", line.c_str());
			return false;
		}
		else if (name == "END")
		{
			break;
		}
		else if (name == "Input")
		{
			if (value.size() != 3)
			{
				server->GameStringf("ÿc:AutoChitÿc0: Expected item code of length 3, but got length %d for '%s'\n", value.size(), value.c_str());
				return false;
			}

			recipe.inputCode = value;
		}
		else if (name == "Output")
		{
			if (value.size() != 3 && value != "?")
			{
				server->GameStringf("ÿc:AutoChitÿc0: Expected item code of length 3 (or 'anything'), but got length %d for '%s'\n", value.size(), value.c_str());
				return false;
			}

			if (value == "?")
			{
				recipe.outputCode = "";
			}
			else
			{
				recipe.outputCode = value;
			}
		}		
		else if (name == "UnexpectedOutput")
		{
			recipe.unexpectedOutputAction = ParseOutputAction(value);
			if (recipe.unexpectedOutputAction == OutputAction::Invalid)
			{
				server->GameStringf("ÿc:AutoChitÿc0: Unexpected action '%s' for UnexpectedOutput. Valid actions are save,drop,delete\n", value.c_str());
				return false;
			}
		}
		else if (name == "Unique")
		{
			recipe.uniqueOutputAction = ParseOutputAction(value);
			if (recipe.uniqueOutputAction == OutputAction::Invalid)
			{
				server->GameStringf("ÿc:AutoChitÿc0: Unexpected action '%s' for Unique. Valid actions are save,drop,delete\n", value.c_str());
				return false;
			}
		}
		else if (name == "Rare")
		{
			recipe.rareOutputAction = ParseOutputAction(value);
			if (recipe.rareOutputAction == OutputAction::Invalid)
			{
				server->GameStringf("ÿc:AutoChitÿc0: Unexpected action '%s' for Rare. Valid actions are save,drop,delete\n", value.c_str());
				return false;
			}
		}
		else if (name == "Set")
		{
			recipe.setOutputAction = ParseOutputAction(value);
			if (recipe.setOutputAction == OutputAction::Invalid)
			{
				server->GameStringf("ÿc:AutoChitÿc0: Unexpected action '%s' for Set. Valid actions are save,drop,delete\n", value.c_str());
				return false;
			}
		}
		else if (name == "Magic")
		{
			recipe.magicOutputAction = ParseOutputAction(value);
			if (recipe.magicOutputAction == OutputAction::Invalid)
			{
				server->GameStringf("ÿc:AutoChitÿc0: Unexpected action '%s' for Magic. Valid actions are save,drop,delete\n", value.c_str());
				return false;
			}
		}
		else if (name == "Normal")
		{
			recipe.normalOutputAction = ParseOutputAction(value);
			if (recipe.normalOutputAction == OutputAction::Invalid)
			{
				server->GameStringf("ÿc:AutoChitÿc0: Unexpected action '%s' for Normal. Valid actions are save,drop,delete\n", value.c_str());
				return false;
			}
		}
	}

	if (recipe.inputCode == "" ||
		recipe.unexpectedOutputAction == OutputAction::Invalid ||
		recipe.uniqueOutputAction == OutputAction::Invalid ||
		recipe.rareOutputAction == OutputAction::Invalid ||
		recipe.setOutputAction == OutputAction::Invalid ||
		recipe.magicOutputAction == OutputAction::Invalid ||
		recipe.normalOutputAction == OutputAction::Invalid) 
	{
		server->GameStringf("ÿc:AutoChitÿc0: invalid recipe line: %s\n", line.c_str());
		return false;
	}

	return true;
}

bool AutoChit::ReadRecipes(const std::string &fileName, std::unordered_map<std::string, Recipe> &recipes)
{
	recipes.clear();

	std::ifstream inFile(fileName.c_str());
	if (!inFile)
	{
		server->GameStringf("ÿc:AutoChitÿc0: Failed to open %s", fileName.c_str());
		this->Abort();
		return false;
	}

	while (inFile.good())
	{
		std::string readBuff;
		std::getline(inFile, readBuff);
		if (readBuff.length() <= 0 || readBuff[0] == ';' || readBuff[0] == '/')
		{
			continue;
		}

		Recipe recipe;
		if (!ReadRecipeLine(readBuff, recipe))
		{
			continue;
		}

		recipes[recipe.inputCode] = recipe;
	}

	inFile.close();
	return true;
}

bool AutoChit::Start()
{
	server->GameStringf("ÿc:AutoChitÿc0: Starting");

	ReadRecipes(".//plugin//AutoChit_Chits.txt", this->recipes);

	this->expectedItemsToCube = 0;
	this->itemIdToTransmute = -1;
	this->itemIdToDrop = -1;

	if (!emptyCubeLoaded)
	{
		emptyCubeLoaded = true;
		server->GameCommandLine("load emptycube");
	}

	// Notify the user about the current settings
	server->GameStringf("ÿc:AutoChitÿc0: Starting");

	// Open the player's cube
	if (!OpenCube())
	{
		Abort();
		return false;
	}

	FindRecipe();

	return true;
}

void AutoChit::FindRecipe()
{
	this->itemIdToTransmute = -1;

	me->EnumStorageItems(STORAGE_INVENTORY, enumItemProc, (LPARAM)this);
	PickupItemToTransmuteFromInventory();
}

void AutoChit::PickupItemToTransmuteFromInventory()
{
	if (itemIdToTransmute == -1)
	{
		server->GameStringf("ÿc:AutoChitÿc0: No more items to move to the cube");
		Abort();
		return;
	}

	SetState(State::PickupItemToTransmuteFromInventory);
	if (!me->PickStorageItemToCursor(itemIdToTransmute))
	{
		server->GameStringf("ÿc:AutoChitÿc0: Failed to pick up item");

		Abort();
		return;
	}
}

void AutoChit::DropItemToTransmuteToCube()
{
	if (me->GetCursorItem() != this->itemIdToTransmute)
	{
		server->GameStringf("ÿc:AutoChitÿc0: DropOrganToCube - Unknown cursor item. Expected the organ");
		return;
	}

	SetState(State::DropOrganToCube);
	if (!me->DropCursorItemToStorage(STORAGE_CUBE))
	{
		server->GameStringf("ÿc:AutoChitÿc0: Failed to drop the organ");

		Abort();
		return;
	}
}

void AutoChit::TransmuteTarget()
{
	this->expectedItemsToCube = this->currentRecipe.numOutputs;
	SetState(State::TransmuteTargetAndOrgan);
	if (!me->Transmute())
	{
		server->GameStringf("ÿc:AutoChitÿc0: Failed to transmute");

		Abort();
		return;
	}
}


/// <summary>
/// Called whenever an item is pickedup to the cursor from the player's inventory
/// </summary>
/// <param name="item">Item picked up from the player's inventory.</param>
void AutoChit::OnItemFromInventory(const ITEM &item)
{
	if (this->currentState == State::Uninitialized)
	{
		return;
	}

	const auto itemCode = std::string_view(item.szItemCode);

	if (this->currentState == State::PickupItemToTransmuteFromInventory)
	{
		if (item.dwItemID != this->itemIdToTransmute)
		{
			server->GameStringf("ÿc:AutoChitÿc0: I was expecting to pickup our item to transmute, but got a %s instead", itemCode.data());
			return;
		}

		this->DropItemToTransmuteToCube();
		return;
	}
}

void AutoChit::OnItemDropped(DWORD itemId)
{
	if (this->currentState != State::DropItemToGround)
	{
		return;
	}

	if (itemId != this->itemIdToDrop)
	{
		return;
	}

	itemIdToDrop = -1;
	this->FindRecipe();
}

void AutoChit::OnItemFromCube(const ITEM &item)
{
	if (this->currentState == State::Uninitialized)
	{
		return;
	}

	if (this->currentState == State::PickupItemFromCubeToDrop)
	{
		SetState(State::DropItemToGround);
		this->itemIdToDrop = item.dwItemID;
		me->DropCursorItemToGround();
		return;
	}
	if (this->currentState == State::PickupItemFromCubeToDelete)
	{
		server->GameStringf("ÿc:AutoChitÿc0: Delete item is not implemented.");
		this->Abort();
		return;
	}
}

/// <summary>
/// Called whenever an item is moved to the cube
/// </summary>
/// <param name="item">The item that was moved to the cube.</param>
void AutoChit::OnItemToCube(const ITEM &item)
{
	if (this->currentState == State::Uninitialized)
	{
		return;
	}

	const auto itemCode = std::string_view(item.szItemCode);

	if (this->currentState == State::DropOrganToCube)
	{
		if (itemCode != this->currentRecipe.inputCode)
		{
			server->GameStringf("ÿc:AutoChitÿc0: Cube was expecting item code %s, but got a %s instead", this->currentRecipe.inputCode, itemCode.data());
			return;
		}

		TransmuteTarget();
		return;
	}
	else if (currentState == State::TransmuteTargetAndOrgan)
	{
		this->expectedItemsToCube--;
		if (expectedItemsToCube <= 0)
		{
			if (this->currentRecipe.outputCode != "" &&  itemCode != this->currentRecipe.inputCode)
			{
				HandlePostTransmuteAction(this->currentRecipe.unexpectedOutputAction, item);
				return;
			}

			switch (item.iQuality)
			{
				case ITEM_LEVEL_UNIQUE:
					HandlePostTransmuteAction(this->currentRecipe.uniqueOutputAction, item);
					return;
				case ITEM_LEVEL_RARE:
					HandlePostTransmuteAction(this->currentRecipe.rareOutputAction, item);
					return;
				case ITEM_LEVEL_SET:
					HandlePostTransmuteAction(this->currentRecipe.setOutputAction, item);
					return;
				case ITEM_LEVEL_MAGIC:
					HandlePostTransmuteAction(this->currentRecipe.magicOutputAction, item);
					return;
				case ITEM_LEVEL_NORMAL:
					HandlePostTransmuteAction(this->currentRecipe.normalOutputAction, item);
					return;
				default:
					server->GameStringf("ÿc:AutoChitÿc0: We got a weird item type to the cube and I don't know what to do");
					this->Abort();
					return;
			}
		}
		return;
	}
}

void AutoChit::HandlePostTransmuteAction(OutputAction action, const ITEM &item) 
{
	switch (action)
	{
	case OutputAction::DropToInventory:
		RunEmptyCube();
		return;
	case OutputAction::DropToGround:
		SetState(State::PickupItemFromCubeToDrop);
		if (!me->PickStorageItemToCursor(item.dwItemID))
		{
			server->GameStringf("ÿc:AutoChitÿc0: Failed to pick up item to drop");

			Abort();
			return;
		}
		return;
	case OutputAction::Delete:
		SetState(State::PickupItemFromCubeToDelete);
		if (!me->PickStorageItemToCursor(item.dwItemID))
		{
			server->GameStringf("ÿc:AutoChitÿc0: Failed to pick up item to delete");

			Abort();
			return;
		}
		return;
	default:
		server->GameStringf("ÿc:AutoChitÿc0: Invalid action found. This shouldn't happen. Check your config file");
		this->Abort();
		return;
	}
}

void AutoChit::RunEmptyCube()
{
	SetState(State::RunEmptyCube);
	server->GameCommandLine("emptycube start chat");
}

bool AutoChit::OnEmptyCubeMessage(const std::string_view &message)
{
	if (currentState != State::RunEmptyCube)
	{
		return false;
	}

	if (message == "EmptyCube Ended")
	{
		// We've emptied the cube like the current recipe required. Time to repeate the whole process again
		this->FindRecipe();
	}

	return true;
}

bool AutoChit::OpenCube()
{
	if (!me->OpenCube())
	{
		server->GameStringf("ÿc:AutoChitÿc0: You must open your cube before using");

		Abort();
		return false;
	}

	return true;
}

bool AutoChit::CheckCubeUI()
{
	if (!me->IsUIOpened(UI_CUBE))
	{
		server->GameStringf("ÿc:AutoChitÿc0: Cube UI closed, aborting");
		Abort();
	}

	return true;
}

void AutoChit::Abort()
{
	if (currentState != State::Uninitialized)
	{
		server->GameStringf("ÿc:AutoChitÿc0: Done");
		SetState(State::Uninitialized);
	}
}

bool AutoChit::ProcessInventoryItem(const ITEM *item)
{
	auto recipeIter = this->recipes.find(item->szItemCode);
	if (recipeIter != this->recipes.end())
	{
		this->itemIdToTransmute = item->dwItemID;
		this->currentRecipe = recipeIter->second;
		return true;
	}

	return false;
}

BOOL CALLBACK enumItemProc(LPCITEM item, LPARAM lParam)
{
	auto autoChit = (AutoChit *)lParam;
	
	if (autoChit->ProcessInventoryItem(item)) {
		return FALSE;
	}

	return TRUE;
}
