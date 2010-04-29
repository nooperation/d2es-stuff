#include "../../d2hackit/includes/ClientCore.cpp"
#include "AutoExtractor.h"

AutoExtractor autoExtractor;

int ticksSkipped = 0;
int speed = 0;

CLIENTINFO
(
	0,1,
	"",
	"",
	"Auto Extractor",
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


BOOL PRIVATE Speed(char** argv, int argc)
{
	if(argc == 3)
	{
		speed = atoi(argv[2]);

		if(speed < 0)
			speed = 0;

		server->GameStringf("Setting speed to %d", speed);
		return TRUE;
	}

	return FALSE;
}

DWORD EXPORT OnGameTimerTick()
{
	if(ticksSkipped++ < speed)
		return 0;

	autoExtractor.OnTick();
	ticksSkipped = 0;

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
			autoExtractor.OnItemFromCube(item.dwItemID);
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
	{
		"Speed",
		Speed,
		"Speed # where # is the number of ticks to wait per command. Each tick is 100ms"
	},
	{NULL}
};


