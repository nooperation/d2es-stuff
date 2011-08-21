//////////////////////////////////////////////////////////////////////
// ClientHandler.cpp
// -------------------------------------------------------------------
//
// <Dan_Goon@diabloworld.com>
//////////////////////////////////////////////////////////////////////
#include "D2HackItServer.h"
#include "D2HackIt.h"

//********************************************************************************************
// LinkedList Derived Class Implementations
//

//============================================================================
//	CClients Overrides
//----------------------------------------------------------------------------
//============================================================================
CClients::~CClients()
{
	DGTRACE((DBGHDR "CClients::~CClients()\n"));

	RemoveAll();
}

void CClients::RemoveAll()
{
	LinkedItem* pItem = pFirst;
	while(pItem && IsValidListItem(pItem))
	{
		pItem = RemoveItem(pItem);
	}
}

PLinkedItem CClients::RemoveItem(PLinkedItem pItem, BOOL bUnlock)
{
	if(!pFirst || !pLast || !IsValidListItem(pItem)) return NULL;

	EnterCriticalSection(&csMutex);
	if(bUnlock) pItem->UnlockData();
	bUnlock = FALSE;

	if(!pItem->IsDataLocked())
	{
		PCIS pCIS = GetCIS(pItem, TRUE);
		if(pCIS)
		{
			bUnlock = TRUE;
			if(pCIS->Name) delete [] pCIS->Name;
			pCIS->Name = NULL;
			if(pCIS->pDir) delete [] pCIS->pDir;
			pCIS->pDir = NULL;
		}
	}
	LeaveCriticalSection(&csMutex);

	return LinkedList::RemoveItem(pItem, bUnlock);
}

PCIS CClients::GetCIS(PLinkedItem pItem, BOOL bLock)
{
	if(!IsValidListItem(pItem)) return NULL;
	return (PCIS)pItem->GetData(bLock);
}



//============================================================================
//	CLoadedGroup Implementations
//----------------------------------------------------------------------------
//============================================================================
CLoadedGroup::~CLoadedGroup()
{
	DGTRACE((DBGHDR "CLoadedGroup::~CLoadedGroup()\n"));
	RemoveAll();
}

void CLoadedGroup::RemoveAll()
{
	PLinkedItem pItem = pFirst;
	while(pItem && IsValidListItem(pItem))
	{
		// just set to NULL (Do not free-up this pointer!)
		PVOID& pData = pItem->GetData(FALSE);
		pData = NULL;
		PLinkedItem pNext = GetNextItem(pItem);
		delete pItem;
		pItem = pNext;
	}

	pFirst = pLast = NULL;
	cItemCount = 0;
}

PLinkedItem CLoadedGroup::RemoveItem(PLinkedItem pItem)
{
	if(!pFirst || !pLast || !IsValidListItem(pItem)) return NULL;

	// just set to NULL (Do not free-up this pointer!)
	PVOID& pData = pItem->GetData(FALSE);
	pData = NULL;

	return LinkedList::RemoveItem(pItem, FALSE);
}


void CLoadedGroup::UnLoadAll()
{
	PLinkedItem pItem = pFirst;
	while(pItem && IsValidListItem(pItem))
	{
		PLinkedItem pModule = (PLinkedItem)pItem->GetData();
		if(pModule)
		{
			UnloadClientModule(pModule);
		}

		pItem = GetNextItem(pItem);
	}

	RemoveAll();
}

//============================================================================
//	CClientGroup Implementations
//----------------------------------------------------------------------------
//============================================================================
CClientGroup::~CClientGroup()
{
	PLinkedItem pItem = pFirst;
	while(pItem && IsValidListItem(pItem))
	{
		PVOID& pData = pItem->GetData(FALSE);
		if(pData) delete [] pData;
		pData = NULL;

		PLinkedItem pNext = GetNextItem(pItem);
		delete pItem;
		pItem = pNext;
	}

	pFirst = pLast = NULL;
	cItemCount = 0;
}


PLinkedItem CClientGroup::AddItem(LPSTR pszName)
{
	if(!pszName || !pszName[0]) return NULL;

	DGTRACE((DBGHDR "CClientGroup::AddItem(%s)...\n", pszName));

	// Check if this name is already in the list....
	LinkedItem* pItem = pFirst;
	while(pItem)
	{
		LPSTR pName = (LPSTR)pItem->GetData();
		if(!_stricmp(pszName, pName)) return NULL;		// return NULL for duplicated name
		pItem = GetNextItem(pItem);
	}

	DGTRACE((DBGHDR "CClientGroup::DupeName Checked...\n"));

	// Check if this module is already loaded
	pItem = ClientModules.GetFirstItem();
	while(pItem)
	{
		PCIS pCIS = ClientModules.GetCIS(pItem);
		if(pCIS && !_stricmp(pszName, pCIS->Name))
		{
			DGTRACE((DBGHDR "CClientGroup::Found Already Loaded (%s)\n", pszName));
			LoadedModules.AddItem(pItem);
		}
		pItem = ClientModules.GetNextItem(pItem);
	}

	DGTRACE((DBGHDR "CClientGroup::Loaded Module Checked...\n"));

	LPSTR pNewName = new char[strlen(pszName)+1];
	strcpy(pNewName, pszName);

	return LinkedList::AddItem(pNewName);
}


void CClientGroup::LoadAll()
{
	// Unload loaded modules, first
	if(LoadedModules.GetItemCount())
	{
		LoadedModules.UnLoadAll();
	}

	LinkedItem* pItem = pFirst;
	while(pItem &&  IsValidListItem(pItem))
	{
		LPSTR pszName = (LPSTR)pItem->GetData();
		DGTRACE((DBGHDR "CClientGroup::Loading Module (%s)...\n", pszName));

		LoadClientModule(pszName);
		pItem = GetNextItem(pItem);
	}
}

//********************************************************************************************
// Client Module Load/Unload Handler and some enumeration utilities
//

// Search for loaded module by it's module handle
LinkedItem* PRIVATE FindClientStruct(HMODULE hModule)
{
	LinkedItem *li = ClientModules.GetFirstItem();
	while(li)
	{
		PCIS pCIS = ClientModules.GetCIS(li);
		if(pCIS && pCIS->hModule == hModule)
		{
			return li;
		}
		li = ClientModules.GetNextItem(li);
	}

	return NULL;
}

// Search for loaded module by it's module name
LinkedItem* PRIVATE FindClientStructByName(LPCSTR szName)
{
	LinkedItem *li = ClientModules.GetFirstItem();
	while(li)
	{	
		PCIS pCIS = ClientModules.GetCIS(li);
		if(pCIS && !_stricmp(pCIS->Name, szName))
		{
			return li;
		}
		li = ClientModules.GetNextItem(li);
	}

	return NULL;
}

DWORD g_dwClientVer = 0;

LinkedItem* PRIVATE LoadClientModule(LPCSTR szName)
{
	char t[1024];

	// Check if already loaded
	LinkedItem* li = FindClientStructByName(szName);
	if(li)
	{
		sprintf(t, "Module '%s' is already loaded!", szName);
		GamePrintError(t);
		return NULL;
	}

	// Get full path
	char szTemp[64];
	sprintf(szTemp, "%s.d2h", szName);			// try .d2h
	if(!D2FindFile(t, szTemp))
	{
		sprintf(szTemp, "%s.dll", szName);		// try .dll
		if(!D2FindFile(t, szTemp))
		{
			sprintf(t, "Unable to load module '%s' : file not found!", szName);
			GamePrintError(t);
			return NULL;
		}
	}

	// Load the Client Module
	g_dwClientVer = 0;
	HMODULE hModule = (HMODULE)LoadLibrary(t);

	if(!hModule)
	{
		// Didn't find any
		DGTRACE((DBGHDR "Unable to LoadLibrary[%s]! (Err = %d)", t, GetLastError()));
		sprintf(t, "Unable to load module '%s'!", szName);
		GamePrintError(t);
		return NULL;
	}

	PCIS pCIS = new CLIENTINFOSTRUCT;
	ZeroMemory(pCIS, sizeof(CLIENTINFOSTRUCT));
	
	DGTRACE((DBGHDR "Returned Client's Version = .%d.%.2d\n", LOWORD(g_dwClientVer), HIWORD(g_dwClientVer)));

	pCIS->hModule = hModule;
	pCIS->dwVersion = g_dwClientVer;
	pCIS->Name = new char[strlen(szName)+1];
	strcpy(pCIS->Name, szName);

	int p = strlen(t);
	while(p) 
	{
		if(t[p] == '\\') { t[p] = 0; p=0; }
		else p--;
	}

	pCIS->pDir = new char[strlen(t)+1];
	strcpy(pCIS->pDir, t);

	// Bind event handlers
	if(psi->bSupportBnet)
	{
		BIND(OnBnetPacketBeforeSent);
		BIND(OnBnetPacketBeforeReceived);
		BIND(OnRealmPacketBeforeSent);
		BIND(OnRealmPacketBeforeReceived);
	}

	BIND(OnGamePacketBeforeSent);
	BIND(OnGamePacketBeforeReceived);
	BIND(OnGamePacketAfterReceived);
	BIND(OnGameCommandLine);
	BIND(OnGameJoin);
	BIND(OnGameLeave);
	BIND(OnGameTimerTick);
	BIND(OnClientStart);
	BIND(OnClientStop);	
	BIND(OnUnitMessage); // Added v2.00 by Abin
	BIND(OnThisPlayerMessage); // Added v2.00 by Abin
	BIND(OnQueryModuleData); // Added v2.00 by Abin
	BIND(OnGameKeyDown); // Added v2.00 by Abin

	// Bind property gets
	BIND(GetModuleAuthor);
	BIND(GetModuleWebsite);
	BIND(GetModuleEmail);
	BIND(GetModuleDescription);
	BIND(GetModuleVersion);

	li = ClientModules.AddItem(pCIS);
	li->LockData();

	// Initiate client
	if(pCIS->OnClientStart && !pCIS->OnClientStart())
	{
		li->UnlockData();
		UnloadClientModule(li);
		return NULL;
	}

	if((pCIS->OnGameJoin) && (thisgame->player)) pCIS->OnGameJoin(thisgame);
	li->UnlockData();

	sprintf(t, "Module ÿc4'%s'ÿc0%s%s loaded!",
		pCIS->Name,
		(pCIS->GetModuleAuthor? " by " : ""),
		(pCIS->GetModuleAuthor? pCIS->GetModuleAuthor() : "")
		);

	GamePrintInfo(t);
	return li;
}


BOOL PRIVATE UnloadClientModule(LinkedItem* li)
{
	PCIS pCIS = ClientModules.GetCIS(li);
	if(!pCIS) return FALSE;

	HMODULE hModule = (HMODULE)pCIS->hModule;
	char t[256];
	sprintf(t, "Module ÿc4'%s'ÿc0 unloaded!", pCIS->Name);

	CleanUpClient(li);

	DGTRACE((DBGHDR "Freeing Library...\n"));
	if(FreeLibrary(hModule))
	{
		GamePrintInfo(t);
		return TRUE;
	}

	return FALSE;
}


BOOL PRIVATE UnloadClientModuleByName(LPCSTR szName)
{
	char t[256];

	LinkedItem* li = FindClientStructByName(szName);
	if(!li)
	{
		sprintf(t, "Could not unload module '%s'. It's not loaded!", szName);
		GamePrintError(t);
		return FALSE;
	}

	return UnloadClientModule(li);
}


BOOL EXPORT CleanMeUp(HMODULE hModule)
{
	if(!psi)
	{
		DGTRACE((DBGHDR "CleanMeUp() ###### Dummy Call after server destructed!\n"));
		return TRUE;
	}

	DGTRACE((DBGHDR "CleanMeUp(%X)...\n", hModule));

	LinkedItem* li = FindClientStruct(hModule);
	if(!li)
	{
		DGTRACE((DBGHDR "Already CleanUp'd!\n"));
		return TRUE;
	}

	return CleanUpClient(li);
}


BOOL PRIVATE CleanUpClient(LinkedItem *li)
{
	PCIS pCIS = ClientModules.GetCIS(li, TRUE);		// Lock pCIS !!
	if(!pCIS) return FALSE;

	// Tell module that we leave game
	if((pCIS->OnGameLeave) && (thisgame->player))
	{
		DGTRACE((DBGHDR "Calling OnGameLeave...\n"));
		pCIS->OnGameLeave(thisgame);
	}

	// Prepare module to be unloaded
	if(pCIS->OnClientStop)
	{
		// If this fails, let the user know
		DGTRACE((DBGHDR "Calling ClientStop...(%X)\n", pCIS->hModule));
		if(!pCIS->OnClientStop())
		{
			GamePrintInfo("Module not unloaded cleanly! This could be a problem!");
		}
	}

	DGTRACE((DBGHDR "Removing LinkedList...\n"));

	ClientModules.RemoveItem(li, TRUE);	// Unlock Data and Remove it!
	return TRUE;
}

//********************************************************************************************
// Some sort of the functions below are for Subclassing the Diablo II Window
//
WNDPROC	m_pOrgWndProc = NULL;

BOOL EXPORT HookD2WndProc(HMODULE hModule, PF_HookProc pHookProc)
{
	LinkedItem* li = FindClientStruct(hModule);
	if(!li)
	{
		DGTRACE((DBGHDR "HookD2WndProc : Could not find loaded module! (hModule = %X)\n", hModule));
		GamePrintError("Could not Hook D2WndProc : module not loaded!");
		return FALSE;
	}

	PCIS pCIS = ClientModules.GetCIS(li);
	if(pCIS)
	{
		pCIS->ClientWndProc = pHookProc;

		if(!pHookProc)
		{
			DGTRACE((DBGHDR "Client UnHooked successfully!\n"));
		}
	}
	return TRUE;
}

BOOL g_bInHook = FALSE;

BOOL CALLBACK ClientWndProc(LRESULT* pResult, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(!psi) return FALSE;

	PCIS pCIS = NULL;

	LinkedItem *li = ClientModules.GetFirstItem();
	while(li)
	{	
		pCIS = ClientModules.GetCIS(li);
		if(!pCIS) continue;

		PF_HookProc pHookProc = pCIS->ClientWndProc;
		if(pHookProc)
		{
			g_bInHook = TRUE;
			if(pHookProc(pResult, hWnd, uMsg, wParam, lParam))
			{
				// Return the result and stop routing if message has been processed!
				g_bInHook = FALSE;
				return TRUE;
			}
			g_bInHook = FALSE;
		}
		li = ClientModules.GetNextItem(li);
	}

	return FALSE;
}


LRESULT __declspec(naked) CALLBACK HackitWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult;

	_asm
	{
		mov		eax, m_pOrgWndProc
		or		eax, eax
		jz		quit

		push	ebp
		mov		ebp, esp
		sub		esp, 8

		mov		eax, lParam
		push	eax
		mov		eax, wParam
		push	eax
		mov		eax, uMsg
		push	eax
		mov		eax, hWnd
		push	eax
		lea		eax, lResult
		push	eax
		call	ClientWndProc

		or		eax, eax
		jz		goto_default		// not processed. go default process

		mov		eax, lResult		// processed. return the result
		mov		esp, ebp
		pop		ebp

quit:
		ret		16

goto_default:

		// JUMP (!) to original WindowProc if
		// no client hook proceeses or
		// all client process did not handle the message

		mov		esp, ebp
		pop		ebp

		jmp		DWORD PTR m_pOrgWndProc
	}
}


BOOL PRIVATE HookHackitWndProc(HWND hD2, BOOL bHook)
{
	if(!bHook)
	{
		// Unhook prev set HookProc

		if(m_pOrgWndProc)
		{
			DGTRACE((DBGHDR "D2WndProc UnHooking.... (%d)\n", g_bInHook));
			SetWindowLong(hD2, GWL_WNDPROC, (LONG)m_pOrgWndProc);
			m_pOrgWndProc = NULL;
			DGTRACE((DBGHDR "D2WndProc UnHooked successfully! (%d)\n", g_bInHook));
			return TRUE;
		}

		GamePrintError("HookProc never had been set!");
		DGTRACE((DBGHDR "HookProc never had been set!\n"));
		return FALSE;
	}

	if(m_pOrgWndProc)
	{
		// HookProc already set!!

		GamePrintError("HookProc already had been set!");
		DGTRACE((DBGHDR "HookProc already had been set!\n"));
		return FALSE;
	}

	// Subclass Hook D2 WndProc

	m_pOrgWndProc = (WNDPROC)SetWindowLong(hD2, GWL_WNDPROC, (LONG)HackitWndProc);

	if(!m_pOrgWndProc)
	{
		GamePrintError("Failed to Hook D2WndProc!");
		DGTRACE((DBGHDR "Failed to Hook D2WndProc!\n"));
		return FALSE;
	}

	DGTRACE((DBGHDR "D2WndProc Hooked successfully!\n"));
	return TRUE;
}

