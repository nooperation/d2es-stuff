#ifndef _AutoExtractor_H_
#define _AutoExtractor_H_

#include <vector>
#include <windows.h>
#include <string>
#include <hash_set>

enum States
{
	STATE_UNINITIALIZED = 0,
	STATE_COMPLETE,
	STATE_STARTEXTRACTION,
	STATE_TRANSMUTE,
	STATE_TRANSMUTE_COMPLETE,
	STATE_PICKUPEXTRACTEDITEM,
	STATE_EXTRACTEDTOINVENTORY,
	STATE_EXTRACTIONCOMPLETE,
	STATE_WAITINGFORNEXTSTATE,
};

class AutoExtractor
{
	public:
		AutoExtractor();
		bool Start(bool useChat);
		bool StartExtraction();
		bool Init(bool useChat);

		void OnItemToCube(const ITEM &itemID);
		void OnItemToInventory(const ITEM &item);
		void OnItemFromCube(DWORD itemID);
		void OnTick();
		void Abort();		

	private:
		void StartStocking(); 
		bool CheckCubeUI();
		bool CheckExtractedItem(const ITEM &item);
		bool IsGoodPrefix(int prefixId);
		bool IsGoodSuffix(int suffixId);

		bool ReadConfig(std::string configPath, stdext::hash_set<int> &readTo);

		bool useChat;
		DWORD extractedItemID;
		int itemsExpectedToCube;
		int ticksTillTransmuteTimeout;
		std::vector<std::string> extractors;
		stdext::hash_set<int> goodPrefix;
		stdext::hash_set<int> goodSuffix;

		States currentState;
};

BOOL CALLBACK enumFindExtractors(LPCITEM item, LPARAM lParam);

#endif
