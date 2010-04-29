#ifndef _GAMBLER_H_
#define _GAMBLER_H_

#include "../../d2hackit/includes/D2Client.h"
#include <hash_set>
#include <string>
#include <queue>

enum States
{
	STATE_UNINITIALIZED = 0,
	STATE_HALTED,
	STATE_READYTORUN,
	STATE_NPC_LISTINGITEMS,
	STATE_NPC_DONELISTINGITEMS,
	STATE_GAMBLE_NEXTITEM,
	STATE_GAMBLE_WAITFORITEM,
	STATE_GAMBLE_DONE,
	STATE_GAMBLE_SELLITEM,
	STATE_GAMBLE_SOLDITEM,
	STATE_GAMBLE_DONESELLING,
	STATE_GOLD_NEEDMORE,
	STATE_GOLD_WAIT,
	STATE_GOLD_REFILLED,
	STATE_AUTOSTOCK_START,
	STATE_AUTOSTOCK_RUNNING,
	STATE_AUTOSTOCK_ENDED,
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
		void ToggleGambleSell();
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
		
	private:
		void GambleQueuedItems();
		void SellQueuedItems();
		void Reset(bool haltGambling);
		bool WillItemFit(DWORD dwItemId);
		bool WillItemFit(const char *itemCode);
		DWORD FindGamblingNpc();

		int ticksTillGambleItemTimeout;
		int ticksTillNextPurchase;
		int ticksTillAutostock;
		int autostockStartDelay;

		int requestedGoldSplitBy;

		bool isSellingGambledItems;
		bool isRequestingGold;

		bool transmuteEnabled;
		bool transmuteSet;
		bool transmuteRare;
		bool transmuteUnique;

		GambleStats stats;
		States currentState;
		GAMEUNIT gamblingNpc;
		std::queue<DWORD> gambleQueue;
		std::queue<DWORD> itemsToSell;
		stdext::hash_set<std::string> itemsToGamble;
};

#endif