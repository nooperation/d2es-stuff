#pragma once

#include <cstdint>
#include "D2DrlgDrlg.h"
#include "D2DrlgDrlgGrid.h"
#include "../DataTbls/LevelsTbls.h"


#pragma pack(1)


struct D2UnkOutdoorStrc
{
	D2DrlgLevelStrc* pLevel;			//0x00
	int32_t* field_4;					//0x04
	D2DrlgGridStrc* pGrid1;				//0x08
	D2DrlgGridStrc* pGrid2;				//0x0C
	int32_t nLevelPrestId;				//0x10
	int32_t field_14;					//0x14
	int32_t nLvlSubId;					//0x18
};

struct D2UnkOutdoorStrc2
{
	D2DrlgRoomStrc* pDrlgRoom;									//0x00
	D2DrlgOutdoorRoomStrc* pOutdoorRooms[DRLG_MAX_WALL_LAYERS];	//0x04
	D2DrlgGridStrc* pWallsGrids[DRLG_MAX_WALL_LAYERS];			//0x14
	D2DrlgGridStrc* pFloorGrid;									//0x24
	int32_t field_28;											//0x28
	int32_t field_2C;											//0x28
	int32_t nSubWaypoint_Shrine;								//0x2C
	int32_t nSubTheme;											//0x30
	int32_t nSubThemePicked;									//0x34
};

struct D2UnkOutdoorStrc3
{
	int32_t nLevelId;						//0x00
	int32_t nExcludedLevel1;				//0x04
	int32_t nExcludedLevel2;				//0x08
	int32_t nRand;							//0x0C
	int32_t nNextRand;						//0x10
	int32_t nFlags;							//0x14
};

struct D2DrlgSubstGroupStrc
{
	D2DrlgCoordStrc tBox;
	int32_t field_10;
	int32_t field_14;
};


#pragma pack()

