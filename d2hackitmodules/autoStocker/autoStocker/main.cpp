#include "../../d2hackit/includes/ClientCore.cpp"
#include "AutoStocker.h"

#define CONFIG_FILE ".\\plugin\\autostockerMisc.ini"

void WriteConfig();
void ReadConfig();

AutoStocker autoStocker;

bool settingHotkey = false;
int hotkey = 0;

CLIENTINFO
(
	1,6,
	"Auto Stocker v1.6",
	"",
	"Auto Stocker v1.6",
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

	autoStocker.Start(useChat);

	return TRUE;
}


BOOL PRIVATE StartRares(char** argv, int argc)
{
	bool useChat = false;
	bool uniques = false;
	bool rares = false;
	bool sets = false;

	if(argc < 3)
		return FALSE;

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

	autoStocker.StartRares(sets, rares, uniques, useChat);

	return TRUE;
}

//DWORD EXPORT OnGamePacketBeforeReceived(BYTE* aPacket, DWORD aLen)
//{
//	if(aPacket[0] == 0x2c)
//	{
//		if(aPacket[6] == 0x04 && aPacket[7] == 0x00)
//		{
//			autoStocker.OnTransmute();
//		}
//	}
//
//	return aLen;
//}

VOID EXPORT OnGameJoin(THISGAMESTRUCT* thisgame)
{
	ReadConfig();
}

DWORD EXPORT OnGameTimerTick()
{
	autoStocker.OnTick();

	return 0;
}

VOID EXPORT OnThisPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	if(nMessage == PM_UICLOSED)
	{
		autoStocker.Abort();
	}
}

VOID EXPORT OnUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam, LPARAM lParam)
{
	if(nMessage == UM_ITEMEVENT)
	{
		ITEM item = *(ITEM *)lParam;

		if(wParam == 0x04 && item.iStorageID == 0x04)
		{
			autoStocker.OnItemToCube(item);
		}
		else if(wParam == 0x04 && item.iStorageID == 0x01)
		{
			autoStocker.OnItemToInventory(item);
		}
		else if(wParam == 0x05 && item.iStorageID == 0x01)
		{
			autoStocker.OnItemFromInventory(item);
		}
		else if(wParam == 0x05 && item.iStorageID == 0x04)
		{
			autoStocker.OnItemFromCube(item);
		}
	}
}

BOOL PRIVATE SetHotkey(char** argv, int argc)
{
	settingHotkey = true;

	server->GameStringf("ÿc:Autostockerÿc0: Next key pressed will be the hotkey...");

	return TRUE;
}


BYTE EXPORT OnGameKeyDown(BYTE iKeyCode)
{
	if(settingHotkey)
	{
		settingHotkey = false;

		if(iKeyCode == 0 || iKeyCode == VK_ESCAPE || iKeyCode == VK_RETURN)
		{
			server->GameStringf("ÿc:Autostockerÿc0: Unbound hotkey");
			hotkey = 0;
		}
		else
		{
			hotkey = iKeyCode;
			server->GameStringf("ÿc:Autostockerÿc0: Set hotkey to %d", iKeyCode);
		}

		WriteConfig();
	}
	else
	{
		if(iKeyCode == hotkey)
		{
			Start(NULL, 0);
		}
	}

	switch(iKeyCode)
	{
		case VK_SPACE:
			autoStocker.Abort();
			break;
	}

	return iKeyCode;
}

void ReadConfig()
{
	hotkey = GetPrivateProfileInt("Autostocker", "Hotkey", 0, CONFIG_FILE);
}

void WriteConfig()
{
	char configBuff[64];

	sprintf_s(configBuff, sizeof(configBuff)/sizeof(configBuff[0]), "%d", hotkey);
	WritePrivateProfileString("Autostocker", "Hotkey", configBuff, CONFIG_FILE);
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
		"Start_rares",
		StartRares,
		"Usage: Start_rares [sets] [rares] [uniques] [chat]"
	},
	{
		"Hotkey",
		SetHotkey,
		"Sets the hotkey to start autostocker with default settings (.as start)"
	},
	{NULL}
};


