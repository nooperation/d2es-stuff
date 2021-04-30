#pragma once

#include <vector>
#include <queue>
#include <windows.h>
#include <string>
#include <unordered_set>
#include "../../Includes/D2Client.h"

enum States
{
	STATE_UNINITIALIZED = 0,
	STATE_START,
	STATE_COMPLETE,
	STATE_TRANSMUTE,
	STATE_PICKUPEXTRACTEDITEM,
	STATE_EXTRACTEDTOINVENTORY,
};

struct ExtractorInfo
{
	std::string itemCode;
	std::vector<ITEMPROPERTY> oreCounts;
};

class AutoExtractor
{
	public:
		AutoExtractor();
		bool Start(int itemCount, bool useChat);
		bool StartExtraction();
		bool Init(bool useChat);

		void OnItemToCube(const ITEM &itemID);
		void OnItemToInventory(const ITEM &item);
		void OnItemFromCube(const ITEM &item);
		void OnTick();
		void Abort();		

	private:
		void StartStocking(); 
		bool CheckCubeUI();
		bool CheckExtractedItem(const ITEM &item);
		bool IsGoodPrefix(int prefixId);
		bool IsGoodSuffix(int suffixId);
		bool IsItemAnExtractor(const ITEM &item);
		bool ReadAffixFile(const std::string &configPath, std::unordered_set<int> &readTo);
		int GetNumberOfExpectedOutputs() const;
		void PickupNextExtractedItem();

		bool useChat;
		std::queue<DWORD> extractedItemIDs;
		int itemsExpectedToCube;
		int ticksTillTransmuteTimeout;
		std::vector<ExtractorInfo> extractors;
		std::unordered_set<int> goodPrefix;
		std::unordered_set<int> goodSuffix;
		int numberOfRunsRemaining;

		States currentState;
};

BOOL CALLBACK enumFindExtractors(LPCITEM item, LPARAM lParam);
