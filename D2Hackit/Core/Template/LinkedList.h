//////////////////////////////////////////////////////////////////////
// LinkedList.h
//
// CLinkedList is a template class which implements a double linked list
// data structure without the need of MFC or STL libraries. 
//
// 2003-9-08 Abin
//////////////////////////////////////////////////////////////////////

#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

//////////////////////////////////////////////////////////////////////
// Necessary Definitions for Non-MFC applications
//////////////////////////////////////////////////////////////////////
#ifndef __AFXWIN_H__ // If non-MFC ...
#include <windows.h>
#include <assert.h>
#ifndef ASSERT
#define ASSERT(x) assert(x)
#endif
typedef LPCVOID POSITION; // The generic iterator
#endif // __AFXWIN_H__

// Position moving type after node deletion.
enum { LL_MOVE_NULL = 0, LL_MOVE_PREV, LL_MOVE_NEXT }; 

class CLLMutex
{
public:

	// construct the mutex with an unique string
	CLLMutex(LPCVOID lpObject = NULL)
	{
		m_bLocked = FALSE;
		if (lpObject == NULL)
			strcpy_s(m_szUniqueStr, "{3FBB26D9-D352-409A-9352-7BDD78A1EBA1}");
		else 
			sprintf_s(m_szUniqueStr, "%s%08X", "{3FBB26D9-D352-409A-9352-7BDD78A1EBA1}", (DWORD)lpObject);
		m_hMutex = ::CreateMutexA(NULL, FALSE, m_szUniqueStr);
	}

	~CLLMutex()
	{
		Unlock();
		::CloseHandle(m_hMutex);
	}

	BOOL Lock(DWORD dwTimeout = 500)
	{
		if (m_bLocked)
			return TRUE; // Prevents multiple locking

		m_bLocked = ::WaitForSingleObject(m_hMutex, dwTimeout) == WAIT_OBJECT_0;
		return m_bLocked;
	}

	void Unlock()
	{
		::ReleaseMutex(m_hMutex);
		m_bLocked = FALSE;
	}

private:
	HANDLE m_hMutex; // The mutex handle
	BOOL m_bLocked; // Wether the mutex is currently locked
	char m_szUniqueStr[65];
};


//////////////////////////////////////////////////////////////////////
// The CLinkedList Template Class Definition
//
// Template Parameters:
//
// TYPE: Stored object type.
// ARG_TYPE: Argument type.
// FREE_TYPE: called before destroying items, useful if TYPE is heap pointer.
//////////////////////////////////////////////////////////////////////
template <class TYPE, class ARG_TYPE, void (CALLBACK *FREE_TYPE)(TYPE&)>
class CLinkedList 
{
public:

	//////////////////////////////////////////////////////////////////
	// Constructors & Destructor
	//////////////////////////////////////////////////////////////////
	CLinkedList();
	virtual ~CLinkedList();

	//////////////////////////////////////////////////////////////////
	// Item Insertion
	//////////////////////////////////////////////////////////////////
	POSITION InsertFront(ARG_TYPE data, int nCount = 1);
	POSITION InsertRear(ARG_TYPE data, int nCount = 1);
	POSITION InsertBefore(ARG_TYPE data, POSITION pos, int nCount = 1);
	POSITION InsertAfter(ARG_TYPE data, POSITION pos, int nCount = 1);

	//////////////////////////////////////////////////////////////////
	// Item Removal
	//////////////////////////////////////////////////////////////////	
	int RemoveFront(int nCount = 1);
	int RemoveRear(int nCount = 1);
	int RemoveBefore(POSITION pos, int nCount = 1);
	int RemoveAfter(POSITION pos, int nCount = 1);
	BOOL RemoveAt(POSITION& pos, int nMoveToAfterDelete = LL_MOVE_NULL);
	int RemoveMatchedItems(ARG_TYPE data, POSITION startFrom = NULL);
	void RemoveAll();

	//////////////////////////////////////////////////////////////////
	// CLinkedList Status
	//////////////////////////////////////////////////////////////////
	int GetItemCount() const { return m_nItemCount; }
	BOOL IsEmpty() const { return m_nItemCount == 0; }

	//////////////////////////////////////////////////////////////////
	// Item Iteration
	//////////////////////////////////////////////////////////////////
	POSITION GetHeadPosition() const;
	POSITION GetTailPosition() const;
	const TYPE& GetNext(POSITION& rPosition) const;
	TYPE& GetNext(POSITION& rPosition);
	const TYPE& GetPrev(POSITION& rPosition) const;
	TYPE& GetPrev(POSITION& rPosition);
	BOOL IsValidPosition(POSITION pos) const;

	//////////////////////////////////////////////////////////////////
	// Item Data Access
	//////////////////////////////////////////////////////////////////
	const TYPE& GetAt(POSITION pos) const;
	TYPE& GetAt(POSITION pos);
	BOOL SetAt(ARG_TYPE data, POSITION pos);

	//////////////////////////////////////////////////////////////////
	// Item Searching
	//////////////////////////////////////////////////////////////////
	POSITION Find(ARG_TYPE data) const;
	POSITION ReverseFind(ARG_TYPE data) const;

protected:	

	//////////////////////////////////////////////////////////////////
	// NODE Structure Definition
	//
	// The basic elements in a CLinkedList data structure. Nodes are
	// chained up each other in both directions via "pPrev" and "pNext"
	// pointers. 
	//////////////////////////////////////////////////////////////////
	typedef struct tagNODE
	{
		TYPE data; // Stored data
		tagNODE* pPrev;
		tagNODE* pNext;	
		const CLinkedList* pParent; // The CLinkedList object which created this node
	} NODE, *LPNODE;

	//////////////////////////////////////////////////////////////////
	// Protected Member Methods
	//////////////////////////////////////////////////////////////////
	BOOL __IsValidDataNode(const LPNODE pTarget) const; // Node validation.
	BOOL __Unlink(LPNODE pTarget); // Unlink and delete a node.
	void __Link(LPNODE pPrev, LPNODE pTarget, LPNODE pNext); // Link a new node in.
	POSITION __CreateDataNode(ARG_TYPE data); // Create a node that stores data
	POSITION __CreateBasicNode(); // Creates a generic node

	// Compares two item data, used for "Find", "ReversedFind" and
	// "RemoveMatchedItems". This method is decalared as static so
	// that operator "==" is required for "TYPE" only if the user
	// actually calls "Find", "ReversedFind", or "RemoveMatchedItems".
	static BOOL __ItemsEqual(const TYPE& item1, ARG_TYPE item2);

	//////////////////////////////////////////////////////////////////
	// Protected Data Members
	//////////////////////////////////////////////////////////////////
	LPNODE m_pHead; // The head node, invisible to the user
	LPNODE m_pTail; // The tail node, invisible to the user
	int m_nItemCount; // Number of total nodes, excluding head and tail.
};
//////////////////////////////////////////////////////////////////////
// End of CLinkedList Template Class Definition
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// CLinkedList Template Class Implementation
//////////////////////////////////////////////////////////////////////
template <class TYPE, class ARG_TYPE, void (CALLBACK *FREE_TYPE)(TYPE&)>
CLinkedList<TYPE, ARG_TYPE, FREE_TYPE>::CLinkedList()
{
	m_pHead = NULL;
	m_pTail = NULL;
	m_nItemCount = 0;

	// Create the head node and tail node, they are the only "life-long"
	// nodes of a CLinkedList object, and are only utilized internally.
	m_pHead = (LPNODE)__CreateBasicNode();
	m_pTail = (LPNODE)__CreateBasicNode();	
	m_pHead->pNext = m_pTail;
	m_pTail->pPrev = m_pHead;	
}

template <class TYPE, class ARG_TYPE, void (CALLBACK *FREE_TYPE)(TYPE&)>
CLinkedList<TYPE, ARG_TYPE, FREE_TYPE>::~CLinkedList()
{
	if (m_pHead != NULL && m_pTail != NULL)
	{
		RemoveAll();
		
		// Finally delete the head and tail nodes.
		delete m_pHead;
		delete m_pTail;
	}
}

template <class TYPE, class ARG_TYPE, void (CALLBACK *FREE_TYPE)(TYPE&)>
POSITION CLinkedList<TYPE, ARG_TYPE, FREE_TYPE>::__CreateDataNode(ARG_TYPE data)
{
	LPNODE p = (LPNODE)__CreateBasicNode();	
	p->data = data;
	return (POSITION)p;
}

template <class TYPE, class ARG_TYPE, void (CALLBACK *FREE_TYPE)(TYPE&)>
POSITION CLinkedList<TYPE, ARG_TYPE, FREE_TYPE>::__CreateBasicNode()
{
	LPNODE p = new NODE;
	p->pPrev = NULL;
	p->pNext = NULL;
	p->pParent = this;
	return (POSITION)p;
}

template <class TYPE, class ARG_TYPE, void (CALLBACK *FREE_TYPE)(TYPE&)>
BOOL CLinkedList<TYPE, ARG_TYPE, FREE_TYPE>::__Unlink(LPNODE pTarget)
{
	ASSERT(pTarget != NULL && m_nItemCount > 0);

	// Invoke the callback function
	if (FREE_TYPE != NULL)
		FREE_TYPE(pTarget->data);

	// Break up the chains and re-chain "prev" and "next".
	LPNODE pPrev = pTarget->pPrev;
	LPNODE pNext = pTarget->pNext;	
	pPrev->pNext = pNext;
	pNext->pPrev = pPrev;

	// Destroy the isolated node.
	pTarget->pPrev = NULL;
	pTarget->pNext = NULL;
	pTarget->pParent = NULL;
	delete pTarget;
	return TRUE;
}

template <class TYPE, class ARG_TYPE, void (CALLBACK *FREE_TYPE)(TYPE&)>
void CLinkedList<TYPE, ARG_TYPE, FREE_TYPE>::__Link(LPNODE pPrev, LPNODE pTarget, LPNODE pNext)
{
	ASSERT(pPrev != NULL && pTarget != NULL && pNext != NULL);

	// Chain the node up with its "prev" and "next".
	pPrev->pNext = pTarget;
	pTarget->pNext = pNext;
	pNext->pPrev = pTarget;
	pTarget->pPrev = pPrev;
}

template <class TYPE, class ARG_TYPE, void (CALLBACK *FREE_TYPE)(TYPE&)>
BOOL CLinkedList<TYPE, ARG_TYPE, FREE_TYPE>::__IsValidDataNode(const LPNODE pTarget) const
{
	// Node validation
	return (pTarget != NULL
		&& m_nItemCount > 0
		&& !IsBadReadPtr((LPVOID)pTarget, sizeof(NODE))
		&& pTarget->pParent == this);
}

template <class TYPE, class ARG_TYPE, void (CALLBACK *FREE_TYPE)(TYPE&)>
POSITION CLinkedList<TYPE, ARG_TYPE, FREE_TYPE>::Find(ARG_TYPE data) const
{
	CLLMutex mutex(this);
	if (!mutex.Lock())
		return NULL;

	// Search for a matched item, start from the front
	for (LPNODE p = m_pHead->pNext; p != m_pTail; p = p->pNext)
	{
		if (__ItemsEqual(p->data, data))
			return (POSITION)p;
	}

	return NULL;
}

template <class TYPE, class ARG_TYPE, void (CALLBACK *FREE_TYPE)(TYPE&)>
POSITION CLinkedList<TYPE, ARG_TYPE, FREE_TYPE>::ReverseFind(ARG_TYPE data) const
{
	CLLMutex mutex(this);
	if (!mutex.Lock())
		return NULL;
	
	// Search for a matched item, start from the rear
	for (LPNODE p = m_pTail->pPrev; p != m_pHead; p = p->pPrev)
	{
		if (__ItemsEqual(p->data, data))
			return (POSITION)p;
	}

	return NULL;
}

template <class TYPE, class ARG_TYPE, void (CALLBACK *FREE_TYPE)(TYPE&)>
POSITION CLinkedList<TYPE, ARG_TYPE, FREE_TYPE>::GetHeadPosition() const
{
	CLLMutex mutex(this);
	if (!mutex.Lock())
		return NULL;

	return (m_nItemCount > 0) ? POSITION(m_pHead->pNext) : NULL;
}

template <class TYPE, class ARG_TYPE, void (CALLBACK *FREE_TYPE)(TYPE&)>
POSITION CLinkedList<TYPE, ARG_TYPE, FREE_TYPE>::GetTailPosition() const
{
	CLLMutex mutex(this);
	if (!mutex.Lock())
		return NULL;

	return (m_nItemCount > 0) ? POSITION(m_pTail->pPrev) : NULL;
}

template <class TYPE, class ARG_TYPE, void (CALLBACK *FREE_TYPE)(TYPE&)>
const TYPE& CLinkedList<TYPE, ARG_TYPE, FREE_TYPE>::GetNext(POSITION& rPosition) const
{
	CLLMutex mutex(this);
	mutex.Lock();
	
	LPNODE p = (LPNODE)(rPosition);
	ASSERT(__IsValidDataNode(p));
	rPosition = p->pNext == m_pTail ? NULL : (POSITION)p->pNext;
	return p->data;
}

template <class TYPE, class ARG_TYPE, void (CALLBACK *FREE_TYPE)(TYPE&)>
TYPE& CLinkedList<TYPE, ARG_TYPE, FREE_TYPE>::GetNext(POSITION& rPosition)
{
	CLLMutex mutex(this);
	mutex.Lock();

	LPNODE p = (LPNODE)(rPosition);
	ASSERT(__IsValidDataNode(p));
	rPosition = p->pNext == m_pTail ? NULL : (POSITION)p->pNext;
	return p->data;
}

template <class TYPE, class ARG_TYPE, void (CALLBACK *FREE_TYPE)(TYPE&)>
const TYPE& CLinkedList<TYPE, ARG_TYPE, FREE_TYPE>::GetPrev(POSITION& rPosition) const
{
	CLLMutex mutex(this);
	mutex.Lock();

	LPNODE p = (LPNODE)(rPosition);
	ASSERT(__IsValidDataNode(p));
	rPosition = p->pPrev == m_pHead ? NULL : (POSITION)p->pPrev;
	return p->data;
}

template <class TYPE, class ARG_TYPE, void (CALLBACK *FREE_TYPE)(TYPE&)>
TYPE& CLinkedList<TYPE, ARG_TYPE, FREE_TYPE>::GetPrev(POSITION& rPosition)
{
	CLLMutex mutex(this);
	mutex.Lock();

	LPNODE p = (LPNODE)(rPosition);
	ASSERT(__IsValidDataNode(p));
	rPosition = p->pPrev == m_pHead ? NULL : (POSITION)p->pPrev;
	return p->data;
}

template <class TYPE, class ARG_TYPE, void (CALLBACK *FREE_TYPE)(TYPE&)>
void CLinkedList<TYPE, ARG_TYPE, FREE_TYPE>::RemoveAll()
{	
	RemoveFront(m_nItemCount);
}

template <class TYPE, class ARG_TYPE, void (CALLBACK *FREE_TYPE)(TYPE&)>
int CLinkedList<TYPE, ARG_TYPE, FREE_TYPE>::RemoveBefore(POSITION pos, int nCount)
{
	CLLMutex mutex(this);
	if (!mutex.Lock())
		return 0;

	// Remove the last nCount nodes before "pos".
	LPNODE pBefore = (LPNODE)(pos);
	if (!__IsValidDataNode(pBefore))
	{
		ASSERT(FALSE);
		return 0;
	}

	nCount = min(nCount, m_nItemCount);
	for (int i = 0; i < nCount; i++)
	{
		if (!__Unlink(pBefore->pPrev))
			break;
		
		m_nItemCount--;	
	}

	return i;
}

template <class TYPE, class ARG_TYPE, void (CALLBACK *FREE_TYPE)(TYPE&)>
int CLinkedList<TYPE, ARG_TYPE, FREE_TYPE>::RemoveAfter(POSITION pos, int nCount)
{
	CLLMutex mutex(this);
	if (!mutex.Lock())
		return 0;

	// Remove the first nCount nodes after "pos".
	LPNODE pAfter = (LPNODE)(pos);
	if (!__IsValidDataNode(pAfter))
	{
		ASSERT(FALSE);
		return 0;
	}

	nCount = min(nCount, m_nItemCount);
	for (int i = 0; i < nCount; i++)
	{
		if (!__Unlink(pAfter->pNext))
			break;

		m_nItemCount--;	
	}

	return i;
}

template <class TYPE, class ARG_TYPE, void (CALLBACK *FREE_TYPE)(TYPE&)>
int CLinkedList<TYPE, ARG_TYPE, FREE_TYPE>::RemoveFront(int nCount)
{
	CLLMutex mutex(this);
	if (!mutex.Lock())
		return 0;

	int i = 0;

	// Remove the first nCount nodes after m_pHead.
	nCount = min(nCount, m_nItemCount);
	for (i = 0; i < nCount; i++)
	{
		if (!__Unlink(m_pHead->pNext))
			break;

		m_nItemCount--;	
	}

	return i;
}

template <class TYPE, class ARG_TYPE, void (CALLBACK *FREE_TYPE)(TYPE&)>
int CLinkedList<TYPE, ARG_TYPE, FREE_TYPE>::RemoveRear(int nCount)
{
	CLLMutex mutex(this);
	if (!mutex.Lock())
		return 0;

	// Remove the last nCount nodes before m_pTail.
	nCount = min(nCount, m_nItemCount);
	for (int i = 0; i < nCount; i++)
	{
		if (!__Unlink(m_pTail->pPrev))
			break;

		m_nItemCount--;
	}

	return i;
}

template <class TYPE, class ARG_TYPE, void (CALLBACK *FREE_TYPE)(TYPE&)>
BOOL CLinkedList<TYPE, ARG_TYPE, FREE_TYPE>::RemoveAt(POSITION& pos, int nMoveToAfterDelete)
{
	CLLMutex mutex(this);
	if (!mutex.Lock())
		return FALSE;

	// Remove one particular node.
	LPNODE p = (LPNODE)(pos);
	if (!__IsValidDataNode(p))
	{
		ASSERT(FALSE);
		return FALSE;
	}

	LPNODE pMove = NULL;
	switch (nMoveToAfterDelete)
	{
	case LL_MOVE_PREV:
		pMove = p->pPrev;
		break;

	case LL_MOVE_NEXT:
		pMove = p->pNext;
		break;

	default:
		pMove = NULL;
		break;
	}
	
	if (!__Unlink(p))
	{
		pos = NULL;
		return FALSE;
	}

	m_nItemCount--;
	pos = (pMove == m_pHead || pMove == m_pTail) ? NULL : (POSITION)pMove;
	return TRUE;
}

template <class TYPE, class ARG_TYPE, void (CALLBACK *FREE_TYPE)(TYPE&)>
int CLinkedList<TYPE, ARG_TYPE, FREE_TYPE>::RemoveMatchedItems(ARG_TYPE data, POSITION startFrom)
{
	CLLMutex mutex(this);
	if (!mutex.Lock())
		return 0;

	// Remove all nodes whose stored data match to the argument.
	if (m_nItemCount == 0)
		return 0;

	int nCount = 0;
	LPNODE p = (LPNODE)(startFrom);
	if (!__IsValidDataNode(p))
		p = m_pHead->pNext; // Start from the head

	while (p != m_pTail)
	{
		LPNODE pPrev = p->pPrev;
		if (__ItemsEqual(p->data, data))
		{
			if (!__Unlink(p))
				return nCount;

			m_nItemCount--;
			nCount++;
			p = pPrev;
		}

		p = p->pNext;
	}
	return nCount;
}

template <class TYPE, class ARG_TYPE, void (CALLBACK *FREE_TYPE)(TYPE&)>
POSITION CLinkedList<TYPE, ARG_TYPE, FREE_TYPE>::InsertFront(ARG_TYPE data, int nCount)
{
	CLLMutex mutex(this);
	if (!mutex.Lock())
		return NULL;

	// Insert nCount new data nodes after m_pHead
	LPNODE pNewNode = NULL;
	LPNODE pPrev = m_pHead;

	for (int i = 0; i < nCount; i++)
	{	
		pNewNode = (LPNODE)__CreateDataNode(data);
		__Link(pPrev, pNewNode, pPrev->pNext);
		pPrev = pNewNode;
		m_nItemCount++;
	}
	
	return (POSITION)m_pHead->pNext;
}

template <class TYPE, class ARG_TYPE, void (CALLBACK *FREE_TYPE)(TYPE&)>
POSITION CLinkedList<TYPE, ARG_TYPE, FREE_TYPE>::InsertRear(ARG_TYPE data, int nCount)
{
	CLLMutex mutex(this);
	if (!mutex.Lock())
		return NULL;

	// Insert nCount new data nodes before m_ptail
	LPNODE pNewNode = NULL;
	LPNODE pFirstNew = NULL;

	for (int i = 0; i < nCount; i++)
	{	
		pNewNode = (LPNODE)__CreateDataNode(data);
		__Link(m_pTail->pPrev, pNewNode, m_pTail);
		m_nItemCount++;

		if (pFirstNew == NULL)
			pFirstNew = pNewNode;
	}
	
	return (POSITION)pFirstNew;
}

template <class TYPE, class ARG_TYPE, void (CALLBACK *FREE_TYPE)(TYPE&)>
POSITION CLinkedList<TYPE, ARG_TYPE, FREE_TYPE>::InsertBefore(ARG_TYPE data, POSITION pos, int nCount)
{
	CLLMutex mutex(this);
	if (!mutex.Lock())
		return NULL;

	// Insert nCount new data nodes before "pos"
	LPNODE pBefore = (LPNODE)(pos);
	if (!__IsValidDataNode(pBefore))
	{
		ASSERT(FALSE);
		return NULL;
	}

	LPNODE pNewNode = NULL;
	LPNODE pPrev = pBefore->pPrev;
	LPNODE pFirstNew = NULL;

	for (int i = 0; i < nCount; i++)
	{	
		pNewNode = (LPNODE)__CreateDataNode(data);
		__Link(pPrev, pNewNode, pPrev->pNext);
		pPrev = pNewNode;
		m_nItemCount++;

		if (pFirstNew == NULL)
			pFirstNew = pNewNode;
	}	
	
	return (POSITION)pFirstNew;
}

template <class TYPE, class ARG_TYPE, void (CALLBACK *FREE_TYPE)(TYPE&)>
POSITION CLinkedList<TYPE, ARG_TYPE, FREE_TYPE>::InsertAfter(ARG_TYPE data, POSITION pos, int nCount)
{
	CLLMutex mutex(this);
	if (!mutex.Lock())
		return NULL;

	// Insert a new data node after "pos"
	LPNODE pAfter = (LPNODE)(pos);
	if (!__IsValidDataNode(pAfter))
	{
		ASSERT(FALSE);
		return NULL;
	}

	LPNODE pNewNode = NULL;
	LPNODE pPrev = pAfter;
	LPNODE pFirstNew = NULL;

	for (int i = 0; i < nCount; i++)
	{	
		pNewNode = (LPNODE)__CreateDataNode(data);
		__Link(pPrev, pNewNode, pPrev->pNext);
		pPrev = pNewNode;
		m_nItemCount++;

		if (pFirstNew == NULL)
			pFirstNew = pNewNode;
	}	
	
	return (POSITION)pFirstNew;
}

template <class TYPE, class ARG_TYPE, void (CALLBACK *FREE_TYPE)(TYPE&)>
const TYPE& CLinkedList<TYPE, ARG_TYPE, FREE_TYPE>::GetAt(POSITION pos) const
{
	CLLMutex mutex(this);
	mutex.Lock();
	
	// Retrive the data stored at a node.
	LPNODE p = (LPNODE)(pos);
	ASSERT(__IsValidDataNode(p));
	return p->data;
}

template <class TYPE, class ARG_TYPE, void (CALLBACK *FREE_TYPE)(TYPE&)>
TYPE& CLinkedList<TYPE, ARG_TYPE, FREE_TYPE>::GetAt(POSITION pos)
{
	CLLMutex mutex(this);
	mutex.Lock();

	// Retrive the data stored at a node.
	LPNODE p = (LPNODE)(pos);
	ASSERT(__IsValidDataNode(p));
	return p->data;
}

template <class TYPE, class ARG_TYPE, void (CALLBACK *FREE_TYPE)(TYPE&)>
BOOL CLinkedList<TYPE, ARG_TYPE, FREE_TYPE>::SetAt(ARG_TYPE data, POSITION pos)
{
	CLLMutex mutex(this);
	if (!mutex.Lock())
		return FALSE;
	
	// Set new value to the data stored at a node.
	LPNODE p = (LPNODE)(pos);
	if (!__IsValidDataNode(p))
	{
		ASSERT(FALSE);
		return FALSE;
	}

	p->data = data;
	return TRUE;
}

template <class TYPE, class ARG_TYPE, void (CALLBACK *FREE_TYPE)(TYPE&)>
BOOL CLinkedList<TYPE, ARG_TYPE, FREE_TYPE>::__ItemsEqual(const TYPE& item1, ARG_TYPE item2)
{
	return item1 == item2;
}

template <class TYPE, class ARG_TYPE, void (CALLBACK *FREE_TYPE)(TYPE&)>
BOOL CLinkedList<TYPE, ARG_TYPE, FREE_TYPE>::IsValidPosition(POSITION pos) const
{
	CLLMutex mutex(this);
	if (!mutex.Lock())
		return FALSE;

	return __IsValidDataNode((LPNODE)pos);
}
//////////////////////////////////////////////////////////////////////
// End of CLinkedList Template Class Implementation
//////////////////////////////////////////////////////////////////////

#endif // __LINKEDLIST_H__