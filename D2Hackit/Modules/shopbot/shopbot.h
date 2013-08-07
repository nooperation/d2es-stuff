#include <queue>
#include <hash_set>
#include <hash_map>
#include "../../Includes/D2Client.h"

enum States
{
	STATE_UNINITIALIZED = 0,
	STATE_COMPLETE,
	STATE_NEXTTELEPORT,
	STATE_TELEPORTING,
	STATE_TELEPORTCOMPLETE,
	STATE_OPENWP,
	STATE_NPC_LISTINGITEMS,
	STATE_NPC_DONELISTINGITEMS,
	STATE_PURCHASE_NEXTITEM,
	STATE_PURCHASE_WAITFORITEM,
	STATE_PURCHASE_DONE,
	STATE_WAYPOINT_WAITFOROPEN,
	STATE_WAYPOINT_REQUESTED,
	STATE_WAYPOINT_COMPLETE,
	STATE_WAITINGFORNEXTSTATE,
};

class ShopBot
{
	public:
		ShopBot();
		bool Start(const std::vector<MAPPOS> &customPath, std::string merchant);
		void OnTick();
		void OnMapBlink();
		void OnNpcSession(int success);
		void OnNpcItemList(ITEM &merchantItem);
		void OnItemToStorage(ITEM &merchantItem);
		void OnNotEnoughMoney();
		void OnWpOpen();
		void Stop();

	private:
		bool ReadConfig(std::string configPath, stdext::hash_set<int> &readTo);
		bool LoadItemMap(std::string fileName, stdext::hash_map<std::string, std::string> &itemMap);
		bool IsItemGood(const ITEM &item);
		void PurchaseQueuedItems();
		bool WillItemFit(DWORD dwItemId);
		std::queue<DWORD> gambleQueue;

		std::vector<MAPPOS> teleportPath;

		DWORD FindMerchant();
		int currentTeleport;
		int minPrefix;
		int minSuffix;
		bool backToTown;
		States currentState;
		std::string merchantName;
		stdext::hash_set<int> goodPrefix;
		stdext::hash_set<int> goodSuffix;
		stdext::hash_map<std::string, std::string> targetItems;

		GAMEUNIT merchantNpc;

};

BOOL CALLBACK FindWaypointProc(LPCPRESETUNIT presetUnit, LPARAM lParam);