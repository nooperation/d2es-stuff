#include "../../Includes/ClientCore.cpp"

#include "ammoguard.h"

#define CONFIG_FILE ".\\plugin\\ammoGuard.ini"

AmmoGuard ammoGuard;
bool settingHotkey = false;
int hotkey = 0;

BOOL PRIVATE ReloadAmmo(char** argv, int argc);
BOOL PRIVATE SetHotkey(char** argv, int argc);
void ReadConfig();
void WriteConfig();

CLIENTINFO
(
	2,2,
	"Ammo Guard v2.2",
	"",
	"Ammo Guard v2.2",
	""
)

void ReadConfig()
{
	hotkey = GetPrivateProfileInt("Ammoguard", "Hotkey", 0, CONFIG_FILE);
}

void WriteConfig()
{
	char configBuff[64];

	sprintf_s(configBuff, sizeof(configBuff)/sizeof(configBuff[0]), "%d", hotkey);
	WritePrivateProfileString("Ammoguard", "Hotkey", configBuff, CONFIG_FILE);
}

BOOL PRIVATE SetHotkey(char** argv, int argc)
{
	settingHotkey = true;

	server->GameStringf("ÿc5AmmoGuardÿc0: Next key pressed will be the hotkey...");

	return TRUE;
}

BOOL PRIVATE ReloadAmmo(char** argv, int argc)
{
	ammoGuard.ReloadAmmo();
	return TRUE;
}

VOID EXPORT OnGameJoin(THISGAMESTRUCT* thisgame)
{
	ReadConfig();
}

BYTE EXPORT OnGameKeyDown(BYTE iKeyCode)
{
	if(settingHotkey)
	{
		settingHotkey = false;

		if(iKeyCode == 0 || iKeyCode == VK_ESCAPE || iKeyCode == VK_RETURN)
		{
			server->GameStringf("ÿc:AmmoGuardÿc0: Unbound hotkey");
			hotkey = 0;
		}
		else
		{
			hotkey = iKeyCode;
			server->GameStringf("ÿc:AmmoGuardÿc0: Set hotkey to %d", iKeyCode);
		}

		WriteConfig();
	}
	else
	{
		if(iKeyCode == hotkey)
		{
			ReloadAmmo(NULL, 0);
		}
	}

	switch(iKeyCode)
	{
		case VK_SPACE:
			ammoGuard.Abort();
			break;
	}

	return iKeyCode;
}

VOID EXPORT OnUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam, LPARAM lParam)
{
	if(nMessage == UM_ITEMEVENT)
	{
		ITEM *item = (ITEM *)lParam;

		switch(wParam)
		{
			case ITEM_ACTION_FROM_EQUIP:
			case ITEM_ACTION_FROM_STORAGE:
			case ITEM_ACTION_TO_CURSOR:
				ammoGuard.OnItemToCursor(item->dwItemID);
				break;
			case ITEM_ACTION_TO_EQUIP:
				ammoGuard.OnItemToEquip(item->dwItemID);
				break;
			case ITEM_ACTION_TO_STORAGE:
				if(item->iStorageID == 0x04)
				{
					ammoGuard.OnItemToCube(item->dwItemID);
				}
				break;
		}		
	}
}

DWORD EXPORT OnGameTimerTick()
{
	ammoGuard.Tick();

	return 0;
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
		"ReloadAmmo",
		ReloadAmmo,
		"Cubes your ammo.\n",
	},
	{
		"Hotkey",
		SetHotkey,
		"Sets the hotkey to reload ammo"
	},
	{NULL}
};