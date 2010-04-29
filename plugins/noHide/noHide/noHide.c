#include <windows.h>
#include "plugin.h"

BOOL CALLBACK EnumWndProc(HWND hWnd, LPARAM lParam);
LRESULT APIENTRY WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
DWORD __stdcall PluginEntry(DWORD dwReason, LPVOID lpData);
BOOL IsFullscreen(HWND hWnd);

WNDPROC originalWndProc = NULL;
HWND targetWindow = NULL;
PLUGIN_INTERFACE Interface;

LPPLUGIN_INTERFACE __stdcall QueryInterface(VOID) 
{
	Interface.dwMagicword = PLUGIN_MAGICWORD;
	Interface.dwVersion = PLUGIN_VERSION;
	Interface.szDescription = "no hide that was implemented in future versions of d2loader";
	Interface.fpEntry = PluginEntry;
	return &Interface;
}

DWORD __stdcall PluginEntry(DWORD dwReason, LPVOID lpData) 
{
	if(dwReason == REASON_ENTER_MAIN_MENU)
	{
		if(EnumWindows(&EnumWndProc, (LPARAM)&targetWindow))
		{
			MessageBox(0, "Failed to find window", 0, 0);
			return FALSE;
		}

		if(IsFullscreen(targetWindow))
		{
			return TRUE;
		}

		originalWndProc = (WNDPROC)SetWindowLong(targetWindow, GWL_WNDPROC, (LONG)WndProc);	
	}

	return TRUE;
}

BOOL IsFullscreen(HWND hWnd)
{
	WINDOWINFO windowInfo;

	if(!GetWindowInfo(hWnd, &windowInfo))
	{
		return TRUE;
	}

	return windowInfo.dwStyle & WS_POPUP;
}

BOOL WINAPI DllMain(HINSTANCE hDll, DWORD dwReason, LPVOID lpReserved) 
{
	return TRUE;
}

LRESULT APIENTRY WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(msg == WM_ACTIVATEAPP)
	{
		return 0;
	}

	return CallWindowProc(originalWndProc, hWnd, msg, wParam, lParam);
}

BOOL CALLBACK EnumWndProc(HWND hWnd, LPARAM lParam)
{
	char className[512];

	if(GetWindowThreadProcessId(hWnd, NULL) == GetCurrentThreadId())
	{
		GetClassName(hWnd, className, sizeof(className)/sizeof(className[0]));
		
		if(strcmp(className, "Diablo II") == 0)
		{
			*(HWND *)lParam = hWnd;
			return FALSE;
		}
	}

	return TRUE;
}
