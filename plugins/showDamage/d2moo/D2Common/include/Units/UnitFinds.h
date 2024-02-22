#pragma once

#include <cstdint>

#include "../DataTbls/FieldTbls.h"

struct D2ActiveRoomStrc;
struct D2UnitStrc;

#pragma pack(1)

#define UNIT_FIND_ARRAY_SIZE 15

using UNITFINDTEST = int32_t(__fastcall*)(D2UnitStrc* pUnit, struct D2UnitFindArgStrc* pUnitFindArg);

struct D2UnitFindArgStrc
{
	int32_t nFlags;								//0x00
	int32_t unk0x04;							//0x04
	D2UnitStrc* pUnit;							//0x08
	int32_t nX;									//0x0C
	int32_t nY;									//0x10
	int32_t nSize;								//0x14
	int32_t nMaxArrayEntries;					//0x18
	int32_t nIndex;								//0x1C
	D2FieldStrc* pField;						//0x20
	UNITFINDTEST pfnUnitTest;					//0x24
	int32_t unk0x28;							//0x28
	int32_t unk0x2C;							//0x2C
	int32_t unk0x30;							//0x30
	int32_t unk0x34;							//0x34
};

struct D2UnitFindDataStrc
{
	void* pMemPool;								//0x00
	D2UnitStrc** pUnitsArray;					//0x04
	D2ActiveRoomStrc* pRoom;					//0x08
	UNITFINDTEST pfnUnitTest;					//0x0C
	D2UnitFindArgStrc* pUnitFindArg;			//0x10
	int32_t nFlags;								//0x14
	int32_t nIndex;								//0x18
	int32_t nMaxArrayEntries;					//0x1C
	int32_t nX;									//0x20
	int32_t nY;									//0x24
	int32_t nSize;								//0x28
};

#pragma pack()
