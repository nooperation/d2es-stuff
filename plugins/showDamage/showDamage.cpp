#include <windows.h>
#include "../Includes/plugin.h"
#include "d2moo/D2Common/include/D2DataTbls.h"
#include "d2moo/D2Common/include/D2StatList.h"
#include "d2moo/D2Common/include/D2Items.h"
#include <cstdint>
#include <iostream>
#include <vector>
#include <chrono>
#include <sstream>
#include <locale>

#include "magic.h"

#undef min
#undef max

enum class Font
{
    D2FONT_FONT8,					//Font8
    D2FONT_FONT16,					//Font16
    D2FONT_FONT30,					//Font30
    D2FONT_FONT42,					//Font42
    D2FONT_FONTFORMAL10,			//FontFormal10
    D2FONT_FONTFORMAL12,			//FontFormal12
    D2FONT_FONT6,					//Font6
    D2FONT_FONT24,					//Font24
    D2FONT_FONTFORMAL11,			//FontFormal11
    D2FONT_FONTEXOCET10,			//FontExocet10
    D2FONT_FONTRIDICULOUS,			//FontRidiculous
    D2FONT_FONTEXOCET8,				//FontExocet8
    D2FONT_REALLYTHELASTSUCKER,		//ReallyTheLastSucker
    D2FONT_FONTINGAMECHAT,			//FontInGameChat
    NUM_FONTS,
};

DWORD __stdcall PluginEntry(DWORD dwReason, LPVOID lpData);
bool RewriteCode(void* targetAddress, uint8_t* newCode, std::size_t newCodeLength);

typedef D2UnitStrc* (__fastcall* GetUnit_t)(uint32_t unitId, uint32_t unitType);
GetUnit_t GetUnit = nullptr;

typedef D2UnitStrc* (__fastcall* GetPlayerUnit_t)();
GetPlayerUnit_t GetPlayerUnit = nullptr;

typedef void(__fastcall* D2Win_10117_DrawText_t)(const wchar_t* wszText, int nX, int nY, int nColor, BOOL bCentered);
D2Win_10117_DrawText_t D2Win_10117_DrawText = nullptr;

typedef Font(__fastcall* D2Win_10127_SetFont_t)(Font nFontSize);
D2Win_10127_SetFont_t D2Win_10127_SetFont = nullptr;

typedef int32_t(__fastcall* D2GetViewXOffset_t)();
D2GetViewXOffset_t D2GetViewXOffset = nullptr;

typedef int32_t(__fastcall* D2GetViewYOffset_t)();
D2GetViewYOffset_t D2GetViewYOffset = nullptr;

typedef void(__fastcall* D2Win_10131_GetTextDimensions_t)(const wchar_t* pText, int* pWidth, int* pHeight);
D2Win_10131_GetTextDimensions_t D2Win_10131_GetTextDimensions = nullptr;

typedef uint32_t(__fastcall* PacketHandlerFunc_t)(const char* packet, uint32_t packetLength);

#pragma pack(push,1)
struct PacketHandlerEntry
{
    PacketHandlerFunc_t Callback;
    uint32_t PacketSize;
    uint32_t UnknownFlag;
};

struct D2DamagePacket
{
    uint8_t unknownA;
    uint8_t unknownB;
    uint8_t unknownC;
    uint32_t unitId;
    uint32_t damage;
};
#pragma pack(pop)

struct DamageInfo
{
    bool isActive;
    bool isPlayerBeingAttacked;
    uint32_t unitId;
    uint64_t tickCreated;
    std::wstring text;
};

PLUGIN_INTERFACE Interface;
uint32_t OriginalFunc;

LPPLUGIN_INTERFACE __stdcall QueryInterface(VOID)
{
    Interface.dwMagicword = PLUGIN_MAGICWORD;
    Interface.dwVersion = PLUGIN_VERSION;
    Interface.szDescription = "Show damage https://github.com/nooperation/d2es-stuff/";
    Interface.fpEntry = PluginEntry;
    return &Interface;
}

std::vector<DamageInfo> damageInfos(64);

bool GetMonsterPositionScreenCoords(const D2UnitStrc* unit, int32_t& x, int32_t& y)
{
    if (unit == nullptr || (unit->dwUnitType != 1 && unit->dwUnitType != 0) || unit->pDynamicPath == nullptr)
    {
        return false;
    }

    auto offsetX = D2GetViewXOffset();
    auto offsetY = D2GetViewYOffset();

    x = unit->pDynamicPath->dwClientCoordX;
    y = unit->pDynamicPath->dwClientCoordY;

    x -= offsetX;
    y -= offsetY;

    return true;
}

void __cdecl RenderHook()
{
    const int32_t kDamageLifetimeInMs = 1000;

    auto currentTick = GetTickCount64();
    auto originalFont = D2Win_10127_SetFont(Font::D2FONT_FONTFORMAL12);

    for (std::size_t i = 0; i < damageInfos.size(); ++i)
    {
        if (!damageInfos[i].isActive)
        {
            continue;
        }

        auto ticksRemaining = (currentTick - damageInfos[i].tickCreated);
        if (ticksRemaining > kDamageLifetimeInMs)
        {
            damageInfos[i].isActive = false;
            continue;
        }

        int32_t unitX = 0;
        int32_t unitY = 0;
        int32_t color = 0;
        D2UnitStrc* unit = nullptr;

        if (damageInfos[i].isPlayerBeingAttacked)
        {
            unit = GetPlayerUnit();
            color = 1;
        }
        else
        {
            unit = GetUnit(damageInfos[i].unitId, 1);
            color = 0;
        }

        if (unit == nullptr || unit->pDynamicPath == nullptr)
        {
            damageInfos[i].isActive = false;
            continue;
        }

        GetMonsterPositionScreenCoords(unit, unitX, unitY);

        int32_t textWidth;
        int32_t textHeight;
        D2Win_10131_GetTextDimensions(damageInfos[i].text.c_str(), &textWidth, &textHeight);

        unitY -= (65 + ticksRemaining >> 3);
        unitX -= textWidth / 2;

        D2Win_10117_DrawText(damageInfos[i].text.c_str(), unitX, unitY, color, FALSE);
    }

    D2Win_10127_SetFont(originalFont);
}

void UpdateUnitDamage(uint32_t unitId, int32_t damage, bool isPlayerBeingAttacked)
{
    for (std::size_t i = 0; i < damageInfos.size(); ++i)
    {
        if (damageInfos[i].isActive)
        {
            continue;
        }

        damageInfos[i].isActive = true;
        damageInfos[i].unitId = unitId;
        damageInfos[i].tickCreated = GetTickCount64();
        damageInfos[i].isPlayerBeingAttacked = isPlayerBeingAttacked;

        std::wstringstream wss;
        wss.imbue(std::locale(""));
        wss << (damage >> 8);

        damageInfos[i].text = wss.str();
        return;
    }
}

uint32_t __fastcall HandleDamagePacket(const char* packet, uint32_t packetLength)
{
    auto damage = (D2DamagePacket*)&packet[1];
    UpdateUnitDamage(damage->unitId, damage->damage, damage->unknownA == 2);

    return 1;
}

DWORD __stdcall PluginEntry(DWORD dwReason, LPVOID lpData)
{
    if (dwReason == REASON_ENTER_MAIN_MENU)
    {
        const auto d2ClientHandle = GetModuleHandle("D2Client.dll");
        if (d2ClientHandle == 0)
        {
            return FALSE;
        }
        const auto d2CommonHandle = GetModuleHandle("D2Common.dll");
        if (d2CommonHandle == 0)
        {
            return FALSE;
        }
        const auto d2WinHandle = GetModuleHandle("D2Win.dll");
        if (d2WinHandle == 0)
        {
            return FALSE;
        }

        PacketHandlerEntry* packetHandlerTable = (PacketHandlerEntry*)((unsigned char*)d2ClientHandle + 0xD6270);
        packetHandlerTable[0x45].Callback = HandleDamagePacket;
        packetHandlerTable[0x45].PacketSize = 13;

        GetUnit = (GetUnit_t)((unsigned char*)d2ClientHandle + 0x869F0);
        GetPlayerUnit = (GetPlayerUnit_t)((unsigned char*)d2ClientHandle + 0x883D0);
        D2GetViewXOffset = (D2GetViewXOffset_t)((unsigned char*)d2ClientHandle + 0x15890);
        D2GetViewYOffset = (D2GetViewYOffset_t)((unsigned char*)d2ClientHandle + 0x158A0);

        D2Win_10117_DrawText = (D2Win_10117_DrawText_t)GetProcAddress(d2WinHandle, (LPCSTR)10117);
        D2Win_10127_SetFont = (D2Win_10127_SetFont_t)GetProcAddress(d2WinHandle, (LPCSTR)10127);
        D2Win_10131_GetTextDimensions = (D2Win_10131_GetTextDimensions_t)GetProcAddress(d2WinHandle, (LPCSTR)10131);

        uint8_t renderHookBytecode[] = {
            0x00, 0x00, 0x00, 0x00,   // renderHook_intercept
        };

        const auto patchAddress = (unsigned char*)d2ClientHandle + 0x82116;
        OriginalFunc = (uint32_t)((unsigned char*)d2ClientHandle + 0x5E650);

        *((uint32_t*)&renderHookBytecode[0]) = (uint32_t)((int32_t)intercept_RenderHook - ((int32_t)patchAddress + 4));

        RewriteCode(patchAddress, renderHookBytecode, sizeof(renderHookBytecode));
    }

    return TRUE;
}

bool RewriteCode(void* targetAddress, uint8_t* newCode, std::size_t newCodeLength)
{
    DWORD oldProtection = 0;
    if (!VirtualProtect(targetAddress, newCodeLength, PAGE_EXECUTE_READWRITE, &oldProtection))
    {
        OutputDebugStringA("Failed to VirtualProtect address...\n");
    }

    memcpy(targetAddress, newCode, newCodeLength);

    if (!VirtualProtect(targetAddress, newCodeLength, oldProtection, &oldProtection))
    {
        OutputDebugStringA("Failed to restore VirtualProtect protection...\n");
    }

    return memcmp(targetAddress, newCode, newCodeLength) == 0;
}

BOOL WINAPI DllMain(HINSTANCE hDll, DWORD dwReason, LPVOID lpReserved)
{
    return TRUE;
}
