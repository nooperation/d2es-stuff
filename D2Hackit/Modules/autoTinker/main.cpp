#include "../../Includes/ClientCore.cpp"
#include "AutoTinker.h"

AutoTinker autoTinker;

bool settingHotkey = false;
int hotkey = 0;

CLIENTINFO
(
	1,0,
	"Auto Tinker v1.0",
	"",
	"Auto Tinker v1.0",
	""
)

BOOL PRIVATE Start(char** argv, int argc)
{
	autoTinker.Start();

	return TRUE;
}

VOID EXPORT OnThisPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	if(nMessage == PM_UICLOSED)
	{
		autoTinker.Abort();
	}
}

VOID EXPORT OnUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam, LPARAM lParam)
{
	if(nMessage == UM_ITEMEVENT)
	{
		ITEM item = *(ITEM *)lParam;

		if(wParam == 0x04 && item.iStorageID == 0x04)
		{
			autoTinker.OnItemToCube(item);
		}
		else if(wParam == 0x05 && item.iStorageID == 0x01)
		{
			autoTinker.OnItemFromInventory(item);
		}
	}
}

BYTE EXPORT OnGameKeyDown(BYTE iKeyCode)
{
	switch(iKeyCode)
	{
		case VK_SPACE:
			autoTinker.Abort();
			break;
	}

	return iKeyCode;
}

MODULECOMMANDSTRUCT ModuleCommands[]=
{
	{
		"help",
		OnGameCommandHelp,
		"helpÿc0 List commands available in this module.\n"
		"<command> helpÿc0 Shows detailed help for <command> in this module."
	},
	{
		"Start",
		Start,
		"Usage: Start [chat]",
	},
	{NULL}
};


