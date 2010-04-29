#ifndef _AutoExtractor_H_
#define _AutoExtractor_H_

#include <vector>
#include <hash_set>
#include <windows.h>
#include <string>

struct GemCanStuff
{
	DWORD GemCanID;
	bool GemCanMoved;
	DWORD CanOpenerID;
	bool CanOpenerMoved;
};

enum States
{
	STATE_UNINITIALIZED = 0,
	STATE_COMPLETE,
	STATE_STARTEXTRACTION,
	STATE_TRANSMUTE,
	STATE_TRANSMUTE_COMPLETE,
	STATE_NEXTEXTRACTOR,
	STATE_PICKUPEXTRACTEDITEM,
	STATE_PICKUPEXTRACTOR,
	STATE_PICKUPGEMCANSTUFF,
	STATE_NEXTGEMCANSTUFF,
	STATE_GEMCANSTUFFTOCUBE,
	STATE_EXTRACTORTOCUBE,
	STATE_EXTRACTEDTOINVENTORY,
	STATE_EXTRACTEDTOCUBE,
	STATE_EXTRACTIONCOMPLETE,
	STATE_RUNNINGEMPTYCUBE,
	STATE_RUNNINGAUTOEXTRACTOR,
	STATE_FINISHEDEMPTYCUBE,
	STATE_FINISHEDAUTOEXTRACTOR,
	STATE_WAITINGFORNEXTSTATE,
};

#define CONFIG_PATH ".\\plugin\\autoreroll.ini"
class AutoReroll
{
	public:
		AutoReroll();
		bool Start(int numGems, bool useChat);
		bool StartExtraction();
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
		bool CheckExtractedItem(const ITEM &item);

		bool ReadConfig(std::string configPath, stdext::hash_set<int> &readTo);

		bool loadedEmptyCube;
		bool justRanAutoExtractor;
		bool useChat;
		bool isExtractedItemGood;
		int minPrefix;
		int minSuffix;
		int numGemsToUse;
		DWORD extractedItemID;
		int currentExtractor;
		std::vector<DWORD> extractors;
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
