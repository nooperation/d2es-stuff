#include <iostream>
#include "../../d2hackit/includes/ClientCore.cpp"
#include "../../d2hackit/includes/itemPrefix.h"
#include "../../d2hackit/includes/itemSuffix.h"
#include "ItemWatcher.h"

BOOL PRIVATE ToggleTownPickup(char **argv, int argc);
BOOL PRIVATE ShowEthSoc(char **argv, int argc);
BOOL PRIVATE Radius(char** argv, int argc);
BOOL PRIVATE MinGold(char** argv, int argc);
BOOL PRIVATE ToggleShowItemLevel(char** argv, int argc);
BOOL PRIVATE ReloadItemDefs(char** argv, int argc);

ItemWatcher itemWatcher;
bool enabled = true;
bool waitingForItemPickup = false;

BOOL PRIVATE Enable(char** argv, int argc)
{
	server->GameInfof("Pick Enabled");
	enabled = true;

	return TRUE;
}

BOOL PRIVATE Disable(char** argv, int argc)
{
	server->GameInfof("Pick Disabled");
	enabled = false;

	return TRUE;
}

BOOL PRIVATE Toggle(char** argv, int argc)
{
	if(enabled)
		return Disable(argv, argc);

	return Enable(argv, argc);
}


BOOL PRIVATE SetTownPickup(char **argv, int argc)
{
	if(argc == 3)
	{
		itemWatcher.SetTownPickup(atoi(argv[2]) == 1);
		return TRUE;
	}

	server->GameErrorf("Invalid input, correct usage is .pick townpick n");
	return FALSE;
}

BOOL PRIVATE ShowEthSoc(char **argv, int argc)
{
	if(argc == 3)
	{
		itemWatcher.ShowEthSoc(atoi(argv[2]) == 1);
		return TRUE;
	}

	server->GameErrorf("Invalid input, correct usage is .pick ethsoc n");
	return FALSE;
}

BOOL PRIVATE ShowEthereal(char **argv, int argc)
{
	if(argc == 3)
	{
		itemWatcher.ShowEthereal(atoi(argv[2]) == 1);
		return TRUE;
	}

	server->GameErrorf("Invalid input, correct usage is .pick eth n");
	return FALSE;
}

BOOL PRIVATE Radius(char** argv, int argc)
{
	if(argc == 3)
	{
		itemWatcher.SetRadius(atoi(argv[2]));
		return TRUE;
	}

	server->GameErrorf("Invalid input, correct usage is .pick radius n");
	return FALSE;
}

BOOL PRIVATE Items(char **argv, int argc)
{
	itemWatcher.TogglePickItems();
	return TRUE;
}

BOOL PRIVATE MinGold(char** argv, int argc)
{
	if(argc == 3)
	{
		itemWatcher.SetMinGold(atoi(argv[2]));
		return TRUE;
	}

	server->GameErrorf("Invalid input, correct usage is .pick min n");
	return FALSE;
}

BOOL PRIVATE ToggleShowItemLevel(char** argv, int argc)
{
	waitingForItemPickup = !waitingForItemPickup;

	if(waitingForItemPickup)
	{
		server->GameInfof("Waiting for item pickup to cursor...");
	}
	else
	{
		server->GameInfof("No longer waiting for item pick to cursor");
	}

	return TRUE;
}

BOOL PRIVATE ShowColors(char** argv, int argc)
{
	server->GameStringf("ÿc1 1 - abcdefghijklmnopqrstuvwxyz!@#$%^&*()_+1234567890-=");
	server->GameStringf("ÿc2 2 - abcdefghijklmnopqrstuvwxyz!@#$%^&*()_+1234567890-=");
	server->GameStringf("ÿc3 3 - abcdefghijklmnopqrstuvwxyz!@#$%^&*()_+1234567890-=");
	server->GameStringf("ÿc4 4 - abcdefghijklmnopqrstuvwxyz!@#$%^&*()_+1234567890-=");
	server->GameStringf("ÿc5 5 - abcdefghijklmnopqrstuvwxyz!@#$%^&*()_+1234567890-=");
	server->GameStringf("ÿc6 6 - abcdefghijklmnopqrstuvwxyz!@#$%^&*()_+1234567890-=");
	server->GameStringf("ÿc7 7 - abcdefghijklmnopqrstuvwxyz!@#$%^&*()_+1234567890-=");
	server->GameStringf("ÿc8 8 - abcdefghijklmnopqrstuvwxyz!@#$%^&*()_+1234567890-=");
	server->GameStringf("ÿc9 9 - abcdefghijklmnopqrstuvwxyz!@#$%^&*()_+1234567890-=");
	server->GameStringf("ÿc; ; - abcdefghijklmnopqrstuvwxyz!@#$%^&*()_+1234567890-=");
	server->GameStringf("ÿc: : - abcdefghijklmnopqrstuvwxyz!@#$%^&*()_+1234567890-=");
	server->GameStringf("ÿc0 0 - abcdefghijklmnopqrstuvwxyz!@#$%^&*()_+1234567890-=");
	server->GameStringf("ÿc/ / - abcdefghijklmnopqrstuvwxyz!@#$%^&*()_+1234567890-=");
	server->GameStringf("ÿc( ( - abcdefghijklmnopqrstuvwxyz!@#$%^&*()_+1234567890-=");
	server->GameStringf("ÿc++ - abcdefghijklmnopqrstuvwxyz!@#$%^&*()_+1234567890-=");
	server->GameStringf("ÿc- - - abcdefghijklmnopqrstuvwxyz!@#$%^&*()_+1234567890-=");

	return TRUE;
}

BOOL PRIVATE ReloadItemDefs(char** argv, int argc)
{
	server->GamePrintInfo("Reloading item definitions...");

	if(!itemWatcher.loadItems())
	{
		server->GamePrintError("There was an error loading the item files");
	}

	return TRUE;
}

////////////////////////////////////////////
//
//               EXPORTS
//
/////////////////////////////////////////////


CLIENTINFO
(
	0,1,
	"",
	"",
	"Gold picker",
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
		"on",
		Enable,
		"Enable pick"
	},
	{
		"off",
		Disable,
		"Diable pick"
	},
	{
		"toggle",
		Toggle,
		"Toggle pick"
	},
	{
		"radius",
		Radius,
		"Set radius, default = 4"
	},
	{
		"min",
		MinGold,
		"Set minimum gold to pickup, default = 50"
	},
	{
		"lvl",
		ToggleShowItemLevel,
		"Shows the item level"
	},
	{
		"colors",
		ShowColors,
		"Displays sample colors"
	},
	{
		"reload",
		ReloadItemDefs,
		"Reloads the item definitions"
	},
	{
		"townpick",
		SetTownPickup,
		"Sets town pickit on or off. Args: 1|0"
	},
	{
		"ethsoc",
		ShowEthSoc,
		"Sets ethereal socketed item display on and off. Args: 1|0"
	},
	{
		"eth",
		ShowEthereal,
		"Sets ethereal item display on and off. Args: 1|0"
	},
	{
		"items",
		Items,
		"Toggles picking of items"
	},
	{NULL}
};

BOOL EXPORT OnClientStart()
{
	if(!itemWatcher.loadItems())
	{
		server->GameErrorf("Failed to load item tables. Do you have pickItems.txt?");
		server->GameErrorf(" in the plugin directory?");
		return FALSE;
	}

	enabled = true;

	return TRUE;
}

int ticksLeft = 3;
DWORD EXPORT OnGameTimerTick()
{
	if(enabled)
	{
		itemWatcher.CheckWatchedItems();
	}

	return 0;
}

VOID EXPORT OnUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam, LPARAM lParam)
{
	if(nMessage == UM_DESTROY)
	{
		if(lpUnit->dwUnitType == UNIT_TYPE_ITEM)
		{
			itemWatcher.OnItemDestroy(lpUnit->dwUnitID);
		}
	}
}

void DumpItemInfo(const ITEM& item)
{
	char chatBuff[128];
	char itemColor[4];

	switch(item.iQuality)
	{
		case ITEM_QUALITY_MAGIC:
			strcpy_s(itemColor, 4, "ÿc3");
			break;
		case ITEM_QUALITY_RARE:
			strcpy_s(itemColor, 4, "ÿc9");
			break;
		case ITEM_QUALITY_CRAFT:
			strcpy_s(itemColor, 4, "ÿc8");
			break;
		default:
			return;
	}

	server->GameStringf("%s%sÿc0 affixes:", itemColor, server->GetItemName(item.szItemCode));

	for(int i = 0; i < 3; i++)
	{
		if(item.wPrefix[i] != 0)
		{
			if(item.wPrefix[i] > ITEM_PREFIX_COUNT)
			{
				server->GameStringf("Prefix too large: %d", item.wPrefix[i]);
			}
			else
			{
				sprintf(chatBuff, "  ÿc;Prefixÿc0 %d: %s - %s",item.wPrefix[i], Prefix[item.wPrefix[i]], PrefixDetails[item.wPrefix[i]]);
				server->GamePrintString(chatBuff);
			}
		}
	}
	for(int i = 0; i < 3; i++)
	{
		if(item.wSuffix[i] != 0)
		{
			if(item.wSuffix[i] > ITEM_SUFFIX_COUNT)
			{
				server->GameStringf("Suffix too large: %d", item.wSuffix[i]);
			}
			else
			{
				sprintf(chatBuff, "  ÿc:Suffixÿc0 %d: %s - %s", item.wSuffix[i], Suffix[item.wSuffix[i]], SuffixDetails[item.wSuffix[i]]);
				server->GamePrintString(chatBuff);
			}
		}
	}
}

VOID EXPORT OnGamePacketAfterReceived(BYTE* aPacket, DWORD aLen)
{   

	if(enabled && (aPacket[0] == 0x9c || aPacket[0] == 0x9d))
	{
		ITEM item;
		if(!server->ParseItem(aPacket, aLen, item))
			return;
		//server->GameStringf("Packet %02X %02X", aPacket[0], aPacket[1]);

		if(aPacket[1] == ITEM_ACTION_FROM_STORAGE || aPacket[1] == ITEM_ACTION_PICK_UP || aPacket[1] == ITEM_ACTION_TO_CURSOR)
		{
			if(waitingForItemPickup)
			{
				server->GameStringf("%s: Level %d", item.szItemCode, item.iLevel);
				DumpItemInfo(item);
			}
		}
		else
		{
			itemWatcher.OnItemAction(item);
		}
	}
}
