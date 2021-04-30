#ifndef _GAMBLER_H_
#define _GAMBLER_H_

#include <unordered_set>
#include <string>
#include <queue>
#include "../../Includes/D2Client.h"

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
		bool Init(const std::vector<std::string> &itemCodes);
		void StopGambling();
		bool StartGambling();
		void SetAutostockStartDelay(int ticks);
		void ToggleGambleSell(bool magics, bool sellSet, bool sellRare, bool sellUnique);
		void ToggleRequestGold(int splitBy);
		void ToggleAutostock(bool transmuteSet, bool transmuteRare, bool transmuteUnique);
		void SetNoSellList(const std::vector<std::string> &noSellList);
		
		void OnItemToStorage(ITEM &gambleItem);
		void OnNpcSession(int success);
		void OnNpcGambleItemList(ITEM &gambleItem);
		void OnTick();
		void OnNotEnoughMoney();
		void OnItemSold();
		void OnGoldPickup();
		bool OnAutostockerMessage(const std::string_view &message);
		void OnItemIdentifiedSounded();
		void OnUIClosed();
		
	private:
		void GambleQueuedItems();
		void SellQueuedItems();
		void ResetQueues();
		bool WillItemFit(DWORD dwItemId);
		bool WillItemFit(const char *itemCode);
		void AttemptToMakeRoom();
		DWORD FindGamblingNpc();

		void StartAutostocker();
		void RequestMoreGold();

		int ticksTillGambleItemTimeout;

		int requestedGoldSplitBy;

		bool isRequestingGold;

		bool transmuteEnabled;
		bool transmuteSet;
		bool transmuteRare;
		bool transmuteUnique;

		bool sellMagic;
		bool sellRare;
		bool sellSet;
		bool sellUnique;
		bool isUsingNoSellList;

		GambleStats stats;
		States currentState;
		GAMEUNIT gamblingNpc;
		std::queue<DWORD> gambleQueue;
		std::queue<DWORD> itemsToSell;
		std::unordered_set<std::string> noSellList;
		std::unordered_set<std::string> itemsToGamble;
};

#endif