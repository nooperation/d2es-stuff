#pragma once

#include "d2moo/D2Common/include/Units/Units.h"

extern "C"
{
    void intercept_BuildItemPropertyLine();

    extern unsigned long ReturnPoint_BuildItemPropertyLine;

    bool __cdecl MyBuildItemPropertyLine(D2UnitStrc* pUnit, D2StatListStrc* pStatListEx, int statId, int charStatsTxtRecordIndex, int statValue, wchar_t* outputBuffer128);
};
