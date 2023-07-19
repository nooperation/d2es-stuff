#include "../../Includes/ClientCore.cpp"
#include "AutoJewel.h"

AutoJewel autoJewel;

bool settingHotkey = false;
int hotkey = 0;

CLIENTINFO
(
	0, 1,
	"Auto Jewel v0.1",
	"",
	"Auto Jewel v0.1",
	""
)

BOOL PRIVATE Start(char **argv, int argc)
{
	if (argc < 2)
	{
		return FALSE;
	}

	autoJewel.Start();

	return TRUE;
}

VOID EXPORT OnThisPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	if (nMessage == PM_UICLOSED)
	{
		autoJewel.Abort();
	}
}

VOID EXPORT OnUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam, LPARAM lParam)
{
	if (nMessage == UM_ITEMEVENT)
	{
		ITEM item = *(ITEM *)lParam;

		if (wParam == ITEM_ACTION_TO_STORAGE && item.iStorageID == 0x04)
		{
			autoJewel.OnItemToCube(item);
		}
		else if (wParam == ITEM_ACTION_TO_STORAGE && item.iStorageID == 0x01)
		{
			autoJewel.OnItemToInventory(item);
		}
		else if (wParam == ITEM_ACTION_FROM_STORAGE && item.iStorageID == 0x01)
		{
			autoJewel.OnItemFromInventory(item);
		}
		else if (wParam == ITEM_ACTION_FROM_STORAGE && item.iStorageID == 0x04)
		{
			autoJewel.OnItemFromCube(item);
		}
	}
}

BYTE EXPORT OnGameKeyDown(BYTE iKeyCode)
{
	switch (iKeyCode)
	{
		case VK_SPACE:
			autoJewel.Abort();
			break;
	}

	return iKeyCode;
}

MODULECOMMANDSTRUCT ModuleCommands[] =
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


