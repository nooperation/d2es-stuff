#include "../../Includes/ClientCore.cpp"
#include "AutoChit.h"

AutoChit autoChit;

bool settingHotkey = false;
int hotkey = 0;

CLIENTINFO
(
	0, 1,
	"Auto Chit v0.1",
	"",
	"Auto Chit v0.1",
	""
)

BOOL PRIVATE Start(char **argv, int argc)
{
	if (argc < 2)
	{
		return FALSE;
	}

	autoChit.Start();

	return TRUE;
}

DWORD EXPORT OnGamePacketBeforeSent(BYTE *aPacket, DWORD aLen)
{
	if (aPacket[0] == 0x15 && aPacket[1] == 0x01)
	{
		char *chatMessage = (char *)(aPacket + 3);

		if (strncmp(chatMessage, "ÿc5EmptyCubeÿc0:", 16) == 0)
		{
			const auto message = std::string_view(chatMessage + 17);
			if (!autoChit.OnEmptyCubeMessage(message))
			{
				return aLen;
			}
			else
			{
				return 0;
			}
		}
	}

	return aLen;
}

VOID EXPORT OnThisPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	if (nMessage == PM_UICLOSED)
	{
		autoChit.Abort();
	}
}

VOID EXPORT OnUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam, LPARAM lParam)
{
	if (nMessage == UM_ITEMEVENT)
	{
		ITEM item = *(ITEM *)lParam;

		if (wParam == ITEM_ACTION_TO_STORAGE && item.iStorageID == 0x04)
		{
			autoChit.OnItemToCube(item);
		}
		else if (wParam == ITEM_ACTION_FROM_STORAGE && item.iStorageID == 0x01)
		{
			autoChit.OnItemFromInventory(item);
		}
		else if(wParam == ITEM_ACTION_FROM_STORAGE && item.iStorageID == 0x04)
		{
			autoChit.OnItemFromCube(item);
		}
		else if (wParam == ITEM_ACTION_DROP)
		{
			autoChit.OnItemDropped(item.dwItemID);
		}
	}
}

BYTE EXPORT OnGameKeyDown(BYTE iKeyCode)
{
	switch (iKeyCode)
	{
		case VK_SPACE:
			autoChit.Abort();
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


