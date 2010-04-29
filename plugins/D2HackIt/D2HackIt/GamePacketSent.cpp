//////////////////////////////////////////////////////////////////////
// GamePacketSent.cpp
// -------------------------------------------------------------------
//
// <thohell@home.se>
//////////////////////////////////////////////////////////////////////
#include "D2HackItServer.h"
#include "Server20.h"

//////////////////////////////////////////////////////////////////////
//	FormatString : Moved here from FormatString.cpp by Dan_Goon
//////////////////////////////////////////////////////////////////////
DWORD PRIVATE FormatString(LPSTR aString)
{
	DWORD aLength=strlen(aString);
	char* p;

	// Replace all occurances of $$ with ÿc
	while (strstr(aString, "$$"))
	{
		p=strstr(aString, "$$");
		p[0]='ÿ';
		p[1]='c';
	}

	return aLength;
}

//////////////////////////////////////////////////////////////////////
// GamePacketSent()
// -------------------------------------------------------------------
// Our 'clean' GamePacketSent()
//////////////////////////////////////////////////////////////////////
DWORD __fastcall GamePacketSentIntercept(BYTE* aPacket, DWORD aLength)
{
	// If packet 0x15 or 0x14, format for color codes
	if ((aPacket[0]==0x15) || (aPacket[0]==0x14))
		FormatString((char*)&aPacket[3]);

	// If packet 0x15 and it starts with *one* period, its a command line
	if (aPacket[0] == 0x15)
	{
		// si->PlayerSlot=aPacket[1];		// Why is this here ?? 2001-12-08 
		if ((aPacket[3] == psi->CommandCharacter) && (aPacket[4] != psi->CommandCharacter)) 
		{
			GameCommandLine((char*)&aPacket[4]);
			return 0;
		}
	}

	// Added v2.00 by Abin
	if (!Proc_OnGamePacketBeforeSent(aPacket, aLength))
		return 0;

	// Check if clients wants to snoop these too
	LinkedItem *li = ClientModules.GetFirstItem();
	while(li)
	{	
		PCIS pCIS = ClientModules.GetCIS(li);
		if(pCIS && pCIS->OnGamePacketBeforeSent)
		{
			if(!(aLength = pCIS->OnGamePacketBeforeSent(aPacket, aLength))) break;
		}
		li = ClientModules.GetNextItem(li);
	}

	return aLength;
}

//////////////////////////////////////////////////////////////////////
// GamePacketSentSTUB()
// -------------------------------------------------------------------
// esp+0x0c	pointer to packet
// esp+0x10	length
//////////////////////////////////////////////////////////////////////
void __declspec(naked) GamePacketSentInterceptSTUB()
{
	__asm {
		nop								// Make room for original code
		nop
		nop
		nop
		nop
		nop
		nop
		pushad
		mov ecx, dword ptr [esp+0x20+0x0c]	// ptr to packet
		mov edx, dword ptr [esp+0x20+0x10]	// length
		call GamePacketSentIntercept		// Call c function
		mov [esp+0x20+0x10], eax			// set my length
		popad
		ret
	}
}

//////////////////////////////////////////////////////////////////////
// BnetPacketSent()
// -------------------------------------------------------------------
// Our 'clean' BnetPacketSent()
//////////////////////////////////////////////////////////////////////
DWORD __fastcall BnetPacketSentIntercept(BYTE* aPacket, DWORD aLength, DWORD connStruct)
{
	return aLength;
}

//////////////////////////////////////////////////////////////////////
// BnetPacketSentInterceptSTUB()
// -------------------------------------------------------------------
void __declspec(naked) BnetPacketSentInterceptSTUB()
{
}
