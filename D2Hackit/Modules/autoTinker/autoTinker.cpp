#include "../../Includes/D2Client.h"
#include "AutoTinker.h"
#include <string>
#include <sstream>
#include <fstream>

AutoTinker::AutoTinker()
{
	currentState = State::Uninitialized;
}

void AutoTinker::SetState(State newState)
{
	if (false)
	{
		const char * const stateNames[] = {
			"Uninitialized",
			"PickupOrganFromInventory",
			"DropOrganToCube",
			"TransmuteTargetAndOrgan"
		};
		server->GameStringf("ÿc:AutoTinkerÿc0: State %s -> %s", stateNames[(int)this->currentState], stateNames[(int)newState]);
	}

	this->currentState = newState;
}

bool AutoTinker::Start()
{
	ReadRecipes(".//plugin//autoTinker.txt");

	// Clear previous settings
	this->itemWaitingOn = 0;
	this->itemsBeingMovedToCube.clear();

	// Notify the user about the current settings
	server->GameStringf("ÿc:AutoTinkerÿc0: Starting");

	// Open the player's cube
	if (!OpenCube())
	{
		Abort();
		return false;
	}

	if (IsCubeEmpty())
	{
		server->GameStringf("ÿc:AutoTinkerÿc0: Please place an item in your cube that you want to tinker");
		Abort();
		return false;
	}

	FindRecipe();

	return true;
}

bool AutoTinker::ReadRecipes(const std::string &fileName)
{
	this->requiredOrganCounts.clear();

	std::ifstream inFile(fileName.c_str());
	if (!inFile)
	{
		return false;
	}

	while (inFile.good())
	{
		std::string readBuff;
		std::getline(inFile, readBuff);

		if (readBuff.length() <= 0)
		{
			continue;
		}

		if (!isdigit(readBuff[0]))
		{
			continue;
		}

		int itemCount;
		std::string itemCode;

		std::stringstream ss(readBuff);
		ss >> itemCount >> itemCode;

		this->requiredOrganCounts[itemCode] = itemCount;
	}

	inFile.close();
	return true;
}

void AutoTinker::FindRecipe()
{
	this->organsIdsByType.clear();
	this->itemsBeingMovedToCube.clear();

	me->EnumStorageItems(STORAGE_INVENTORY, enumItemProc, (LPARAM)this);

	if (this->organsIdsByType.empty())
	{
		Abort();
		return;
	}

	for (auto &item : this->organsIdsByType)
	{
		const auto requiredAmountIter = this->requiredOrganCounts.find(item.first);
		if (requiredAmountIter == this->requiredOrganCounts.end())
		{
			continue;
		}

		if (item.second.size() >= requiredAmountIter->second)
		{
			for (auto i = 0; i < requiredAmountIter->second; ++i)
			{
				const auto organId = item.second.back();
				item.second.pop_back();

				this->itemsBeingMovedToCube.push_back(organId);
			}

			PickupOrganFromInventory();
			return;
		}
	}

	// No more valid organs/recpies were found
	Abort();
}

void AutoTinker::PickupOrganFromInventory()
{
	if (this->itemsBeingMovedToCube.empty())
	{
		server->GameStringf("ÿc:AutoTinkerÿc0: No more organs to move");
		Abort();
		return;
	}

	this->itemWaitingOn = itemsBeingMovedToCube.back();
	this->itemsBeingMovedToCube.pop_back();

	SetState(State::PickupOrganFromInventory);
	if (!me->PickStorageItemToCursor(itemWaitingOn))
	{
		server->GameStringf("ÿc:AutoTinkerÿc0: Failed to pick up organ");

		Abort();
		return;
	}
}

void AutoTinker::DropOrganToCube()
{
	if (me->GetCursorItem() != this->itemWaitingOn)
	{
		server->GameStringf("ÿc:AutoTinkerÿc0: DropOrganToCube - Unknown cursor item. Expected the organ");
		return;
	}

	SetState(State::DropOrganToCube);
	if(!me->DropCursorItemToStorage(STORAGE_CUBE))
	{
		server->GameStringf("ÿc:AutoTinkerÿc0: Failed to drop the organ");

		Abort();
		return;
	}
}

void AutoTinker::TransmuteTargetAndOrgan()
{
	// TODO: Check cube items for organs ?
	SetState(State::TransmuteTargetAndOrgan);
	if(!me->Transmute())
	{
		server->GameStringf("ÿc:AutoTinkerÿc0: Failed to transmute");

		Abort();
		return;
	}
}


/// <summary>
/// Called whenever an item is pickedup to the cursor from the player's inventory
/// </summary>
/// <param name="item">Item picked up from the player's inventory.</param>
void AutoTinker::OnItemFromInventory(const ITEM &item)
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
			server->GameStringf("ÿc:AutoTinkerÿc0: I was expecting to pickup an organ, but got a %s instead", itemCode.data());
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
void AutoTinker::OnItemToCube(const ITEM &item)
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
			server->GameStringf("ÿc:AutoTinkerÿc0: Cube was expecting an organ, but got a %s instead", itemCode.data());
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
		FindRecipe();
		return;
	}
}

bool AutoTinker::OpenCube()
{
	if (!me->OpenCube())
	{
		server->GameStringf("ÿc:AutoTinkerÿc0: You must open your cube before using");

		Abort();
		return false;
	}

	return true;
}

bool AutoTinker::CheckCubeUI()
{
	if (!me->IsUIOpened(UI_CUBE))
	{
		server->GameStringf("ÿc:AutoTinkerÿc0: Cube UI closed, aborting");
		Abort();
	}

	return true;
}

void AutoTinker::Abort()
{
	if (currentState != State::Uninitialized)
	{
		server->GameStringf("ÿc:AutoTinkerÿc0: Done");
		SetState(State::Uninitialized);
	}
}

bool AutoTinker::IsCubeEmpty()
{
	int itemCount = 0;
	me->EnumStorageItems(STORAGE_CUBE, enumItemCountProc, (LPARAM)&itemCount);

	if (itemCount != 0)
	{
		return false;
	}

	return true;
}

bool AutoTinker::IsOrgan(const std::string_view &itemCode) const
{
	return this->requiredOrganCounts.find(itemCode.data()) != this->requiredOrganCounts.end();
}

void AutoTinker::ProcessInventoryItem(const ITEM *item)
{
	if (IsOrgan(item->szItemCode))
	{
		this->organsIdsByType[item->szItemCode].push_back(item->dwItemID);
	}
}

BOOL CALLBACK enumItemProc(LPCITEM item, LPARAM lParam)
{
	auto autoStocker = (AutoTinker *)lParam;
	autoStocker->ProcessInventoryItem(item);

	return TRUE;
}

BOOL CALLBACK enumItemCountProc(LPCITEM item, LPARAM lParam)
{
	auto pItemCount = (int *)lParam;
	(*pItemCount)++;

	return TRUE;
}