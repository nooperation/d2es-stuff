#include <fstream>
#include "ItemFilter.h"

#define FILTER_SETTINGS_PATH ".\\plugin\\filter.ini"

bool ItemFilter::OnItemAction(ITEM &item)
{
	switch(item.iAction)
	{
		case ITEM_ACTION_NEW_GROUND:
		case ITEM_ACTION_OLD_GROUND:
		case ITEM_ACTION_DROP:
		{
			return OnItemFind(item);
		}
		break;
	}

	return true;
}

bool ItemFilter::LoadItems()
{
	if(!LoadItemMap(".\\plugin\\filterDeny.txt", filteredItems))
	{
		return false;
	}
	if(!LoadItemMap(".\\plugin\\filterAllow.txt", allowedItems))
	{
		return false;
	}
	if(!LoadItemMap(".\\plugin\\filterUniqueWhitelist.txt", allowedItemsUnique))
	{
		return false;
	}
	if(!LoadItemMap(".\\plugin\\filterSetWhitelist.txt", allowedItemsSet))
	{
		return false;
	}
	//filterTown		= GetPrivateProfileInt("Filter", "FilterTown", 1, FILTER_SETTINGS_PATH); 

	showUnique			= GetPrivateProfileInt("WeaponArmor", "Unique", 1, FILTER_SETTINGS_PATH) == TRUE;
	showRare			= GetPrivateProfileInt("WeaponArmor", "Rare", 1, FILTER_SETTINGS_PATH) == TRUE;
	showCraft			= GetPrivateProfileInt("WeaponArmor", "Craft", 1, FILTER_SETTINGS_PATH) == TRUE;
	showSet				= GetPrivateProfileInt("WeaponArmor", "Set", 1, FILTER_SETTINGS_PATH) == TRUE;
	showMagic			= GetPrivateProfileInt("WeaponArmor", "Magic", 0, FILTER_SETTINGS_PATH) == TRUE;
	showEthSoc			= GetPrivateProfileInt("WeaponArmor", "EthSock", 0, FILTER_SETTINGS_PATH) == TRUE; 
	showEthereal		= GetPrivateProfileInt("WeaponArmor", "Ethereal", 0, FILTER_SETTINGS_PATH) == TRUE; 
	showSocketed		= GetPrivateProfileInt("WeaponArmor", "Socketed", 0, FILTER_SETTINGS_PATH) == TRUE; 
	showSuperior		= GetPrivateProfileInt("WeaponArmor", "Superior", 0, FILTER_SETTINGS_PATH) == TRUE; 
	showNormal			= GetPrivateProfileInt("WeaponArmor", "Normal", 0, FILTER_SETTINGS_PATH) == TRUE;
	showInferior		= GetPrivateProfileInt("WeaponArmor", "Inferior", 0, FILTER_SETTINGS_PATH) == TRUE;

	showMagicRingAmulet	= GetPrivateProfileInt("Jewellery", "MagicRingAmulet", 1, FILTER_SETTINGS_PATH) == TRUE;
    showRareRingAmulet	= GetPrivateProfileInt("Jewellery", "RareRingAmulet", 1, FILTER_SETTINGS_PATH) == TRUE;

	showMagicCharms		= GetPrivateProfileInt("Charms", "MagicCharms", 1, FILTER_SETTINGS_PATH) == TRUE;
    showRareCharms		= GetPrivateProfileInt("Charms", "RareCharms", 1, FILTER_SETTINGS_PATH) == TRUE;

	showMagicJewels     = GetPrivateProfileInt("Jewels", "MagicJewels", 1, FILTER_SETTINGS_PATH) == TRUE;
	showRareJewels      = GetPrivateProfileInt("Jewels", "RareJewels", 1, FILTER_SETTINGS_PATH) == TRUE;

	minGoldAmount		= GetPrivateProfileInt("Misc", "MinGoldAmount", 0, FILTER_SETTINGS_PATH);
	return true;
}

bool ItemFilter::IsAllowed(char *itemCode)
{
	if(allowedItems.count(itemCode) > 0)
		return true;

	return false;
}

bool ItemFilter::IsFiltered(char *itemCode)
{
	if(filteredItems.count(itemCode) > 0)
		return true;

	return false;
}

bool ItemFilter::OnItemFind(ITEM &item)
{
	//if(!filterTown && me->IsInTown())
	//	return true;

	if(IsAllowed(item.szItemCode))
		return true;

	if(item.iQuality == ITEM_LEVEL_UNIQUE && allowedItemsUnique.find(item.szItemCode) != allowedItemsUnique.end())  
		return true;
	
	if(item.iQuality == ITEM_LEVEL_SET && allowedItemsSet.find(item.szItemCode) != allowedItemsSet.end())  
		return true;

	if(IsFiltered(item.szItemCode))
		return false;

	if (strcmp(item.szItemCode, "gld") == 0) {
		return item.dwGoldAmount >= minGoldAmount;
	}

	if(IsGoodItemCode(item.szItemCode))
		return true;

    if(IsRingAmulet(item.szItemCode))
    {
		if(item.iSocketed)
			return true;

		if(!showMagicRingAmulet && item.iQuality == ITEM_LEVEL_MAGIC)
    		return false;
        
        if(!showRareRingAmulet &&  item.iQuality == ITEM_LEVEL_RARE)
    		return false;
        
        return true;
    }
    
    if(IsCharm(item.szItemCode))
    {
		if(item.iSocketed)
			return true;

    	if(!showMagicCharms && item.iQuality == ITEM_LEVEL_MAGIC)
    		return false;
        
        if(!showRareCharms && item.iQuality == ITEM_LEVEL_RARE)
    		return false;

        return true;
    }

	if (IsJewel(item.szItemCode))
	{
		if (!showMagicJewels && item.iQuality == ITEM_LEVEL_MAGIC)
			return false;
		if (!showRareJewels && item.iQuality == ITEM_LEVEL_RARE)
			return false;

		return true;
	}
    
	if(server->IsWeapon(item.szItemCode) || server->IsArmor(item.szItemCode))
	{
		if(showEthereal && item.iEthereal)
			return true;

		if(showSocketed && item.iSocketed)
			return true;

		if(showEthSoc && item.iEthereal && item.iSocketed)
			return true;

		if(!showUnique && item.iQuality == ITEM_LEVEL_UNIQUE)  
			return false;

		if(!showRare && item.iQuality == ITEM_LEVEL_RARE)
			return false;

		if(!showCraft && item.iQuality == ITEM_LEVEL_CRAFT)
			return false;

		if(!showSet && item.iQuality == ITEM_LEVEL_SET)
			return false;

		if(!showMagic && item.iQuality == ITEM_LEVEL_MAGIC)
			return false;

		if(!showSuperior && item.iQuality == ITEM_LEVEL_SUPERIOR)
			return false;

		if(!showInferior && item.iQuality == ITEM_LEVEL_INFERIOR)
			return false;

		if(!showNormal && item.iQuality == ITEM_LEVEL_NORMAL)
			return false;
	}

	return true;
}

bool ItemFilter::LoadItemMap(const std::string &fileName, std::unordered_map<std::string, std::string> &itemMap)
{
	std::ifstream inFile(fileName.c_str());

	std::string itemName;
	std::string itemDesc;
	std::string readBuff;

	if(!inFile)
	{
		server->GameErrorf("ÿc:Filterÿc0: ÿc1failed to read the follow item table:ÿc0 %s", fileName.c_str());
		return false;
	}

	itemMap.clear();

	while(inFile.good())
	{
		std::getline(inFile, readBuff);

		if(readBuff.length() <= 0)
		{
			continue;
		}

		itemName = readBuff.substr(0, 3);
		itemDesc = readBuff.substr(4);

		if(itemName.length() < 3)
		{
			continue;
		}

		if(itemMap.count(itemName) == 0)
			itemMap[itemName] = itemDesc;
	}

	inFile.close();
	return true;
}

bool ItemFilter::IsValidItem(ITEM &item)
{
	if(IsGoodItemCode(item.szItemCode))
	{
		return true;
	}

	return false;
}

bool ItemFilter::IsGoodItemCode(char *itemCode)
{
	if(strlen(itemCode) < 3)
	{
		return false;
	}

	if(IsRareSpecial(itemCode) || IsRuneDecalScroll(itemCode))
	{
		return true;
	}

	if(strcmp(itemCode, "xu0") == 0 || 
		strcmp(itemCode, "s51") == 0 || 
		strcmp(itemCode, "s01") == 0 || 
		strcmp(itemCode, "t51") == 0 ||
		strcmp(itemCode, "s22") == 0 ||
		strcmp(itemCode, "w56") == 0 || 
		strcmp(itemCode, "w31") == 0 || 
		strcmp(itemCode, "t01") == 0 ||
		strcmp(itemCode, "kv0") == 0 || 
		strcmp(itemCode, "ko0") == 0 ||
		strcmp(itemCode, "leg") == 0)
	{
		return true;
	}

	return false;
}

bool ItemFilter::IsCharm(char *itemCode)
{
	if(itemCode[0] == 'c' && (itemCode[1] == 'x' || itemCode[1] == 'm') && isdigit(itemCode[2]))
	{
		return true;
	}

	if((itemCode[0] == 'm' || itemCode[0] == 'n') && itemCode[1] == 'c' && isdigit(itemCode[2]))
	{
		// Charms and such
		return true;
	}

	return false;
}

bool ItemFilter::IsJewel(char* itemCode)
{
	if (strcmp(itemCode, "jew") == 0)
	{
		return true;
	}

	return false;
}

bool ItemFilter::IsRareSpecial(char *itemCode)
{
	// Unique boss parts
	if(itemCode[0] == '0' && itemCode[1] == '0')
	{
		if(itemCode[2] == 'g' || itemCode[2] == 't' || itemCode[2] == 'r' || itemCode[2] == 'h')
		{
			return true;
		}
	}

	// D-Stone, cookbook, ancient decipherer, etc
	if(itemCode[0] >= 'd' && itemCode[0] < 'z' && itemCode[0] == itemCode[1] && itemCode[1] == itemCode[2])
	{
		return true;
	}

	// Fake note, Maple leaf, Forging hammer, Holy symbol, socket donut
	if(strcmp(itemCode, "fkn") == 0 ||
		strcmp(itemCode, "map") == 0 ||
		strcmp(itemCode, "hh2") == 0 ||
		strcmp(itemCode, "hly") == 0 ||
		strcmp(itemCode, "sdo") == 0)
	{
		return true;
	}

	// special potions
	if(itemCode[0] == 'p' && itemCode[1] == 'o' && isdigit(itemCode[2]))
	{
		return true;
	}

	// Aura Stones, unique stones
	if(itemCode[0] == 'a' && itemCode[1] == 'n' && (itemCode[2] >= '0' && itemCode[2] <= '9'))
	{
		return true;
	}

	if(strcmp(itemCode, "sbs") == 0 ||
		strcmp(itemCode, "srj") == 0 ||
		strcmp(itemCode, "sbk") == 0 ||
		strcmp(itemCode, "toa") == 0 ||
		strcmp(itemCode, "zz0") == 0)
	{
		return true;
	}

	return false;
}

bool ItemFilter::IsRuneDecalScroll(char *itemCode)
{
	// Runes
	if(itemCode[0] == 'r' && (itemCode[1] >= '0' && itemCode[1] <= '9'))
	{
		return true;
	}

	// Decals, unidentified ancient scrolls
	if(isdigit(itemCode[0]) && isdigit(itemCode[1]) && (itemCode[2] == 'b' || itemCode[2] == 'l' ))
	{
		return true;
	}

	return false;
}

bool ItemFilter::IsRingAmulet(char *itemCode)
{
	if(strcmp(itemCode, "amu") == 0 ||
		strcmp(itemCode, "rin") == 0 ||
		strcmp(itemCode, "zrn") == 0 ||
		strcmp(itemCode, "srn") == 0 ||
		strcmp(itemCode, "nrn") == 0 ||
		strcmp(itemCode, "prn") == 0 ||
		strcmp(itemCode, "brg") == 0 ||
		strcmp(itemCode, "drn") == 0 ||
		strcmp(itemCode, "arn") == 0 ||
		strcmp(itemCode, "zam") == 0 ||
		strcmp(itemCode, "sam") == 0 ||
		strcmp(itemCode, "nam") == 0 ||
		strcmp(itemCode, "pam") == 0 ||
		strcmp(itemCode, "bam") == 0 ||
		strcmp(itemCode, "dam") == 0 ||
		strcmp(itemCode, "aam") == 0)

	{
		return true;
	}

	return false;
}

bool ItemFilter::IsMonsterPart(char *itemCode)
{
	if(strcmp(itemCode, "hrt") == 0 ||
		strcmp(itemCode, "brz") == 0 ||
		strcmp(itemCode, "jaw") == 0 ||
		strcmp(itemCode, "eyz") == 0 ||
		strcmp(itemCode, "hrn") == 0 ||
		strcmp(itemCode, "tal") == 0 ||
		strcmp(itemCode, "flg") == 0 ||
		strcmp(itemCode, "fng") == 0 ||
		strcmp(itemCode, "qll") == 0 ||
		strcmp(itemCode, "sol") == 0 ||
		strcmp(itemCode, "scz") == 0 ||
		strcmp(itemCode, "ear") == 0 ||
		strcmp(itemCode, "spe") == 0)
	{
		return true;
	}

	return false;
}
