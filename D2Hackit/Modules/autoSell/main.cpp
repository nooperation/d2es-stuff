#include "../../Includes/ClientCore.cpp"
#include "autoSell.h"

AutoSell autoSell;

CLIENTINFO
(
	1,0,
	"Auto Sell v1.0",
	"http://madbrahmin.googlepages.com/d2es/",
	"Auto Sell v1.0",
	""
)

BOOL PRIVATE Start(char** argv, int argc)
{
	autoSell.Start();

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
			autoSell.OnNPCShopScreenOpened();
		}
	}
	else
	{
		isTrading = false;
	}

	return 0;
}

void EXPORT OnGamePacketAfterReceived(BYTE *aPacket, DWORD aLen)
{
	if (aPacket[0] == 0x2a) // npc transation
	{
		if (aPacket[2] == 0x01) // 0x0C = Not enough money, 0x00 = bought, 0x01 = sold
		{
			autoSell.OnItemSold();
		}
	}
}

BYTE EXPORT OnGameKeyDown(BYTE iKeyCode)
{
	switch(iKeyCode)
	{
		case VK_SPACE:
			autoSell.Stop();
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
			autoSell.OnNpcItemList(currentItem);
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
		"Usage: Start",
	},
	{
		"All",
		Start,
		"Alias for start",
	},
	{NULL}
};


