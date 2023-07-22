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

	autoBuy.Start(quantity, itemCode);

	return TRUE;
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
	{NULL}
};


