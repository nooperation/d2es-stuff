#include "../../Includes/ClientCore.cpp"
#include "BuffMe.h"

BuffMe buff;
bool hotkeyEnabled = true;

int tickRate = 100;
int tickCount = 0;
bool isShowingSelectedSkill = false;

CLIENTINFO
(
	1,0,
	"",
	"",
	"buffMe",
	""
)


BOOL PRIVATE Buff(char** argv, int argc)
{
	buff.Start();
	
	return TRUE;
}

BOOL PRIVATE ID(char** argv, int argc)
{
	isShowingSelectedSkill = !isShowingSelectedSkill;
	server->GameStringf("Show skill id %s", isShowingSelectedSkill ? "Enabled" : "Disabled");
	
	return TRUE;
}


BOOL PRIVATE ToggleHotkey(char** argv, int argc)
{
	hotkeyEnabled = !hotkeyEnabled;

	if(hotkeyEnabled)
	{
		server->GameStringf("Hotkey enabled");
	}
	else
	{
		server->GameStringf("Hotkey disabled");
	}
	
	return TRUE;
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
		"Buff",
		Buff,
		"Buff"
	},
	{
		"show",
		ID,
		"shows id of selected skill"
	},
	{
		"id",
		ID,
		"shows id of selected skill"
	},
	{
		"hotkey",
		ToggleHotkey,
		"Toggles hotkey (+ on keypad)"
	},
	{NULL}
};

BYTE EXPORT OnGameKeyDown(BYTE iKeyCode)
{
	if(iKeyCode == VK_DELETE)
	{
		tickCount = 0;
		buff.Start();
	}

	return iKeyCode;
}

DWORD EXPORT OnGameTimerTick()
{
	if(tickCount++ >= tickRate)
	{
		buff.OnTick();
		tickCount = 0;
	}
	return 0;
}

VOID EXPORT OnThisPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	if(nMessage == PM_AFFECT)
	{
		if(isShowingSelectedSkill)
		{
			server->GameStringf("On affect: %X", lParam);
		}
		buff.OnAffect(lParam);
		tickCount = 0;
	}
	else if(nMessage == PM_DISAFFECT)
	{
		if(isShowingSelectedSkill)
		{
			server->GameStringf("On disaffect: %X", lParam);
		}
		buff.OnDisAffect(lParam);
	}
	else if(isShowingSelectedSkill && nMessage == PM_SPELLSELECT)
	{
		// shock armor = 0x28
		server->GameStringf("Selected: %02X", lParam);
	}
}