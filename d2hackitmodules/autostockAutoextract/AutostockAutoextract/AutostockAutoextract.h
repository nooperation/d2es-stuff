#ifndef _AutostockAutoextract_H_
#define _AutostockAutoextract_H_

#include <windows.h>

enum States
{
	STATE_UNINITIALIZED = 0,
	STATE_COMPLETE,
	STATE_RUNAUTOSTOCKER,
	STATE_RUNAUTOEXTRACTOR,
	STATE_RUNEMPTYCUBE,
	STATE_FINISHAUTOSTOCKER,
	STATE_FINISHAUTOEXTRACTOR,
	STATE_FINISHEMPTYCUBE,
	STATE_NEXTEXTRACTORSTUFF,
	STATE_PICKUPEXTRACTORSTUFF,
	STATE_EXTRACTORSTUFFTOCUBE,
	STATE_WAITINGFORNEXTSTATE,
};

enum TransmuteStages
{
	TRANSMUTE_GEM = 0,
	TRANSMUTE_RUNEA,
	TRANSMUTE_RUNEB,
	TRANSMUTE_DECAL,
	TRANSMUTE_MULTISTOCKER,
	TRANSMUTE_TOMEA,
	TRANSMUTE_TOMEB,
	TRANSMUTE_CRYSTAL,
	TRANSMUTE_REROLL,
	TRANSMUTE_END
};

struct ExtractorStuff
{
	char restockerCode[4];
	DWORD restockerID;
	bool movedRestocker;
	char extractorCode[4];
	DWORD extractorID;
	bool movedExtractor;
};

class AutostockAutoextract
{
	public:
		AutostockAutoextract();
		bool Init(bool useChat);
		bool Start(bool transmuteSet, bool transmuteRare, bool transmuteUnique, bool useChat);
		bool StartAsAe();

		bool OnAutostockerEnded();
		bool OnAutoExtractorEnded();
		bool OnEmptyCubeEnded();
		void OnItemFromInventory(ITEM &item);
		void OnItemToCube(ITEM &item);
		void OnItemToInventory(ITEM &item);

		void OnTick();
		void Abort();		

	private:
		void StartStocking();
		bool useChat;
		bool transmuteRare;
		bool transmuteSet;
		bool transmuteUnique;
		bool autoExtractorLoaded;
		bool autoStockerLoaded;
		bool emptyCubeLoaded;

		ExtractorStuff extractorStuff;
		States currentState;
};

bool GetStockerType(DWORD itemId, int *stockerType);
BOOL CALLBACK enumFindCubeItems(LPCITEM item, LPARAM lParam);

#endif
