#include "../../Includes/ClientCore.cpp"
#include "AutoOre.h"

AutoOre autoOre;

CLIENTINFO
(
	2,0,
	"AutoOre v2.0",
	"",
	"AutoOre v2.0",
	""
)

BOOL PRIVATE Start(char** argv, int argc)
{
	bool useChat = false;

	if(argc >= 3)
	{
		for (int i = 2; i < argc; i++)
		{
			if (_stricmp(argv[i], "chat") == 0)
			{
				useChat = true;
			}
		}
	}

	autoOre.Start(useChat);
	return TRUE;
}

DWORD EXPORT OnGamePacketBeforeSent(BYTE* aPacket, DWORD aLen)
{
	if(aPacket[0] == 0x15 && aPacket[1] == 0x01)
	{
		char *chatMessage = (char *)(aPacket+3);

		if (strncmp(chatMessage, "ÿc5EmptyCubeÿc0:", 16) == 0)
		{
			const auto message = std::string_view(chatMessage + 17);
			if (!autoOre.OnEmptyCubeMessage(message))
			{
				return aLen;
			}
			else
			{
				return 0;
			}
		}
		else if(strncmp(chatMessage, "ÿc:Autostockerÿc0:", 18) == 0)
		{
			const auto message = std::string_view(chatMessage + 19);
			if(!autoOre.OnAutoStockerMessage(message))
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
		autoOre.Abort();
	}
}

VOID EXPORT OnUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam, LPARAM lParam)
{
	if(nMessage == UM_ITEMEVENT)
	{
		ITEM item = *(ITEM *)lParam;

		//server->GameStringf("%s: wParam = %d | iStorageId = %d", item.szItemCode, wParam, item.iStorageID);

		if(wParam == ITEM_ACTION_TO_STORAGE && item.iStorageID == 0x04)
		{
			autoOre.OnItemDroppedToCube(item); 
		}
		else if(wParam == ITEM_ACTION_FROM_STORAGE && item.iStorageID == 0x01)
		{
			autoOre.OnItemPickedUpFromInventory(item.dwItemID);
		}
		else if(wParam == ITEM_ACTION_TO_STORAGE && item.iStorageID == 0x01)
		{
			autoOre.OnItemDroppedToInventory(item);
		}
		else if(wParam == ITEM_ACTION_FROM_STORAGE && item.iStorageID == 0x04)
		{
			autoOre.OnItemPickedUpFromCube(item.dwItemID);
		}
		else if (wParam == ITEM_ACTION_DROP)
		{
			autoOre.OnItemDropped(item.dwItemID);
		}
	}
}

DWORD EXPORT OnGameTimerTick()
{
	// OnGameTimerTick happens after all the modules have processed OnGamePacketAfterReceived (which is when the OnUnitMessage above is called)
	autoOre.OnTick();
	return 0;
}

/*
void EXPORT OnGamePacketAfterReceived(BYTE *aPacket, DWORD aLen)
{
	if (aPacket == nullptr)
	{
		return;
	}

	if (aPacket[0] == 0x9c && aPacket[1] == ITEM_ACTION_DROP)
	{
		const auto itemId = *((DWORD *)&aPacket[4]);
		autoOre.OnItemDropped(itemId);
	}
}*/

BYTE EXPORT OnGameKeyDown(BYTE iKeyCode)
{
	switch(iKeyCode)
	{
		case VK_SPACE:
			autoOre.Abort();
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
		"Usage: Start",
	},
	{NULL}
};


