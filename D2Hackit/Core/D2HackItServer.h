//////////////////////////////////////////////////////////////////////
// D2HackItServer.h
// -------------------------------------------------------------------
// Main incude file for D2HackIt Server (Exports + Privates).
//
// <thohell@home.se>
// <Dan_Goon@diabloworld.com>
//////////////////////////////////////////////////////////////////////
#define THIS_IS_SERVER

#pragma once

#include "D2Hackit.h"
#include "ServerStructs.h"

#define LOADERMAGIC			0xD1AB101D

// DllMain.cpp
extern SERVERINFO					*si;
extern PRIVATESERVERINFO			*psi;
extern PRIVATEFUNCTIONENTRYPOINTS	*pfep;
extern THISGAMESTRUCT*				thisgame;
extern HWND							g_hD2;			// 0.80
extern HANDLE						g_hD2HackIt;	// 0.80
extern LinkedList					szBufferList;	// List of GetHackProfile created strings
extern CClients						ClientModules;

// ServerStartStop.cpp

BOOL	PRIVATE ServerStart(HANDLE hModule);
BOOL	PRIVATE ServerStop();

// ClientHandler.cpp
LinkedItem* PRIVATE FindClientStruct(HMODULE hModule);
LinkedItem* PRIVATE FindClientStructByName(LPCSTR szName);
LinkedItem*	PRIVATE LoadClientModule(LPCSTR szName);
BOOL	PRIVATE UnloadClientModuleByName(LPCSTR szName);
BOOL	PRIVATE UnloadClientModule(LinkedItem* li);
BOOL	PRIVATE CleanUpClient(LinkedItem *li);
BOOL	PRIVATE HookHackitWndProc(HWND hD2, BOOL bHook);

// MemorySearchFunctions.cpp
BOOL	PRIVATE PatternEquals(LPBYTE buf, LPWORD pat, DWORD plen);
LPVOID	PRIVATE PatternSearch(LPBYTE buf, DWORD blen, LPWORD pat, DWORD plen);
VOID	PRIVATE MakeSearchPattern(LPCSTR pString, LPWORD pat);
DWORD	PRIVATE GetMemoryAddressFromPattern(LPSTR szDllName, LPCSTR szSearchPattern, DWORD offset);
DWORD	PRIVATE GetBaseAddress(LPSTR szModuleName);
DWORD	PRIVATE GetImageSize(LPSTR szModuleName);
void	PRIVATE SetMemToolType(void);

// psapi.cpp
DWORD	PRIVATE GetBaseAddress_psapi(LPSTR szModuleName);
DWORD	PRIVATE GetImageSize_psapi(LPSTR szModuleName);
BOOL	PRIVATE FindImage_psapi(LPSTR ModuleName, MODULEINFO* mi);

// toolhelp.cpp
DWORD	PRIVATE GetBaseAddress_toolhelp(LPSTR szModuleName);
DWORD	PRIVATE GetImageSize_toolhelp(LPSTR szModuleName);
BOOL	PRIVATE FindImage_toolhelp(LPSTR ModuleName, MODULEENTRY32* lpme);

// GameSendPacketToServer.cpp

// GameSendPacketToGame.cpp

// IniFileHandlers.cpp

// GamePrintFunctions.cpp

// GamePacketReceived.cpp
DWORD __fastcall GamePacketReceivedIntercept(BYTE* aPacket, DWORD aLength);
void	GamePacketReceivedInterceptSTUB();
BOOL __fastcall GamePacketReceivedIntercept2();
void	GamePacketReceivedIntercept2STUB();
DWORD __fastcall BnetPacketReceivedIntercept(BYTE* aPacket, DWORD aLength, DWORD connStruct);
void	BnetPacketReceivedSaveSTUB();
void	BnetPacketReceivedInterceptSTUB();

// GamePacketSent.cpp
DWORD __fastcall GamePacketSentIntercept(BYTE* aPacket, DWORD aLength);
void	GamePacketSentInterceptSTUB();
DWORD __fastcall BnetPacketSentIntercept(BYTE* aPacket, DWORD aLength, DWORD connStruct);
void	BnetPacketSentInterceptSTUB();

// GameSendPacketToServer.cpp

// OtherExportedFunctions.cpp

// HelperFunctions.cpp
VOID*	PRIVATE d2memcpy(DWORD lpDest, DWORD lpSource, int len);
VOID*	PRIVATE GetFuncAddress(VOID* addr);

// GameCommandLine.cpp
extern GAMECOMMANDSTRUCT		GameCommands[];
GAMECOMMANDSTRUCT* PRIVATE FindGameCommand(char* name);

BOOL	PRIVATE LoadGroupList(char* list, char* name);

// GamePlayerInfoIntercept.cpp
VOID __fastcall GamePlayerInfoIntercept(PLAYERINFOSTRUCT* pis);
VOID			GamePlayerInfoInterceptSTUB();

// TickThread.cpp
DWORD WINAPI	TickThread(LPVOID lpParameter);

// AntiDetection
DWORD __stdcall GlobalGetMappingAddress(DWORD addr);
void AntiDetectionPacket64STUB();
void AntiDetectionPacket65STUB();
HMODULE __stdcall AntiDetectionVersionCheckingDllSTUB();
HMODULE __stdcall AntiDetectionExtraWorkDllSTUB();

// GameKeyDown.cpp
DWORD __fastcall GameKeyDownIntercept(DWORD aKeyCode);
void GameKeyDownInterceptSTUB();


// Debug helpers
typedef struct regs_t
{
	DWORD eax;
	DWORD ebx;
	DWORD ecx;
	DWORD edx;
	DWORD esp;
	DWORD ebp;
	DWORD esi;
	DWORD edi;
} REGS;

#define SHOW_REGS(x)	REGS __regs;\
	GamePrintInfo("Debug: ÿc4" x);\
	__asm {mov __regs.eax, eax}\
	__asm {mov __regs.ebx, ebx}\
	__asm {mov __regs.ecx, ecx}\
	__asm {mov __regs.edx, edx}\
	__asm {mov __regs.esp, esp}\
	__asm {mov __regs.ebp, ebp}\
	__asm {mov __regs.esi, esi}\
	__asm {mov __regs.edi, edi}\
	char __regs_text[128];\
	sprintf_s(__regs_text, "eax: ÿc4%.8xÿc0 ebx: ÿc4%.8xÿc0 ecx: ÿc4%.8xÿc0 edx: ÿc4%.8xÿc0", __regs.eax, __regs.ebx, __regs.ecx, __regs.edx);\
	GamePrintInfo(__regs_text);\
	sprintf_s(__regs_text, "esp: ÿc4%.8xÿc0 ebp: ÿc4%.8xÿc0 esi: ÿc4%.8xÿc0 edi: ÿc4%.8xÿc0", __regs.esp, __regs.ebp, __regs.edi, __regs.edi);\
	GamePrintInfo(__regs_text);\

