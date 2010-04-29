//#include "stdafx.h"
#include "item.h"
#include "bitfields.h"
#include "itemsize.h"
#include <stdio.h>
#include <fstream>
#include "criticalSections.h"

bool itemsLoaded = false;
stdext::hash_map<std::string, std::string> itemMapAll;
stdext::hash_map<std::string, std::string> itemMapSets;
stdext::hash_map<std::string, std::string> itemMapWeapons;
stdext::hash_map<std::string, std::string> itemMapArmor;
stdext::hash_map<std::string, std::string> itemMapStackable;
CRITICAL_SECTION csLoadingItems;

SIZE D2GetItemSize(LPCTSTR lpszItemCode)
{
	SIZE val = { 0 };
	if (lpszItemCode == NULL)
		return val;

	for (int i = 0; ITEM_SIZE_TB[i].code; i++)
	{
		if (_stricmp(lpszItemCode, ITEM_SIZE_TB[i].code) == 0)
		{
			val.cx = ITEM_SIZE_TB[i].x;
			val.cy = ITEM_SIZE_TB[i].y;
			return val;
		}
	}

	return val;
}

BOOL D2IsWeapon(LPCSTR itemCode)
{
	if(!itemsLoaded)
		LoadItems();

	return itemMapWeapons.count(itemCode) > 0;
}

BOOL D2IsArmor(LPCSTR itemCode)
{
	if(!itemsLoaded)
		LoadItems();

	return itemMapArmor.count(itemCode) > 0;
}

BOOL D2IsStackable(LPCSTR itemCode)
{
	if(!itemsLoaded)
		LoadItems();

	return itemMapStackable.count(itemCode) > 0;
}

LPCSTR D2GetItemSetName(LPCSTR itemCode)
{
	if(!itemsLoaded)
		LoadItems();

	if(itemMapSets.count(itemCode) == 0)
	{
		return "Unknown";
	}
	else
	{
		return itemMapSets[itemCode].c_str();
	}
}

LPCSTR D2GetItemName(LPCSTR itemCode)
{
	if(!itemsLoaded)
		LoadItems();

	if(itemMapAll.count(itemCode) == 0)
	{
		return "Unknown";
	}
	else
	{
		return itemMapAll[itemCode].c_str();
	}
}

bool LoadItems()
{
	// I figure it's better to just wait for one thread to read and set the data
	//  instead of N threads reading and setting the data N times - reason for the
	//  mutex covering the whole function.
	EnterCriticalSection(&csLoadingItems);
	if(itemsLoaded)
	{
		return true;
	}

	if(!LoadItemMap(".\\plugin\\AllItems.txt", itemMapAll))
	{
		GameErrorf("Unable to open .\\plugin\\AllItems.txt");
		return false;
	}
	if(!LoadItemMap(".\\plugin\\Armor.txt", itemMapArmor))
	{
		GameErrorf("Unable to open .\\plugin\\Armor.txt");
		return false;
	}
	if(!LoadItemMap(".\\plugin\\Weapons.txt", itemMapWeapons))
	{
		GameErrorf("Unable to open .\\plugin\\Weapons.txt");
		return false;
	}
	if(!LoadItemMap(".\\plugin\\Sets.txt", itemMapSets))
	{
		GameErrorf("Unable to open .\\plugin\\Weapons.txt");
		return false;
	}

	itemMapStackable["jav"] = "Javelin";
	itemMapStackable["pil"] = "Pilum";
	itemMapStackable["ssp"] = "Short Spear";
	itemMapStackable["glv"] = "Glaive";
	itemMapStackable["tsp"] = "Throwing Spear";
	itemMapStackable["9ja"] = "War Javelin";
	itemMapStackable["9pi"] = "Great Pilum";
	itemMapStackable["9s9"] = "Simbilan";
	itemMapStackable["9gl"] = "Spiculum";
	itemMapStackable["9ts"] = "Harpoon";
	itemMapStackable["0ja"] = "Ancient Shards";
	itemMapStackable["0pi"] = "Dire Pilum";
	itemMapStackable["0s9"] = "Lissom Spear";
	itemMapStackable["0gl"] = "Ahab Spear";
	itemMapStackable["0ts"] = "Whale Hunter";
	itemMapStackable["am5"] = "Maiden Javelin";
	itemMapStackable["ama"] = "Ceremonial Javelin";
	itemMapStackable["amf"] = "MatriarchalJavelin";
	itemMapStackable["tkf"] = "Throwing Knife";
	itemMapStackable["tax"] = "Throwing Axe";
	itemMapStackable["bkf"] = "Balanced Knife";
	itemMapStackable["bal"] = "Balanced Axe";
	itemMapStackable["9tk"] = "Battle Dart";
	itemMapStackable["9ta"] = "Francisca";
	itemMapStackable["9bk"] = "War Dart";
	itemMapStackable["9b8"] = "Hurlbat";
	itemMapStackable["0tk"] = "Throwing Dagger";
	itemMapStackable["0ta"] = "Throwing Hatchet";
	itemMapStackable["0bk"] = "Balanced Dagger";
	itemMapStackable["0b6"] = "Balanced Hatchet";
	itemMapStackable["bts"] = "Tribal Hatchet";
	itemMapStackable["9xb"] = "Highland Hatchet";
	itemMapStackable["0xb"] = "Arreat's Hatchet";
	itemMapStackable["gps"] = "Rancid Gas Potion";
	itemMapStackable["ops"] = "Oil Potion";
	itemMapStackable["gpm"] = "Choking Gas Potion";
	itemMapStackable["opm"] = "Exploding Potion";
	itemMapStackable["gpl"] = "Strangling Gas Potion";
	itemMapStackable["opl"] = "Fulminating Potion";
	itemMapStackable["aqv"] = "Arrows";
	itemMapStackable["cqv"] = "Bolts";
	itemMapStackable["aq2"] = "Magic Arrows";
	itemMapStackable["cq2"] = "Magic Bolts";
	itemMapStackable["aq0"] = "Magic Arrows (soc)";
	itemMapStackable["cq0"] = "Magic Bolts (soc)";
	//itemMapStackable["tbk"] = "Town Portal Book";
	//itemMapStackable["ibk"] = "Identify Book";
	itemMapStackable["key"] = "Key";

	itemsLoaded = true;
	LeaveCriticalSection(&csLoadingItems);

	return true;
}

bool LoadItemMap(std::string fileName, stdext::hash_map<std::string, std::string> &itemMap)
{
	std::ifstream inFile(fileName.c_str());

	std::string itemName;
	std::string itemDesc;
	std::string readBuff;

	if(!inFile)
	{
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
			itemMap[itemName] = itemDesc.c_str();
	}

	inFile.close();

	return true;
}

BOOL D2ParseItem(const BYTE *aPacket, DWORD aLen, ITEM& item)
{
	::memset(&item, 0 ,sizeof(ITEM));

	if (aPacket[0] != 0x9c && aPacket[0] != 0x9d)
		return FALSE;
	
	item.iLevel = 1;
	BitFields iPacket(aPacket,aLen);	

	item.iMessageID = (BYTE)iPacket.GetField(8); //message
	item.iAction = (BYTE)iPacket.GetField(8);
	iPacket.Skip(8); //message size
	item.iType = (BYTE)iPacket.GetField(8);
	item.dwItemID = iPacket.GetField(32);

	if (aPacket[0] == 0x9d)
	{
		item.iOwnerAction = (BYTE)iPacket.GetField(8);
		item.dwOwnerID = iPacket.GetField(32);
	}

	if (aPacket[1] == ITEM_ACTION_TO_BELTSLOT || aPacket[1] == ITEM_ACTION_FROM_BELTSLOT || aPacket[1] == ITEM_ACTION_SWITCH_BELTSLOT)
	{
		// determine column & row
		item.iAtBeltRow = (((aPacket[14] % 32) - (aPacket[14] % 8)) / 8) % 4;
		item.iAtBeltColumn = ((aPacket[14] % 8) / 2) % 4;
	}
	else if (aPacket[1] == ITEM_ACTION_SHIFT_BELTSLOT)
	{
		// determine column & row
		item.iAtBeltRow = (((aPacket[19] % 32) - (aPacket[19] % 8)) / 8) % 4;
		item.iAtBeltColumn = ((aPacket[19] % 8) / 2) % 4;
	}

	item.iIsEquipped = (BYTE)iPacket.GetField(1);
	iPacket.Skip(2); // Unknown 1
	item.iInSocket = (BYTE)iPacket.GetField(1);
	item.iIdentified = (BYTE)iPacket.GetField(1);
	iPacket.Skip(1); // Unknown 2
	item.iSwtichIn = (BYTE)iPacket.GetField(1);
	item.iSwitchOut = (BYTE)iPacket.GetField(1);
	item.iBroken = (BYTE)iPacket.GetField(1);
	iPacket.Skip(1); // Unknown 3 +
	iPacket.Skip(1); // Potion (?)
	item.iSocketed = (BYTE)iPacket.GetField(1);
	iPacket.Skip(1); // Unknown 4
	item.iStore = (BYTE)iPacket.GetField(1);
	iPacket.Skip(1); // NotInSocket (?)
	iPacket.Skip(1); // Unknown 5
	item.iEar = (BYTE)iPacket.GetField(1);
	item.iStarterItem = (BYTE)iPacket.GetField(1);
	iPacket.Skip(3); // Unknown 6
	item.iSimpleItem = (BYTE)iPacket.GetField(1);
	item.iEthereal = (BYTE)iPacket.GetField(1);
	iPacket.Skip(1); // Any (?)

	item.iPersonalized = (BYTE)iPacket.GetField(1);
	item.iGamble = (BYTE)iPacket.GetField(1);
	item.iRuneword = (BYTE)iPacket.GetField(1);
	iPacket.Skip(15);
	item.iLocation = (BYTE)iPacket.GetField(3);

	if(aPacket[1] == ITEM_ACTION_DROP || aPacket[1] == ITEM_ACTION_NEW_GROUND || aPacket[1] == ITEM_ACTION_OLD_GROUND)
	{
		item.wPositionX = (WORD)iPacket.GetField(16);
		item.wPositionY = (WORD)iPacket.GetField(16);
	}
	else
	{
		item.iPositionOnBody = (BYTE)iPacket.GetField(4);
		item.wPositionX = (WORD)iPacket.GetField(4);
		item.wPositionY = (WORD)iPacket.GetField(4);
		item.iStorageID = (BYTE)iPacket.GetField(3);
	}

	if (item.iEar)
	{
		BYTE earClass = (BYTE)iPacket.GetField(3);
		BYTE earLevel = (BYTE)iPacket.GetField(7);
		// earName is the rest of the packet

		item.iLevel = earLevel;
		sprintf_s(item.szItemCode, 4, "ear");
		
		return TRUE;
	}
	
	item.szItemCode[0] = (char)iPacket.GetField(8);
	item.szItemCode[1] = (char)iPacket.GetField(8);
	item.szItemCode[2] = (char)iPacket.GetField(8);
	item.szItemCode[3] = '\0';
	iPacket.Skip(8); // last bit of itemcode ignored

	for(int i = 0; i < 3; i++)
	{
		if(!isalnum(item.szItemCode[i]))
		{
			::memset(&item, 0 ,sizeof(ITEM));
			
			return FALSE;
		}
	}

	if (!_stricmp(item.szItemCode, "gld"))
	{
		if(iPacket.GetField(1))
			item.dwGoldAmount = iPacket.GetField(32);
		else
			item.dwGoldAmount = iPacket.GetField(12);
		return TRUE;
	}

	if(item.iSimpleItem || item.iGamble ||
		!_stricmp(item.szItemCode, "tbk") ||
		!_stricmp(item.szItemCode, "ibk") ||
		!_stricmp(item.szItemCode, "box"))
	{
		return TRUE;
	}

	if(iPacket.GetPos() + 14 >= iPacket.GetMax())
	{
		GameErrorf("Item %s has no level data", item.szItemCode);
		return FALSE;
	}

	item.iGemCount = (BYTE)iPacket.GetField(3);
	item.iLevel = (BYTE)iPacket.GetField(7);
	item.iQuality = (BYTE)iPacket.GetField(4);

	item.iHasGraphic = (BYTE)iPacket.GetField(1);
	if(item.iHasGraphic)
	{
		item.iGraphic = (BYTE)iPacket.GetField(3);
	}

	// No idea what this flag is. Some say Class data others say Color data
	if(iPacket.GetField(1))
	{
		iPacket.Skip(11);
	}

	if(!item.iIdentified)
	{
		return TRUE;
	}

	switch(item.iQuality)
	{
		case ITEM_LEVEL_INFERIOR:
		{
			iPacket.Skip(3);
			break;
		}
		case ITEM_LEVEL_SUPERIOR:
		{
			iPacket.Skip(3);
			break;
		}
		case ITEM_LEVEL_MAGIC:
		{
			item.wPrefix[0] = (WORD)iPacket.GetField(11);
			item.wSuffix[0] = (WORD)iPacket.GetField(11);
			break;
		}
		case ITEM_LEVEL_RARE:
		case ITEM_LEVEL_CRAFT:
		{
			iPacket.Skip(8); // Prefix item name
			iPacket.Skip(8); // Suffix item name

			for(int i = 0; i < ITEM_MAX_PREFIX; i++)
			{
				if(iPacket.GetField(1))
				{
					item.wPrefix[i] = (WORD)iPacket.GetField(11);
				}
				if(iPacket.GetField(1))
				{
					item.wSuffix[i] = (WORD)iPacket.GetField(11);
				}
			}

			break;
		}
		case ITEM_LEVEL_SET:
		case ITEM_LEVEL_UNIQUE:
		{
			item.wSetUniqueID = (WORD)iPacket.GetField(12);
			break;
		}
	}

	if(item.iPersonalized)
	{
		iPacket.Skip(15);
	}
	if(item.iRuneword)
	{
		iPacket.Skip(16);
	}

	if(D2IsArmor(item.szItemCode))
	{
		// 1095 from ItemStatCost.txt. It just allows for a minimum of -1095 defense
		item.wDefense = (WORD)(iPacket.GetField(12) - 1095); 
	}

	if(D2IsWeapon(item.szItemCode) || D2IsArmor(item.szItemCode))
	{
		item.iMaxDurability = (BYTE)iPacket.GetField(8);

		if(item.iMaxDurability != 0)
		{
			item.iDurability = (BYTE)iPacket.GetField(8);
		}
		else
		{
			item.iIndestructable = TRUE;
		}
	}

	if(item.iSocketed)
	{
		item.iSocketNumber = (BYTE)iPacket.GetField(4);
	}

	if(D2IsStackable(item.szItemCode))
	{
		item.iQuantity = (WORD)iPacket.GetField(9);
	}

	if(!item.iIdentified)
		return TRUE;

	return TRUE;
}


BYTE D2GetBeltRows(LPCTSTR lpszItemCode)
{
	if (lpszItemCode == NULL)
		return 0;

	if (_stricmp(lpszItemCode, "lbl") == 0 // sash
		|| _stricmp(lpszItemCode, "vbl") == 0) // light belt
		return 2;
	else if (_stricmp(lpszItemCode, "mbl") == 0 // belt
		|| _stricmp(lpszItemCode, "tbl") == 0) // heavy belt
		return 3;
	else if (_stricmp(lpszItemCode, "hbl") == 0 // plated belt
		|| _stricmp(lpszItemCode, "zlb") == 0 // demonhide sash
		|| _stricmp(lpszItemCode, "zvb") == 0 // sharkskin belt
		|| _stricmp(lpszItemCode, "zmb") == 0 // mesh belt
		|| _stricmp(lpszItemCode, "ztb") == 0 // battle belt
		|| _stricmp(lpszItemCode, "zhb") == 0 // war belt
		|| _stricmp(lpszItemCode, "xms") == 0 //xms Giant Wrap
		|| _stricmp(lpszItemCode, "avb") == 0 //avb Rogue Belt
		|| _stricmp(lpszItemCode, "bmb") == 0 //bmb Merchant Belt
		|| _stricmp(lpszItemCode, "atb") == 0 //atb Gladiator Belt
		|| _stricmp(lpszItemCode, "ahb") == 0 //ahb Titan Belt
		|| _stricmp(lpszItemCode, "yms") == 0 //yms Herculean Wrap
		|| _stricmp(lpszItemCode, "alb") == 0 //alb Hemp Band
		|| _stricmp(lpszItemCode, "ulc") == 0 // Spiderweb Sash
		|| _stricmp(lpszItemCode, "uvc") == 0 // Vampirefang Belt
		|| _stricmp(lpszItemCode, "umc") == 0 // Mithril Coil
		|| _stricmp(lpszItemCode, "utc") == 0 // Troll Belt
		|| _stricmp(lpszItemCode, "uhc") == 0) // Colossus Girdle
		return 4;
	else
		return 0;
}

int D2IsPotion(LPCSTR lpszItemCode)
{
	if (lpszItemCode == NULL)
		return POTION_UNKNOWN;
	
	if (!_stricmp(lpszItemCode, "rvl")
		|| !_stricmp(lpszItemCode, "rvs"))
		return POTION_PURPLE;

	if (!_stricmp(lpszItemCode, "hp1")
		|| !_stricmp(lpszItemCode, "hp2")
		|| !_stricmp(lpszItemCode, "hp3")
		|| !_stricmp(lpszItemCode, "hp4")
		|| !_stricmp(lpszItemCode, "hp5"))
		return POTION_RED;

	if (!_stricmp(lpszItemCode, "mp1")
		|| !_stricmp(lpszItemCode, "mp2")
		|| !_stricmp(lpszItemCode, "mp3")
		|| !_stricmp(lpszItemCode, "mp4")
		|| !_stricmp(lpszItemCode, "mp5"))
		return POTION_BLUE;


	if (!_stricmp(lpszItemCode, "yps"))
		return POTION_ANTIDOTE;

	if (!_stricmp(lpszItemCode, "vps"))
		return POTION_STAMINA;

	if (!_stricmp(lpszItemCode, "wms"))
		return POTION_THAWING;

	if(!_stricmp(lpszItemCode, "po1") // Potion of Heroism
		|| !_stricmp(lpszItemCode, "po2") // Potion of Super-Heroism
		|| !_stricmp(lpszItemCode, "po3") // Mountain Dew
		|| !_stricmp(lpszItemCode, "po4")) // Nirvana Grass
		return POTION_SPECIAL;

	return POTION_UNKNOWN;
}

/*
// Keeping for comedy value
DWORD D2ItemCode2Dword(LPCTSTR lpszItemCode) // convert a string into dword
{
	if (lpszItemCode == NULL)
		return 0;

	DWORD dw = 0;
	::memcpy(&dw, lpszItemCode, ITEM_CODE_LEN);
	return dw;
}
*/