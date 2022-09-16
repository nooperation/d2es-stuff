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
	bool isTpScroll;
	bool isIdScroll;
	byte keyCount;
	SIZE itemSize;
	float distanceToItem;
};

class ItemWatcher
{
	public:
		ItemWatcher();
		void SetRadius(int radius);
		void SetMinGold(int amount);
		void SetTownPickup(bool enabled);
		void SetGoldSpeed(int goldSpeed);
		void TogglePickItems();
		void Mute();
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
		bool IsOkToPick(const std::string &itemCode);
		bool IsOkToAnnounce(const char *itemCode);

		std::unordered_set<DWORD> destroyedItemsSinceLastCheck;
		std::vector<WatchedItemData> watchedItems;

		std::unordered_map<std::string, std::string> uniquesToAnnounce;
		std::unordered_map<std::string, std::string> setsToAnnounce;
		std::unordered_map<std::string, std::string> itemsToAnnounce;
		std::unordered_map<std::string, std::string> itemsToPick;

		unsigned int radius;
		unsigned int minGold;
		bool townPickup;
		bool isPickingItems;
		unsigned int goldSpeed;
		bool isInGame;
		bool isMute;
		bool isAnnouncingAllSets;
		bool isAnnouncingAllUniques;
		bool isAnnouncingAllCrafted;
		bool isOverridingUniqueItemNames;
		bool isOverridingSetItemNames;
		bool isWalkToGold;
		bool isWalkToItems;
};
#endif