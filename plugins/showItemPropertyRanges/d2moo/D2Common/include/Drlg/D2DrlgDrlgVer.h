#pragma once

#include <cstdint>

struct D2DrlgOrthStrc;

#pragma pack(1)

struct D2DrlgCoordStrc
{
	int32_t nPosX;								//0x00
	int32_t nPosY;								//0x04
	int32_t nWidth;								//0x08
	int32_t nHeight;							//0x0C
};

struct D2DrlgVertexStrc
{
	int32_t nPosX;							//0x00
	int32_t nPosY;							//0x04
	uint8_t nDirection;						//0x08
	uint8_t pad0x09[3];						//0x09
	int32_t dwFlags;						//0x0C
	D2DrlgVertexStrc* pNext;				//0x10
};

#pragma pack()
