#include "../../Includes/ClientCore.cpp"
#include <queue>

#define TOTAL_DSTONE_TYPES 7

CLIENTINFO
(
	0,1,
	"",
	"",
	"DStone Changer",
	""
)

bool CheckDStone(DWORD dwItemID);
BOOL CALLBACK enumItemCountProc(DWORD dwItemID, LPARAM lParam);
bool SearchForDStones();
void OnItemToStorage(DWORD dwItemID);
BOOL UseDStone(DWORD dwItemID);

std::queue<DWORD> itemsToUse;
int targetDStoneType = 1;
bool started = false;

const char *dStoneTypes[TOTAL_DSTONE_TYPES] = 
{
	"ppp", //ÿc;Dragon", //Stone
	"hly", //ÿc/Holy", //Symbol
	"eee", //ÿc1Blackmoor
	"hh2", //ÿc7Forging", //Hammer	
	"ggg", //ÿc3Crushed", //Gem
	"sss", //ÿc5Spider's", //Silk
	"ttt", //ÿc5Tyranium", //Ore
};

bool CheckDStone(DWORD dwItemID)
{
	char itemCode[4];
	server->GetItemCode(dwItemID, itemCode, sizeof(itemCode)/sizeof(itemCode[0]));

	for(int i = 0; i < TOTAL_DSTONE_TYPES; i++)
	{
		if(strcmp(itemCode, dStoneTypes[i]) == 0)
		{
			if(i != targetDStoneType-1)
			{
				itemsToUse.push(dwItemID);
				return true;
			}

			return false;
		}
	}

	return false;
}

void OnItemToStorage(DWORD dwItemID)
{
	if(started && itemsToUse.empty())
	{
		SearchForDStones();
	}
}

bool SearchForDStones()
{
	int itemCount = 0;

	me->EnumStorageItems(STORAGE_INVENTORY, (fnEnumItemProc)enumItemCountProc, (LPARAM)&itemCount); 

	if(itemCount == 0)
	{
		started = false;
		server->GameStringf("ÿc5DCÿc0: Done");
		return false;
	}

	return true;
}

BOOL UseDStone(DWORD dwItemID)
{
	if (dwItemID == 0)
		return FALSE;

	BYTE aPacket[5] = {0};
	aPacket[0] = 0x4c;
	::memcpy(aPacket + 1, &dwItemID, 4);

	return server->GameSendPacketToServer(aPacket, 5);
}

BOOL PRIVATE PrintDStoneTypes(char** argv, int argc)
{
	server->GameStringf("Dstone Types:");
	server->GameStringf("  1 - ÿc;Dragon Stone");
	server->GameStringf("  2 - ÿc/Holy Symbol");
	server->GameStringf("  3 - ÿc1Blackmoor");
	server->GameStringf("  4 - ÿc7Forging Hammer");
	server->GameStringf("  5 - ÿc3Crushed Gem");
	server->GameStringf("  6 - ÿc5Spider's Silk");
	server->GameStringf("  7 - ÿc5Tyranium Ore");

	return TRUE;
}

BOOL PRIVATE Start(char** argv, int argc)
{
	bool useChat = false;

	if(me->GetCursorItem() != 0)
	{
		server->GameStringf("ÿc5DCÿc0: Do not touch or hold any items when dc is running");
		return FALSE;
	}

	if(argc >= 3)
	{
		targetDStoneType = atoi(argv[2]);

		if(targetDStoneType <= 0 || targetDStoneType > TOTAL_DSTONE_TYPES)
			return FALSE;
	}

	started = true;
	SearchForDStones();

	return TRUE;
}

DWORD EXPORT OnGameTimerTick()
{
	if(!started || itemsToUse.empty())
		return 0;

	DWORD itemToUse = itemsToUse.front();
	itemsToUse.pop();

	UseDStone(itemToUse);

	return 0;
}

DWORD EXPORT OnGamePacketBeforeSent(BYTE* aPacket, DWORD aLen)
{
	if(started)
	{
		if(aPacket[0] == 0x19 || aPacket[0] == 0x16 || aPacket[0] == 0x60 || aPacket[0] == 0x1f || aPacket[0] == 0x1c || aPacket[0] == 0x1d)
		{ 
			server->GameStringf("ÿc5DCÿc0: Do not touch anything while DC is running");
			return 0;
		}
	}

	return aLen;
}

VOID EXPORT OnUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam, LPARAM lParam)
{
	if(nMessage == UM_ITEMEVENT)
	{
		ITEM *item = (ITEM *)lParam;

		if(wParam == 0x04 && item->iStorageID == 0x01)
		{
			OnItemToStorage(item->dwItemID);
		}
	}
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
		"Types",
		PrintDStoneTypes,
		"Usage: Types",
	},
	{
		"Start",
		Start,
		"Usage: Start [type 1-7]",
	},

	{NULL}
};

BOOL CALLBACK enumItemCountProc(DWORD dwItemID, LPARAM lParam)
{
	if(CheckDStone(*(DWORD *)dwItemID))
	{
		(*(int *)lParam)++;
	}

	return TRUE;
}
