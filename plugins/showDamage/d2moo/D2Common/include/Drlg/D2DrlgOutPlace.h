#pragma once

#include <cstdint>
#include "D2DrlgDrlg.h"


#pragma pack(1)

enum D2Jungle {
	JUNGLE_MAX_ATTACH = 3,
	JUNGLE_PRESET2_ATTACH_POINT = 2
};


enum D2JunglePresetFlags {
	// Related to pPreset1 having a value in the direction
	JUNGLE_FLAG_LEFT_	= 0b0001, // West : deltaX < 0
	JUNGLE_FLAG_RIGHT_	= 0b0010, // East : deltaX > 0
	JUNGLE_FLAG_BOTTOM_	= 0b0100, // South: deltaY > 0
	JUNGLE_FLAG_TOP_	= 0b1000, // North: deltaY < 0
};

struct D2JungleStrc
{
	D2DrlgCoordStrc pDrlgCoord;							//0x00
	int32_t field_10;									//0x10 Orientation of the jungle ?
	int32_t nBranch;									//0x14
	D2JungleStrc* pBasedOnJungle;						//0x18 Jungle we are based from
	D2JungleStrc* pJungleBranches[JUNGLE_MAX_ATTACH];	//0x1C Jungles that are based on this one
	int32_t nPresetsBlocksX;							//0x18
	int32_t nPresetsBlocksY;							//0x2C
	int32_t* pJungleDefs;								//0x30
	int32_t nJungleDefs;								//0x34
};


#pragma pack()
