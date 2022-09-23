#include <windows.h>
#include "../Includes/plugin.h"

#include <stdio.h>


LRESULT APIENTRY WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
DWORD __stdcall PluginEntry(DWORD dwReason, LPVOID lpData);
void WriteAddr(DWORD address, BYTE* data, DWORD dataSize);

PLUGIN_INTERFACE Interface;

LPPLUGIN_INTERFACE __stdcall QueryInterface(VOID)
{
    Interface.dwMagicword = PLUGIN_MAGICWORD;
    Interface.dwVersion = PLUGIN_VERSION;
    Interface.szDescription = "Sleepy (lowers cpu usage, but at what cost)";
    Interface.fpEntry = PluginEntry;
    return &Interface;
}

DWORD __stdcall PluginEntry(DWORD dwReason, LPVOID lpData)
{
    DWORD client = 0;
    DWORD win = 0;
    BYTE data[1];

    if (dwReason == REASON_ENTER_MAIN_MENU)
    {
        client = (DWORD)GetModuleHandle("D2Client.dll");
        if (client == 0)
        {
            return FALSE;
        }

        win = (DWORD)GetModuleHandle("D2Win.dll");
        if (win == 0)
        {
            return FALSE;
        }

        auto commandLine = GetCommandLine();
        if (strstr(commandLine, "-sleepy") == NULL)
        {
            OutputDebugStringA("Skipping sleepy plugin");
            return FALSE;
        }

        // Sleep(0) -> Sleep(1) as stated in d2loader readme
        data[0] = 1;
        WriteAddr(client + 0xB4CA2, data, 1);

        // Sleep(EAX), EAX was being set from 0x14 to 0x00 due to a flag,
        //   this just ignores that flag and keeps Sleep(0x14)
        data[0] = 0xEB;
        WriteAddr(win + 0xD071, data, 1); // 0x6f8ad071

        OutputDebugStringA("Sleepy applied");
        return FALSE;
    }

    return TRUE;
}

void WriteAddr(DWORD address, BYTE* data, DWORD dataSize)
{
    DWORD OldProtect;

    VirtualProtect((void*)address, dataSize, PAGE_EXECUTE_READWRITE, &OldProtect);

    memcpy((void*)address, data, dataSize);

    VirtualProtect((void*)address, dataSize, OldProtect, &OldProtect);
}


BOOL WINAPI DllMain(HINSTANCE hDll, DWORD dwReason, LPVOID lpReserved)
{
    return TRUE;
}
