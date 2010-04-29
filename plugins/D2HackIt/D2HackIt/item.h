#ifndef __ITEM_H__
#define __ITEM_H__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "D2HackIt.h"
#include "CommonStructs.h"
#include <hash_map>
#include <string>

bool LoadItems();
bool LoadItemMap(std::string fileName, stdext::hash_map<std::string, std::string> &itemMap);

// Exports in server
BOOL D2ParseItem(const BYTE* aPacket, DWORD aLen, ITEM& item); // Parses item info from a packet
SIZE D2GetItemSize(LPCSTR lpszItemCode);
LPCSTR D2GetItemName(LPCSTR itemCode);
LPCSTR D2GetItemSetName(LPCSTR itemCode);
BOOL D2IsStackable(LPCSTR itemCode);
BOOL D2IsWeapon(LPCSTR itemCode);
BOOL D2IsArmor(LPCSTR itemCode);
BYTE D2GetBeltRows(LPCSTR lpszItemCode); // if it's a belt, get the row count(1-4)
int D2IsPotion(LPCSTR lpszItemCode);

#endif // __ITEM_H__
