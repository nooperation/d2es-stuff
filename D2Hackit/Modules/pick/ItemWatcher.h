#ifndef _ITEMWATCHER_H_
#define _ITEMWATCHER_H_

#include <list>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <chrono>
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
		bool LoadItemMap(const std::string &fileName, std::unordered_map<std::string, std::string> &itemMap);
		void SortWatchedItems();
		float Distance(float x1, float x2, float y1, float y2) const;

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

		std::chrono::system_clock::time_point nextPickAttemptTime;

		unsigned int radius = 4;
		unsigned int minGold = 50;
		bool townPickup = true;
		bool isPickingItems = true;
		unsigned int goldSpeed = 1;
		bool isInGame = false;
		bool isMute = false;
		bool isAnnouncingAllSets = true;
		bool isAnnouncingAllUniques = true;
		bool isAnnouncingAllCrafted = true;
		bool isOverridingUniqueItemNames = false;
		bool isOverridingSetItemNames = false;
		bool isWalkToGold = true;
		bool isWalkToItems = true;
};
#endif