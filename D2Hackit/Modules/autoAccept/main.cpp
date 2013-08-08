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


DWORD EXPORT OnGamePacketBeforeReceived(BYTE* aPacket, DWORD aLen)
{   


	if(aPacket[0] == 0x5b)
	{
		// Player info packet
		//5B 24 00 XX XX XX XX 01 (Char Name) 00 01 00 FF FF 00 00 00 00 00 00 00 00
		//(WORD) Packet Lengh
		//(DWORD) Player's ID
		//(BYTE) Player's Class
		//(BYTE[16]) Character's Name
		//(WORD) Character's Level
		//(WORD) Party Number (0xFFFF = none)
		//(VOID) Null-Padding

		unsigned short length = *((unsigned short*)(aPacket+1));
		unsigned int playerId = *((unsigned int*)(aPacket+3));
		unsigned char playerClass = *((unsigned char*)(aPacket+7));
		char *playerName = (char *)(aPacket+8);
		unsigned short playerLevel = *((unsigned short*)(aPacket + 24));
		unsigned short playerPartyId = *((unsigned short*)(aPacket + 26));

		// Skip ourselves
		if(playerId == me->GetID())
		{
			return aLen;
		}

		// 0xffff indicates player is not in a party, ignore players already in a party
		if(playerPartyId != 0xffff)
		{
			return aLen;
		}
		
		//Party Request
		//5E 08 3E 82 EB 2B
		//(BYTE) Action type
		//(DWORD) Player ID (Of The Other person)

		// Action type:
		//0x06 = Invite A Player To Party With You
		//0x07 = Cancel A Party Invite To Another Player
		//0x08 = Accept A Party Invite From Another Player

		unsigned char invitePacket[6] = {0x5e, 0x06};
		*((unsigned int *)&invitePacket[2]) = playerId;

		server->GameInfof("Inviting %s...", playerName);
		server->GameSendPacketToServer(invitePacket, sizeof(invitePacket));
	}



	//server->GamePrintPacket(TRUE, aPacket, aLen);

	return aLen;
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