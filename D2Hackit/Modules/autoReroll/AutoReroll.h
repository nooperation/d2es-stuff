#ifndef _AutoExtractor_H_
#define _AutoExtractor_H_

#include <vector>
#include <unordered_set>
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
		void OnItemFromInventory(DWORD itemID);
		bool OnEmptyCubeMessage(const std::string_view &message);
		bool OnAutoExtractorMessage(const std::string_view &message);
		void OnTick();
		void Abort();		

	private:
		bool CheckRerolledItem(const ITEM &item);
		void ExtractMoreGems();
		void ProcessGemCanToCube();
		void MoveNextGemToCube();

		bool ReadAffixConfig(const std::string &configPath, std::unordered_set<int> &readTo);
		bool ReadStatConfig(const std::string& configPath, std::map<int, int>& readTo);

		bool loadedEmptyCube;
		bool rerollItemNeedsToGoBackToCube;
		bool useChat;
		int minPrefix;
		int minSuffix;
		int numGemsToUse;
		DWORD itemToRerollID;
		int currentGemIndex;
		int numFails;
		int uniqueStatCount;
		bool incGemCan;
		bool increment;
		std::vector<DWORD> gemsInInventory;
		std::unordered_set<int> goodPrefix;
		std::unordered_set<int> goodSuffix;
		std::map<int, int> goodStats;

		States currentState;
		GemCanStuff gemCanAndOpener;
};

BOOL CALLBACK enumFindGems(LPCITEM item, LPARAM lParam);
BOOL CALLBACK enumItemCountProc(LPCITEM item, LPARAM lParam);
BOOL CALLBACK enumFindGemCanStuff(LPCITEM item, LPARAM lParam);
BOOL CALLBACK enumFindItemToReroll(LPCITEM item, LPARAM lParam);

#endif
