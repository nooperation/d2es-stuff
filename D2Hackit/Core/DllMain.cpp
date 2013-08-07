//////////////////////////////////////////////////////////////////////
// DllMain.cpp
// -------------------------------------------------------------------
// Default Dll entrypoint.
//
// <thohell@home.se>
//////////////////////////////////////////////////////////////////////
#include "D2HackItServer.h"

// Global structures
SERVERINFO					*si = NULL;		
PRIVATESERVERINFO			*psi = NULL;
PRIVATEFUNCTIONENTRYPOINTS	*pfep = NULL;
THISGAMESTRUCT				*thisgame = NULL;

HWND						g_hD2 = NULL;		// 0.80
HANDLE						g_hD2HackIt = NULL;	// 0.80

LinkedList					szBufferList;	// List of GetHackProfile created strings
CClients					ClientModules;	// List of Loaded Modules

#ifdef D2HACKIT_DEBUG
char g_szDebug[512];
#endif

char g_szModuleDir[MAX_PATH + 1] = "";

BOOL FindD2Window();
void LaterGetD2Window();

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	BOOL hResult = TRUE;
	int nLen = 0;

    switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		
		::GetModuleFileName((HMODULE)hModule, g_szModuleDir, MAX_PATH);		
		nLen = ::strlen(g_szModuleDir);
		while (nLen)
		{
			if (g_szModuleDir[nLen] == '\\')
			{
				g_szModuleDir[nLen] = '\0';
				nLen = 0;
			}
			else
			{
				nLen--;
			}
		}

		// initiate server
		DGTRACE((DBGHDR "DLL_PROCESS_ATTACH\n"));

		g_hD2HackIt = hModule;
		if(FindD2Window())
		{			
			// Find Diablo II window already loaded.
			// Keep on process as normal

			hResult = ServerStart(hModule);			
		}
		else
		{
			// Failed to find Diablo II Window Handle
			// This is casued by loaded as a D2Loader's plugin module
			DWORD base = (DWORD)LoadLibrary("d2gfx.dll");
			if (base)
			{
				Intercept(INST_CALL, base+0x46C7, (DWORD)GetFuncAddress(&LaterGetD2Window), 5);
			}
			else
			{
				DGTRACE((DBGHDR "Failed to load d2gfx.dll\n"));
			}
		}

		break;

	case DLL_THREAD_ATTACH:
		DGTRACE((DBGHDR "DLL_THREAD_ATTACH\n"));
		break;
	case DLL_THREAD_DETACH:
		DGTRACE((DBGHDR "DLL_THREAD_DETACH\n"));
		break;

	case DLL_PROCESS_DETACH:

		DGTRACE((DBGHDR "DLL_PROCESS_DETACH\n"));

		hResult = ServerStop();
		break;
    } 

    return hResult;
}

BOOL LaterGetD2WindowPart2()
{
	HMODULE h = GetModuleHandle("d2gfx.dll");
	Intercept(INST_CALL, (DWORD)GetFuncAddress(&LaterGetD2Window), ((DWORD)h)+0x46C7, 5);
	FreeLibrary(h);
	return g_hD2 ? ServerStart(g_hD2HackIt) : FALSE;
}

void __declspec(naked)  LaterGetD2Window()
{
	__asm {
		nop;
		nop;
		nop;
		nop;
		nop;
		add dword ptr[esp], 2;
		jz unloadmyself;
		add dword ptr[esp], 0x37;
		mov g_hD2, eax;
		call LaterGetD2WindowPart2;
		test eax, eax;
		jz unloadmyself;
		mov eax, g_hD2;
		ret;
unloadmyself:
		pop eax;
		push eax;
		push g_hD2;
		push 0xccc35858; // pop eax; pop eax; ret; int 3;
		push g_hD2HackIt;
		lea eax, [esp+4];
		push eax;
		push FreeLibrary;
		ret;
	}
}

LPCSTR EXPORT GetModuleDirectory()
{
	return g_szModuleDir;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lparam)
{
	char classname[20];

	GetClassName(hwnd, classname, sizeof(classname));

	if(!strcmp(classname, "Diablo II"))
	{
		DWORD pid, tid;
		tid = GetWindowThreadProcessId( hwnd, &pid );
		if( pid == GetCurrentProcessId() )
		{
			//		*(HWND*)lparam = hwnd;
			g_hD2 = hwnd;
			return FALSE;	// Stop enum
		}
	}

	return TRUE;	// Continue enum
}


BOOL FindD2Window()
{
	g_hD2 = NULL;
	EnumWindows(EnumWindowsProc, (LPARAM)&g_hD2);
	//	EnumThreadWindows(GetCurrentThreadId(), EnumWindowsProc, (LPARAM)&g_hD2);
	return g_hD2 != NULL;
}

//##################################################################################################
//
//	The codes below are moved from d2loader\d2hackit.cpp (by Dan_Goon)
//
//##################################################################################################

//#include "Plugin.h"

//PLUGIN_INTERFACE 	Interface;
//LPD2PARAM			g_pD2Param;

//extern "C" DWORD __stdcall PluginEntry(DWORD dwReason, LPVOID lpData)
//{
//	LPPLUGIN_DATA_INIT	lpDataInit;

//	DGTRACE((DBGHDR "PluginEntry (%X, %p)!\n", dwReason, lpData));

//	switch (dwReason)
//	{
//		case REASON_INIT:
//			lpDataInit = (LPPLUGIN_DATA_INIT)lpData;
//			break;

//		case REASON_CLEANUP:
//			g_pD2Param = (LPD2PARAM)lpData;
//			break;
			
//		default:
		/* for other reasons, you should use & to test if 
		   certain flag is set.
		   e.g: if (dwReason & REASON_ENTER_GAME) {} */

//		case REASON_ENTER_MAIN_MENU:
//		case REASON_ENTER_GAME:
//		case REASON_ENTER_CHANNEL:
//		case REASON_LEAVE_GAME:
//		case REASON_LEAVE_CHANNEL:
//		case REASON_LEAVE_MAIN_MENU:
//			g_pD2Param = (LPD2PARAM)lpData;
//			break;

//			break;
//	}

	// We have to call ServerStart here if
	// D2HackIt had been loaded as a plugin module of D2Loader,
//	if((dwReason & REASON_ENTER_MAIN_MENU) && !g_hD2)
//	{
//		DGTRACE((DBGHDR "REASON_ENTER_MAIN_MENU...\n"));

//		FindD2Window();
		
//		if(g_hD2)
//		{
//			DGTRACE((DBGHDR "D2 Window Found\n"));
//			if(!ServerStart(g_hD2HackIt))
//			{
//				DGTRACE((DBGHDR "Failed to ServerStart!\n"));
//				return FALSE;
//			}
//		}
//	}

//	return TRUE;

//}

//extern "C" LPPLUGIN_INTERFACE __declspec( dllexport ) QueryInterface()
//{
//	static char szDesc[64];
//	sprintf(szDesc, "thohell's D2HackIt!, DG Version %d.%.2d", LOWORD(__SERVERVERSION__), HIWORD(__SERVERVERSION__));
//	Interface.dwMagicword = PLUGIN_MAGICWORD;
//	Interface.dwVersion = PLUGIN_VERSION;
//	Interface.szDescription = szDesc;
//	Interface.fpEntry = PluginEntry;
//
//	return &Interface;
//}


extern "C" PVOID __declspec( dllexport ) QueryInterface() //NOTE :- needs to be exported
{
	char filename[MAX_PATH];
	GetModuleFileName((HMODULE)g_hD2HackIt, filename, MAX_PATH);
	LoadLibrary(filename);
	return NULL;
}
