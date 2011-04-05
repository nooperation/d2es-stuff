#ifndef _EmptyCube_H_
#define _EmptyCube_H_

#include <vector>
#include <windows.h>

enum States
{
	STATE_UNINITIALIZED = 0,
	STATE_COMPLETE,
	STATE_STARTCLEANING,
	STATE_NEXTCUBEITEM,
	STATE_PICKUPCUBEITEM,
	STATE_CUBEITEMTOINVENTORY,
	STATE_WAITINGFORNEXTSTATE,
};

class EmptyCube
{
	public:
		EmptyCube();
		bool Init(bool useChat);
		bool Start(bool useChat);
		bool StartCleaning();

		void OnItemToInventory(const ITEM &item);
		void OnItemFromCube(DWORD itemID);
		void OnTick();
		void Abort();		

	private:
		void StartStocking();
		void ProcessNextCubeItem();

		bool useChat;
		int currentItemToMove;

		std::vector<DWORD> itemsInCube;

		States currentState;
};

BOOL CALLBACK enumFindCubeItems(LPCITEM item, LPARAM lParam);

#endif
