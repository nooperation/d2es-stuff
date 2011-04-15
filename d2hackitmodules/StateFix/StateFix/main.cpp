#include "../../d2hackit/includes/ClientCore.cpp"


CLIENTINFO
(
	1,0,
	"State Fix v1.0",
	"",
	"State Fix v1.0",
	""
)

DWORD EXPORT OnGamePacketBeforeReceived(BYTE* aPacket, DWORD aLen)
{   
	// http://67.207.140.4/viewtopic.php?f=182&t=442140&start=0
	if (aPacket[0] == 0xAA)
	{
		*(DWORD*)(aPacket+aPacket[6]) = -1;
		aPacket[6] += 4;
	}

	return aLen;
}


MODULECOMMANDSTRUCT ModuleCommands[]=
{
	{
		"help",
			OnGameCommandHelp,
			"helpÿc0 List commands available in this module.\n"
			"<command> helpÿc0 Shows detailed help for <command> in this module."
	},
	{NULL}
};