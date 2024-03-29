#include "../../Includes/ClientCore.cpp"
#include "AutoStocker.h"

#define CONFIG_FILE ".\\plugin\\autostockerMisc.ini"

void WriteConfig();
void ReadConfig();

AutoStocker autoStocker;

bool settingHotkey = false;
int hotkey = 0;
bool hotkeyIncludesRares = false;
bool hotkeyIncludesSets = false;
bool hotkeyIncludesUniques = false;

CLIENTINFO
(
	1,7,
	"Auto Stocker v1.7",
	"",
	"Auto Stocker v1.7",
	""
)

BOOL PRIVATE Start(char** argv, int argc)
{
	bool useChat = false;
	std::unordered_set<std::string> ignoredItemCodes;

	if(argc >= 3)
	{
		for(int i = 2; i < argc; i++)
		{
			if (_stricmp(argv[i], "chat") == 0)
			{
				useChat = true;
			}
			else if (argv[i][0] == '-' && strlen(argv[i]) == 4)
			{
				auto itemCode = &argv[i][1];
				ignoredItemCodes.insert(itemCode);
			}
		}
	}

	autoStocker.Start(useChat, ignoredItemCodes);

	return TRUE;
}


BOOL PRIVATE StartRares(char** argv, int argc)
{
	bool useChat = false;
	bool uniques = false;
	bool rares = false;
	bool sets = false;
	std::unordered_set<std::string> ignoredItemCodes;

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
		else if (argv[i][0] == '-' && strlen(argv[i]) == 4)
		{
			auto itemCode = &argv[i][1];
			ignoredItemCodes.insert(itemCode);
		}
	}

	autoStocker.StartRares(sets, rares, uniques, useChat, ignoredItemCodes);

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

DWORD EXPORT OnGamePacketBeforeSent(BYTE* aPacket, DWORD aLen)
{
	if(aPacket[0] == 0x15 && aPacket[1] == 0x01)
	{
		char *chatMessage = (char *)(aPacket+3);

		if(strncmp(chatMessage, "�c:AutoAncientScroll�c0:", 24) == 0)
		{
			const auto message = std::string_view(chatMessage + 25);
			if (!autoStocker.OnAutoAncientScrollMessage(message))
			{
				return aLen;
			}
			else
			{
				return 0;
			}
		}
		else if(strncmp(chatMessage, "�c5AutoOre�c0:", 14) == 0)
		{
			const auto message = std::string_view(chatMessage + 15);
			if (!autoStocker.OnAutoOreMessage(message))
			{
				return aLen;
			}
			else
			{
				return 0;
			}
		}
	}

	return aLen;
}

BOOL PRIVATE SetHotkey(char** argv, int argc)
{
	settingHotkey = true;

	server->GameStringf("�c:Autostocker�c0: Next key pressed will be the hotkey...");

	return TRUE;
}


BYTE EXPORT OnGameKeyDown(BYTE iKeyCode)
{
	if(settingHotkey)
	{
		settingHotkey = false;

		if(iKeyCode == 0 || iKeyCode == VK_ESCAPE || iKeyCode == VK_RETURN)
		{
			server->GameStringf("�c:Autostocker�c0: Unbound hotkey");
			hotkey = 0;
		}
		else
		{
			hotkey = iKeyCode;
			server->GameStringf("�c:Autostocker�c0: Set hotkey to %d", iKeyCode);
		}

		WriteConfig();
	}
	else
	{
		if(iKeyCode == hotkey)
		{
			std::unordered_set<std::string> ignoredItemCodes;
			autoStocker.StartRares(hotkeyIncludesSets, hotkeyIncludesRares, hotkeyIncludesUniques, false, ignoredItemCodes);
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
	hotkeyIncludesRares = GetPrivateProfileInt("Autostocker", "HotkeyIncludesRares", 0, CONFIG_FILE);
	hotkeyIncludesSets = GetPrivateProfileInt("Autostocker", "HotkeyIncludesSets", 0, CONFIG_FILE);
	hotkeyIncludesUniques = GetPrivateProfileInt("Autostocker", "HotkeyIncludesUniques", 0, CONFIG_FILE);
}

void WriteConfig()
{
	char configBuff[64];

	sprintf_s(&configBuff[0], sizeof(configBuff) / sizeof(configBuff[0]), "%d", hotkey);
	WritePrivateProfileString("Autostocker", "Hotkey", &configBuff[0], CONFIG_FILE);

	sprintf_s(&configBuff[0], sizeof(configBuff)/sizeof(configBuff[0]), "%d", hotkeyIncludesRares ? 1 : 0);
	WritePrivateProfileString("Autostocker", "HotkeyIncludesRares", &configBuff[0], CONFIG_FILE);

	sprintf_s(&configBuff[0], sizeof(configBuff)/sizeof(configBuff[0]), "%d", hotkeyIncludesSets ? 1 : 0);
	WritePrivateProfileString("Autostocker", "HotkeyIncludesSets", &configBuff[0], CONFIG_FILE);

	sprintf_s(&configBuff[0], sizeof(configBuff)/sizeof(configBuff[0]), "%d", hotkeyIncludesUniques ? 1 : 0);
	WritePrivateProfileString("Autostocker", "HotkeyIncludesUniques", &configBuff[0], CONFIG_FILE);
}

MODULECOMMANDSTRUCT ModuleCommands[]=
{
	{
		"help",
		OnGameCommandHelp,
		"help�c0 List commands available in this module.\n"
		"<command> help�c0 Shows detailed help for <command> in this module."
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


