#pragma once

#include <cstdint>
#include "D2DrlgDrlgGrid.h"

struct D2DrlgRoomStrc;
struct D2DrlgLevelLinkDataStrc;

#pragma pack(1)


struct D2DrlgOutdoorRoomStrc
{
	D2DrlgGridStrc pTileTypeGrid;			//0x00 aka pOrientationGrid
	D2DrlgGridStrc pWallGrid;				//0x14
	D2DrlgGridStrc pFloorGrid;				//0x28
	D2DrlgGridStrc pDirtPathGrid;			//0x3C
	D2DrlgVertexStrc* pVertex;				//0x50
	int32_t dwFlags;						//0x54
	int32_t dwFlagsEx;						//0x58
	int32_t unk0x5C;						//0x5C
	int32_t unk0x60;						//0x60
	int32_t nSubType;						//0x64
	int32_t nSubTheme;						//0x68
	int32_t nSubThemePicked;				//0x6C
};

#pragma pack()
