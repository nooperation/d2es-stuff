#pragma once

#include <cstdint>
#include "D2DrlgDrlg.h"
#include "D2DrlgDrlgGrid.h"

#pragma pack(1)

struct D2UnkDrlgLogicStrc
{
	D2DrlgRoomStrc* pDrlgRoom;				//0x00
	D2DrlgGridStrc* field_4;				//0x04
	D2DrlgGridStrc* pTileTypeGrid;			//0x08
	D2DrlgGridStrc* pWallGrid;				//0x0C
	D2DrlgGridStrc* pFloorGrid;				//0x10
	D2DrlgGridStrc* field_14;				//0x14
	int32_t field_18;						//0x18
	int32_t nFlags;							//0x1C
};

enum D2DrlgLogicalRoomInfoFlags
{
	DRLGLOGIC_ROOMINFO_HAS_COORD_LIST = 0x1,
	DRLGLOGIC_ROOMINFO_HAS_GRID_CELLS = 0x2,
};

struct D2DrlgLogicalRoomInfoStrc // aka D2DrlgCoordListStrc
{
	int32_t dwFlags;						//0x00 D2DrlgLogicalRoomInfoFlags
	int32_t nLists;							//0x04
	D2DrlgGridStrc pIndexX;					//0x08
	D2DrlgGridStrc pIndexY;					//0x1C
	D2RoomCoordListStrc* pCoordList;		//0x30
};

#pragma pack()
