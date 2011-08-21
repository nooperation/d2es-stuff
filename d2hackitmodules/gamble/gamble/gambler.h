#ifndef _GAMBLER_H_
#define _GAMBLER_H_

#include "../../d2hackit/includes/D2Client.h"
#include <hash_set>
#include <string>
#include <queue>

enum States
{
	STATE_UNINITIALIZED = 0,
	STATE_NPC_LISTINGITEMS,
	STATE_GAMBLE_WAITFORITEM,
	STATE_GAMBLE_SOLDITEM,
	STATE_GOLD_WAIT,
	STATE_AUTOSTOCK_RUNNING,
	STATE_AUTOSTOCK_ENDED,
	STATE_UI_CLOSING_FOR_AUTOSTOCKER,
};

struct GambleStats
{
	GambleStats()
	{
		uniques = 0;
		rares = 0;
		sets = 0;
		magic = 0;
	}

	int uniques;
	int rares;
	int sets;
	int magic;
};

class Gambler
{
	public:
		Gambler();
		bool Init(std::vector<std::string> itemCodes);
		void StopGambling();
		bool StartGambling();
		void SetAutostockStartDelay(int ticks);
		void ToggleGambleSell(bool sellSet, bool sellRare, bool sellUnique);
		void ToggleRequestGold(int splitBy);
		void ToggleAutostock(bool transmuteSet, bool transmuteRare, bool transmuteUnique);
		
		void OnItemToStorage(ITEM &gambleItem);
		void OnNpcSession(int success);
		void OnNpcGambleItemList(ITEM &gambleItem);
		void OnTick();
		void OnNotEnoughMoney();
		void OnItemSold();
		void OnGoldPickup();
		bool OnAutostockerEnded();
		void OnItemIdentifiedSounded();
		void OnUIClosed();
		
	private:
		void GambleQueuedItems();
		void SellQueuedItems();
		void Reset(bool haltGambling);
		bool WillItemFit(DWORD dwItemId);
		bool WillItemFit(const char *itemCode);
		DWORD FindGamblingNpc();

		void StartAutostocker();
		void RequestMoreGold();

		int ticksTillGambleItemTimeout;

		int requestedGoldSplitBy;

		bool isSellingGambledItems;
		bool isRequestingGold;

		bool transmuteEnabled;
		bool transmuteSet;
		bool transmuteRare;
		bool transmuteUnique;

		bool sellRare;
		bool sellSet;
		bool sellUnique;

		GambleStats stats;
		States currentState;
		GAMEUNIT gamblingNpc;
		std::queue<DWORD> gambleQueue;
		std::queue<DWORD> itemsToSell;
		stdext::hash_set<std::string> itemsToGamble;
};

#endif