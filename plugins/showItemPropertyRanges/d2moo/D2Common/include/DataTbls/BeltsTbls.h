#pragma once 

#include <cstdint>
#include "../D2Inventory.h"

#pragma pack(1)

enum D2C_BeltsIndices
{
	// 640x480
	BELT_IDX_Belt,
	BELT_IDX_Sash,
	BELT_IDX_Default,
	BELT_IDX_Girdle,
	BELT_IDX_LightBelt,
	BELT_IDX_HeavyBelt,
	BELT_IDX_UberBelt,
	BELT_IDX_COUNT_PER_RESOLUTION,
	// 800x600
	BELT_IDX_800x600_Belt = BELT_IDX_COUNT_PER_RESOLUTION,
	BELT_IDX_800x600_Sash,
	BELT_IDX_800x600_Default,
	BELT_IDX_800x600_Girdle,
	BELT_IDX_800x600_LightBelt,
	BELT_IDX_800x600_HeavyBelt,
	BELT_IDX_800x600_UberBelt,
};

struct D2BeltsTxt
{
	uint8_t nIndex;						//0x00
	uint8_t pad0x01[3];					//0x01
	int32_t nBoxes;						//0x04
	D2InvRectStrc beltBoxes[16];		//0x08
};

#pragma pack()