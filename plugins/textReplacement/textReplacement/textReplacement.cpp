#include <windows.h>
#include <map>
#include <fstream>
#include <string>
#include "plugin.h"

struct ItemTxt 
{
	BYTE garbage[0x80];
	char szCode[4];
};

struct ItemReplacementData
{
	std::wstring prefix;
	std::wstring suffix;
	bool replaceColor;
	bool prefixReplacesAll;
};


LRESULT APIENTRY WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
DWORD __stdcall PluginEntry(DWORD dwReason, LPVOID lpData);
void WriteAddr(DWORD address, BYTE *data, DWORD dataSize);
void InterceptItemText();
bool LoadTextReplacementMap();

std::map<std::string, ItemReplacementData> textReplacementMap;
PLUGIN_INTERFACE Interface;
DWORD maphackBase = 0;
DWORD d2CommonBase = 0;
DWORD d2ClientBase = 0;

typedef ItemTxt *__stdcall D2COMMON_GetItemTxt_t(DWORD);
D2COMMON_GetItemTxt_t *D2COMMON_GetItemTxt = NULL;

LPPLUGIN_INTERFACE __stdcall QueryInterface(VOID) 
{
	Interface.dwMagicword = PLUGIN_MAGICWORD;
	Interface.dwVersion = PLUGIN_VERSION;
	Interface.szDescription = "text replacement";
	Interface.fpEntry = PluginEntry;
	return &Interface;
}

BOOL WINAPI DllMain(HINSTANCE hDll, DWORD dwReason, LPVOID lpReserved) 
{
	return TRUE;
}

DWORD __stdcall PluginEntry(DWORD dwReason, LPVOID lpData) 
{
	BYTE data[7];
	DWORD InterceptItemTextAddr;

	if(dwReason == REASON_ENTER_MAIN_MENU)
	{
		maphackBase = (DWORD)GetModuleHandle("D2Maphack110t.dll");

		d2CommonBase = (DWORD)GetModuleHandle("D2Common.dll");
		if(d2CommonBase == 0)
		{
			OutputDebugString("Failed to locate D2Common.dll");
			return FALSE;
		}

		d2ClientBase = (DWORD)GetModuleHandle("D2Client.dll");
		if(d2ClientBase == 0)
		{
			OutputDebugString("Failed to locate D2Client.dll");
			return FALSE;
		}

		if(!LoadTextReplacementMap())
		{
			OutputDebugString("Failed to load text replacement map");
			return FALSE;
		}

		D2COMMON_GetItemTxt = (D2COMMON_GetItemTxt_t *)(d2CommonBase + 0x17680);

		if(maphackBase != 0)
		{
			InterceptItemTextAddr = (DWORD)InterceptItemText - (maphackBase + 0x95A9 + 4);

			data[8] = 0x90;
			data[7] = 0x90;
			data[6] = 0x90;
			data[5] = 0x90;
			data[4] = (BYTE)((InterceptItemTextAddr & 0xff000000) >> 24);
			data[3] = (BYTE)((InterceptItemTextAddr & 0x00ff0000) >> 16);
			data[2] = (BYTE)((InterceptItemTextAddr & 0x0000ff00) >> 8);
			data[1] = (BYTE)((InterceptItemTextAddr & 0x000000ff) >> 0);
			data[0] = 0xE9;
			WriteAddr(maphackBase + 0x95A8, data, 9);
		}
		
		else
		{
			InterceptItemTextAddr = (DWORD)InterceptItemText - (d2ClientBase + 0x3E7DC + 3);

			data[3] = (BYTE)((InterceptItemTextAddr & 0xff000000) >> 24);
			data[2] = (BYTE)((InterceptItemTextAddr & 0x00ff0000) >> 16);
			data[1] = (BYTE)((InterceptItemTextAddr & 0x0000ff00) >> 8);
			data[0] = (BYTE)((InterceptItemTextAddr & 0x000000ff) >> 0);

			WriteAddr(d2ClientBase + 0x3E7DD, data, 4);
		}
	}

	return TRUE;
}

bool LoadTextReplacementMap()
{
	std::ifstream inFile(".\\Plugin\\textReplacement.txt");
	std::string nonWideBuff;
	std::string inBuff;
	std::string itemCode;
	wchar_t wideBuff[256];
	int tokenPosition = 0;
	size_t amountConverted;
	ItemReplacementData replacementData;
	
	if(!inFile)
	{
		return false;
	}

	while(inFile.good())
	{
		memset(&replacementData, 0, sizeof(ItemReplacementData));
		std::getline(inFile, inBuff);

		tokenPosition = inBuff.find("%N");
		if(tokenPosition == -1)
		{
			tokenPosition = inBuff.find("%n");
			if(tokenPosition == -1)
			{
				replacementData.prefixReplacesAll = true;
			}

			replacementData.replaceColor = true;
		}

		itemCode = inBuff.substr(0, 3);
		if(itemCode.length() != 3 || textReplacementMap.count(itemCode) > 0)
		{
			continue;
		}

		if(replacementData.prefixReplacesAll)
		{
			nonWideBuff = inBuff.substr(4);

			mbstowcs_s(&amountConverted, wideBuff, sizeof(wideBuff)/sizeof(wideBuff[0]), nonWideBuff.c_str(), nonWideBuff.length());

			replacementData.prefix = wideBuff;
			replacementData.suffix = L"";
		}
		else
		{
			tokenPosition = inBuff.find_first_of('%');

			nonWideBuff = inBuff.substr(4, tokenPosition - 4);
			mbstowcs_s(&amountConverted, wideBuff, sizeof(wideBuff)/sizeof(wideBuff[0]), nonWideBuff.c_str(), nonWideBuff.length());
			replacementData.prefix = wideBuff;

			nonWideBuff = inBuff.substr(tokenPosition + 2);
			mbstowcs_s(&amountConverted, wideBuff, sizeof(wideBuff)/sizeof(wideBuff[0]), nonWideBuff.c_str(), nonWideBuff.length());
			replacementData.suffix = wideBuff;
		}

		textReplacementMap[itemCode] = replacementData;
	}

	inFile.close();
	return true;
}

void WriteAddr(DWORD address, BYTE *data, DWORD dataSize)
{
	DWORD OldProtect;

	VirtualProtect((void *)address, dataSize, PAGE_EXECUTE_READWRITE, &OldProtect);

	memcpy((void *)address, data, dataSize);

	VirtualProtect((void *)address, dataSize, OldProtect, &OldProtect);
}

// referenced Mousepad's v4.4 maphack.cpp
void __fastcall ItemNamePatch(wchar_t *name)
{
	int itemNumber;
	char itemCode[4];
	wchar_t newName[256];

	__asm
	{
		MOV EAX,DWORD PTR DS:[EBX+4]
		MOV itemNumber, EAX
	}
	
	ItemTxt *itemtxt = D2COMMON_GetItemTxt(itemNumber);
	memcpy(itemCode, itemtxt->szCode, 3);
	itemCode[3] = '\0';

	if(textReplacementMap.count(itemCode) > 0)
	{
		if(textReplacementMap[itemCode].prefixReplacesAll)
		{
			swprintf_s(newName, sizeof(newName)/sizeof(newName[0]), L"%s", textReplacementMap[itemCode].prefix.c_str());
		}
		else
		{
			if(textReplacementMap[itemCode].replaceColor && name[0] == 0x00ff && name[1] == L'c')
			{
				swprintf_s(newName, sizeof(newName)/sizeof(newName[0]), L"%s%s%s", textReplacementMap[itemCode].prefix.c_str(), (name+3), textReplacementMap[itemCode].suffix.c_str());
			}
			else
			{
				swprintf_s(newName, sizeof(newName)/sizeof(newName[0]), L"%s%s%s", textReplacementMap[itemCode].prefix.c_str(), name, textReplacementMap[itemCode].suffix.c_str());
			}
		}

		memcpy(name, newName, wcslen(newName)*sizeof(newName[0]));
	}
}

__declspec(naked) void InterceptItemText()
{
	__asm
	{
		MOV EAX, ECX
		JMP ItemNamePatch
	}
}

