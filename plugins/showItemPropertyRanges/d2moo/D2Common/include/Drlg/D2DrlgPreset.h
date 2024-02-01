#pragma once

#include <cstdint>
#include "D2DrlgDrlg.h"
#include "D2DrlgDrlgGrid.h"
#include "../Path/Path.h"

struct D2LvlPrestTxt;

#pragma pack(1)

enum D2DrlgPresetRoomFlags
{
	DRLGPRESETROOMFLAG_NONE = 0,
	DRLGPRESETROOMFLAG_SINGLE_ROOM = 1 << 0,
	DRLGPRESETROOMFLAG_HAS_MAP_DS1 = 1 << 1, // needs confirmation
};

struct D2LevelFileListStrc
{
	char szPath[D2_MAX_PATH];			//0x00
	long nRefCount;						//0x104
	D2DrlgFileStrc* pFile;				//0x108
	D2LevelFileListStrc* pNext;				//0x10C
};

struct D2PresetUnitStrc
{
	int32_t nUnitType;						//0x00
	int32_t nIndex;							//0x04 see D2Common.#11278
	int32_t nMode;							//0x08
	int32_t nXpos;							//0x0C
	int32_t nYpos;							//0x10
	int32_t bSpawned;							//0x14 
	D2MapAIStrc* pMapAI;					//0x18
	D2PresetUnitStrc* pNext;				//0x1C
};

struct D2DrlgMapStrc
{
	int32_t nLevelPrest;					//0x00
	int32_t nPickedFile;					//0x04
	D2LvlPrestTxt* pLvlPrestTxtRecord;		//0x08
	D2DrlgFileStrc* pFile;					//0x0C
	D2DrlgCoordStrc pDrlgCoord;				//0x10
	int32_t bHasInfo;							//0x20
	D2DrlgGridStrc pMapGrid;				//0x24
	D2PresetUnitStrc* pPresetUnit;			//0x38
	int32_t bInited;							//0x3C
	int32_t nPops;							//0x40
	int32_t* pPopsIndex;					//0x44
	int32_t* pPopsSubIndex;					//0x48
	int32_t* pPopsOrientation;				//0x4C
	D2DrlgCoordStrc* pPopsLocation;			//0x50
	D2DrlgMapStrc* pNext;					//0x54
};

struct D2DrlgPresetRoomStrc
{
	int32_t nLevelPrest;						//0x00
	int32_t nPickedFile;						//0x04
	D2DrlgMapStrc* pMap;					//0x08
	union
	{
		struct
		{
			uint8_t nFlags;					//0x0C
			uint8_t unk0x0D[3];				//0x0D
		};
		uint32_t dwFlags;						//0x0C
	};
	D2DrlgGridStrc pWallGrid[4];			//0x10
	D2DrlgGridStrc pTileTypeGrid[4];		//0x60 aka pOrientationGrid
	D2DrlgGridStrc pFloorGrid[2];			//0xB0
	D2DrlgGridStrc pCellGrid;				//0xD8
	D2DrlgGridStrc* pMazeGrid;				//0xEC
	D2CoordStrc* pTombStoneTiles;			//0xF0
	int32_t nTombStoneTiles;					//0xF4
};

#pragma pack()

