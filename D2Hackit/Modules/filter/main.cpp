#include "../../Includes/ClientCore.cpp"
#include "ItemFilter.h"

ItemFilter ItemFilter;
bool enabled = true;

BOOL PRIVATE Enable(char** argv, int argc)
{
	server->GameInfof("Filtering Enabled");
	enabled = true;

	return TRUE;
}

BOOL PRIVATE Disable(char** argv, int argc)
{
	server->GameInfof("Filtering Disabled");
	enabled = false;

	return TRUE;
}

BOOL PRIVATE Reload(char** argv, int argc)
{
	if(!ItemFilter.LoadItems())
	{
		server->GameErrorf("Unable to reload files, settings restored to default values");
	}

	server->GamePrintInfo("Filter reloaded");

	return TRUE;
}

////////////////////////////////////////////
//
//               EXPORTS
//
/////////////////////////////////////////////


CLIENTINFO
(
	0,1,
	"",
	"",
	"",
	""
)

MODULECOMMANDSTRUCT ModuleCommands[]=
{
	{
		"help",
		OnGameCommandHelp,
		"helpÿc0 List commands available in this module.\n"
		"<command> helpÿc0 Shows detailed help for <command> in this module."
	},
	{
		"on",
		Enable,
		"Enables filtering"
	},
	{
		"off",
		Disable,
		"Diables filtering"
	},
	{
		"reload",
		Reload,
		"Reloads filter list and settings"
	},
	{NULL}
};

BOOL EXPORT OnClientStart()
{
	if(!ItemFilter.LoadItems())
	{
		server->GameErrorf("Failed to load all item tables.");
		return FALSE;
	}

	return TRUE;
}

DWORD EXPORT OnGamePacketBeforeReceived(BYTE* aPacket, DWORD aLen)
{   
	if (aPacket == nullptr)
	{
		return 0;
	}

	if(enabled && (aPacket[0] == 0x9c))
	{
		ITEM item;
		if(!server->ParseItem(aPacket, aLen, item))
			return aLen;

		if(aPacket[1] == ITEM_ACTION_NEW_GROUND || aPacket[1] == ITEM_ACTION_OLD_GROUND)
		{
			if(!ItemFilter.OnItemAction(item))
			{
				return 0;
			}
		}
	}

	return aLen;
}

