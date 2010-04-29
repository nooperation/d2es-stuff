//////////////////////////////////////////////////////////////////////
// AntiDetection.cpp
// -------------------------------------------------------------------
// <stingxp@yahoo.com>
//////////////////////////////////////////////////////////////////////
#include "D2HackItServer.h"
#include "d2functions.h"

#define CHECKPOINT1 0xC0B0B8A1 // D2Net_10005Entry
#define CHECKPOINT2 0x0424448B // D2Net_10021Entry
#define CHECKPOINT3 0xF18B5653 // D2Win_10129Entry
#define CHECKPOINT4 0xFA8B5756 // D2Win_10131Entry

struct I386Registers
{
	DWORD edi;
	DWORD esi;
	DWORD ebp;
	DWORD esp;
	DWORD ebx;
	DWORD edx;
	DWORD ecx;
	DWORD eax;
};

// 0x6FB4F331, 7 bytes
void __declspec(naked) AntiDetectionPacket64STUB()
{
	__asm {
		nop;								// Make room for original code
		nop;
		nop;
		nop;
		nop;
		nop;
		nop;
		add dword ptr[esp], 2;
		xor ecx, edx;
		shl ecx, 0x10;
		or ecx, edx;

// eax -- checkpoint 1
// ecx -- checkpoint 2
		pushad;

		push eax;
		call GlobalGetMappingAddress;
		mov [esp]I386Registers.eax, eax;
		push [esp]I386Registers.ecx;
		call GlobalGetMappingAddress;
		mov [esp]I386Registers.ecx, eax;
		
		popad;

		mov edx, ecx;
		and edx, 0xFFFF;
		shr ecx, 0x10;
		xor ecx, edx;

//		mov [ebp-0x1C], ecx;
		ret;
	}
}

// 0x6FB52862, 6 bytes
void __declspec(naked) AntiDetectionPacket65STUB()
{
	__asm {
		nop;
		nop;
		nop;
		nop;
		nop;
		nop;
		mov [ebp-0x20], CHECKPOINT1;
		mov edi, CHECKPOINT2;
		mov esi, CHECKPOINT3;
		mov [ebp-0x1C], esi;
		mov eax, CHECKPOINT4;
		mov [ebp-0x24], eax;

		// original code
		mov ecx, eax;
		and ecx, 0xFFFF;
		ret;
	}
}

/*
HMODULE __stdcall AntiDetectionSuspiciousDll(BYTE flag, LPCSTR lpDllFileName)
{
	typedef HMODULE (__stdcall *LoadLibraryA_t)(LPCSTR);
	if (flag == 0)
		return ((LoadLibraryA_t)*p_BNCLIENT_fnLoadLibraryA)(lpDllFileName);
	if (flag == (BYTE)-1)
	{
		char szPath[MAX_PATH];
		strcpy(szPath, lpDllFileName);
		strcat(szPath, ".bak");
		CopyFileA(lpDllFileName, szPath, FALSE);
	}
	return 0;
}

HMODULE __declspec(naked) __stdcall AntiDetectionSuspiciousDllSTUB()
{
	__asm
	{
		push [esp+4];
		push eax;
		call AntiDetectionSuspiciousDll;
		test eax, eax;
		jz unloadmyself;
		ret 4;
unloadmyself:
		add dword ptr [esp], 1;	// adjust return address
		push g_hD2HackIt;			// parameter of FreeLibrary
		// d2loader patchs the imported address of LoadLibraryA in the IAT of BNCLIENT.DLL.
		// we must let it do the loading job in that case.
		mov eax, dword ptr [p_BNCLIENT_fnLoadLibraryA];
		push dword ptr [eax]; // return address of FreeLibrary
		push FreeLibrary;		// function to call
		ret;					// call FreeLibrary to unload myself
	}
}

// Ideally we should manually check all of version-checking&extrawork dlls one by one and
// label them as safe or not safe. But, I'm toooooooo lazy. &_+
// 0x6FF045AB, 6 bytes
HMODULE __declspec(naked) __stdcall AntiDetectionVersionCheckingDllSTUB(LPCSTR lpFileName)
{
	__asm
	{
		mov eax, 0; // keep me loaded, i.e. assuming version checking dlls are safe.
		jmp AntiDetectionSuspiciousDllSTUB;
	}
}

// 0x6FF0390B, 6 bytes
HMODULE __declspec(naked) __stdcall AntiDetectionExtraWorkDllSTUB(LPCSTR lpFileName)
{
	__asm
	{
		mov eax, 1; // set me free, i.e. assuming all extrawork dlls are NOT safe.
		jmp AntiDetectionSuspiciousDllSTUB;
	}
}
*/

BOOL __cdecl AntiDetectionSuspiciousDll(BYTE flag, LPCSTR lpDllFileName)
{
	if (flag == 0)
		return TRUE;
	if (flag == (BYTE)-1)
	{
		char szPath[MAX_PATH];
		strcpy(szPath, lpDllFileName);
		strcat(szPath, ".bak");
		CopyFileA(lpDllFileName, szPath, FALSE);
	}
	return FALSE;
}

HMODULE __declspec(naked) __stdcall AntiDetectionSuspiciousDllSTUB()
{
	__asm
	{
		call AntiDetectionSuspiciousDll;
		test eax, eax;
		pop eax; // flag
		pop ecx; // lpDllFileName
		pop eax; // ret addr
		push ecx; // lpDllFileName for LoadLibraryA
		jz unloadmyself;
		jmp eax;
unloadmyself:
		push g_hD2HackIt;			// parameter of FreeLibrary
		// d2loader patchs the imported address of LoadLibraryA in the IAT of BNCLIENT.DLL.
		// we must let it do the loading job in that case.
		push eax;
		push FreeLibrary;		// function to call
		ret;					// call FreeLibrary to unload myself
	}
}

// 0x6FF045A3, 8 bytes
HMODULE __declspec(naked) __stdcall AntiDetectionVersionCheckingDllSTUB()
{
	__asm
	{
		nop;
		nop;
		nop;
		nop;
		nop;
		nop;
		nop;
		nop;
		add dword ptr[esp], 4; // lpDllFileName
		add dword ptr[esp+4], 3; // retaddr
		push 0; // keep me loaded, i.e. assuming version checking dlls are safe.
		jmp AntiDetectionSuspiciousDllSTUB;
	}
}

// 0x6FF038E2, 5 bytes
HMODULE __declspec(naked) __stdcall AntiDetectionExtraWorkDllSTUB()
{
	__asm
	{
		nop;
		nop;
		nop;
		nop;
		nop;
		add dword ptr[esp], 4;
		push 1; // set me free, i.e. assuming all extrawork dlls are NOT safe.
		jmp AntiDetectionSuspiciousDllSTUB;
	}
}

// test
/*
DWORD __declspec(naked) __fastcall CastAddress(DWORD addr)
{
	__asm
	{
		mov edx, ecx;
		and edx, 0xFFFF;
		xor eax, eax;
		shr ecx, 0x10;
		mov ax, cx;
		xor eax, edx;
		shl eax, 0x10;
		or eax, edx;
		ret;
	}
}
void TestAntiDetectionRoutine64()
{
	DWORD* pgdwDetectResult1 = (DWORD*)0x6FBBC244;
	DWORD* pgdwDetectResult2 = (DWORD*)0x6FBBC248;
	DWORD* pgdwGameType = (DWORD*)0x6FBA7960;
	DWORD oldgt = *pgdwGameType;
	*pgdwGameType = 2;

	void (*fnCheckDetectResult)() = (void (*)())0x6FB4F2D0;

	DWORD aD2HackitPatchs[] = {
		0x100, 0x6FAA1C5A, 0x6FB4F33D, 0x6FB52862, 0x6FF045AB, 0x6FF0390B
	};
	// d2loader patch points
	DWORD aD2LoaderPatchs[] = {0x6FF160C4, 0x6FF160D4, 0x6FA7C144};
	
	for( int i = 0; i < sizeof(aD2HackitPatchs)/sizeof(aD2HackitPatchs[0]); i++)
	{
		DWORD mixaddr = CastAddress(aD2HackitPatchs[i]);
		*pgdwDetectResult1 = mixaddr;
		*pgdwDetectResult2 = CastAddress(aD2LoaderPatchs[i%3]);
		fnCheckDetectResult();
	}
	*pgdwGameType = oldgt;
}

void TestAntiDetectionRoutine65()
{
	DWORD* gdwCheckResultFlag2 = (DWORD*)0x6FBBD470;
	DWORD* pgdwGameType = (DWORD*)0x6FBA7960;
	DWORD oldgt = *pgdwGameType;
	*pgdwGameType = 2;
	*gdwCheckResultFlag2 = 0x4000;

	void (*fnCheckDetectResult)() = (void (*)())0x6FB527A0;

	fnCheckDetectResult();
	*pgdwGameType = oldgt;
}

void TestAntiDetectionRoutines()
{
	TestAntiDetectionRoutine64();
	TestAntiDetectionRoutine65();
}
*/