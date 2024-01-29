//#include "stdafx.h"
#include "item.h"
#include "bitfields.h"
#include <stdio.h>
#include <fstream>
#include <unordered_set>
#include <filesystem>

#include "criticalSections.h"
#include "d2functions.h"

bool itemsLoaded = false;
std::unordered_map<std::string, std::string> itemMapAll;
std::unordered_map<std::string, std::string> itemMapSets;
std::unordered_map<std::string, std::string> itemMapWeapons;
std::unordered_map<std::string, std::string> itemMapArmor;
std::unordered_map<std::string, std::string> itemMapStackable;

std::unordered_set<std::string> itemIddqdUniques;
std::unordered_set<std::string> itemRingsAmulets;
std::unordered_set<std::string> itemRunes;
std::unordered_set<std::string> itemDecals;
std::unordered_set<std::string> itemAncientScrolls;
std::unordered_set<std::string> itemSpecialItems;

std::vector<std::string> propertyNames;
std::vector<std::string> statNames;

CRITICAL_SECTION csLoadingItems;

bool ReadFirstColumn(const std::string& filename, std::vector<std::string>& firstColumnOut);
bool LoadItemMap(std::string fileName, std::unordered_map<std::string, std::string>& itemMap);

DWORD ItemCodeStringToNum(LPCTSTR szItemCode) {
	return  ' ' << 24 | szItemCode[2] << 16 | szItemCode[1] << 8 | szItemCode[0];
}

SIZE D2GetItemSize(LPCTSTR szItemCode)
{
	const auto dwItemCode = ItemCodeStringToNum(szItemCode);
	DWORD itemsTxtIndex = 0;

	const auto itemTxt = D2COMMON_GetItemTxtFromItemCode(dwItemCode, &itemsTxtIndex);
	if (itemTxt == nullptr) {
		GameErrorf("Failed to get item record from item code '%s' while attempting to get item size", szItemCode);
		return SIZE({ 1, 1 });
	}

	const auto itemSize = SIZE({ itemTxt->nInvWidth, itemTxt->nInvHeight });
	return itemSize;
}

BOOL D2IsWeapon(LPCSTR itemCode)
{
	if(!itemsLoaded)
		LoadItems();

	return itemMapWeapons.find(itemCode) != itemMapWeapons.end();
}

BOOL D2IsArmor(LPCSTR itemCode)
{
	if(!itemsLoaded)
		LoadItems();

	return itemMapArmor.find(itemCode) != itemMapArmor.end();
}

BOOL D2IsStackable(LPCSTR itemCode)
{
	if(!itemsLoaded)
		LoadItems();

	return itemMapStackable.find(itemCode) != itemMapStackable.end();
}

LPCSTR D2GetItemSetName(LPCSTR itemCode)
{
	if(!itemsLoaded)
		LoadItems();

	auto itemMapSetsIter = itemMapSets.find(itemCode);
	if(itemMapSetsIter == itemMapSets.end())
	{
		return "Unknown";
	}

	return itemMapSetsIter->second.c_str();
}

LPCSTR D2GetItemName(LPCSTR itemCode)
{
	if(!itemsLoaded)
		LoadItems();

	auto itemMapIter = itemMapAll.find(itemCode);
	if(itemMapIter == itemMapAll.end())
	{
		return "Unknown";
	}

	return itemMapIter->second.c_str();
}

LPCSTR D2GetPropertyName(int propertyId)
{
	if (!itemsLoaded)
	{
		LoadItems();
	}

	if (propertyId < 0 || propertyId >= propertyNames.size())
	{
		return "Unknown";
	}

	return propertyNames[propertyId].c_str();
}

std::size_t D2GetNumProperties()
{
	if (!itemsLoaded)
	{
		LoadItems();
	}

	return propertyNames.size();
}

LPCSTR D2GetStatName(int statId)
{
	if (!itemsLoaded)
	{
		LoadItems();
	}

	if (statId < 0 || statId >= statNames.size())
	{
		return "Unknown";
	}

	return statNames[statId].c_str();
}

std::size_t D2GetNumStats()
{
	if (!itemsLoaded)
	{
		LoadItems();
	}

	return statNames.size();
}

std::wstring FixDisplayText(std::wstring input)
{
	std::wstring output = L"";

	for (size_t i = 0; i < input.length(); i++)
	{
		if (input[i] == '\n')
		{
			output += ' ';
		}
		else
		{
			output += input[i];
		}
	}

	return output;
}

void DumpAllItems(char *path)
{
	FILE *outFile = nullptr;
	fopen_s(&outFile, path, "w");

	if (outFile == nullptr)
	{
		return;
	}

	for (int i = 0; i < 1000000; i++)
	{
		const auto itemTxt = D2COMMON_GetItemTxt(i);
		if (itemTxt == nullptr)
		{
			break;
		}

		auto rawLocalTxt = D2LANG_GetLocaleText(itemTxt->wNameStr);
		auto displayTxt = FixDisplayText(rawLocalTxt);

		if (itemTxt->szCode[0] == ' ')
		{
			fprintf(outFile, "??? ");
		}
		if (itemTxt->szCode[1] == ' ')
		{
			fprintf(outFile, "%c ", itemTxt->szCode[0]);
		}
		else if (itemTxt->szCode[2] == ' ')
		{
			fprintf(outFile, "%c%c ", itemTxt->szCode[0], itemTxt->szCode[1]);
		}
		else if (itemTxt->szCode[3] == ' ')
		{
			fprintf(outFile, "%c%c%c ", itemTxt->szCode[0], itemTxt->szCode[1], itemTxt->szCode[2]);
		}
		else
		{
			fprintf(outFile, "%c%c%c%c ", itemTxt->szCode[0], itemTxt->szCode[1], itemTxt->szCode[2], itemTxt->szCode[3]);
		}

		fwprintf(outFile, L"%s\n", displayTxt.c_str());
	}

	fclose(outFile);
}

bool LoadItems()
{
	// I figure it's better to just wait for one thread to read and set the data
	//  instead of N threads reading and setting the data N times - reason for the
	//  mutex covering the whole function.
	EnterCriticalSection(&csLoadingItems);
	if(itemsLoaded)
	{
		LeaveCriticalSection(&csLoadingItems);
		return true;
	}

	if (!std::filesystem::exists(".\\plugin\\AllItems.txt"))
	{
		DumpAllItems(".\\plugin\\AllItems.txt");
	}

	if (!ReadFirstColumn(".\\plugin\\Properties.txt", propertyNames)) {
		GameErrorf("Unable to open .\\plugin\\Properties.txt");
		LeaveCriticalSection(&csLoadingItems);
		return false;
	}
	if (!ReadFirstColumn(".\\plugin\\ItemStatCost.txt", statNames)) {
		GameErrorf("Unable to open .\\plugin\\ItemStatCost.txt");
		LeaveCriticalSection(&csLoadingItems);
		return false;
	}

	if(!LoadItemMap(".\\plugin\\AllItems.txt", itemMapAll))
	{
		GameErrorf("Unable to open .\\plugin\\AllItems.txt");
		LeaveCriticalSection(&csLoadingItems);
		return false;
	}
	if(!LoadItemMap(".\\plugin\\Armor.txt", itemMapArmor))
	{
		GameErrorf("Unable to open .\\plugin\\Armor.txt");
		LeaveCriticalSection(&csLoadingItems);
		return false;
	}
	if(!LoadItemMap(".\\plugin\\Weapons.txt", itemMapWeapons))
	{
		GameErrorf("Unable to open .\\plugin\\Weapons.txt");
		LeaveCriticalSection(&csLoadingItems);
		return false;
	}
	if(!LoadItemMap(".\\plugin\\Sets.txt", itemMapSets))
	{
		GameErrorf("Unable to open .\\plugin\\Weapons.txt");
		LeaveCriticalSection(&csLoadingItems);
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


	itemIddqdUniques.insert("yul");
	itemIddqdUniques.insert("0cr"); 
	itemIddqdUniques.insert("0gi");
	itemIddqdUniques.insert("ci3");
	itemIddqdUniques.insert("yft");
	itemIddqdUniques.insert("amf");
	itemIddqdUniques.insert("yrb");

	itemRingsAmulets.insert("amu");
	itemRingsAmulets.insert("rin");
	itemRingsAmulets.insert("zrn");
	itemRingsAmulets.insert("srn");
	itemRingsAmulets.insert("nrn");
	itemRingsAmulets.insert("prn");
	itemRingsAmulets.insert("brg");
	itemRingsAmulets.insert("drn");
	itemRingsAmulets.insert("arn");
	itemRingsAmulets.insert("zam");
	itemRingsAmulets.insert("sam");
	itemRingsAmulets.insert("nam");
	itemRingsAmulets.insert("pam");
	itemRingsAmulets.insert("bam");
	itemRingsAmulets.insert("dam");
	itemRingsAmulets.insert("aam");

	itemRunes.insert("r01"); // �c/I Rune
	itemRunes.insert("r02"); // �c/U Rune
	itemRunes.insert("r03"); // �c/Shi Rune
	itemRunes.insert("r04"); // �c/Ka Rune
	itemRunes.insert("r05"); // �c/N Rune
	itemRunes.insert("r06"); // �c/Ku Rune
	itemRunes.insert("r07"); // �c/Yo Rune
	itemRunes.insert("r08"); // �c1Ki Rune
	itemRunes.insert("r09"); // �c1Ri Rune
	itemRunes.insert("r10"); // �c1Mi Rune
	itemRunes.insert("r11"); // �c1Ya Rune
	itemRunes.insert("r12"); // �c1A Rune
	itemRunes.insert("r14"); // �c1Chi Rune
	itemRunes.insert("r15"); // �c9Sa Rune
	itemRunes.insert("r17"); // �c9Ke Rune
	itemRunes.insert("r18"); // �c9E Rune
	itemRunes.insert("r19"); // �c9Ko Rune
	itemRunes.insert("r20"); // �c9Ra Rune
	itemRunes.insert("r21"); // �c9O Rune
	itemRunes.insert("r22"); // �c8Ho Rune
	itemRunes.insert("r23"); // �c8Me Rune
	itemRunes.insert("r24"); // �c8Ru Rune
	itemRunes.insert("r25"); // �c8Ta Rune
	itemRunes.insert("r26"); // �c8To Rune
	itemRunes.insert("r27"); // �c8Wa Rune
	itemRunes.insert("r28"); // �c8Ha Rune
	itemRunes.insert("r29"); // �c2Na Rune
	itemRunes.insert("r30"); // �c2Ni Rune
	itemRunes.insert("r32"); // �c2Fu Rune
	itemRunes.insert("r33"); // �c2Ma Rune
	itemRunes.insert("r34"); // �c2Hi Rune
	itemRunes.insert("r35"); // �c2Mo Rune
	itemRunes.insert("r36"); // �c4No Rune
	itemRunes.insert("r37"); // �c4Te Rune
	itemRunes.insert("r38"); // �c4Ro Rune
	itemRunes.insert("r39"); // �c4So Rune
	itemRunes.insert("r40"); // �c4Mu Rune
	itemRunes.insert("r41"); // �c4Ne Rune
	itemRunes.insert("r42"); // �c4Re Rune
	itemRunes.insert("r43"); // �c;Su Rune
	itemRunes.insert("r44"); // �c;He Rune
	itemRunes.insert("r45"); // �c;Nu Rune
	itemRunes.insert("r46"); // �c;Wo Rune
	itemRunes.insert("r47"); // Ka Rune
	itemRunes.insert("r51"); // El Rune
	itemRunes.insert("r52"); // Eld Rune
	itemRunes.insert("r53"); // Tir Rune
	itemRunes.insert("r54"); // Nef Rune
	itemRunes.insert("r55"); // Eth Rune
	itemRunes.insert("r56"); // Ith Rune
	itemRunes.insert("r57"); // Tal Rune
	itemRunes.insert("r58"); // Ral Rune
	itemRunes.insert("r59"); // Ort Rune
	itemRunes.insert("r60"); // Thul Rune
	itemRunes.insert("r61"); // Amn Rune
	itemRunes.insert("r62"); // Sol Rune
	itemRunes.insert("r63"); // Shael Rune
	itemRunes.insert("r64"); // Dol Rune
	itemRunes.insert("r65"); // Hel Rune
	itemRunes.insert("r66"); // Io Rune
	itemRunes.insert("r67"); // Lum Rune
	itemRunes.insert("r68"); // Ko Rune
	itemRunes.insert("r69"); // Fal Rune
	itemRunes.insert("r70"); // Lem Rune
	itemRunes.insert("r71"); // Pul Rune
	itemRunes.insert("r72"); // Um Rune
	itemRunes.insert("r73"); // Mal Rune
	itemRunes.insert("r74"); // Ist Rune
	itemRunes.insert("r75"); // Gul Rune
	itemRunes.insert("r76"); // Vex Rune
	itemRunes.insert("r77"); // Ohm Rune
	itemRunes.insert("r78"); // Lo Rune
	itemRunes.insert("r79"); // Sur Rune
	itemRunes.insert("r80"); // Ber Rune
	itemRunes.insert("r81"); // Jah Rune
	itemRunes.insert("r82"); // Cham Rune
	itemRunes.insert("r83"); // Zod Rune

	itemDecals.insert("01l"); // (I -> El) �c4Ancient Decal
	itemDecals.insert("02l"); // (U -> Eld) �c4Ancient Decal
	itemDecals.insert("03l"); // (Shi -> Tir) �c4Ancient Decal
	itemDecals.insert("04l"); // (Ka -> Nef) �c4Ancient Decal
	itemDecals.insert("05l"); // (N -> Eth) �c4Ancient Decal
	itemDecals.insert("06l"); // (Ku -> Ith) �c4Ancient Decal
	itemDecals.insert("07l"); // (Yo -> Tal) �c4Ancient Decal
	itemDecals.insert("08l"); // (Ki -> Ral) �c4Ancient Decal
	itemDecals.insert("09l"); // (Ri -> Ort) �c4Ancient Decal
	itemDecals.insert("10l"); // (Mi -> Thul) �c4Ancient Decal
	itemDecals.insert("11l"); // (Ya -> Amn) �c4Ancient Decal
	itemDecals.insert("12l"); // (A -> Sol) �c4Ancient Decal
	itemDecals.insert("13l"); // (Tsu -> Shael) �c4Ancient Decal
	itemDecals.insert("14l"); // (Chi -> Dol) �c4Ancient Decal
	itemDecals.insert("15l"); // (Sa -> Hel) �c4Ancient Decal
	itemDecals.insert("16l"); // (Yu -> Io) �c4Ancient Decal
	itemDecals.insert("17l"); // (Ke -> Lum) �c4Ancient Decal
	itemDecals.insert("18l"); // (E -> Ko) �c4Ancient Decal
	itemDecals.insert("19l"); // (Ko -> Fal) �c4Ancient Decal
	itemDecals.insert("20l"); // (Ra -> Lem) �c4Ancient Decal
	itemDecals.insert("21l"); // (Ho -> Pul) �c4Ancient Decal
	itemDecals.insert("22l"); // (Ru -> Um) �c4Ancient Decal
	itemDecals.insert("23l"); // (To -> Mal) �c4Ancient Decal
	itemDecals.insert("24l"); // (Ha -> Ist) �c4Ancient Decal
	itemDecals.insert("25l"); // (Ni -> Gul) �c4Ancient Decal
	itemDecals.insert("26l"); // (Fu -> Vex) �c4Ancient Decal
	itemDecals.insert("27l"); // (Hi -> Ohm) �c4Ancient Decal
	itemDecals.insert("28l"); // (No -> Lo) �c4Ancient Decal
	itemDecals.insert("29l"); // (Ro -> Sur) �c4Ancient Decal
	itemDecals.insert("30l"); // (Mu -> Ber) �c4Ancient Decal
	itemDecals.insert("31l"); // (Re -> Jah) �c4Ancient Decal
	itemDecals.insert("32l"); // (He -> Cham) �c4Ancient Decal
	itemDecals.insert("33l"); // (Wo -> Zod) �c4Ancient Decal

	itemAncientScrolls.insert("00a"); // �c4Ancient Scroll
	itemAncientScrolls.insert("01a"); // �c4Ancient Scroll 1
	itemAncientScrolls.insert("02a"); // �c4Ancient Scroll 2
	itemAncientScrolls.insert("03a"); // �c4Ancient Scroll 3
	itemAncientScrolls.insert("04a"); // �c4Ancient Scroll 4
	itemAncientScrolls.insert("05a"); // �c4Ancient Scroll 5
	itemAncientScrolls.insert("06a"); // �c4Ancient Scroll 6
	itemAncientScrolls.insert("07a"); // �c4Ancient Scroll 7
	itemAncientScrolls.insert("08a"); // �c4Ancient Scroll 8
	itemAncientScrolls.insert("09a"); // �c4Ancient Scroll 9
	itemAncientScrolls.insert("10a"); // �c4Ancient Scroll 10
	itemAncientScrolls.insert("11a"); // �c4Ancient Scroll 11
	itemAncientScrolls.insert("12a"); // �c4Ancient Scroll 12
	itemAncientScrolls.insert("13a"); // �c4Ancient Scroll 13
	itemAncientScrolls.insert("14a"); // �c4Ancient Scroll 14
	itemAncientScrolls.insert("15a"); // �c4Ancient Scroll 15
	itemAncientScrolls.insert("16a"); // �c4Ancient Scroll 16
	itemAncientScrolls.insert("17a"); // �c4Ancient Scroll 17
	itemAncientScrolls.insert("18a"); // �c4Ancient Scroll 18
	itemAncientScrolls.insert("19a"); // �c4Ancient Scroll 19
	itemAncientScrolls.insert("20a"); // �c4Ancient Scroll 20
	itemAncientScrolls.insert("21a"); // �c4Ancient Scroll 21
	itemAncientScrolls.insert("22a"); // �c4Ancient Scroll 22
	itemAncientScrolls.insert("23a"); // �c4Ancient Scroll 23
	itemAncientScrolls.insert("24a"); // �c4Ancient Scroll 24
	itemAncientScrolls.insert("25a"); // �c4Ancient Scroll 25
	itemAncientScrolls.insert("26a"); // �c4Ancient Scroll 26
	itemAncientScrolls.insert("27a"); // �c4Ancient Scroll 27
	itemAncientScrolls.insert("28a"); // �c4Ancient Scroll 28
	itemAncientScrolls.insert("29a"); // �c4Ancient Scroll 29
	itemAncientScrolls.insert("30a"); // �c4Ancient Scroll 30
	itemAncientScrolls.insert("31a"); // �c4Ancient Scroll 31
	itemAncientScrolls.insert("32a"); // �c4Ancient Scroll 32
	itemAncientScrolls.insert("33a"); // �c4Ancient Scroll 33
	itemAncientScrolls.insert("34a"); // �c4Ancient Scroll 34
	itemAncientScrolls.insert("35a"); // �c4Ancient Scroll 35
	itemAncientScrolls.insert("36a"); // �c4Ancient Scroll 36
	itemAncientScrolls.insert("37a"); // �c4Ancient Scroll 37
	itemAncientScrolls.insert("38a"); // �c4Ancient Scroll 38
	itemAncientScrolls.insert("39a"); // �c4Ancient Scroll 39
	itemAncientScrolls.insert("40a"); // �c4Ancient Scroll 40
	itemAncientScrolls.insert("41a"); // �c4Ancient Scroll 41
	itemAncientScrolls.insert("42a"); // �c4Ancient Scroll 42
	itemAncientScrolls.insert("43a"); // �c4Ancient Scroll 43
	itemAncientScrolls.insert("44a"); // �c4Ancient Scroll 44
	itemAncientScrolls.insert("45a"); // �c4Ancient Scroll 45
	itemAncientScrolls.insert("46a"); // �c4Ancient Scroll 46
	itemAncientScrolls.insert("47a"); // �c4Ancient Scroll 47
	itemAncientScrolls.insert("48a"); // �c4Ancient Scroll 48
	itemAncientScrolls.insert("49a"); // �c4Ancient Scroll 49
	itemAncientScrolls.insert("50a"); // �c4Ancient Scroll 50
	itemAncientScrolls.insert("00b"); // �c4Ancient Scroll
	itemAncientScrolls.insert("01b"); // �c4Ancient Scroll 1
	itemAncientScrolls.insert("02b"); // �c4Ancient Scroll 2
	itemAncientScrolls.insert("03b"); // �c4Ancient Scroll 3
	itemAncientScrolls.insert("04b"); // �c4Ancient Scroll 4
	itemAncientScrolls.insert("05b"); // �c4Ancient Scroll 5
	itemAncientScrolls.insert("06b"); // �c4Ancient Scroll 6
	itemAncientScrolls.insert("07b"); // �c4Ancient Scroll 7
	itemAncientScrolls.insert("08b"); // �c4Ancient Scroll 8
	itemAncientScrolls.insert("09b"); // �c4Ancient Scroll 9
	itemAncientScrolls.insert("10b"); // �c4Ancient Scroll 10
	itemAncientScrolls.insert("11b"); // �c4Ancient Scroll 11
	itemAncientScrolls.insert("12b"); // �c4Ancient Scroll 12
	itemAncientScrolls.insert("13b"); // �c4Ancient Scroll 13
	itemAncientScrolls.insert("14b"); // �c4Ancient Scroll 14
	itemAncientScrolls.insert("15b"); // �c4Ancient Scroll 15
	itemAncientScrolls.insert("16b"); // �c4Ancient Scroll 16
	itemAncientScrolls.insert("17b"); // �c4Ancient Scroll 17
	itemAncientScrolls.insert("18b"); // �c4Ancient Scroll 18
	itemAncientScrolls.insert("19b"); // �c4Ancient Scroll 19
	itemAncientScrolls.insert("20b"); // �c4Ancient Scroll 20
	itemAncientScrolls.insert("21b"); // �c4Ancient Scroll 21
	itemAncientScrolls.insert("22b"); // �c4Ancient Scroll 22
	itemAncientScrolls.insert("23b"); // �c4Ancient Scroll 23
	itemAncientScrolls.insert("24b"); // �c4Ancient Scroll 24
	itemAncientScrolls.insert("25b"); // �c4Ancient Scroll 25
	itemAncientScrolls.insert("26b"); // �c4Ancient Scroll 26
	itemAncientScrolls.insert("27b"); // �c4Ancient Scroll 27
	itemAncientScrolls.insert("28b"); // �c4Ancient Scroll 28
	itemAncientScrolls.insert("29b"); // �c4Ancient Scroll 29
	itemAncientScrolls.insert("30b"); // �c4Ancient Scroll 30
	itemAncientScrolls.insert("31b"); // �c4Ancient Scroll 31
	itemAncientScrolls.insert("32b"); // �c4Ancient Scroll 32
	itemAncientScrolls.insert("33b"); // �c4Ancient Scroll 33
	itemAncientScrolls.insert("34b"); // �c4Ancient Scroll 34
	itemAncientScrolls.insert("35b"); // �c4Ancient Scroll 35
	itemAncientScrolls.insert("36b"); // �c4Ancient Scroll 36
	itemAncientScrolls.insert("37b"); // �c4Ancient Scroll 37
	itemAncientScrolls.insert("38b"); // �c4Ancient Scroll 38
	itemAncientScrolls.insert("39b"); // �c4Ancient Scroll 39
	itemAncientScrolls.insert("40b"); // �c4Ancient Scroll 40
	itemAncientScrolls.insert("41b"); // �c4Ancient Scroll 41
	itemAncientScrolls.insert("42b"); // �c4Ancient Scroll 42
	itemAncientScrolls.insert("43b"); // �c4Ancient Scroll 43
	itemAncientScrolls.insert("44b"); // �c4Ancient Scroll 44
	itemAncientScrolls.insert("45b"); // �c4Ancient Scroll 45
	itemAncientScrolls.insert("46b"); // �c4Ancient Scroll 46
	itemAncientScrolls.insert("47b"); // �c4Ancient Scroll 47
	itemAncientScrolls.insert("48b"); // �c4Ancient Scroll 48
	itemAncientScrolls.insert("49b"); // �c4Ancient Scroll 49
	itemAncientScrolls.insert("50b"); // �c4Ancient Scroll 50

	itemsLoaded = true;
	LeaveCriticalSection(&csLoadingItems);

	return true;
}

bool LoadItemSizeMap(std::string fileName, std::unordered_map<std::string, SIZE> &itemSizeMap)
{
	std::ifstream inFile(fileName.c_str());
	std::string readBuff;

	if (!inFile)
	{
		return false;
	}

	itemSizeMap.clear();

	while (inFile.good())
	{
		std::getline(inFile, readBuff);
		if (readBuff.length() <= 0)
		{
			continue;
		}

		auto itemName = readBuff.substr(0, 3);
		auto itemSize = readBuff.substr(4);

		if (itemName.length() < 3)
		{
			continue;
		}

		int width = 0;
		int height = 0;
		if (sscanf_s(itemSize.c_str(), "%d %d", &width, &height) != 2)
		{
			throw std::exception(("Invalid item size in " + fileName).c_str());
		}

		itemSizeMap[itemName].cx = width;
		itemSizeMap[itemName].cy = height;
	}

	inFile.close();

	return true;
}

bool ReadFirstColumn(const std::string& filename, std::vector<std::string> &firstColumnOut)
{
	firstColumnOut.clear();

	std::ifstream inFile(filename);

	if (!inFile.is_open()) 
	{
		return false;
	}

	std::string line;
	std::getline(inFile, line);

	while (std::getline(inFile, line))
	{
		const auto pos = line.find('\t');
		if (pos != std::string::npos) 
		{
			auto firstCell = line.substr(0, pos);

			if (firstCell != "Expansion") 
			{
				firstColumnOut.push_back(firstCell);
			}
		}
	}

	inFile.close();
	return true;
}

bool LoadItemMap(std::string fileName, std::unordered_map<std::string, std::string> &itemMap)
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

// This can actually read the majority of item properties, except for thoses with a 'Send Param' - these properties contain nested and potentially recursive properties. I only care about ores right now for autoextraction stuff.
void ReadOreProperties(BitFields iPacket, ITEM &item)
{
	while (item.iNumProperties < ITEM_PROPERTIES_MAX)
	{
		// Property IDs are all 9bits. All item property lists end with the propertyId of 0x1ff
		const auto currentPropertyID = iPacket.GetField(9);
		if (currentPropertyID == 0x1ff)
		{
			break;
		}

		// xtal_00 - xtal_11 (ItemStatCost.txt)
		if (currentPropertyID < 470 || currentPropertyID > 481)
		{
			break;
		}

		// HACK: See 'Send Bits' field in ItemStatCosts.txt. This is hardcoded for the ore proerties "xtal_00 - xtal_11"
		const auto val = iPacket.GetField(18);

		item.aProperties[item.iNumProperties].id = currentPropertyID;
		item.aProperties[item.iNumProperties].value = val;
		item.iNumProperties++;
	}
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
		{
			item.dwGoldAmount = 
				(iPacket.GetField(8) & 0xFF) << 0 |
				(iPacket.GetField(8) & 0xFF) << 8 |
				(iPacket.GetField(8) & 0xFF) << 16 |
				(iPacket.GetField(8) & 0xFF) << 24;
		}
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

	// Useless because d2hackit doesn't update quantity of inventory items on use. Keeping for a future note though.
	//if(strcmp(item.szItemCode, "tbk") == 0)
	//{
	//	auto unknown = iPacket.GetField(5);
	//	auto amount = iPacket.GetField(9);
	//	item.iQuantity = amount;
	//	return TRUE;
	//}

	if(D2IsStackable(item.szItemCode))
	{
		item.iQuantity = (WORD)iPacket.GetField(9);
	}
	
	if(!item.iIdentified)
		return TRUE;

	if (_stricmp(item.szItemCode, "ore") == 0)
	{
		ReadOreProperties(iPacket, item);
	}

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