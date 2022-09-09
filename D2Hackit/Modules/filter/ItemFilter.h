#ifndef _ITEMWATCHER_H_
#define _ITEMWATCHER_H_

#include <unordered_map>
#include <string>
#include "../../Includes/D2Client.h"

class ItemFilter
{
	public:
		bool OnItemAction(ITEM &item);
		bool LoadItems();
		
	private:
		std::unordered_map<std::string, std::string> filteredItems;
		std::unordered_map<std::string, std::string> allowedItems;
		std::unordered_map<std::string, std::string> weaponArmorItems;
		//bool filterTown;
		bool showEthSoc;
		bool showEthereal;
		bool showSocketed;
		bool showSuperior;
		bool showNormal;
		bool showInferior;
		bool showUnique;
		bool showSet;
		bool showRare;
		bool showCraft;
		bool showMagic;
		bool showMagicRingAmulet;
		bool showRareRingAmulet;
		bool showMagicCharms;
		bool showRareCharms;
		int minGoldAmount;

		bool OnItemFind(ITEM &item);
		bool IsFiltered(char *itemCode);
		bool IsAllowed(char *itemCode);
		bool LoadItemMap(const std::string &fileName, std::unordered_map<std::string, std::string> &itemMap);
		bool IsValidItem(ITEM &item);
		bool IsGoodItemOther(ITEM &item);
		bool IsGoodItemCode(char *itemCode);
		bool IsCharm(char *itemCode);
		bool IsRareSpecial(char *itemCode);
		bool IsRuneDecalScroll(char *itemCode);
		bool IsRingAmulet(char *itemCode);
		bool IsMonsterPart(char *itemCode);
		bool IsWeaponArmor(char *itemCode);


};
#endif