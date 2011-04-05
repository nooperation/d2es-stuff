#include "../../d2hackit/includes/ClientCore.cpp"
#include "AutostockAutoextract.h"

AutostockAutoextract autostockAutoextract;

CLIENTINFO
(
	1,2,
	"Autostock Autoextract v1.2",
	"",
	"Autostock Autoextract v1.2",
	""
)

BOOL PRIVATE Start(char** argv, int argc)
{
	bool useChat = false;
	bool uniques = false;
	bool rares = false;
	bool sets = false;

	if(argc < 2)
	{
		return FALSE;
	}

	for(int i = 2; i < argc; i++)
	{
		if(_stricmp(argv[i], "sets") == 0 || _stricmp(argv[i], "set") == 0)
		{
			sets = true;
		}
		else if(_stricmp(argv[i], "rares") == 0 || _stricmp(argv[i], "rare") == 0)
		{
			rares = true;
		}
		else if(_stricmp(argv[i], "uniques") == 0 || _stricmp(argv[i], "unique") == 0)
		{
			uniques = true;
		}
		else if(_stricmp(argv[i], "chat") == 0)
		{
			useChat = true;
		}
	}

	autostockAutoextract.Start(sets, rares, uniques, useChat);

	return TRUE;
}



DWORD EXPORT OnGameTimerTick()
{
	autostockAutoextract.OnTick();

	return 0;
}

DWORD EXPORT OnGamePacketBeforeSent(BYTE* aPacket, DWORD aLen)
{
	if(aPacket[0] == 0x15 && aPacket[1] == 0x01)
	{
		char *chatMessage = (char *)(aPacket+3);

		if(strncmp(chatMessage, "ÿc5EmptyCubeÿc0:", 16) == 0)
		{
			if(strcmp(chatMessage, "ÿc5EmptyCubeÿc0: EmptyCube Ended") == 0)
			{
				if(!autostockAutoextract.OnEmptyCubeEnded())
				{
					return aLen;
				}
				else
				{
					return 0;
				}
			}
			else
			{
				return 0;
			}
		}
		else if(strncmp(chatMessage, "ÿc:AutoExtractorÿc0:", 20) == 0)
		{
			if(strcmp(chatMessage, "ÿc:AutoExtractorÿc0: AutoExtractor Ended") == 0)
			{
				if(!autostockAutoextract.OnAutoExtractorEnded())
				{
					return aLen;
				}
				else
				{
					return 0;
				}
			}
			else
			{
				return 0;
			}
		}
		else if(strncmp(chatMessage, "ÿc:Autostockerÿc0:", 18) == 0)
		{
			if(strcmp(chatMessage, "ÿc:Autostockerÿc0: Autostocker Ended") == 0)
			{
				if(!autostockAutoextract.OnAutostockerEnded())
				{
					return aLen;
				}
				else
				{
					return 0;
				}
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
		autostockAutoextract.Abort();
	}
}

VOID EXPORT OnUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam, LPARAM lParam)
{
	if(nMessage == UM_ITEMEVENT)
	{
		ITEM item = *(ITEM *)lParam;

		if(wParam == 0x04 && item.iStorageID == 0x04)
		{
			autostockAutoextract.OnItemToCube(item);
		}
		else if(wParam == 0x05 && item.iStorageID == 0x01)
		{
			autostockAutoextract.OnItemFromInventory(item);
		}
		else if(wParam == 0x05 && item.iStorageID == 0x04)
		{
			autostockAutoextract.OnItemToInventory(item);
		}
	}
}

BYTE EXPORT OnGameKeyDown(BYTE iKeyCode)
{
	switch(iKeyCode)
	{
		case VK_SPACE:
			autostockAutoextract.Abort();
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
		"Usage: Start [sets] [rares] [uniques]",
	},
	{NULL}
};


