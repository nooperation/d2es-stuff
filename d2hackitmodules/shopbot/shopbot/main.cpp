#include "../../d2hackit/includes/ClientCore.cpp"
#include "shopbot.h"

ShopBot shopbot;

std::vector<MAPPOS> customPath;

BOOL PRIVATE Shop(char** argv, int argc)
{
	if(customPath.size() == 0)
	{
		switch(me->GetCurrentMapID())
		{
			case MAP_A4_THE_PANDEMONIUM_FORTRESS:
			{
				std::vector<MAPPOS> a4Path;
				MAPPOS pos;

				pos.x = 5087;
				pos.y = 5034;
				a4Path.push_back(pos);

				pos.x = 5134;
				pos.y = 5070;
				a4Path.push_back(pos);

				pos.x = 5179;
				pos.y = 5096;
				a4Path.push_back(pos);

				break;
			}
			default:
				server->GameStringf("ÿc:Shopbotÿc0: No path defined");
		}
	}
	else
	{
		if(argc >= 3)
		{
			shopbot.Start(customPath, argv[2]);
		}
		else
		{
			shopbot.Start(customPath, "");
		}
	}
	return TRUE;
}

BOOL PRIVATE Pos(char** argv, int argc)
{
	MAPPOS pos = me->GetPosition();
	server->GameStringf("ÿc:Shopbotÿc0: Current position: X: %d Y: %d", pos.x, pos.y);

	return TRUE;
}

BOOL PRIVATE Mark(char** argv, int argc)
{
	customPath.push_back(me->GetPosition());
	server->GameStringf("ÿc:Shopbotÿc0: Location %d: X: %d Y: %d", customPath.size(), customPath.back().x, customPath.back().y);

	return TRUE;
}

BOOL PRIVATE Undo(char **argv, int argc)
{
	if(customPath.size() > 0)
	{
		customPath.pop_back();
		server->GameStringf("ÿc:Shopbotÿc0: Removed last marked spot");
	}

	return TRUE;
}

BOOL PRIVATE Clear(char **argv, int argc)
{
	customPath.clear();
	server->GameStringf("ÿc:Shopbotÿc0: Custom path cleared");

	return TRUE;
}

DWORD EXPORT OnGameTimerTick()
{
	shopbot.OnTick();

	return 0;
}

VOID EXPORT OnThisPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	switch(nMessage)
	{
		case PM_MAPBLINK:
			shopbot.OnMapBlink();
			break;
		case PM_NPCSESSION:
			shopbot.OnNpcSession(wParam);
			break;
	}
}

BYTE EXPORT OnGameKeyDown(BYTE iKeyCode)
{
	switch(iKeyCode)
	{
		case VK_SPACE:
			shopbot.Stop();
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
			shopbot.OnNpcItemList(currentItem);
		}
		else if(currentItem.iAction == ITEM_ACTION_TO_STORAGE)
		{
			shopbot.OnItemToStorage(currentItem);
		}
	}
	else if(aPacket[0] == 0x2a) // npc transation
	{
		if(aPacket[2] == 0x0C) // 0x0C = Not enough money, 0x00 = bought, 0x01 = sold
		{
			shopbot.OnNotEnoughMoney();
		}
	}


	return aLen;
}



CLIENTINFO
(
	0,1,
	"",
	"",
	"",
	""
)

MODULECOMMANDSTRUCT ModuleCommands[]=
{
	{
		"help",
		OnGameCommandHelp,
		"helpÿc0 List commands available in this module.\n"
		"<command> helpÿc0 Shows detailed help for <command> in this module."
	},
	{
		"shop",
		Shop,
		"Shop - starts shopping"
	},
	{
		"pos",
		Pos,
		"pos - displays current position"
	},
	{
		"mark",
		Mark,
		"marks location as teleport spot",
	},
	{
		"undo",
		Undo,
		"removes last marked spot",
	},
	{
		"clear",
		Clear,
		"clears custom path",
	},
	{NULL}
};
