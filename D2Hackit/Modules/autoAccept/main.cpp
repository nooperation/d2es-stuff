#include "../../Includes/ClientCore.cpp"

bool autoAccept = true;

BOOL PRIVATE Toggle(char** argv, int argc)
{
	autoAccept = !autoAccept;

	if(autoAccept)
	{
		server->GameInfof("AutoAccept enabled");
	}
	else
	{
		server->GameInfof("AutoAccept disabled");
	}

	return TRUE;
}

VOID EXPORT OnThisPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	switch(nMessage)
	{
		case PM_INVITATION:
			BYTE acceptInvitationPacket[6];

			acceptInvitationPacket[0] = 0x5e;
			acceptInvitationPacket[1] = 0x08;
			*((DWORD *)(acceptInvitationPacket+2)) = wParam;

			server->GameSendPacketToServer(acceptInvitationPacket, 6);
			break;
	}
}


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
		"toggle",
		Toggle,
		"Toggles autoaccept"
	},
	{NULL}
};