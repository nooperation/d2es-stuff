///////////////////////////////////////////////////////////
// npc.h
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////
#ifndef __NPC_H__
#define __NPC_H__

#include <windows.h>

struct NPC_DATA
{
	DWORD dwClassID; // class id
	int nMenuIndex; // npc menu item index for trading and gambling
};

// Vendors
static const NPC_DATA VENDORS[] = { {154, 1}, // Charis
									{147, 1}, // Gheed
									{148, 1}, // Akara
									{199, 1}, // Elzix
									{292, 1}, // lysander
									{178, 1}, // fara
									{177, 1}, // drognan
									{254, 1}, // alkor
									{252, 2}, // asheara
									{255, 1}, // ormus
									{253, 1}, // hratil
									{405, 0}, // jamella
									{257, 0}, // halbu
									{511, 1}, // larzuk
									{513, 1}, // malah
									{512, 1}, // anya
									{0, -1}};

// Gamblers
static const NPC_DATA GAMBLERS[] = { {147, 2}, // Gheed
									{199, 2}, // Elzix
									{254, 2}, // alkor
									{405, 1}, // jamella
									{512, 2}, // anya
									{514, 1}, // Nihlathak									
									{0, -1}};

// Resurrectors
static const DWORD RESURRECTORS[] = { 150, // Kashya
									198, // Greiz
									252, // asheara
									367, // tyrael
									515, // qual-khek
									0};

int GetNpcTradeMenuID(DWORD dwClassID);
int GetNpcGambleMenuID(DWORD dwClassID);
BOOL IsResurrector(DWORD dwClassID);

#endif // __NPC_H__