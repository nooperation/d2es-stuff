// ModuleWalker.cpp: implementation of the CModuleWalker class.
// -------------------------------------------------------------------
// <stingxp@yahoo.com>
//////////////////////////////////////////////////////////////////////

#include "D2HackItServer.h"
#include "ModuleWalker.h"
#include <tlhelp32.h>

static BOOL isNT()
{
	OSVERSIONINFO osversioninfo;
	osversioninfo.dwOSVersionInfoSize = sizeof(osversioninfo);
	
	// Get the current OS version
	if (!GetVersionEx(&osversioninfo))
		return FALSE;
	return osversioninfo.dwPlatformId == VER_PLATFORM_WIN32_NT;
}

BOOL CModuleWalker::FindModuleByAddress(DWORD pid, DWORD addr, ModuleInfo *pmOutModuleInfo)
{
	if (!pmOutModuleInfo) return FALSE;
//	return (isNT() && FindModuleByAddressNT(pid, addr, pmOutModuleInfo)) || FindModuleByAddressToolhelp32(pid, addr, pmOutModuleInfo);
	return FindModuleByAddressToolhelp32(pid, addr, pmOutModuleInfo);
}

BOOL CModuleWalker::FindModuleByAddressToolhelp32(DWORD pid, DWORD addr, ModuleInfo *pmOutModuleInfo)
{
	// Typedefs for toolhelp32
	typedef BOOL (WINAPI *fnModule32First)(HANDLE hSnapshot, LPMODULEENTRY32 lpme);
	typedef BOOL (WINAPI *fnModule32Next)(HANDLE hSnapshot, LPMODULEENTRY32 lpme);
	typedef HANDLE (WINAPI *fnCreateToolhelp32Snapshot)(DWORD dwFlags, DWORD th32ProcessID);  
	
	static fnModule32First Module32First;
	static fnModule32Next Module32Next;
	static fnCreateToolhelp32Snapshot CreateToolhelp32Snapshot;
	if(!Module32First) {
		// Find out if the toolhelp API exists in kernel32
		HMODULE k32=GetModuleHandleA("kernel32.dll");
		if (!k32)
			return FALSE;
		
		Module32First = (fnModule32First)GetProcAddress(k32, "Module32First");
		Module32Next = (fnModule32Next)GetProcAddress(k32, "Module32Next");
		CreateToolhelp32Snapshot=(fnCreateToolhelp32Snapshot)GetProcAddress(k32,"CreateToolhelp32Snapshot");
		
		// Verify that the ToolHelp32 API is available
		if (!(Module32First) || !(Module32Next) || !(CreateToolhelp32Snapshot))
		{
			Module32First = NULL;
			
			return FALSE;
		}
	}
	
	
	// toolhelp code
	
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
	if ((int)hSnapshot == -1) {
		return FALSE;
	}
	
	MODULEENTRY32 lpme; lpme.dwSize = sizeof(MODULEENTRY32);
	
	// Get first module, this is needed for win9x/ME
	if (!Module32First(hSnapshot, &lpme)) {
		CloseHandle(hSnapshot);
		return FALSE;
	}
	
	// Loop through all other modules
	BOOL bFound = FALSE;
	while (TRUE)
	{
		if ((DWORD)lpme.modBaseAddr < addr && ((DWORD)lpme.modBaseAddr)+lpme.modBaseSize  > addr)
		{
			pmOutModuleInfo->dwImageBase = (DWORD)lpme.modBaseAddr;
			pmOutModuleInfo->dwImageSize = lpme.modBaseSize;
			memcpy(pmOutModuleInfo->szImageName, lpme.szExePath,  sizeof(pmOutModuleInfo->szImageName));
			bFound = TRUE;
			break;
		}
		if (!Module32Next(hSnapshot, &lpme))
		{
			break;
		}
	}
	
	CloseHandle(hSnapshot);
	return bFound;
}

BOOL CModuleWalker::FindModuleByAddressNT(DWORD pid, DWORD addr, ModuleInfo *pmOutModuleInfo)
{
	typedef struct _DEBUGMODULEINFO	{
		DWORD	ImageBase;
		DWORD	ImageSize;
		DWORD	Unknown01;			// possibly some kind of version info. nthandle doesnt use it
		USHORT	DllSequenceNum;  // if 0 then EXE
		USHORT	NumDlls;			// only know if seqnum is 0
		DWORD	GrantedAccess;
		CHAR	Name[MAX_PATH];
		DWORD	Unknown02;
	}DEBUGMODULEINFO, *PDEBUGMODULEINFO;
	
	typedef struct _QUERYDEBUGBUFFER_HEADER	{
		DWORD	Unkown12[12];
		DWORD*	ModArrayHeader;
		DWORD	Unkown11[11];
		DWORD	NumNames;         // total entries including the EXE
		DWORD	Reserved[2];
		DEBUGMODULEINFO ModInfo[1];
	}QUERYDEBUGBUFFER, *PQUERYDEBUGBUFFER;
	
	typedef DWORD* (WINAPI *PRtlCreateQueryDebugBuffer)( DWORD, DWORD );
	typedef DWORD (WINAPI *PRtlDestroyQueryDebugBuffer)( PDWORD );
	typedef DWORD (WINAPI *PRtlQueryProcessDebugInformation)(HANDLE, DWORD, PVOID );
	
	static PRtlCreateQueryDebugBuffer RtlCreateQueryDebugBuffer;
	static PRtlDestroyQueryDebugBuffer RtlDestroyQueryDebugBuffer;
	static PRtlQueryProcessDebugInformation RtlQueryProcessDebugInformation;
	if(!RtlCreateQueryDebugBuffer)
	{
		HMODULE hNtDll = GetModuleHandleA("ntdll.dll");
		if(!hNtDll)
			return FALSE;
		
		RtlCreateQueryDebugBuffer = (PRtlCreateQueryDebugBuffer)
			GetProcAddress( hNtDll,
			"RtlCreateQueryDebugBuffer" );
		
		RtlDestroyQueryDebugBuffer = (PRtlDestroyQueryDebugBuffer)
			GetProcAddress( hNtDll,
			"RtlDestroyQueryDebugBuffer" );
		
		RtlQueryProcessDebugInformation = (PRtlQueryProcessDebugInformation)
			GetProcAddress( hNtDll,
			"RtlQueryProcessDebugInformation" );
		if(!RtlCreateQueryDebugBuffer || !RtlDestroyQueryDebugBuffer || !RtlQueryProcessDebugInformation)
		{
			RtlCreateQueryDebugBuffer = NULL;
			return FALSE;
		}
	}
	
	DWORD *pRtlBuffer = RtlCreateQueryDebugBuffer(NULL, NULL);
	
	if( pRtlBuffer == NULL )
		return FALSE;
	
	
	DWORD error = RtlQueryProcessDebugInformation((HANDLE)pid, 1, pRtlBuffer);
	if( error != 0 )
	{
		RtlDestroyQueryDebugBuffer( pRtlBuffer );
		return FALSE;
	}
	
	QUERYDEBUGBUFFER *pDebugInfo = (QUERYDEBUGBUFFER*)pRtlBuffer;
	
	for(DWORD i = 0; i < pDebugInfo->NumNames; i++)
	{
		DEBUGMODULEINFO *p = &pDebugInfo->ModInfo[i];
		if (p->ImageBase < addr && p->ImageBase+p->ImageSize > addr)
		{
			pmOutModuleInfo->dwImageBase = p->ImageBase;
			pmOutModuleInfo->dwImageSize = p->ImageSize;
			memcpy(pmOutModuleInfo->szImageName, p->Name,  sizeof(pmOutModuleInfo->szImageName));
			break;
		}
	}
	RtlDestroyQueryDebugBuffer( pRtlBuffer );
	return TRUE;
}
