#pragma once

#include <cstdint>

#pragma pack(1)

struct D2HoverTextStrc
{
	uint32_t dwDisplayTime;					//0x00
	uint32_t dwExpireTime;						//0x04
	uint8_t nLangId;							//0x08
	uint8_t pad0x09[3];						//0x09
	int32_t bUsed;								//0x0C
	char szMsg[256];						//0x10
};
#pragma pack()
