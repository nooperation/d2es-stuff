//////////////////////////////////////////////////////////////////////
// GameSendPacketToServer.cpp
// -------------------------------------------------------------------
//
// <thohell@home.se>
//////////////////////////////////////////////////////////////////////
#include "D2HackItServer.h"


//////////////////////////////////////////////////////////////////////
// GameSendPacketToServer
// -------------------------------------------------------------------
// Helper function to send a packet of data to the server.
//
// 0.21: This now works perfectly ok even in single player as we're 
//       no longer sending to a socket, but to the game itself.
//////////////////////////////////////////////////////////////////////
DWORD EXPORT GameSendPacketToServer(LPBYTE buf, DWORD len)
{
	DWORD p = psi->fps.GamePacketSentIntercept.AddressFound;
	__asm
	{
		push len
		push buf
		push 0
		call p
	}
	return len;
}

DWORD EXPORT BnetSendPacketToServer(LPBYTE buf, DWORD len)
{
	if(!psi->bSupportBnet)
		return 0;
	return len;
}

DWORD EXPORT RealmSendPacketToServer(LPBYTE buf, DWORD len)
{
	if(!psi->bSupportBnet)
		return 0;
	return len;
}

//////////////////////////////////////////////////////////////////////
// GameSendMessageToChat
// -------------------------------------------------------------------
// Sends text message to chat channel (temporary until raw packet
// sender implemented)
//////////////////////////////////////////////////////////////////////
BOOL EXPORT GameSendMessageToChat(LPSTR msg)
{
	DWORD p = psi->GameSendMessageToChatLocation;
	DWORD s = (DWORD)msg;
	if(p)
	{
		_asm
		{
			mov  ecx, s
			call p
		}
	}
	return !!p;
}
