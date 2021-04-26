#include "../../Includes/ClientCore.cpp"
#include "AutoAncientScroll.h"

AutoAncientScroll autoAncientScroll;

bool settingHotkey = false;
int hotkey = 0;

CLIENTINFO
(
	1,0,
	"Auto Ancient Scroll v1.0",
	"",
	"Auto Ancient Scroll v1.0",
	""
)

BOOL PRIVATE Start(char** argv, int argc)
{
	autoAncientScroll.Start();

	return TRUE;
}

DWORD EXPORT OnGamePacketBeforeSent(BYTE *aPacket, DWORD aLen)
{
	if (aPacket[0] == 0x15 && aPacket[1] == 0x01)
	{
		char *chatMessage = (char *)(aPacket + 3);

		if (strncmp(chatMessage, "ÿc:AutoExtractorÿc0:", 20) == 0)
		{
			const auto message = std::string_view(chatMessage + 21);
			if (!autoAncientScroll.OnAutoExtractorMessage(message))
			{
				return aLen;
			}
			else
			{
				return 0;
			}
		}
		else if (strncmp(chatMessage, "ÿc:Autostockerÿc0:", 18) == 0)
		{
			const auto message = std::string_view(chatMessage + 19);
			if (!autoAncientScroll.OnAutoStockerMessage(message))
			{
				return aLen;
			}
			else
			{
				return 0;
			}
		}
		else if (strncmp(chatMessage, "ÿc:Transmuteÿc0:", 16) == 0)
		{
			const auto message = std::string_view(chatMessage + 17);
			if (!autoAncientScroll.OnTransmuteMessage(message))
			{
				return aLen;
			}
			else
			{
				return 0;
			}
		}
		else if (strncmp(chatMessage, "ÿc5EmptyCubeÿc0:", 16) == 0)
		{
			const auto message = std::string_view(chatMessage + 17);
			if (!autoAncientScroll.OnEmptyCubeMessage(message))
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
	if(nMessage == PM_UICLOSED)
	{
		autoAncientScroll.Abort();
	}
}

VOID EXPORT OnUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam, LPARAM lParam)
{
	if(nMessage == UM_ITEMEVENT)
	{
		ITEM item = *(ITEM *)lParam;

		if(wParam == 0x04 && item.iStorageID == 0x04)
		{
			autoAncientScroll.OnItemToCube(item);
		}
		else if(wParam == 0x04 && item.iStorageID == 0x01)
		{
			autoAncientScroll.OnItemToInventory(item);
		}
		else if(wParam == 0x05 && item.iStorageID == 0x01)
		{
			autoAncientScroll.OnItemFromInventory(item);
		}
		else if(wParam == 0x05 && item.iStorageID == 0x04)
		{
			autoAncientScroll.OnItemFromCube(item);
		}
	}
}

BYTE EXPORT OnGameKeyDown(BYTE iKeyCode)
{
	switch(iKeyCode)
	{
		case VK_SPACE:
			autoAncientScroll.Abort();
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


