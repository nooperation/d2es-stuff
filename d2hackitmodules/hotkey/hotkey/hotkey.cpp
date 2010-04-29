#include "../../d2hackit/includes/ClientCore.cpp"

#define CONFIG_FILE ".\\plugin\\hotkeys.ini"

bool settingDropToCubeHotkey = 0;
bool settingTransmuteHotkey = 0;
int dropToCubeHotkey = 0;
int transmuteHotkey = 0;

BOOL PRIVATE SetDropToCubeHotkey(char** argv, int argc);
BOOL PRIVATE SetTransmuteHotkey(char** argv, int argc);
void ReadConfig();
void WriteConfig();

CLIENTINFO
(
	0,1,
	"",
	"",
	"Hotkey",
	""
)

void ReadConfig()
{
	dropToCubeHotkey = GetPrivateProfileInt("Hotkeys", "DropToCube", 0, CONFIG_FILE);
	transmuteHotkey = GetPrivateProfileInt("Hotkeys", "Transmute", 0, CONFIG_FILE);
}

void WriteConfig()
{
	char configBuff[64];

	sprintf_s(configBuff, sizeof(configBuff)/sizeof(configBuff[0]), "%d", dropToCubeHotkey);
	WritePrivateProfileString("Hotkeys", "DropToCube", configBuff, CONFIG_FILE);

	sprintf_s(configBuff, sizeof(configBuff)/sizeof(configBuff[0]), "%d", transmuteHotkey);
	WritePrivateProfileString("Hotkeys", "Transmute", configBuff, CONFIG_FILE);
}

BOOL PRIVATE SetDropToCubeHotkey(char** argv, int argc)
{
	settingDropToCubeHotkey = true;

	server->GameStringf("ÿc5Transmuteÿc0: Next key pressed will be the drop to cube hotkey...");

	return TRUE;
}

BOOL PRIVATE SetTransmuteHotkey(char** argv, int argc)
{
	settingTransmuteHotkey = true;

	server->GameStringf("ÿc5Transmuteÿc0: Next key pressed will be the transmute hotkey...");

	return TRUE;
}

VOID EXPORT OnGameJoin(THISGAMESTRUCT* thisgame)
{
	ReadConfig();
}

BYTE EXPORT OnGameKeyDown(BYTE iKeyCode)
{
	if(settingDropToCubeHotkey || settingTransmuteHotkey)
	{
		
		int hotkey = 0;

		if(iKeyCode == 0 || iKeyCode == VK_ESCAPE || iKeyCode == VK_RETURN)
		{
			server->GameStringf("ÿc:Hotkeyÿc0: Unbound hotkey");
			hotkey = 0;
		}
		else
		{
			hotkey = iKeyCode;
			server->GameStringf("ÿc:Hotkeyÿc0: Hotkey set to %d", iKeyCode);
		}

		if(settingDropToCubeHotkey)
		{
			dropToCubeHotkey = hotkey;
		}
		if(settingTransmuteHotkey)
		{
			transmuteHotkey = hotkey;
		}

		settingDropToCubeHotkey = false;
		settingTransmuteHotkey = false;

		WriteConfig();
	}
	else
	{
		if(iKeyCode == transmuteHotkey)
		{
			me->DropCursorItemToStorage(STORAGE_CUBE);
		}
		if(iKeyCode == dropToCubeHotkey)
		{
			me->Transmute();
		}
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
		"SetDropToCube",
		SetDropToCubeHotkey,
		"Sets the hotkey to drop item to cube"
	},
	{
		"SetTransmute",
		SetTransmuteHotkey,
		"Sets the hotkey to transmute"
	},
	{NULL}
};