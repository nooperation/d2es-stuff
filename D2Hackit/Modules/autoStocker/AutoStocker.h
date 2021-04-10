#ifndef _AUTOSTOCKER_H_
#define _AUTOSTOCKER_H_

#include <vector>
#include <unordered_set>
#include <windows.h>
#include "../../Includes/D2Client.h"

enum TransmuteStages
{
	TRANSMUTE_GEM = 0,
	TRANSMUTE_RUNEA,
	TRANSMUTE_RUNEB,
	TRANSMUTE_DECAL,
	TRANSMUTE_MULTISTOCKER,
	TRANSMUTE_TOMEA,
	TRANSMUTE_TOMEB,
	TRANSMUTE_CRYSTAL,
	TRANSMUTE_REROLL,
	TRANSMUTE_END
};

enum States
{
	STATE_UNINITIALIZED = 0,
	STATE_COMPLETE,
	STATE_PICKUPSTOCKER,
	STATE_PICKUPITEM,
	STATE_TRANSMUTE,
	STATE_STOCKERTOCUBE,
	STATE_ITEMTOCUBE,
	STATE_STOCKERFROMCUBE,
	STATE_STOCKERTOINVENTORY,
	STATE_WAITINGFORNEXTSTATE,
};

class AutoStocker
{
	public:
		AutoStocker();
		bool Start(bool useChat);
		bool BeginAutostocking();
		bool Init(bool useChat);
		bool StartRares(bool transmuteSet, bool transmuteRare, bool transmuteUnique, bool useChat);
		void OnItemFromInventory(const ITEM &item);
		void OnItemToCube(const ITEM &item);
		void OnItemToInventory(const ITEM &item);
		void OnItemFromCube(const ITEM &item);
		void OnTick();
		void Abort();
		
		

	private:
		bool ReadConfig(const std::string &iniPath);
		bool ReadAffixConfig(const std::string &configPath, std::unordered_set<int> &readTo);

		void FindItemsToTransmute(const std::vector<ITEM> &itemsInInventory);
		bool FindStockers(const std::vector<ITEM> &itemsInInventory);
		bool IsCubeEmpty();
		void ProcessNextItem();
		void ProcessNextStocker();
		bool OpenCube();
		bool GetStockerTypeByID(DWORD itemId, int *stockerType);
		bool GetStockerTypeByCode(const char *itemCode, int *stockerType);
		bool CheckCubeUI();
		bool CheckItemAffix(const ITEM &item);

		void StartStocking();
		bool IsTomeBItem(LPCSTR itemCode);
		bool IsTomeAItem(LPCSTR itemCode);
		bool IsDecalStockerItem(LPCSTR itemCode);
		bool IsRuneStockerBItem(LPCSTR itemCode);
		bool IsRuneStockerAItem(LPCSTR itemCode);
		bool IsMultiStockerItem(LPCSTR itemCode);
		bool IsCrystalItem(LPCSTR itemCode);
		bool IsGemCanItem(LPCSTR itemCode);
		bool IsRerollItem(const ITEM &item);

		std::vector<std::vector<DWORD>> itemsToTransmute;
		std::vector<DWORD> restockers;
		std::vector<POINT> restockerPositions;
		std::unordered_set<std::string> stockerItems[TRANSMUTE_END];
		DWORD itemWaitingOn;
		bool useChat;
		bool transmuteRare;
		bool transmuteSet;
		bool transmuteUnique;

		bool IsTransmutingUnidentifiedSmallCharms;
		bool IsTransmutingUnidentifiedLargeCharms;
		bool IsTransmutingUnidentifiedGrandCharms;
		int MaxUnidentifiedSCharmLevel;
		int MaxUnidentifiedLCharmLevel;
		int MaxUnidentifiedGCharmLevel;

		unsigned int currentItem;
		unsigned int currentStocker;
		std::unordered_set<int> goodPrefix;
		std::unordered_set<int> goodSuffix;
		States currentState;
};

BOOL CALLBACK enumItemProc(LPCITEM item, LPARAM lParam);
BOOL CALLBACK enumItemCountProc(LPCITEM item, LPARAM lParam);

#endif
