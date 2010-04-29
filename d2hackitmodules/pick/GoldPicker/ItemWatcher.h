#ifndef _ITEMWATCHER_H_
#define _ITEMWATCHER_H_

#include <list>
#include <hash_map>
#include <string>
#include "../../d2hackit/includes/d2hackit.h"
//#include "_item.h"

struct WatchedItemData
{
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
		void OnItemAction(ITEM &item);
		void OnItemDestroy(DWORD itemID);
		void SetTownPickup(bool enabled);
		void CheckWatchedItems();
		void ShowEthSoc(bool show);
		void ShowEthereal(bool show);
		void TogglePickItems();
		bool IsTownPickup();
		bool loadItems();
		

	private:
		std::vector<DWORD> destroyedItemsSinceLastCheck;
		std::list<WatchedItemData> watchedItems;
		stdext::hash_map<std::string, std::string> itemsToPick;
		unsigned int radius;
		unsigned int minGold;
		bool townPickup;
		bool showEtherealSocketed;
		bool showEthereal;
		bool isPickingItems;

		const char *GetItemDesc(ITEM &item);
		void OnItemFind(ITEM &item);
		bool loadItemMap(std::string fileName, stdext::hash_map<std::string, std::string> &itemMap);
		bool IsValidItem(ITEM &item);
		bool IsGoodItemOther(ITEM &item);
		bool IsGoodItemCode(char *itemCode);
		char *GetDirectionFrom(WORD sourceX, WORD sourceY, WORD targetX, WORD targetY);
		bool IsCharm(char *itemCode);
		bool IsRareSpecial(char *itemCode);
		bool IsRuneDecalScroll(char *itemCode);
		bool IsRingAmulet(char *itemCode);
		bool IsOkToPick(char *itemCode);
		bool IsMonsterPart(char *itemCode);

};
#endif