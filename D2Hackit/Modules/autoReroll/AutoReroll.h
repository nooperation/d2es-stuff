#ifndef _AutoExtractor_H_
#define _AutoExtractor_H_

#include <vector>
#include <hash_set>
#include <windows.h>
#include <string>
#include "../../Includes/D2Client.h"

struct GemCanStuff
{
	DWORD GemCanID;
	DWORD CanOpenerID;
	bool GemCanMoved;
	bool CanOpenerMoved;
};

enum States
{
	STATE_UNINITIALIZED = 0,
	STATE_COMPLETE,
	STATE_TRANSMUTE,
	STATE_PICKUPITEMTOREROLLED,
	STATE_PICKUPGEM,
	STATE_PICKUPGEMCANSTUFF,
	STATE_GEMCANSTUFFTOCUBE,
	STATE_GEMTOCUBE,
	STATE_ITEMTOREROLLTOCUBE,
	STATE_RUNNINGEMPTYCUBE,
	STATE_RUNNINGAUTOEXTRACTOR,
};

#define CONFIG_PATH ".\\plugin\\autoreroll.ini"
class AutoReroll
{
	public:
		AutoReroll();
		bool Start(int numGems, bool useChat);
		bool StartRerollingItemInCube();
		bool Init(bool useChat);

		void OnItemToCube(const ITEM &item);
		void OnItemFromCube(DWORD itemID);
		void OnItemFromInventory(DWORD itemID);
		bool OnEmptyCubeEnded();
		bool OnAutoExtractorEnded();
		void OnTick();
		void Abort();		

	private:
		void StartStocking();
		bool CheckRerolledItem(const ITEM &item);

		void ExtractMoreGems();
		void MoveGemCanAndOpenerToCube();
		void MoveNextGemToCube();
		void FinishedEmptyCube();

		bool ReadAffixConfig(const std::string &configPath, stdext::hash_set<int> &readTo);

		bool loadedEmptyCube;
		bool rerollItemNeedsToGoBackToCube;
		bool useChat;
		int minPrefix;
		int minSuffix;
		int numGemsToUse;
		DWORD itemToRerollID;
		int currentGemIndex;
		std::vector<DWORD> gemsInInventory;
		stdext::hash_set<int> goodPrefix;
		stdext::hash_set<int> goodSuffix;

		States currentState;
		GemCanStuff gemCanAndOpener;
};

BOOL CALLBACK enumFindGems(LPCITEM item, LPARAM lParam);
BOOL CALLBACK enumItemCountProc(LPCITEM item, LPARAM lParam);
BOOL CALLBACK enumFindGemCanStuff(LPCITEM item, LPARAM lParam);
BOOL CALLBACK enumFindItemToReroll(LPCITEM item, LPARAM lParam);

#endif
