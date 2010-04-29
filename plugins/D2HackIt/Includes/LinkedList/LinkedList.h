//
#pragma once

#include <windows.h>

typedef class LinkedItem	LinkedItem, *PLinkedItem;

BOOL __stdcall IsValidListItem(PLinkedItem li);

class LinkedItem
{
	friend class LinkedList;

private:
	PLinkedItem		pPrev;
	PLinkedItem		pNext;
	LPVOID			lpData;
	int				nLockData;
	CRITICAL_SECTION csMutex;

public:
	LinkedItem();
	~LinkedItem();

	LPVOID GetData(BOOL bLock = FALSE) const;
	LPVOID& GetData(BOOL bLock = TRUE);
	BOOL SetData(LPVOID pData);
	int LockData();
	int UnlockData();
	BOOL IsDataLocked();
};

class LinkedList
{
protected:
	PLinkedItem		pFirst;
	PLinkedItem		pLast;
	UINT			cItemCount;
	CRITICAL_SECTION csMutex;

public:
	LinkedList();
	virtual ~LinkedList();
	virtual PLinkedItem AddItem(LPVOID lpData);
	virtual PLinkedItem InsertItem(PLinkedItem pItem, LPVOID lpData);
	virtual PLinkedItem RemoveItem(PLinkedItem pItem, BOOL bUnlock = FALSE);
	PLinkedItem GetFirstItem();
	PLinkedItem GetLastItem();
	PLinkedItem GetPrevItem(PLinkedItem pItem);
	PLinkedItem GetNextItem(PLinkedItem pItem);
	UINT GetItemCount();
	virtual void RemoveAll();
};

