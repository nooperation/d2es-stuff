//////////////////////////////////////////////////////////////////////
// Screen.cpp
//
// Implementation of the "screen" API set functions. 
//
// Abin (abinn32@yahoo.com)
// 
//////////////////////////////////////////////////////////////////////

#include "D2Hackit.h"
#include <stdio.h>
#include "Template\\LinkedList.h"

#define D2WIN_DRAWTEXT				0x2785 
#define D2WIN_GETTEXTWIDTH			0x2789
#define D2WIN_GETFONTHEIGHT			0x278d
#define D2WIN_DRAWFONT				0x278F
#define D2CLIENT_FINISHFRAMEDISPLAY	0x6FAA97EA
//#define D2GFX_GETHWND				0x272B
//#define D2GFX_SCREENSIZE			0x2715

#define DEFAULT_FONTINDEX			8
#define MAX_DRAWS					255

typedef struct tagD2ScreenHook
{
	DWORD dwHookID;
	D2DRAWDATA aData[D2DRAW_MAX_DATA];
	int nDataCount;
} D2SCREENHOOK, *LPD2SCREENHOOK;
typedef const tagD2ScreenHook* LPCD2SCREENHOOK;

BYTE g_aOldCode[5] = {0};

//////////////////////////////////////////////////////////////////////
// Drawing Function Definition
//////////////////////////////////////////////////////////////////////
typedef void (__fastcall *fpWinDrawText)(LPCWSTR lpwszText, int x, int y, int nColor, int nReserved);
typedef DWORD (__fastcall *fpWinDrawFont)(int nFontNo);
typedef int (__fastcall *fpWinGetTextWidth)(LPWSTR lpwszText);
typedef BYTE (__fastcall *fpWinGetFontHeight)();
//typedef HWND (__stdcall *fpGfxGetHWND)();
//typedef DWORD (__stdcall *fpGfxGetScreenSize)();

fpWinDrawText g_lpfnDraw = NULL;
fpWinDrawFont g_lpfnFont = NULL;
fpWinGetTextWidth g_lpfnTextWidth = NULL;
fpWinGetFontHeight g_lpfnFontHeight = NULL;
//fpGfxGetScreenSize g_lpfnGfxGetScreenSize = NULL;

BOOL operator==(const D2SCREENHOOK& lhs, const D2SCREENHOOK& rhs)
{
	return lhs.dwHookID == rhs.dwHookID;
}

DWORD g_dwCurMaxHookID = 0;
CLinkedList<D2SCREENHOOK, const D2SCREENHOOK&, NULL> g_aScreenHooks;

int RemoveColorSpecs(LPWSTR lpwsz);
int ReplaceColorSpecs(LPWSTR lpwsz);
void Draw();	// Internal drawing function
BOOL WriteMemory(LPVOID lpvAddress, LPVOID lpvData, DWORD dwSize);
BOOL ReadMemory(LPVOID lpvAddress, LPVOID lpvData, DWORD dwSize);
BOOL PatchCall(DWORD dwAddress, LPVOID lpvFunction);
	
BOOL D2CanDrawNow();
int ValidateD2Font(int nFont); // Whether a font is valid
BOOL IsOldCodeSaved();

BOOL D2InitializeScreenHook()
{
	g_lpfnDraw = NULL;
	g_lpfnFont = NULL;
	g_lpfnTextWidth = NULL;
	g_lpfnFontHeight = NULL;
	//g_lpfnGfxGetScreenSize = NULL;
	::memset(g_aOldCode, 0, 5);

	// save original patch code ASAP
	ReadMemory((LPVOID)D2CLIENT_FINISHFRAMEDISPLAY, g_aOldCode, 5);

	HMODULE hModule = ::GetModuleHandle("D2Win");
	if (hModule != NULL)
	{
		FARPROC fpFunc = ::GetProcAddress(hModule, (LPCTSTR)D2WIN_DRAWTEXT);
		if (fpFunc != NULL) 
			::memcpy(&g_lpfnDraw, &fpFunc, sizeof(FARPROC)); 

		fpFunc = NULL;
		fpFunc = ::GetProcAddress(hModule, (LPCTSTR)D2WIN_DRAWFONT);
		if (fpFunc != NULL) 
			::memcpy(&g_lpfnFont, &fpFunc, sizeof(FARPROC)); 

		fpFunc = NULL;
		fpFunc = ::GetProcAddress(hModule, (LPCTSTR)D2WIN_GETTEXTWIDTH);
		if (fpFunc != NULL) 
			::memcpy(&g_lpfnTextWidth, &fpFunc, sizeof(FARPROC));

		fpFunc = NULL;
		fpFunc = ::GetProcAddress(hModule, (LPCTSTR)D2WIN_GETFONTHEIGHT);
		if (fpFunc != NULL) 
			::memcpy(&g_lpfnFontHeight, &fpFunc, sizeof(FARPROC));
	}

	/*
	hModule = GetModuleHandle("D2Gfx");
	if (hModule != NULL)
	{
		FARPROC fpFunc = ::GetProcAddress(hModule, (LPCTSTR)D2GFX_SCREENSIZE);
		if (fpFunc != NULL) 
			::memcpy(&g_lpfnGfxGetScreenSize, &fpFunc, sizeof(FARPROC)); 
	}
	*/
	
	return D2CanDrawNow();
}

BOOL D2CanDrawNow()
{
	return g_lpfnDraw
		&& g_lpfnFont
		&& g_lpfnTextWidth
		&& g_lpfnFontHeight
	/* && g_lpfnGfxGetScreenSize*/
		&& IsOldCodeSaved();
}


DWORD EXPORT CreateScreenHook()
{
	g_dwCurMaxHookID++;
	D2SCREENHOOK dsh = {0};
	dsh.dwHookID = g_dwCurMaxHookID;
	g_aScreenHooks.InsertRear(dsh);
	return dsh.dwHookID;
}

BOOL EXPORT ReleaseScreenHook(DWORD dwHookID)
{
	POSITION pos = g_aScreenHooks.GetHeadPosition();
	while (pos)
	{
		POSITION posRec = pos;
		const D2SCREENHOOK& dsh = g_aScreenHooks.GetNext(pos);
		if (dsh.dwHookID == dwHookID)
		{
			g_aScreenHooks.RemoveAt(posRec);
			PatchCall(D2CLIENT_FINISHFRAMEDISPLAY, Draw);
			return TRUE;
		}
	}

	return FALSE;	
}

BOOL EXPORT SendToFront(DWORD dwHookID)
{
	POSITION pos = g_aScreenHooks.GetHeadPosition();
	while (pos)
	{
		POSITION posRec = pos;
		const D2SCREENHOOK& dsh = g_aScreenHooks.GetNext(pos);
		if (dsh.dwHookID == dwHookID)
		{
			g_aScreenHooks.InsertFront(dsh);
			g_aScreenHooks.RemoveAt(posRec);
			PatchCall(D2CLIENT_FINISHFRAMEDISPLAY, Draw);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL EXPORT SendToBack(DWORD dwHookID)
{
	POSITION pos = g_aScreenHooks.GetHeadPosition();
	while (pos)
	{
		POSITION posRec = pos;
		const D2SCREENHOOK& dsh = g_aScreenHooks.GetNext(pos);
		if (dsh.dwHookID == dwHookID)
		{
			g_aScreenHooks.InsertRear(dsh);
			g_aScreenHooks.RemoveAt(posRec);
			PatchCall(D2CLIENT_FINISHFRAMEDISPLAY, Draw);
			return TRUE;
		}
	}

	return FALSE;	
}

BOOL D2UndrawAll()
{	
	g_aScreenHooks.RemoveAll();
	g_dwCurMaxHookID = 0;
	
	if (!D2CanDrawNow())
		return FALSE;

	// restore old code to unpatch
	return WriteMemory((LPVOID)D2CLIENT_FINISHFRAMEDISPLAY, g_aOldCode, 5);
}

BOOL EXPORT DrawSingleText(DWORD dwHookID, LPCSTR lpszText, long x, long y, int nFont, BOOL bRightAlign)
{
	if (lpszText == NULL)
		return FALSE;

	D2DRAWDATA data = { 0 };
	data.nFont = nFont;
	data.x = x;
	data.y = y;
	data.bRightAlign = bRightAlign;
	strncpy(data.szText, lpszText, D2DRAW_MAX_TEXT);
	return DrawMultipleTexts(dwHookID, &data, 1);
}

BOOL EXPORT DrawMultipleTexts(DWORD dwHookID, LPCD2DRAWDATA aData, int nDataCount)
{	
	if (!D2CanDrawNow() || aData == NULL || nDataCount == 0)
		return FALSE;
	
	D2SCREENHOOK dsh = {0};
	dsh.dwHookID = dwHookID;
	POSITION pos = g_aScreenHooks.Find(dsh);
	if (pos == NULL)
		return FALSE;

	dsh.nDataCount = min(nDataCount, D2DRAW_MAX_DATA);
	::memcpy(dsh.aData, aData, sizeof(D2DRAWDATA) * dsh.nDataCount);
	g_aScreenHooks.SetAt(dsh, pos);

	return PatchCall(D2CLIENT_FINISHFRAMEDISPLAY, Draw);
}

// I think this should be ackmed's code? I dunno...
BOOL PatchCall(DWORD dwAddress, LPVOID lpvFunction)
{
	LONG lOffset; 
	BYTE bBuffer[5]; 
	LPBYTE lpbAddress; 

	lOffset = (LONG) lpvFunction - ((LONG) dwAddress + 5); 
	lpbAddress = (LPBYTE) dwAddress; 
	bBuffer[0] = 0xE8; 
	CopyMemory(&bBuffer[1], &lOffset, sizeof(LONG)); 
	return WriteMemory(lpbAddress, bBuffer, 5); 
}

// Originally Mousepad's code
BOOL WriteMemory(LPVOID lpvAddress, LPVOID lpvData, DWORD dwSize)
{
	DWORD dwOldProtect, dwDummy; 
	return ::VirtualProtect(lpvAddress, dwSize, PAGE_READWRITE, &dwOldProtect)
		&& ::WriteProcessMemory(::GetCurrentProcess(), lpvAddress, lpvData, dwSize, NULL)
		&& ::VirtualProtect(lpvAddress, dwSize, dwOldProtect, &dwDummy);
}

BOOL ReadMemory(LPVOID lpvAddress, LPVOID lpvData, DWORD dwSize)
{
	DWORD dwOldProtect, dwDummy; 
	return ::VirtualProtect(lpvAddress, dwSize, PAGE_READWRITE, &dwOldProtect)
		&& ::ReadProcessMemory(::GetCurrentProcess(), lpvAddress, lpvData, dwSize, NULL)
		&& ::VirtualProtect(lpvAddress, dwSize, dwOldProtect, &dwDummy);
}

void Draw()
{
	if (!D2CanDrawNow())
		return;

	wchar_t wsz[D2DRAW_MAX_TEXT + 1] = { 0 };	
	
	POSITION pos = g_aScreenHooks.GetHeadPosition();
	while (pos)
	{
		// Iterate through the data list and draw every element
		const D2SCREENHOOK& dsh = g_aScreenHooks.GetNext(pos);

		for (int i = 0; i < dsh.nDataCount && i < D2DRAW_MAX_DATA; i++)
		{
			int nFont = ValidateD2Font(dsh.aData[i].nFont);
			long x = dsh.aData[i].x;
			if (dsh.aData[i].bRightAlign)
				x -= GetTextSize(dsh.aData[i].szText, dsh.aData[i].nFont).cx;

			::MultiByteToWideChar(CP_ACP, 0, dsh.aData[i].szText, -1, wsz, D2DRAW_MAX_TEXT);
			ReplaceColorSpecs(wsz);

			DWORD dwOldFont = g_lpfnFont(nFont);
			g_lpfnDraw(wsz, x, dsh.aData[i].y, 0, 0);
			g_lpfnFont((int)dwOldFont);
		}
	}	
}

int ValidateD2Font(int nFont)
{
	return nFont >= 0 && nFont < 14 ? nFont : DEFAULT_FONTINDEX;
}

SIZE EXPORT GetTextSize(LPCSTR lpszText, int nFont)
{
	SIZE cResult = { 0 };
	
	if (!D2CanDrawNow()
		|| lpszText == NULL
		|| lpszText[0] == '\0')
		return cResult; // invalid

	nFont = ValidateD2Font(nFont);

	wchar_t wsz[D2DRAW_MAX_TEXT + 1] = {0};
	::MultiByteToWideChar(CP_ACP, 0, lpszText, -1, wsz, D2DRAW_MAX_TEXT);
	RemoveColorSpecs(wsz);

	DWORD dwOldFont = g_lpfnFont(nFont);
	cResult.cx = g_lpfnTextWidth(wsz);
	cResult.cy = g_lpfnFontHeight();
	fpWinDrawFont((int)dwOldFont);
	return cResult;
}

int ReplaceColorSpecs(LPWSTR lpwsz)
{
	// Prevent the "ÿc" symbol to be converted into wchar_t
	if (lpwsz == NULL)
		return 0;

	const int LEN = wcslen(lpwsz);
	int nCount = 0;
	for (int i = 0; i < LEN; i++)
	{
		if (lpwsz[i] == 0xf8f5) // Unicode 'ÿ'
		{
			lpwsz[i] = 0xff; // Ansi 'ÿ'
			nCount++;		
		}
	}

	return nCount;
}

int RemoveColorSpecs(LPWSTR lpwsz)
{
	// for checking text size, need to remove "ÿc3"'s first because
	// they shouldn't be counted in as text
	if (lpwsz == NULL)
		return 0;

	const int LEN = wcslen(lpwsz);
	wchar_t* p = new wchar_t[LEN + 1];
	::memset(p, 0, sizeof(wchar_t) * (LEN + 1));

	int nIndex = 0;
	for (int i = 0; i < LEN; i++)
	{
		if (lpwsz[i] == 0xf8f5)
			i += 2;
		else
			p[nIndex++] = lpwsz[i];
	}

	wcscpy(lpwsz, p);
	return wcslen(lpwsz);
}

BOOL IsOldCodeSaved()
{
	for (int i = 0; i < 5; i++)
	{
		if (g_aOldCode[i])
			return TRUE;
	}
	return FALSE;
}

