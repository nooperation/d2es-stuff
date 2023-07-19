#include "../../Includes/ClientCore.cpp"
#include "AutoTransmute.h"

AutoTransmute autoTransmute;

bool settingHotkey = false;
int hotkey = 0;

CLIENTINFO
(
	0, 1,
	"Auto Transmute v0.1",
	"",
	"Auto Transmute v0.1",
	""
)

BOOL PRIVATE Start(char **argv, int argc)
{
	if (argc < 3)
	{
		return FALSE;
	}

	auto recipeName = std::string(argv[2]);
	autoTransmute.Start(recipeName);

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
			if (!autoTransmute.OnEmptyCubeMessage(message))
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
		autoTransmute.Abort();
	}
}

VOID EXPORT OnUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam, LPARAM lParam)
{
	if (nMessage == UM_ITEMEVENT)
	{
		ITEM item = *(ITEM *)lParam;

		if (wParam == 0x04 && item.iStorageID == 0x04)
		{
			autoTransmute.OnItemToCube(item);
		}
		else if (wParam == 0x05 && item.iStorageID == 0x01)
		{
			autoTransmute.OnItemFromInventory(item);
		}
	}
}

BYTE EXPORT OnGameKeyDown(BYTE iKeyCode)
{
	switch (iKeyCode)
	{
		case VK_SPACE:
			autoTransmute.Abort();
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
		"Usage: Start nameFromAutoTransmute.txt [chat]",
	},
	{NULL}
};


