#pragma once

#include <cstdint>

struct D2DrlgStrc;
struct D2ActiveRoomStrc;
struct D2DrlgRoomStrc;
struct D2DrlgLevelStrc;

#pragma pack(1)

struct D2MazeLevelIdStrc
{
	int32_t nLevelPrestId1;					//0x00
	int32_t nLevelPrestId2;					//0x04
	int32_t nPickedFile;					//0x08
	int32_t nDirection;						//0x0C
};

#pragma pack()
