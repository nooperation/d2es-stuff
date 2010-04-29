// This isn't the 1.10 d2loader plugin header, I just changed the PLUGIN_VERSION
#ifndef INCLUDED_PLUGIN_H
#define INCLUDED_PLUGIN_H

#include <windows.h>

typedef DWORD ( __stdcall * PluginEntryFunc)(DWORD dwReason, LPVOID lpData);

typedef struct
{
	DWORD			dwMagicword;	/* should be "x0\x02D" */
	DWORD			dwVersion;	/* plugin version */
	LPCSTR			szDescription;
	PluginEntryFunc		fpEntry;
} PLUGIN_INTERFACE, * LPPLUGIN_INTERFACE;
#define PLUGIN_MAGICWORD		0x44320000
#define PLUGIN_VERSION			0x01000911

#define REASON_NONE			0x00
#define REASON_INIT			0x01
typedef struct
{
	DWORD			dwVersion;
	HANDLE			hEventUnload;
} PLUGIN_DATA_INIT, * LPPLUGIN_DATA_INIT;

#define REASON_CLEANUP			0x02
#define REASON_ENTER_GAME		0x04
#define REASON_LEAVE_GAME		0x08
#define REASON_ENTER_CHANNEL		0x10
#define REASON_LEAVE_CHANNEL		0x20
#define REASON_ENTER_MAIN_MENU		0x40
#define REASON_LEAVE_MAIN_MENU		0x80

typedef LPPLUGIN_INTERFACE ( __stdcall * QueryInterfaceFunc) (VOID);

#endif
