//////////////////////////////////////////////////////////////////////
// GamePlayerInfoIntercept.cpp
// -------------------------------------------------------------------
//
// <thohell@home.se>
//////////////////////////////////////////////////////////////////////
#include "D2HackItServer.h"
#include "Server20.h"


//////////////////////////////////////////////////////////////////////
// GamePlayerInfoIntercept
// -------------------------------------------------------------------
// Our clean function. Updates the *thisgame struct and tells the
// loaded modules that we just entered/left a game
//////////////////////////////////////////////////////////////////////
VOID __fastcall GamePlayerInfoIntercept(PLAYERINFOSTRUCT* pis)
{
	LinkedItem *li = ClientModules.GetFirstItem();
	PCIS pCIS;

	// We're joining a game
	if((pis) && (!thisgame->player))
	{
		SHOW_REGS("GamePlayerInfoIntercept");
		thisgame->player = pis;

		// Added v2.00, by Abin
		Proc_OnGameJoin(thisgame);
		
		while(li)
		{	
			pCIS = ClientModules.GetCIS(li);
			if(pCIS && pCIS->OnGameJoin)
			{
				pCIS->OnGameJoin(thisgame);
			}
				li = ClientModules.GetNextItem(li);
		}
	}

	// We're leaving a game
	else if((!pis) && (thisgame->player))
	{
		// Added v2.00, by Abin
		Proc_OnGameLeave();

		while(li)
		{	
			pCIS = ClientModules.GetCIS(li);
			if(pCIS && pCIS->OnGameLeave)
			{
				pCIS->OnGameLeave(thisgame);
			}
			li = ClientModules.GetNextItem(li);
		}

		// set to NULL after callng all client's OnGameLeave (0.73)
		thisgame->player = NULL;
	}
}


//////////////////////////////////////////////////////////////////////
// GamePlayerInfoInterceptSTUB
// -------------------------------------------------------------------
// esi = pointer to PLAYERINFOSTRUCT or NULL
//////////////////////////////////////////////////////////////////////
VOID __declspec(naked) GamePlayerInfoInterceptSTUB()
{
	__asm {
		nop
		nop
		nop
		nop
		nop
		nop
		pushad
		mov ecx, esi
		call GamePlayerInfoIntercept
		popad
		ret
	}
}

