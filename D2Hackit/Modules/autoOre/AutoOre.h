#pragma once

#include <windows.h>
#include <vector>

#include "../../Includes/D2Client.h"

enum class State
{
	Uninitialized,
	PickupNextItemToDrop,
	DropNextItemToDrop,
	PickupNextOre,
	DropNextOreToCube,
	RunAutoExtractor,
	RunAutoStocker,
};

class AutoOre
{
	public:
		AutoOre();
		void Start();
		void StartAsAe();
		void Abort();

		void ProcessInventoryItem(const ITEM &item);

		void PickupNextItemToDrop();
		void DropNextItemToDrop();

		void PickupNextOre();
		void DropNextOreToCube();

		void RunAutoExtractor();
		bool OnAutoExtractorMessage(const std::string_view &message);

		void RunAutoStocker();
		bool OnAutoStockerMessage(const std::string_view &message);

		void OnItemPickedUpFromInventory(DWORD itemId);
		void OnItemDropped(DWORD itemId);
		void OnItemDroppedToCube(DWORD itemId);

	private:
		void SetState(State newState);

		bool autoExtractorLoaded;
		bool autoStockerLoaded;
		bool emptyCubeLoaded;

		State currentState;
		std::vector<DWORD> oreIds;
		std::vector<DWORD> itemsToDrop;
		DWORD currentItemToDrop;
		DWORD currentOreId;
};

BOOL CALLBACK enumFindCubeItems(LPCITEM item, LPARAM lParam);
BOOL CALLBACK enumFindOre(LPCITEM item, LPARAM lParam);
