#pragma once

#include <windows.h>
#include <vector>

#include "../../Includes/D2Client.h"

enum class State
{
	Uninitialized,
	Initializing,
	PickupNextItemToDrop,
	DropNextItemToDrop,
	PickupNextOre,
	DropNextOreToCube,
	FirstTransmute,
	WaitForFirstTransmuteResults,
	PickupGemFromFirstTransmute,
	DropGemFromFirstTransmute,

	SecondTransmute,
	WaitForSecondTransmuteResults,

	RunEmptyCube,
	RunAutoStocker,
};

class AutoOre
{
	public:
		AutoOre();
		void Start(bool useChat);
		void StartAutoOre();
		void Abort();

		void ProcessInventoryItem(const ITEM &item);

		void PickupNextItemToDrop();
		void DropNextItemToDrop();

		void PickupNextOre();
		void DropNextOreToCube();

		bool OnEmptyCubeMessage(const std::string_view &message);

		void OnTick();
		bool OnAutoExtractorMessage(const std::string_view &message);

		void RunAutoStocker();
		bool OnAutoStockerMessage(const std::string_view &message);

		void OnItemPickedUpFromInventory(DWORD itemId);
		void OnItemDroppedToInventory(const ITEM &itemId);
		void OnItemPickedUpFromCube(DWORD itemId);
		void OnItemDropped(DWORD itemId);
		void OnItemDroppedToCube(const ITEM &itemId);

	private:
		void SetState(State newState);
		void HandleFirstStageTransmuteResults();
		void HandleSecondStageTransmuteResults();

		bool useChat;
		bool autoExtractorLoaded;
		bool autoStockerLoaded;
		bool emptyCubeLoaded;

		bool dropFakeNotes;

		int numTransmuteResults;
		int numExpectedTransmuteResults;
		DWORD expectedItemToHand;

		State currentState;
		std::vector<DWORD> oreIds;
		std::vector<DWORD> itemsToDrop;
		DWORD currentItemToDrop;
		DWORD currentOreId;
};

BOOL CALLBACK enumFindCubeItems(LPCITEM item, LPARAM lParam);
BOOL CALLBACK enumFindOre(LPCITEM item, LPARAM lParam);
