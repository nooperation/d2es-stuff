#include <windows.h>
#include "plugin.h"

DWORD __stdcall PluginEntry(DWORD dwReason, LPVOID lpData);

PLUGIN_INTERFACE Interface;

	HMODULE D2HirelingModule;
	FARPROC D2HirelingInitProc;
	FARPROC D2HirelingReleaseProc;

LPPLUGIN_INTERFACE __stdcall QueryInterface(VOID) 
{
	Interface.dwMagicword = PLUGIN_MAGICWORD;
	Interface.dwVersion = PLUGIN_VERSION;
	Interface.szDescription = "Load hireling.dll";
	Interface.fpEntry = PluginEntry;
	return &Interface;
}

DWORD __stdcall PluginEntry(DWORD dwReason, LPVOID lpData) 
{
	if(dwReason == REASON_ENTER_MAIN_MENU)
	{
		D2HirelingModule = LoadLibrary("D2Hireling.dll");
		if(D2HirelingModule == NULL)
		{
			MessageBox(0, "Unable to load D2Hireling.dll", "loadHireling Error", 0);
			return TRUE;
		}

		D2HirelingInitProc = GetProcAddress(D2HirelingModule, "_Init@4");
		if(D2HirelingInitProc == NULL)
		{
			MessageBox(0, "Unable to find _Init@4 in D2Hireling.dll", "loadHireling Error", 0);
			return TRUE;
		}

		D2HirelingInitProc(0);
	}
	else if(dwReason == REASON_CLEANUP)
	{
		D2HirelingReleaseProc = GetProcAddress(D2HirelingModule, "_Release@0");
		if(D2HirelingReleaseProc == NULL)
		{
			MessageBox(0, "Unable to find _Release@0 in D2Hireling.dll", "loadHireling Error", 0);
			return TRUE;
		}

		D2HirelingReleaseProc();

		FreeLibrary(D2HirelingModule);
	}

	return TRUE;
}


BOOL WINAPI DllMain(HINSTANCE hDll, DWORD dwReason, LPVOID lpReserved) 
{
	return TRUE;
}
