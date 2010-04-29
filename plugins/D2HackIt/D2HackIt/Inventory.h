///////////////////////////////////////////////////////////
// Inventory.h
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////

#ifndef __INVENTORY_H__
#define __INVENTORY_H__

#include <windows.h>
#include "CommonStructs.h"
#include "..\\includes\\utilities\\ArrayEx.h"
#include "..\INCLUDES\CommonStructs.h"	// Added by ClassView

// Moved to constants.h
//#define INV_ROW		9 
//#define INV_COL		11  
//#define STASH_ROW	10
//#define STASH_COL	10
//#define CUBE_ROW	8
//#define CUBE_COL	10
//#define BELT_ROW	4
//#define BELT_COL	4

#define INV_TYPE_INVENTORY	0x01
#define INV_TYPE_STASH		0x05
#define INV_TYPE_CUBE		0x04

typedef CArrayEx<ITEM, const ITEM&> ItemArray;

class CInventory  
{
public:
	LPCITEM GetEquipItem(int nEquipLocation) const;

	BYTE GetLowestEquipItemDurability(BOOL bIncludeSecondarySlots) const;
	void CheckEquip(const ITEM& item);
	void SetValid(BOOL bValid);
	BOOL EnumStorageItems(int nSTorageType, fnEnumItemProc lpfnEnumItemProc, LPARAM lParam) const;
	DWORD GetCubeID() const;
	int FindStorageItem(DWORD dwItemID) const;
	
	CInventory();
	virtual ~CInventory();

	BOOL CheckStoragePosition(int storageType, int row, int col) const;
	BOOL FindCubePosition(int itemWidth, int itemHeight, LPPOINT lpBuffer) const;
	BOOL FindCubePositionEx(int itemWidth, int itemHeight, LPPOINT lpBuffer, POINT startAt) const;
	BOOL FindStashPosition(int itemWidth, int itemHeight, LPPOINT lpBuffer) const;
	BOOL FindStashPositionEx(int itemWidth, int itemHeight, LPPOINT lpBuffer, POINT startAt) const;
	BOOL FindInventoryPosition(int itemWidth, int itemHeight, LPPOINT lpBuffer) const;
	BOOL FindInventoryPositionEx(int itemWidth, int itemHeight, LPPOINT lpBuffer, POINT startAt) const;
	BOOL FindStoragePosition(int storageType, int itemWidth, int itemHeight, LPPOINT lpBuffer, POINT startAt) const;
	
	BOOL FindItemPosition(int storageType, DWORD dwItemID, LPPOINT position) const;

	BOOL RemoveFromStorage(int storageType, DWORD dwItemID);

	void OnGamePacketAfterReceived(const BYTE* aPacket, DWORD aLen);
	void ClearAll();	
	BOOL Dump(LPCSTR lpszFile) const;

private:
	BOOL IsOnSecondary(DWORD dwItemID) const;
	int FindFromEquip(DWORD dwItemID) const;
	void RemoveItem(DWORD dwItemID);

	BOOL GetStorageDimensions(int storageType, int &storageWidth, int &storageHeight) const;

	
	BOOL RemoveFromInventory(DWORD dwItemID);
	BOOL RemoveFromStash(DWORD dwItemID);
	BOOL RemoveFromCube(DWORD dwItemID);

	BOOL AddToStorage(int storageType, DWORD dwItemID, int x, int y, int itemWidth, int itemHeight);
	BOOL AddToCube(DWORD dwItemID, int x, int y, int itemWidth, int itemHeight);
	BOOL AddToStash(DWORD dwItemID, int x, int y, int itemWidth, int itemHeight);
	BOOL AddToInventory(DWORD dwItemID, int x, int y, int itemWidth, int itemHeight);
		
	DWORD m_aInventory[INV_ROW][INV_COL]; // Inventory 10 * 4
	DWORD m_aStash[STASH_ROW][STASH_COL]; // Stash 6 * 8
	DWORD m_aCube[CUBE_ROW][CUBE_COL];    // Cube 3 * 4
	DWORD m_dwCubeID; // cube id

	ItemArray m_aInventoryItems;
	ItemArray m_aStashItems;
	ItemArray m_aCubeItems;
	ItemArray m_aEquipItems;

	BOOL m_bValid;
	BOOL m_bSwitching;
};

#endif // __INVENTORY_H__
