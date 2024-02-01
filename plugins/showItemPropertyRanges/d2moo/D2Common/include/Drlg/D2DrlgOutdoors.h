#pragma once

#include <cstdint>
#include "D2DrlgDrlg.h"
#include "D2DrlgDrlgGrid.h"


#pragma pack(1)

enum D2C_OutDoorInfoFlags 
{
	OUTDOOR_FLAG1 = 0x00000001,
	OUTDOOR_BRIDGE = 0x00000004,
	OUTDOOR_RIVER_OTHER = 0x00000008,
	OUTDOOR_RIVER = 0x00000010,
	OUTDOOR_CLIFFS = 0x00000020,
	OUTDOOR_OUT_CAVES = 0x00000040,
	OUTDOOR_SOUTHWEST = 0x00000080,
	OUTDOOR_NORTHWEST = 0x00000100,
	OUTDOOR_SOUTHEAST = 0x00000200,
	OUTDOOR_NORTHEAST = 0x00000400,
};

struct D2DrlgOutdoorGridStrc
{
	int32_t dwFlags;						//0x00
	D2DrlgGridStrc* pSectors;				//0x04
	int32_t nWidth;							//0x08
	int32_t nHeight;						//0x0C
	int32_t bInit;								//0x10
};

union D2DrlgOutdoorPackedGrid2InfoStrc // TODO: rename
{
	uint32_t nPackedValue;
	struct {
		uint32_t nUnkb00 : 1;		 // Mask 0x00000001
		uint32_t bHasDirection : 1;  // Mask 0x00000002
		uint32_t nUnkb02 : 5;        // Mask 0x0000007C
		uint32_t nUnkb07 : 1;        // Mask 0x00000080 spawn preset level here ?
		uint32_t nUnkb08 : 1;        // Mask 0x00000100 related to being a blank grid cell?
		uint32_t bHasPickedFile : 1; // Mask 0x00000200
		uint32_t bLvlLink : 1;		 // Mask 0x00000400
		uint32_t nUnkb11 : 1;		 // Mask 0x00000800
		uint32_t nUnkb12 : 1;		 // Mask 0x00001000
		uint32_t nUnkb13 : 3;		 // Mask 0x0000E000
		uint32_t nPickedFile : 4;    // Mask 0x000F0000
		uint32_t nUnkb20 : 12;       // Mask 0xFFF00000
	};
};

struct D2DrlgOutdoorInfoStrc
{
	uint32_t dwFlags;						//0x00 D2C_OutDoorInfoFlags
	D2DrlgGridStrc pGrid[4];				//0x04 0: LevelPrestId 1: ? 2: D2DrlgOutdoorPackedGrid2InfoStrc
	union
	{
		struct
		{
			int32_t nWidth;					//0x54
			int32_t nHeight;				//0x58
			int32_t nGridWidth;				//0x5C
			int32_t nGridHeight;			//0x60
		};
		D2DrlgCoordStrc pCoord;
	};
	D2DrlgVertexStrc* pVertex;				//0x64
	D2DrlgVertexStrc* pPathStarts[6];			//0x68
	D2DrlgVertexStrc pVertices[24];			//0x80
	int32_t nVertices;						//0x260 Maybe nPathStarts instead?
	D2DrlgOrthStrc* pRoomData;				//0x264
};

#pragma pack()
