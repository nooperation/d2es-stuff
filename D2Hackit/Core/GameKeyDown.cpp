//////////////////////////////////////////////////////////////////
// GameKeyDown.cpp
//
// From Thohell's Bind.cpp.
//
// Added by Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////
#include "D2HackItServer.h"

/////////////////////////////////////////////////////////////////////////////
// GameKeyDownIntercept()
// --------------------------------------------------------------------------
// Our 'clean' GameKeyDown() 
/////////////////////////////////////////////////////////////////////////////
DWORD __fastcall GameKeyDownIntercept(DWORD aKeyCode)
{
	DWORD dwRet = aKeyCode;

	LinkedItem *li = ClientModules.GetFirstItem();
	while(li)
	{	
		PCIS pCIS = ClientModules.GetCIS(li);
		if(pCIS && pCIS->OnGameKeyDown)
		{
			DWORD dwCode = pCIS->OnGameKeyDown((BYTE)aKeyCode);
			if (dwRet && dwCode != aKeyCode)
				dwRet = dwCode;
		}
		li = ClientModules.GetNextItem(li);
	}
	
	// the "esc" key must not be overridden
	return aKeyCode == VK_ESCAPE ? aKeyCode : dwRet;
}

/////////////////////////////////////////////////////////////////////////////
// GameKeyDownInterceptSTUB()
// --------------------------------------------------------------------------
// esi = Keycode
/////////////////////////////////////////////////////////////////////////////

void __declspec(naked) GameKeyDownInterceptSTUB()
{
	__asm
	{
		nop								// Make room for original code
		nop
		nop
		nop
		nop
		nop
		pushad
		mov ecx, esi
		call GameKeyDownIntercept
		// Shuffle... This is ugly!
		push eax						// Push return code
		add esp, 4						// Move stack
		popad							// Restore registers
		sub esp, 0x24					// move stack
		pop esi							// Pop return code
		add esp, 0x20					// Move stack
		ret
	}
}

