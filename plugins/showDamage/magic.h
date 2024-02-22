#pragma once

#include "d2moo/D2Common/include/Units/Units.h"

extern "C"
{
    void intercept_RenderHook();

    extern uint32_t OriginalFunc;

    void __cdecl RenderHook();
};
