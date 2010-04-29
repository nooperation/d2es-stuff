//////////////////////////////////////////////////////////////////////
// TickThread.cpp
// -------------------------------------------------------------------
// Tick that calls modules approximatly every 1/10th of a second. 
// Great for implementing timers in modules.
//
// <thohell@home.se>
//////////////////////////////////////////////////////////////////////
#include "D2HackItServer.h"
#include "Server20.h"

DWORD WINAPI TickThread(LPVOID lpParameter)
{
	CClients* cl = (CClients*)lpParameter;

	psi->TickThreadActive = TRUE;

	while(psi->TickThreadActive)
	{
		if (IsGameJoined())
		{
			/////////////////////////////////////////////////////
			// Added v2.00 by Abin
			//
			// It had been a big bug of D2Hackit that OnGameTimerTick
			// would be called even if the player was not in a game.
			// This will fix the problem, from 2.00 and on.
			//////////////////////////////////////////////////////
			Proc_OnGameTimerTick();

			LinkedItem *li = cl->GetFirstItem();
			while(li)
			{
				PCIS pCIS = cl->GetCIS(li);
				if(pCIS && pCIS->OnGameTimerTick) pCIS->OnGameTimerTick();
				li = cl->GetNextItem(li);
			}
		}

		Sleep(100);
	}

	psi->TickShutDown = -1;
	// Dummy!
	return 0;
}

