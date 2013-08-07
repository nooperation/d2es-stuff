#include "../../Includes/ClientCore.cpp"
#include "RuneDowngrade.h"

RuneDowngrade runeDowngrade;

CLIENTINFO
(
	0,1,
	"",
	"",
	"asdf",
	""
)

BOOL PRIVATE Start(char** argv, int argc)
{
	if(argc < 3)
	{
		server->GameStringf("RuneDowngrade: Downgrades all runes of type Arg1 Arg2 ... ArgN once. eg: .start r21 r20 r19 r18 r17 r16 to convert runes 21-16 to sa runes");
		return FALSE;
	}

	std::vector<std::string> runeTypesToDowngrade;

	for(int i = 2; i < argc; ++i)
	{
		runeTypesToDowngrade.push_back(argv[i]);
	}

	runeDowngrade.Start(runeTypesToDowngrade);

	return TRUE;
}

BOOL PRIVATE StartSA(char** argv, int argc)
{
	server->GameCommandLine("rd start r28 r27 r26 r25 r24 r23 r22 r21 r20 r19 r18 r17 r16");
	return TRUE;
}

VOID EXPORT OnUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam, LPARAM lParam)
{
	if(nMessage == UM_ITEMEVENT)
	{
		ITEM item = *(ITEM *)lParam;

		if(wParam == 0x04 && item.iStorageID == 0x04)
		{
			runeDowngrade.OnItemToCube(item);
		}
		else if(wParam == 0x04 && item.iStorageID == 0x01)
		{
			runeDowngrade.OnItemToInventory(item);
		}
		else if(wParam == 0x05 && item.iStorageID == 0x01)
		{
			runeDowngrade.OnItemFromInventory(item);
		}
		else if(wParam == 0x05 && item.iStorageID == 0x04)
		{
			runeDowngrade.OnItemFromCube(item);
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
		"start",
		Start,
		"Downgrades all runes of type Arg1 Arg2 ... ArgN once. eg: .start r28 ... r20 r19 r18 r17 r16 to convert runes 21-16 to sa runes"
	},
	{
		"start_sa",
		StartSA,
		"Downgrades all runes in inventory of type Yu Ke E Ko Ra O to Sa type",
	},
	{NULL}
};