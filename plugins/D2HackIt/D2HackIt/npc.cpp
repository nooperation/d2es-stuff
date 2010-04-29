///////////////////////////////////////////////////////////
// npc.cpp
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////
#include "npc.h"

int GetNpcTradeMenuID(DWORD dwClassID)
{
	for (int i = 0; VENDORS[i].dwClassID; i++)
	{
		if (VENDORS[i].dwClassID == dwClassID)
			return VENDORS[i].nMenuIndex;
	}

	return -1;
}

int GetNpcGambleMenuID(DWORD dwClassID)
{
	for (int i = 0; GAMBLERS[i].dwClassID; i++)
	{
		if (GAMBLERS[i].dwClassID == dwClassID)
			return GAMBLERS[i].nMenuIndex;
	}

	return -1;
}

BOOL IsResurrector(DWORD dwClassID)
{
	for (int i = 0; RESURRECTORS[i]; i++)
	{
		if (RESURRECTORS[i] == dwClassID)
			return TRUE;
	}

	return FALSE;
}