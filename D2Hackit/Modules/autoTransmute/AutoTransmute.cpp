#include "../../Includes/D2Client.h"
#include "AutoTransmute.h"
#include <string>
#include <sstream>
#include <fstream>

AutoTransmute::AutoTransmute()
{
	currentState = State::Uninitialized;
	emptyCubeLoaded = false;
}

void AutoTransmute::SetState(State newState)
{
	if (false)
	{
		const char *const stateNames[] = {
			"Uninitialized",
			"PickupOrganFromInventory",
			"DropOrganToCube",
			"TransmuteTargetAndOrgan",
			"RunEmptyCube"
		};
		server->GameStringf("ÿc:AutoTransmuteÿc0: State %s -> %s", stateNames[(int)this->currentState], stateNames[(int)newState]);
	}

	this->currentState = newState;
}


std::pair<std::string, int> AutoTransmute::ReadRecipeConfig(std::stringstream &ss)
{
	std::string name;
	int value;

	ss.seekg(1, std::ios_base::cur);
	if (ss.eof())
	{
		return std::make_pair("EOF", -1);
	}

	std::getline(ss, name, '=');
	if (name == "")
	{
		if (ss.peek() != '>')
		{
			return std::make_pair("EOF", -1);
		}

		ss.seekg(2, std::ios_base::cur);
		return std::make_pair("END", -1);
	}

	ss >> value;

	return std::make_pair(name, value);
}

bool AutoTransmute::ReadRecipeLine(std::string line, Recipe &recipe)
{
	std::string recipeName;
	std::string inputsText;

	std::stringstream ss(line);
	ss >> recipeName;

	int numInputs = 0;
	int numOutputs = 0;
	bool emptyCube = false;

	while (true)
	{
		const auto [name, value] = ReadRecipeConfig(ss);

		if (name == "EOF")
		{
			printf("Failed to parse config line '%s'\n", line.c_str());
			return false;
		}
		else if (name == "END")
		{
			break;
		}
		else if (name == "Inputs")
		{
			numInputs = value;
			if (numInputs < 0 || numInputs > 10)
			{
				printf("Abnormal numInputs %d\n", numInputs);
				return false;
			}
		}
		else if (name == "Outputs")
		{
			numOutputs = value;
			if (numOutputs < 0 || numOutputs > 10)
			{
				printf("Abnormal numOutputs %d\n", numOutputs);
				return false;
			}
		}
		else if (name == "EmptyCube")
		{
			emptyCube = value;
			if (value < 0 || value > 1)
			{
				return false;
			}
		}
	}

	recipe.name = recipeName;
	recipe.outputCount = numOutputs;
	recipe.emptyCube = emptyCube;
	recipe.Ingredients.clear();

	for (auto i = 0; i < numInputs; i++)
	{
		int itemCount = 0;
		std::string itemCode = "";

		ss >> itemCount >> itemCode;

		recipe.Ingredients[itemCode] = itemCount;
	}

	return true;
}

bool AutoTransmute::ReadRecipes(const std::string &fileName, std::unordered_map<std::string, Recipe> &recipes)
{
	recipes.clear();

	std::ifstream inFile(fileName.c_str());
	if (!inFile)
	{
		printf("Failed to open %s", fileName.c_str());
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

		recipes[recipe.name] = recipe;
	}

	inFile.close();
	return true;
}

bool AutoTransmute::Start(const std::string &recipeName)
{
	server->GameStringf("ÿc:AutoTransmuteÿc0: '%s'", recipeName.c_str());

	if (!ReadRecipes(".//plugin//AutoTransmute.txt", this->recipes)) 
	{
		server->GameStringf("ÿc:AutoTransmuteÿc0: Missing AutoTransmute.txt", recipeName.c_str());
		return false;
	}

	auto recipeIter = this->recipes.find(recipeName);
	if (recipeIter == this->recipes.end())
	{
		server->GameStringf("ÿc:AutoTransmuteÿc0: Unknown recipe '%s'", recipeName.c_str());
		return false;
	}

	this->currentRecipe = recipeIter->second;
	this->itemWaitingOn = 0;
	this->expectedItemsToCube = 0;
	this->itemsBeingMovedToCube.clear();

	if (!emptyCubeLoaded)
	{
		emptyCubeLoaded = true;
		server->GameCommandLine("load emptycube");
	}

	// Notify the user about the current settings
	server->GameStringf("ÿc:AutoTransmuteÿc0: Starting");

	// Open the player's cube
	if (!OpenCube())
	{
		Abort();
		return false;
	}

	FindRecipe();

	return true;
}

void AutoTransmute::FindRecipe()
{
	this->organsIdsByType.clear();
	this->itemsBeingMovedToCube.clear();

	me->EnumStorageItems(STORAGE_INVENTORY, enumItemProc, (LPARAM)this);

	if (this->organsIdsByType.empty())
	{
		Abort();
		return;
	}

	int numberOfSatisifedIngredients = 0;
	for (auto &item : this->organsIdsByType)
	{
		const auto requiredAmountIter = this->currentRecipe.Ingredients.find(item.first);
		if (requiredAmountIter == this->currentRecipe.Ingredients.end())
		{
			continue;
		}
		const auto requiredAmount = requiredAmountIter->second;

		if (item.second.size() < requiredAmount)
		{
			server->GameStringf("ÿc:AutoTransmuteÿc0: Not enough '%s' remaining", item.first.c_str());
			break;
		}

		for (auto i = 0; i < requiredAmount; ++i)
		{
			const auto organId = item.second.back();
			item.second.pop_back();

			this->itemsBeingMovedToCube.push_back(organId);
		}

		numberOfSatisifedIngredients++;
	}

	if (numberOfSatisifedIngredients != this->currentRecipe.Ingredients.size())
	{
		// Unable to satisfy all ingredients
		server->GameStringf("ÿc:AutoTransmuteÿc0: Only satisfied %d of %d required ingredients", numberOfSatisifedIngredients, this->currentRecipe.Ingredients.size());
		Abort();
		return;
	}

	PickupOrganFromInventory();
}

void AutoTransmute::PickupOrganFromInventory()
{
	if (this->itemsBeingMovedToCube.empty())
	{
		server->GameStringf("ÿc:AutoTransmuteÿc0: No more organs to move");
		Abort();
		return;
	}

	this->itemWaitingOn = itemsBeingMovedToCube.back();
	this->itemsBeingMovedToCube.pop_back();

	SetState(State::PickupOrganFromInventory);
	if (!me->PickStorageItemToCursor(itemWaitingOn))
	{
		server->GameStringf("ÿc:AutoTransmuteÿc0: Failed to pick up organ");

		Abort();
		return;
	}
}

void AutoTransmute::DropOrganToCube()
{
	if (me->GetCursorItem() != this->itemWaitingOn)
	{
		server->GameStringf("ÿc:AutoTransmuteÿc0: DropOrganToCube - Unknown cursor item. Expected the organ");
		return;
	}

	SetState(State::DropOrganToCube);
	if (!me->DropCursorItemToStorage(STORAGE_CUBE))
	{
		server->GameStringf("ÿc:AutoTransmuteÿc0: Failed to drop the organ");

		Abort();
		return;
	}
}

void AutoTransmute::TransmuteTargetAndOrgan()
{
	this->expectedItemsToCube = this->currentRecipe.outputCount;
	SetState(State::TransmuteTargetAndOrgan);
	if (!me->Transmute())
	{
		server->GameStringf("ÿc:AutoTransmuteÿc0: Failed to transmute");

		Abort();
		return;
	}
}


/// <summary>
/// Called whenever an item is pickedup to the cursor from the player's inventory
/// </summary>
/// <param name="item">Item picked up from the player's inventory.</param>
void AutoTransmute::OnItemFromInventory(const ITEM &item)
{
	if (this->currentState == State::Uninitialized)
	{
		return;
	}

	const auto itemCode = std::string_view(item.szItemCode);

	if (this->currentState == State::PickupOrganFromInventory)
	{
		if (!this->IsOrgan(itemCode))
		{
			server->GameStringf("ÿc:AutoTransmuteÿc0: I was expecting to pickup an organ, but got a %s instead", itemCode.data());
			return;
		}

		this->DropOrganToCube();
		return;
	}
}

/// <summary>
/// Called whenever an item is moved to the cube
/// </summary>
/// <param name="item">The item that was moved to the cube.</param>
void AutoTransmute::OnItemToCube(const ITEM &item)
{
	if (this->currentState == State::Uninitialized)
	{
		return;
	}

	const auto itemCode = std::string_view(item.szItemCode);

	if (this->currentState == State::DropOrganToCube)
	{
		if (!IsOrgan(itemCode))
		{
			server->GameStringf("ÿc:AutoTransmuteÿc0: Cube was expecting an organ, but got a %s instead", itemCode.data());
			return;
		}

		if (this->itemsBeingMovedToCube.empty())
		{
			TransmuteTargetAndOrgan();
		}
		else
		{
			PickupOrganFromInventory();
		}
		return;
	}
	else if (currentState == State::TransmuteTargetAndOrgan)
	{
		this->expectedItemsToCube--;
		if (expectedItemsToCube <= 0)
		{
			if (this->currentRecipe.emptyCube)
			{
				// Start to run the emptyCube process...
				RunEmptyCube();
			}
			else
			{
				// All of the expected items made it back to the cube. Time to repeate the whole process again
				FindRecipe();
			}
			return;
		}
		return;
	}
}

void AutoTransmute::RunEmptyCube()
{
	SetState(State::RunEmptyCube);
	server->GameCommandLine("emptycube start chat");
}

bool AutoTransmute::OnEmptyCubeMessage(const std::string_view &message)
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

bool AutoTransmute::OpenCube()
{
	if (!me->OpenCube())
	{
		server->GameStringf("ÿc:AutoTransmuteÿc0: You must open your cube before using");

		Abort();
		return false;
	}

	return true;
}

bool AutoTransmute::CheckCubeUI()
{
	if (!me->IsUIOpened(UI_CUBE))
	{
		server->GameStringf("ÿc:AutoTransmuteÿc0: Cube UI closed, aborting");
		Abort();
	}

	return true;
}

void AutoTransmute::Abort()
{
	if (currentState != State::Uninitialized)
	{
		server->GameStringf("ÿc:AutoTransmuteÿc0: Done");
		SetState(State::Uninitialized);
	}
}

bool AutoTransmute::IsOrgan(const std::string_view &itemCode) const
{
	return this->currentRecipe.Ingredients.find(itemCode.data()) != this->currentRecipe.Ingredients.end();
}

void AutoTransmute::ProcessInventoryItem(const ITEM *item)
{
	if (IsOrgan(item->szItemCode))
	{
		this->organsIdsByType[item->szItemCode].push_back(item->dwItemID);
	}
}

BOOL CALLBACK enumItemProc(LPCITEM item, LPARAM lParam)
{
	auto autoStocker = (AutoTransmute *)lParam;
	autoStocker->ProcessInventoryItem(item);

	return TRUE;
}
