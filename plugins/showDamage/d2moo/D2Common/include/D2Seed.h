#pragma once

#include <cstdint>

#pragma pack(1)

struct D2SeedStrc
{
	union
	{
		struct
		{
			uint32_t nLowSeed;					//0x00
			uint32_t nHighSeed;					//0x04
		};
		uint64_t lSeed;		//0x00
	};
};
#pragma pack()
