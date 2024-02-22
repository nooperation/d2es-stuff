#pragma once

#include "CommonDefinitions.h"
#include "Units/Missile.h"
#include "DataTbls/ItemsTbls.h"

struct D2StatListStrc;

#pragma pack(1)

struct D2PropertyAssignStrc
{
	void* pfAssign;			//0x00
	int32_t nStatId;					//0x04
};


struct D2ItemCalcStrc
{
	D2UnitStrc* pUnit;					//0x00
	D2UnitStrc* pItem;					//0x04
};

#pragma pack()
