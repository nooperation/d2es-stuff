//
#include "LinkedList.h"
#include <stdio.h>

#ifdef D2HACKIT_DEBUG
extern	"C" char g_szDebug[512];
#define	DBGHDR			g_szDebug, "D2HACKIT : "
#define DGTRACE( _x_ )	sprintf _x_; OutputDebugString(g_szDebug);
#else
#define DGTRACE( _x_ )
#endif


BOOL __stdcall IsValidListItem(PLinkedItem li)
{
	return (li != NULL &&
		!IsBadReadPtr(li, sizeof(LinkedItem)) &&
		!IsBadWritePtr((LPVOID)li, sizeof(LinkedItem)) );
}

//============================================================================
//	LinkedItem Constructor/Destructor
//============================================================================
LinkedItem::LinkedItem()
{
	ZeroMemory(this, sizeof(LinkedItem));
	InitializeCriticalSection(&csMutex);
}

LinkedItem::~LinkedItem()
{
	DGTRACE((DBGHDR "LinkedItem::~LinkedItem()\n"));

	if(lpData)
	{
		if(nLockData > 0)
		{
			DGTRACE((DBGHDR "Tryng to delete LinkedItem(%X) while LockCount = %d\n", this, nLockData));
		}
		else delete lpData;
	}

	DeleteCriticalSection(&csMutex);
}

//============================================================================
//	LinkedItem Operations
//============================================================================
LPVOID LinkedItem::GetData(BOOL bLock) const
{
	LPVOID pD = NULL;
	EnterCriticalSection((CRITICAL_SECTION*)&csMutex);
	if(lpData)
	{
		pD = lpData;
		if(bLock) ++((int)nLockData);
	}
	LeaveCriticalSection((CRITICAL_SECTION*)&csMutex);

	return pD;
}

LPVOID& LinkedItem::GetData(BOOL bLock)
{
	EnterCriticalSection(&csMutex);
	if(lpData && bLock)
	{
		++nLockData;
	}
	LeaveCriticalSection(&csMutex);

	return lpData;
}

BOOL LinkedItem::SetData(LPVOID pData)
{
	BOOL bRet = TRUE;
	EnterCriticalSection(&csMutex);
	if(lpData)
	{
		if(nLockData > 0) bRet = FALSE;
		else delete lpData;
	}
	lpData = pData;
	nLockData = 0;
	LeaveCriticalSection(&csMutex);

	return bRet;
}

int LinkedItem::LockData()
{
	int nLock = 0;
	EnterCriticalSection(&csMutex);
	if(lpData)
	{
		++nLockData;
		nLock = nLockData;
	}
	LeaveCriticalSection(&csMutex);
	return nLock;
}

int LinkedItem::UnlockData()
{
	int nLock = 0;
	EnterCriticalSection(&csMutex);
	if(nLockData > 0) --nLockData;
	nLock = nLockData;
	LeaveCriticalSection(&csMutex);
	return nLock;
}

BOOL LinkedItem::IsDataLocked()
{
	BOOL bLocked = FALSE;
	EnterCriticalSection(&csMutex);
	if(lpData && nLockData > 0) bLocked = TRUE;
	LeaveCriticalSection(&csMutex);

	return bLocked;
}

//============================================================================
//	LinkedList Constructor/Destructor
//============================================================================
LinkedList::LinkedList()
{
	pFirst = NULL;
	pLast = NULL;
	cItemCount = 0;

	InitializeCriticalSection(&csMutex);
}

LinkedList::~LinkedList()
{
	DGTRACE((DBGHDR "LinkedList::~LinkedList()\n"));

	RemoveAll();
	DeleteCriticalSection(&csMutex);
}

//============================================================================
//	RemoveAll
//----------------------------------------------------------------------------
//============================================================================
void LinkedList::RemoveAll()
{
	LinkedItem* pItem = pFirst;
	while(pItem && IsValidListItem(pItem))
	{
		pItem = RemoveItem(pItem);
	}
}
//============================================================================
//	AddItem
//----------------------------------------------------------------------------
//============================================================================
PLinkedItem LinkedList::AddItem(LPVOID lpData)
{
	return InsertItem(NULL, lpData);
}

//============================================================================
//	InsertItem
//----------------------------------------------------------------------------
// if pItem is NULL, adds to end of list, otherwise adds before pItem
//============================================================================
PLinkedItem LinkedList::InsertItem(PLinkedItem pItem, LPVOID lpData)
{
	if(pItem && !IsValidListItem(pItem)) return NULL;

	LinkedItem* pnItem = new LinkedItem;
	if(!pnItem) return NULL;

	EnterCriticalSection(&csMutex);

	pnItem->SetData(lpData);
	pnItem->pNext = pItem;
	if(pItem)
	{
		pnItem->pPrev = pItem->pPrev;
		if(pnItem->pPrev) pnItem->pPrev->pNext = pnItem;
		pItem->pPrev = pnItem;
	}
	else
	{
		pnItem->pPrev = pLast;
		if(pLast) pLast->pNext = pnItem;
		pLast = pnItem;
	}

	if(pItem == pFirst) pFirst = pnItem;

	++cItemCount;
	LeaveCriticalSection(&csMutex);

	return pnItem;
}

//============================================================================
//	RemoveItem
//----------------------------------------------------------------------------
//============================================================================
PLinkedItem LinkedList::RemoveItem(PLinkedItem pItem, BOOL bUnlock)
{
	if(!pFirst || !pLast || !IsValidListItem(pItem)) return NULL;

	EnterCriticalSection(&csMutex);
	PLinkedItem pNext = pItem->pNext;

	if(bUnlock) pItem->UnlockData();

	if(!pItem->IsDataLocked())
	{
		if(pItem->pPrev) pItem->pPrev->pNext = pItem->pNext;
		if(pItem->pNext) pItem->pNext->pPrev = pItem->pPrev;
		if(pItem == pFirst) pFirst = pItem->pNext;
		if(pItem == pLast) pLast = pItem->pPrev;

		delete pItem;

		--cItemCount;
	}
	else
	{
		DGTRACE((DBGHDR "Tryng to delete Locked LinkedItem(%X)\n", pItem));
	}
	LeaveCriticalSection(&csMutex);

	return pNext;
}

//============================================================================
//	GetFirstItem
//----------------------------------------------------------------------------
//============================================================================
PLinkedItem LinkedList::GetFirstItem()
{
	EnterCriticalSection(&csMutex);
	LinkedItem* pItem = pFirst;
	LeaveCriticalSection(&csMutex);

	return pItem;
}

//============================================================================
//	GetLastItem
//----------------------------------------------------------------------------
//============================================================================
PLinkedItem LinkedList::GetLastItem()
{
	EnterCriticalSection(&csMutex);
	LinkedItem* pItem = pLast;
	LeaveCriticalSection(&csMutex);

	return pItem;
}

//============================================================================
//	GetPrevItem
//----------------------------------------------------------------------------
//============================================================================
PLinkedItem LinkedList::GetPrevItem(PLinkedItem pItem)
{
	EnterCriticalSection(&csMutex);
	if(IsValidListItem(pItem)) pItem = pItem->pPrev;
	else pItem  = NULL;
	LeaveCriticalSection(&csMutex);

	return pItem;
}

//============================================================================
//	GetNextItem
//----------------------------------------------------------------------------
//============================================================================
PLinkedItem LinkedList::GetNextItem(PLinkedItem pItem)
{
	EnterCriticalSection(&csMutex);
	if(IsValidListItem(pItem)) pItem = pItem->pNext;
	else pItem  = NULL;
	LeaveCriticalSection(&csMutex);

	return pItem;
}

//============================================================================
//	GetItemCount
//----------------------------------------------------------------------------
//============================================================================
UINT LinkedList::GetItemCount()
{
	EnterCriticalSection(&csMutex);
	UINT nCount = cItemCount;
	LeaveCriticalSection(&csMutex);

	return nCount;
}




