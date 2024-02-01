#include <windows.h>
#include "../Includes/plugin.h"
#include "d2moo/D2Common/include/D2DataTbls.h"
#include "d2moo/D2Common/include/D2StatList.h"
#include "d2moo/D2Common/include/D2Items.h"
#include <cstdint>
#include <iostream>

#include "magic.h"

#undef min
#undef max

DWORD __stdcall PluginEntry(DWORD dwReason, LPVOID lpData);
bool RewriteCode(void* targetAddress, uint8_t* newCode, std::size_t newCodeLength);

typedef int(__fastcall* BuildItemPropertyLineFunc)(D2UnitStrc* pUnit, D2StatListStrc* pStatListEx, int statId, int charStatsTxtRecordIndex, int statValue, wchar_t* outputBuffer256);
typedef D2RunesTxt* (__stdcall* ITEMS_GetRunesTxtRecordFromItem)(const D2UnitStrc* pItem);


PLUGIN_INTERFACE Interface;
D2DataTablesStrc* pDataTables = nullptr;
unsigned long ReturnPoint_BuildItemPropertyLine = 0;

BuildItemPropertyLineFunc originalBuildItemPropertyLineFunc = nullptr;
ITEMS_GetRunesTxtRecordFromItem getRunesTxtRecordFromItem = nullptr;

LPPLUGIN_INTERFACE __stdcall QueryInterface(VOID)
{
    Interface.dwMagicword = PLUGIN_MAGICWORD;
    Interface.dwVersion = PLUGIN_VERSION;
    Interface.szDescription = "Extra stat info https://github.com/nooperation/d2es-stuff/";
    Interface.fpEntry = PluginEntry;
    return &Interface;
}

bool AppendStatPropertyRange(wchar_t* propertyString, D2ItemDataStrc* pItemData, const D2PropertyStrc& prop, const D2ItemStatCostTxt& itemStatCostTxt, int statId, int statValue)
{
    auto propertyTxt = &pDataTables->pPropertiesTxt[prop.nProperty];
    if (propertyTxt == nullptr)
    {
        return false;
    }

    if (propertyTxt->wStat[0] != statId)
    {
        return false;
    }

    if (prop.nMin == prop.nMax)
    {
        return false;
    }

    if (propertyTxt->nFunc[0] == 19) // level {prop.nMax} skill ({prop.nMin} charges)
    {
        return false;
    }

    if (propertyTxt->nFunc[0] == 11) // {prop.nMin}% ctc level %{prop.nMax} skill on action
    {
        return false;
    }

    auto actualValue = statValue >> itemStatCostTxt.nValShift;

    auto minimumValue = std::min(prop.nMin, prop.nMax);
    auto maximumValue = std::max(prop.nMin, prop.nMax);

    wchar_t colorCode = '1';
    if (actualValue == prop.nMax) {
        colorCode = '2';
    }
    else if (actualValue > maximumValue || actualValue < minimumValue) {
        colorCode = '2'; // 8 highlights modified values, but it's probably too much information 
    }

    wsprintfW(propertyString, L"%s ÿc%c[%d - %d]ÿc3", propertyString, colorCode, prop.nMin, prop.nMax);
    return true;
}

bool AppendItemStatRangeMagicAffix(wchar_t* propertyString, D2ItemDataStrc* pItemData, const D2ItemStatCostTxt& itemStatCostTxt, int statId, int statValue, D2MagicAffixTxt* magicAffixTxt)
{
    for (auto propertyIndex = 0; propertyIndex < sizeof(magicAffixTxt->pProperties) / sizeof(magicAffixTxt->pProperties[0]); ++propertyIndex)
    {
        auto& currentProperty = magicAffixTxt->pProperties[propertyIndex];
        if (currentProperty.nProperty < 0)
        {
            break;
        }

        if (AppendStatPropertyRange(propertyString, pItemData, currentProperty, itemStatCostTxt, statId, statValue))
        {
            return true;
        }
    }

    return false;
}

void AppendItemStatRangeRare(wchar_t* propertyString, D2UnitStrc* pItem, D2ItemDataStrc* pItemData, const D2ItemStatCostTxt& itemStatCostTxt, int statId, int statValue)
{
    auto numAffix = pDataTables->pMagicAffixDataTables.nMagicAffixTxtRecordCount;

    if (pItemData->wAutoAffix > 0 && pItemData->wAutoAffix < numAffix)
    {
        auto magicAffixTxt = &pDataTables->pMagicAffixDataTables.pMagicAffixTxt[pItemData->wAutoAffix - 1];
        if (AppendItemStatRangeMagicAffix(propertyString, pItemData, itemStatCostTxt, statId, statValue, magicAffixTxt))
        {
            return;
        }
    }

    if (pItemData->dwItemFlags & IFLAG_RUNEWORD)
    {
        auto runeTxt = getRunesTxtRecordFromItem(pItem);
        if (runeTxt != nullptr)
        {
            for (auto propertyIndex = 0; propertyIndex < sizeof(runeTxt->pProperties) / sizeof(runeTxt->pProperties[0]); ++propertyIndex)
            {
                auto& currentProperty = runeTxt->pProperties[propertyIndex];
                if (currentProperty.nProperty < 0)
                {
                    break;
                }

                if (AppendStatPropertyRange(propertyString, pItemData, currentProperty, itemStatCostTxt, statId, statValue))
                {
                    return;
                }
            }
        }
    }

    for (int i = 0; i < ITEMS_MAX_MODS; i++)
    {
        if (pItemData->wMagicPrefix[i] == 0)
        {
            break;
        }

        if (pItemData->wMagicPrefix[i] >= numAffix)
        {
            continue;
        }

        auto magicAffixTxt = &pDataTables->pMagicAffixDataTables.pMagicAffixTxt[pItemData->wMagicPrefix[i] - 1];
        if (AppendItemStatRangeMagicAffix(propertyString, pItemData, itemStatCostTxt, statId, statValue, magicAffixTxt))
        {
            return;
        }
    }

    for (int i = 0; i < ITEMS_MAX_MODS; i++)
    {
        if (pItemData->wMagicSuffix[i] == 0)
        {
            break;
        }

        if (pItemData->wMagicSuffix[i] >= numAffix)
        {
            continue;
        }

        auto magicAffixTxt = &pDataTables->pMagicAffixDataTables.pMagicAffixTxt[pItemData->wMagicSuffix[i] - 1];
        if (AppendItemStatRangeMagicAffix(propertyString, pItemData, itemStatCostTxt, statId, statValue, magicAffixTxt))
        {
            return;
        }
    }
}

void AppendItemStatRangeSet(wchar_t* propertyString, D2ItemDataStrc* pItemData, const D2ItemStatCostTxt& itemStatCostTxt, int statId, int statValue)
{
    if (pItemData->dwFileIndex <= 0 || pItemData->dwFileIndex >= pDataTables->nSetItemsTxtRecordCount)
    {
        return;
    }

    const auto setTxt = &pDataTables->pSetItemsTxt[pItemData->dwFileIndex];
    if (setTxt == nullptr)
    {
        return;
    }

    for (auto i = 0; i < sizeof(setTxt->pProperties) / sizeof(setTxt->pProperties[0]); ++i)
    {
        const auto& currentProperty = setTxt->pProperties[i];
        if (currentProperty.nProperty <= 0)
        {
            break;
        }

        if (currentProperty.nProperty >= pDataTables->nPropertiesTxtRecordCount)
        {
            continue;
        }

        if (AppendStatPropertyRange(propertyString, pItemData, currentProperty, itemStatCostTxt, statId, statValue))
        {
            return;
        }
    }
}

void AppendItemStatRangeUnique(wchar_t* propertyString, D2ItemDataStrc* pItemData, const D2ItemStatCostTxt& itemStatCostTxt, int statId, int statValue)
{
    if (pItemData->dwFileIndex <= 0 || pItemData->dwFileIndex >= pDataTables->nUniqueItemsTxtRecordCount)
    {
        return;
    }

    const auto uniqueTxt = &pDataTables->pUniqueItemsTxt[pItemData->dwFileIndex];
    if (uniqueTxt == nullptr)
    {
        return;
    }

    for (auto i = 0; i < sizeof(uniqueTxt->pProperties) / sizeof(uniqueTxt->pProperties[0]); ++i)
    {
        const auto& currentProperty = uniqueTxt->pProperties[i];
        if (currentProperty.nProperty <= 0)
        {
            break;
        }

        if (currentProperty.nProperty >= pDataTables->nPropertiesTxtRecordCount)
        {
            continue;
        }

        if (AppendStatPropertyRange(propertyString, pItemData, currentProperty, itemStatCostTxt, statId, statValue))
        {
            return;
        }
    }
}

void AppendItemStatRange(wchar_t* propertyString, D2UnitStrc* pUnit, int statId, int statValue)
{
    auto pItemData = pUnit->pItemData;
    if (pItemData == nullptr)
    {
        return;
    }

    if (statId <= 0 || statId >= pDataTables->nItemStatCostTxtRecordCount)
    {
        return;
    }

    const auto& itemStatCostTxt = pDataTables->pItemStatCostTxt[statId];
    switch (pItemData->dwQualityNo)
    {
        case ITEMQUAL_NORMAL:
        case ITEMQUAL_INFERIOR:
        case ITEMQUAL_SUPERIOR:
        case ITEMQUAL_TEMPERED:
        case ITEMQUAL_MAGIC:
        case ITEMQUAL_RARE:
        case ITEMQUAL_CRAFT:
            AppendItemStatRangeRare(propertyString, pUnit, pItemData, itemStatCostTxt, statId, statValue);
            break;
        case ITEMQUAL_UNIQUE:
            AppendItemStatRangeUnique(propertyString, pItemData, itemStatCostTxt, statId, statValue);
            break;
        case ITEMQUAL_SET:
            AppendItemStatRangeSet(propertyString, pItemData, itemStatCostTxt, statId, statValue);
            break;
    }
}

bool __cdecl MyBuildItemPropertyLine(D2UnitStrc* pUnit, D2StatListStrc* pStatList, int statId, int charStatsTxtRecordIndex, int statValue, wchar_t* outputBuffer128)
{
    const auto result = originalBuildItemPropertyLineFunc(pUnit, pStatList, statId, charStatsTxtRecordIndex, statValue, outputBuffer128);
    if (result != 0)
    {
        AppendItemStatRange(outputBuffer128, pUnit, statId, statValue);
    }

    return result;
}

DWORD __stdcall PluginEntry(DWORD dwReason, LPVOID lpData)
{
    if (dwReason == REASON_ENTER_MAIN_MENU)
    {
        auto d2ClientHandle = GetModuleHandle("D2Client.dll");
        if (d2ClientHandle == 0)
        {
            return FALSE;
        }

        auto d2CommonHandle = GetModuleHandleA("D2Common.dll");
        if (d2CommonHandle == 0)
        {
            return FALSE;
        }

        getRunesTxtRecordFromItem = (ITEMS_GetRunesTxtRecordFromItem)GetProcAddress(d2CommonHandle, (LPCSTR)10822);
        if (getRunesTxtRecordFromItem == nullptr)
        {
            return FALSE;
        }

        auto pSgptDataTables = (D2DataTablesStrc**)GetProcAddress(d2CommonHandle, "sgptDataTables");
        if (pSgptDataTables == nullptr || *pSgptDataTables == nullptr)
        {
            return FALSE;
        }

        pDataTables = *pSgptDataTables;

        uint8_t interceptStatStringBytecode[] = {
            0xBA, 0x00, 0x00, 0x00, 0x00,   // mov edx, [address]
            0xFF, 0xE2,                     // jmp edx
        };

        originalBuildItemPropertyLineFunc = (BuildItemPropertyLineFunc)((uint32_t)d2ClientHandle + 0x521C0);

        const auto offset = 0x51FEC;
        *((uint32_t*)&interceptStatStringBytecode[1]) = (uint32_t)intercept_BuildItemPropertyLine;

        RewriteCode((unsigned char*)d2ClientHandle + offset, interceptStatStringBytecode, sizeof(interceptStatStringBytecode));
        ReturnPoint_BuildItemPropertyLine = (uint32_t)((unsigned char*)d2ClientHandle + offset + sizeof(interceptStatStringBytecode));
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
