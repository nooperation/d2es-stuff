///////////////////////////////////////////////////////////
// Inventory.cpp
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////

#include "Inventory.h"
#include "Constants.h"
#include "definitions.h"
#include "d2functions.h"
#include "CommonStructs.h"
#include "Item.h"
#include "D2Hackit.h"
#include "me.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInventory::CInventory()
{
	m_bValid = FALSE;
	ClearAll();
}

CInventory::~CInventory()
{
	
}

void CInventory::ClearAll()
{
	m_dwCubeID = 0;
	::memset(m_aInventory, 0, sizeof(m_aInventory));
	::memset(m_aStash, 0, sizeof(m_aStash));
	::memset(m_aCube, 0, sizeof(m_aCube));
	m_aInventoryItems.RemoveAll();
	m_aStashItems.RemoveAll();
	m_aCubeItems.RemoveAll();
	m_aEquipItems.RemoveAll();
	m_bSwitching = FALSE;
}

void CInventory::OnGamePacketAfterReceived(const BYTE *aPacket, DWORD aLen)
{	
	if ((aPacket[0] == 0x9c && (aPacket[1] == ITEM_ACTION_TO_STORAGE || aPacket[1] == ITEM_ACTION_SWITCH_STORAGE)) ||
		(aPacket[0] == 0x9d && aPacket[1] == 0x15 ))
	{
		// to storage
		ITEM item;
		if (!D2ParseItem(aPacket, aLen, item))
			return;

		// Identification sends 0x9c15 with ilocation = 0. We must remove the previous item and replace it with the
		//   updated item with the full identified stats on it.
		if(aPacket[0] == 0x9d)
		{
			// Moving potions to/from belt may cause the 0x9c 0x15 packet, so ignore simple items (pots/etc)
			if(item.iSimpleItem)
				return;

			RemoveFromInventory(item.dwItemID);
		}

		SIZE cs = D2GetItemSize(item.szItemCode);
		switch (item.iStorageID)
		{
			case INV_TYPE_INVENTORY:
				if (_stricmp(item.szItemCode, "box") == 0)
					m_dwCubeID = item.dwItemID;

				m_aInventoryItems.Add(item);
				AddToInventory(item.dwItemID, item.wPositionX, item.wPositionY, cs.cx, cs.cy);
				break;

			case INV_TYPE_STASH:
				
				if (_stricmp(item.szItemCode, "box") == 0)
					m_dwCubeID = item.dwItemID;

				m_aStashItems.Add(item);
				AddToStash(item.dwItemID, item.wPositionX, item.wPositionY, cs.cx, cs.cy);
				break;

			case INV_TYPE_CUBE:
				m_aCubeItems.Add(item);
				AddToCube(item.dwItemID, item.wPositionX, item.wPositionY, cs.cx, cs.cy);
				break;

			default:
				break;
		}
	}

	if (aPacket[0] == 0x9d && aPacket[1] == ITEM_ACTION_FROM_STORAGE)
	{
		// from storage
		ITEM item;
		if (!D2ParseItem(aPacket, aLen, item))
			return;

		RemoveItem(item.dwItemID);
		switch (item.iStorageID)
		{
		case INV_TYPE_INVENTORY:
			if (_stricmp(item.szItemCode, "box") == 0)
				m_dwCubeID = 0;			
			RemoveFromInventory(item.dwItemID);
			break;

		case INV_TYPE_STASH:
			if (_stricmp(item.szItemCode, "box") == 0)
				m_dwCubeID = 0;
			RemoveFromStash(item.dwItemID);
			break;

		case INV_TYPE_CUBE:
			RemoveFromCube(item.dwItemID);
			break;

		default:
			break;
		}
	}	
}

BOOL CInventory::Dump(LPCSTR lpszFile) const
{
	FILE* file = fopen(lpszFile, "w");
	if (file == NULL)
		return FALSE;

	int i, j;

	fprintf(file, "%s\n\n", "---------- Inventory ----------");
	for (j = 0; j < INV_ROW; j++)
	{		
		for (i = 0; i < INV_COL; i++)
			fprintf(file, "%d ", !!m_aInventory[j][i]);
		
		fprintf(file, "\n");
	}

	fprintf(file, "\n\n%s\n\n", "---------- Stash ----------");
	for (j = 0; j < STASH_ROW; j++)	
	{
		for (i = 0; i < STASH_COL; i++)
			fprintf(file, "%d ", !!m_aStash[j][i]);
		
		fprintf(file, "\n");
	}

	fprintf(file, "\n\n%s\n\n", "---------- Cube ----------");
	for (j = 0; j < CUBE_ROW; j++)	
	{
		for (i = 0; i < CUBE_COL; i++)
			fprintf(file, "%08X ", m_aCube[j][i]);
		
		fprintf(file, "\n");
	}

	fclose(file);
	return TRUE;
}

BOOL CInventory::GetStorageDimensions(int storageType, int &storageWidth, int &storageHeight) const
{
	switch(storageType)
	{
		case STORAGE_INVENTORY:
			storageWidth = INV_COL;
			storageHeight = INV_ROW;
			break;
		case STORAGE_STASH:
			storageWidth = STASH_COL;
			storageHeight = STASH_ROW;
			break;
		case STORAGE_CUBE:
			storageWidth = CUBE_COL;
			storageHeight = CUBE_ROW;
			break;
		default:
			GameErrorf("GetStorageDimensions: Invalid storage type %d", storageType);
			return FALSE;
	}

	return TRUE;
}

BOOL CInventory::FindItemPosition(int storageType, DWORD dwItemID, LPPOINT position) const
{
	bool foundItem = false;
	int storageWidth = 0;
	int storageHeight = 0;

	if (dwItemID == 0)
		return FALSE;

	if(!GetStorageDimensions(storageType, storageWidth, storageHeight))
		return FALSE;

	for (int row = 0; row < storageHeight; row++)	
	{
		for (int col = 0; col < storageWidth; col++)
		{
			switch(storageType)
			{
				case STORAGE_INVENTORY:
				{
					if (m_aInventory[row][col] == dwItemID)
						foundItem = true;
					break;
				}
				case STORAGE_STASH:
				{
					if (m_aStash[row][col] == dwItemID)
						foundItem = true;
					break;
				}
				case STORAGE_CUBE:
				{
					if (m_aCube[row][col] == dwItemID)
						foundItem = true;
					break;
				}
			}

			if(foundItem)
			{
				if(position)
				{
					position->x = col;
					position->y = row;
				}

				return TRUE;
			}
		}
	}


	return FALSE;
}

BOOL CInventory::AddToStorage(int storageType, DWORD dwItemID, int x, int y, int itemWidth, int itemHeight)
{
	int storageWidth = 0;
	int storageHeight = 0;

	if (dwItemID == 0 
		|| x < 0 || y < 0  
		|| itemWidth == 0 || itemHeight == 0)
	{
		GameErrorf("AddToStorage: dwItemID == 0 || x < 0 || y < 0 || itemWidth == 0 || itemHeight == 0");
		return FALSE;
	}

	if(!GetStorageDimensions(storageType, storageWidth, storageHeight))
	{
		return FALSE;
	}

	if(x + itemWidth > storageWidth || y + itemHeight > storageHeight)
	{
		GameErrorf("AddToStorage: x = %d, itemWidth = %d, storageWidth = %d", x, itemWidth, storageWidth);
		GameErrorf("AddToStorage: y = %d, itemHeight = %d, storageHeight = %d", y, itemHeight, storageHeight);
		return FALSE;
	}

	for (int row = 0; row < itemHeight; row++)	
	{
		for (int col = 0; col < itemWidth; col++)
		{
			switch(storageType)
			{
				case STORAGE_INVENTORY:
					if (m_aInventory[row + y][col + x] != 0) // there's an item, remove it
						RemoveFromInventory(m_aInventory[row + y][col + x]);

					m_aInventory[row + y][col + x] = dwItemID; // add in
					break;
				case STORAGE_STASH:
					if (m_aStash[row + y][col + x] != 0) // there's an item, remove it
						RemoveFromStash(m_aStash[row + y][col + x]);

					m_aStash[row + y][col + x] = dwItemID; // add in
					break;
				case STORAGE_CUBE:
					if (m_aCube[row + y][col + x] != 0) // there's an item, remove it
						RemoveFromCube(m_aCube[row + y][col + x]);

					m_aCube[row + y][col + x] = dwItemID; // add in
					break;
			}
		}
	}

	return TRUE;
}

BOOL CInventory::AddToInventory(DWORD dwItemID, int x, int y, int itemWidth, int itemHeight)
{
	return AddToStorage(STORAGE_INVENTORY, dwItemID, x, y, itemWidth, itemHeight);
}

BOOL CInventory::AddToStash(DWORD dwItemID, int x, int y, int itemWidth, int itemHeight)
{
	return AddToStorage(STORAGE_STASH, dwItemID, x, y, itemWidth, itemHeight);
}

BOOL CInventory::AddToCube(DWORD dwItemID, int x, int y, int itemWidth, int itemHeight)
{
	return AddToStorage(STORAGE_CUBE, dwItemID, x, y, itemWidth, itemHeight);
}

BOOL CInventory::RemoveFromStorage(int storageType, DWORD dwItemID)
{
	int storageWidth = 0;
	int storageHeight = 0;

	if (dwItemID == 0)
		return FALSE;

	if(!GetStorageDimensions(storageType, storageWidth, storageHeight))
	{
		return FALSE;
	}

	for (int row = 0; row < storageHeight; row++)	
	{
		for (int col = 0; col < storageWidth; col++)
		{
			switch(storageType)
			{
				case STORAGE_INVENTORY:
					for(int i = 0; i < m_aInventoryItems.GetSize(); i++)
					{
						if(m_aInventoryItems[i].dwItemID == dwItemID)
						{
							m_aInventoryItems.RemoveAt(i);
							break;
						}
					}

					if (m_aInventory[row][col] == dwItemID)
						m_aInventory[row][col] = 0; // removed
					break;
				case STORAGE_STASH:
					for(int i = 0; i < m_aStashItems.GetSize(); i++)
					{
						if(m_aStashItems[i].dwItemID == dwItemID)
						{
							m_aStashItems.RemoveAt(i);
							break;
						}
					}

					if (m_aStash[row][col] == dwItemID)
						m_aStash[row][col] = 0; // removed
					break;
				case STORAGE_CUBE:
					for(int i = 0; i < m_aCubeItems.GetSize(); i++)
					{
						if(m_aCubeItems[i].dwItemID == dwItemID)
						{
							m_aCubeItems.RemoveAt(i);
							break;
						}
					}

					if (m_aCube[row][col] == dwItemID)
						m_aCube[row][col] = 0; // removed
					break;
			}
		}
	}

	return TRUE;
}

BOOL CInventory::RemoveFromInventory(DWORD dwItemID)
{
	return RemoveFromStorage(STORAGE_INVENTORY, dwItemID);
}

BOOL CInventory::RemoveFromStash(DWORD dwItemID)
{
	return RemoveFromStorage(STORAGE_STASH, dwItemID);
}

BOOL CInventory::RemoveFromCube(DWORD dwItemID)
{
	return RemoveFromStorage(STORAGE_CUBE, dwItemID);
}

BOOL CInventory::CheckStoragePosition(int storageType, int row, int col) const
{
	switch(storageType)
	{
		case STORAGE_INVENTORY:
			return row < INV_ROW && col < INV_COL && m_aInventory[row][col];
		case STORAGE_STASH:
			return row < STASH_ROW && col < STASH_COL && m_aStash[row][col];
		case STORAGE_CUBE:
			return row < CUBE_ROW && col < CUBE_COL && m_aCube[row][col];
	}

	return FALSE;
}

BOOL CInventory::FindStoragePosition(int storageType, int itemWidth, int itemHeight, LPPOINT lpBuffer, POINT startAt) const
{
	int storageWidth = 0;
	int storageHeight = 0;

	if (itemWidth == 0 || itemHeight == 0)
	{
		GameErrorf("FindStoragePosition: itemWidth == 0 || itemHeight == 0");
		return FALSE;
	}

	if(!GetStorageDimensions(storageType, storageWidth, storageHeight))
	{
		GameErrorf("FindStoragePosition: Failed to find storage position (storage = %d)", storageType);
		return FALSE;
	}

	if(startAt.x + itemWidth - 1 >= storageWidth || startAt.y + itemHeight -1 >= storageHeight)
	{
		//GameErrorf("FindStoragePosition: Item too big to fit in that spot");
		return FALSE;
	}

	int col = 0;
	for (int row = startAt.y; row < storageHeight - (itemHeight - 1); row++)
	{
		if(row == startAt.y)
		{
			col = startAt.x;
		}
		else
		{
			col = 0;
		}

		for (; col < storageWidth - (itemWidth - 1); col++)
		{
			if (CheckStoragePosition(storageType, row, col))
			{
				continue;
			}

			bool foundSpot = true;

			// Check to see if area [row][col]...[row+itemHeight][col+itemWidth] is empty
			for (int rowCheck = row; rowCheck <= row + (itemHeight - 1) && foundSpot; rowCheck++)
			{
				for (int colCheck = col; colCheck <= col + (itemWidth - 1); colCheck++)
				{
					if (CheckStoragePosition(storageType, rowCheck, colCheck))
					{
						foundSpot = false;
						//GameStringf("Item doesn't fit, item Width:%d Height:%d", itemWidth, itemHeight);
						break;
					}
				}				
			}

			if (!foundSpot)
				continue;
			
			// OK, we found
			if (lpBuffer)
			{
				lpBuffer->x = col;
				lpBuffer->y = row;
			}

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CInventory::FindInventoryPositionEx(int itemWidth, int itemHeight, LPPOINT lpBuffer, POINT startAt) const
{
	return FindStoragePosition(STORAGE_INVENTORY, itemWidth, itemHeight, lpBuffer, startAt);
}

BOOL CInventory::FindInventoryPosition(int itemWidth, int itemHeight, LPPOINT lpBuffer) const
{
	POINT startAt = {0, 0};

	return FindInventoryPositionEx(itemWidth, itemHeight, lpBuffer, startAt);
}

BOOL CInventory::FindStashPositionEx(int itemWidth, int itemHeight, LPPOINT lpBuffer, POINT startAt) const
{
	if (!m_bValid)
		return FALSE;

	return FindStoragePosition(STORAGE_STASH, itemWidth, itemHeight, lpBuffer, startAt);
}

BOOL CInventory::FindStashPosition(int itemWidth, int itemHeight, LPPOINT lpBuffer) const
{
	POINT startAt = {0, 0};

	return FindStashPositionEx(itemWidth, itemHeight, lpBuffer, startAt);
}

BOOL CInventory::FindCubePositionEx(int itemWidth, int itemHeight, LPPOINT lpBuffer, POINT startAt) const
{
	if (!m_bValid || !m_dwCubeID)
		return FALSE;

	return FindStoragePosition(STORAGE_CUBE, itemWidth, itemHeight, lpBuffer, startAt);
}

BOOL CInventory::FindCubePosition(int itemWidth, int itemHeight, LPPOINT lpBuffer) const
{
	POINT startAt = {0, 0};

	return FindCubePositionEx(itemWidth, itemHeight, lpBuffer, startAt);
}

int CInventory::FindStorageItem(DWORD dwItemID) const
{
	if (!m_bValid || dwItemID == 0)
		return FALSE;

	int i = 0;
	for (i = 0; i < m_aCubeItems.GetSize(); i++)
	{
		if (m_aCubeItems[i].dwItemID == dwItemID)
			return STORAGE_CUBE;
	}

	for (i = 0; i < m_aStashItems.GetSize(); i++)
	{
		if (m_aStashItems[i].dwItemID == dwItemID)
			return STORAGE_STASH;
	}

	for (i = 0; i < m_aInventoryItems.GetSize(); i++)
	{
		if (m_aInventoryItems[i].dwItemID == dwItemID)
			return STORAGE_INVENTORY;
	}

	return STORAGE_NONE;
}

DWORD CInventory::GetCubeID() const
{
	return m_dwCubeID;
}

void CInventory::RemoveItem(DWORD dwItemID)
{
	int i;

	for (i = m_aInventoryItems.GetSize() - 1; i >= 0; i--)
	{
		if (m_aInventoryItems[i].dwItemID == dwItemID)
		{
			m_aInventoryItems.RemoveAt(i);
			//return; // don't return, in case of multiple receive
		}
	}

	for (i = m_aStashItems.GetSize() - 1; i >= 0; i--)
	{
		if (m_aStashItems[i].dwItemID == dwItemID)
		{
			m_aStashItems.RemoveAt(i);
			//return; // don't return, in case of multiple receive
		}
	}

	for (i = m_aCubeItems.GetSize() - 1; i >= 0; i--)
	{
		if (m_aCubeItems[i].dwItemID == dwItemID)
		{
			m_aCubeItems.RemoveAt(i);
			//return; // don't return, in case of multiple receive
		}
	}
}

BOOL CInventory::EnumStorageItems(int nSTorageType, fnEnumItemProc lpfnEnumItemProc, LPARAM lParam) const
{
	if (lpfnEnumItemProc == NULL)
		return FALSE;
 
	switch (nSTorageType)
	{
		case STORAGE_INVENTORY:
			for (int i = 0; i < m_aInventoryItems.GetSize(); i++)
			{
				if (!lpfnEnumItemProc(&m_aInventoryItems[i], lParam))
					return FALSE;
			}
			break;
		
		case STORAGE_STASH:
			for (int i = 0; i < m_aStashItems.GetSize(); i++)
			{
				if (!lpfnEnumItemProc(&m_aStashItems[i], lParam))
					return FALSE;
			}
			break;

		case STORAGE_CUBE:
			for (int i = 0; i < m_aCubeItems.GetSize(); i++)
			{
				if (!lpfnEnumItemProc(&m_aCubeItems[i], lParam))
					return FALSE;
			}
			break;

		default:
			return FALSE;
			break;
	}

	return TRUE;
}

void CInventory::SetValid(BOOL bValid)
{
	m_bValid = bValid;
}

void CInventory::CheckEquip(const ITEM& item)
{
	if (item.iAction == ITEM_ACTION_TO_EQUIP)
	{
		// add equip
		m_aEquipItems.Add(item);
	}
	else if (item.iAction == ITEM_ACTION_SWITCH_EQUIP)
	{
		// switching equip
		if (!m_bSwitching)
		{
			// removing
			int nIdx = FindFromEquip(item.dwItemID);
			if (nIdx != -1)
			{
				m_aEquipItems.RemoveAt(nIdx);
				m_bSwitching = TRUE;
			}
		}
		else
		{
			// adding
			m_aEquipItems.Add(item);
			m_bSwitching = FALSE;
		}
	}
	else if (item.iAction == ITEM_ACTION_FROM_EQUIP)
	{
		// removing equip
		int nIdx = FindFromEquip(item.dwItemID);
		if (nIdx != -1)
			m_aEquipItems.RemoveAt(nIdx);
	}
}

int CInventory::FindFromEquip(DWORD dwItemID) const
{
	if (dwItemID == 0)
		return -1;

	for (int i = 0; i < m_aEquipItems.GetSize(); i++)
	{
		if (m_aEquipItems[i].dwItemID == dwItemID)
			return i;
	}

	return -1;
}

BYTE CInventory::GetLowestEquipItemDurability(BOOL bIncludeSecondarySlots) const
{
	BYTE iMinDura = 100;
	for (int i = 0; i < m_aEquipItems.GetSize(); i++)
	{
		if (m_aEquipItems[i].iMaxDurability == 0)			
			continue; // this item has no durability

		if (!bIncludeSecondarySlots && IsOnSecondary(m_aEquipItems[i].dwItemID))
			continue; // on sercondary weapon switch

		BYTE iDura = CalcPercent(m_aEquipItems[i].iDurability, m_aEquipItems[i].iMaxDurability);
		if (iDura < iMinDura)
			iMinDura = iDura;
	}

	return iMinDura;
}

BOOL CInventory::IsOnSecondary(DWORD dwItemID) const
{
	if (dwItemID == 0)
		return FALSE;

	UnitAny* pUnit = D2CLIENT_GetPlayerUnit();
	if (!pUnit)
		return FALSE;

	Inventory* pInv = pUnit->ptInventory;

	if (!pInv)
		return FALSE;

	for (UnitItem* p = pInv->pFirstItem; p && p->ptItemData; p = p->ptItemData->ptNextInvenItem)
	{
		if (p->dwId == dwItemID && (p->ptItemData->ItemLocation == EQUIP_RIGHT_SECONDARY || p->ptItemData->ItemLocation == EQUIP_LEFT_SECONDARY))
			return TRUE;
	}

	return FALSE;
}

LPCITEM CInventory::GetEquipItem(int nEquipLocation) const
{
	if (nEquipLocation == 0)
		return NULL;

	UnitAny* pUnit = D2CLIENT_GetPlayerUnit();
	if (!pUnit)
		return NULL;

	Inventory* pInv = pUnit->ptInventory;

	if (!pInv)
		return NULL;

	for (UnitItem* p = pInv->pFirstItem; p && p->ptItemData; p = p->ptItemData->ptNextInvenItem)
	{
		if (p->ptItemData->ItemLocation == (BYTE)nEquipLocation)
		{
			int nIdx = FindFromEquip(p->dwId);
			if (nIdx != -1)
				return &m_aEquipItems[nIdx];
		}
	}

	return NULL; // not found
}
