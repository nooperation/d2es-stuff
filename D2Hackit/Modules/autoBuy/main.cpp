#include "../../Includes/ClientCore.cpp"
#include "autoBuy.h"

AutoBuy autoBuy;

CLIENTINFO
(
	1,0,
	"Nop",
	"https://github.com/nooperation/d2es-stuff/",
	"Auto Buy v1.0",
	""
)

BOOL PRIVATE Start(char** argv, int argc)
{
	if (argc != 4)
	{
		return FALSE;
	}

	auto quantity = atoi(argv[2]);
	auto itemCode = std::string(argv[3]);

	autoBuy.Start(quantity, itemCode, false);

	return TRUE;
}


BOOL PRIVATE Auto(char** argv, int argc)
{
	if (argc != 3)
	{
		return FALSE;
	}

	auto itemCode = std::string(argv[2]);

	autoBuy.Start(1, itemCode, true);

	return TRUE;
}

BOOL PRIVATE Gempacks(char** argv, int argc)
{
	autoBuy.Start(1, "6gk", true);
	return TRUE;
}

VOID EXPORT OnThisPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	if (nMessage == PM_UICLOSED)
	{
		autoBuy.OnUIClosed();
	}
	if (nMessage == PM_NPCSESSION)
	{
		autoBuy.OnNpcSession(wParam);
	}
}

bool isTrading = false;
DWORD EXPORT OnGameTimerTick()
{
	if (me->IsUIOpened(UI_NPCSHOP))
	{
		if (!isTrading)
		{
			isTrading = true;
			autoBuy.OnNPCShopScreenOpened();
		}
	}
	else
	{
		isTrading = false;
	}

	return 0;
}


BYTE EXPORT OnGameKeyDown(BYTE iKeyCode)
{
	switch(iKeyCode)
	{
		case VK_SPACE:
			autoBuy.Stop();
			break;
	}

	return iKeyCode;
}

DWORD EXPORT OnGamePacketBeforeReceived(BYTE* aPacket, DWORD aLen)
{
	if(aPacket[0] == 0x9c)
	{
		ITEM currentItem;

		if(!server->ParseItem(aPacket, aLen, currentItem))
		{
			return aLen;
		}

		if(currentItem.iStore)
		{
			autoBuy.OnNpcItemList(currentItem);
		}
	}

	return aLen;
}

void EXPORT OnGamePacketAfterReceived(BYTE* aPacket, DWORD aLen)
{
	if (aPacket[0] == 0x9c)
	{
		ITEM currentItem;

		if (!server->ParseItem(aPacket, aLen, currentItem))
		{
			return;
		}

		if (currentItem.iAction == ITEM_ACTION_TO_STORAGE)
		{
			autoBuy.OnItemToStorageFromStore(currentItem);
		}
	}

	return;
}

DWORD EXPORT OnGamePacketBeforeSent(BYTE* aPacket, DWORD aLen)
{
	if (aPacket[0] == 0x15 && aPacket[1] == 0x01)
	{
		char* chatMessage = (char*)(aPacket + 3);

		// Sneaky message passing, doesn't send autostocker chat messages out
		if (strncmp(chatMessage, "ÿc:Autostockerÿc0:", 18) == 0)
		{
			const auto message = std::string_view(chatMessage + 19);
			if (!autoBuy.OnAutostockerMessage(message))
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
		"Usage: Start <count> <itemcode>",
	},
	{
		"Auto",
		Auto,
		"Usage: Auto <itemcode>",
	},
	{
		"Gempacks",
		Gempacks,
		"Usage: Gempacks",
	},
	{NULL}
};


