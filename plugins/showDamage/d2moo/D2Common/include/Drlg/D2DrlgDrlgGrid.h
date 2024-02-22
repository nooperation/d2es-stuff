#pragma once

#include <cstdint>

#pragma pack(1)

enum FlagOperation
{
	FLAG_OPERATION_OR,
	FLAG_OPERATION_AND,
	FLAG_OPERATION_XOR,
	FLAG_OPERATION_OVERWRITE,
	FLAG_OPERATION_OVERWRITE_IF_ZERO,
	FLAG_OPERATION_AND_NEGATED,
};


struct D2DrlgGridStrc
{
	int32_t* pCellsFlags;					//0x00
	int32_t* pCellsRowOffsets;				//0x04
	int32_t nWidth;							//0x08
	int32_t nHeight;						//0x0C
	int32_t unk0x10;						//0x10 Maybe indicates if uninitialized? No memset when set to 1
};

#pragma pack()
