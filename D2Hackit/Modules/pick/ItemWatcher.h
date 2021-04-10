#ifndef _ITEMWATCHER_H_
#define _ITEMWATCHER_H_

#include <list>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include "../../Includes/D2Client.h"
//#include "_item.h"

struct WatchedItemData
{
public:
	DWORD id;
	int x;
	int y;
	bool isGold;
	SIZE itemSize;
};

class ItemWatcher
{
	public:
		ItemWatcher();
		void SetRadius(int radius);
		void SetMinGold(int amount);
		void SetTownPickup(bool enabled);
		void ShowEthSoc(bool show);
		void ShowEthereal(bool show);
		void TogglePickItems();
		bool LoadItems();

		void OnItemAction(const ITEM &item);
		void OnItemDestroy(DWORD itemID);
		void OnTick();
		void OnGameLeave();
		void OnGameJoin();
		void OnItemFind(const ITEM &item);


	private:
		void AnnounceItem(const ITEM &item);
		void CheckWatchedItems();
		void Cleanup();
		bool loadItemMap(const std::string &fileName, std::unordered_map<std::string, std::string> &itemMap);

		const char *GetItemDesc(const ITEM &item);
		const char *GetDirectionFrom(WORD sourceX, WORD sourceY, WORD targetX, WORD targetY);
		bool IsOkToPick(const char *itemCode);
		bool IsOkToAnnounce(const char *itemCode);

		std::vector<DWORD> destroyedItemsSinceLastCheck;
		std::list<WatchedItemData> watchedItems;

		std::unordered_set<std::string> iddqdUniques;
		std::unordered_map<std::string, std::string> itemsToAnnounce;
		std::unordered_map<std::string, std::string> itemsToPick;

		unsigned int radius;
		unsigned int minGold;
		bool townPickup;
		bool showEtherealSocketed;
		bool showEthereal;
		bool isPickingItems;
		bool isInGame;
};
#endif