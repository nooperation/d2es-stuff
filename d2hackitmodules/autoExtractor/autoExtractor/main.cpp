#include "../../d2hackit/includes/ClientCore.cpp"
#include "AutoExtractor.h"

AutoExtractor autoExtractor;

CLIENTINFO
(
	1,4,
	"Auto Extractor v1.4",
	"http://madbrahmin.googlepages.com/d2es/",
	"Auto Extractor v1.4",
	""
)

BOOL PRIVATE Start(char** argv, int argc)
{
	bool useChat = false;

	if(argc >= 3)
	{
		for(int i = 2; i < argc; i++)
		{
			if(_stricmp(argv[i], "chat") == 0)
				useChat = true;
		}
	}

	autoExtractor.Start(useChat);

	return TRUE;
}

DWORD EXPORT OnGameTimerTick()
{
	autoExtractor.OnTick();

	return 0;
}

VOID EXPORT OnThisPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	if(nMessage == PM_UICLOSED)
	{
		autoExtractor.Abort();
	}
}

VOID EXPORT OnUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam, LPARAM lParam)
{
	if(nMessage == UM_ITEMEVENT)
	{
		ITEM item = *(ITEM *)lParam;

		if(wParam == 0x04 && item.iStorageID == 0x04)
		{
			autoExtractor.OnItemToCube(item);
		}
		else if(wParam == 0x04 && item.iStorageID == 0x01)
		{
			autoExtractor.OnItemToInventory(item);
		}
		else if(wParam == 0x05 && item.iStorageID == 0x04)
		{
			autoExtractor.OnItemFromCube(item);
		}
	}
}

BYTE EXPORT OnGameKeyDown(BYTE iKeyCode)
{
	switch(iKeyCode)
	{
		case VK_SPACE:
			autoExtractor.Abort();
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


