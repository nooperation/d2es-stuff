/////////////////////////////////////////////////////////////////////////////
// D2HackItHelperFunctions.cpp
// --------------------------------------------------------------------------
//
// Helper functions for D2HackIt.dll
// 
// <thohell@home.se>
/////////////////////////////////////////////////////////////////////////////
#include "D2HackItServer.h"

/////////////////////////////////////////////////////////////////////////////
// overloaded memcpy()
// --------------------------------------------------------------------------
// Takes care of giving the regions of memory the righ access rights needed
// when copying blocks of data.
/////////////////////////////////////////////////////////////////////////////
VOID* PRIVATE d2memcpy(DWORD lpDest, DWORD lpSource, int len)
{
	DWORD oldSourceProt,oldDestProt=0;
	VirtualProtect((void*)lpSource,len,PAGE_EXECUTE_READWRITE,&oldSourceProt);
 	VirtualProtect((void*)lpDest,len,PAGE_EXECUTE_READWRITE,&oldDestProt);
	memcpy((void*)lpDest,(void*)lpSource,len);
	VirtualProtect((void*)lpDest,len,oldDestProt,&oldDestProt);
	VirtualProtect((void*)lpSource,len,oldSourceProt,&oldSourceProt);
	return (void*)lpDest;
};

VOID* PRIVATE GetFuncAddress(VOID* addr)
{
#ifdef _DEBUG
	//check if instruction is relative jump (E9)
	if (0xE9 != *((UCHAR*)addr))
		return addr;
	// calculate base of relative jump
	ULONG base = (ULONG)((UCHAR*)addr + 5);
	// calculate offset 
	ULONG *offset = (ULONG*)((UCHAR*)addr + 1);
	return (PVOID)(base + *offset);
#else
	return addr;
#endif // _DEBUG
}

/////////////////////////////////////////////////////////////////////////////
// Intercept()
// --------------------------------------------------------------------------
// Intercepts a code location to route it somewhere else.
// Also inserts original code at the destination.
/////////////////////////////////////////////////////////////////////////////
BOOL EXPORT Intercept(int instruction, DWORD lpSource, DWORD lpDest, int len)
{
//	DGTRACE((DBGHDR "Intercept(%X, %X, %d)...\n", lpSource, lpDest, len));

	char t[1024];
	sprintf(t,"Code at %.8x intercepted and routed to %.8x",lpSource,lpDest);
	GamePrintVerbose(t);

	BYTE* buffer = new BYTE[len];
	buffer[0] = instruction;
	*(DWORD*) (buffer + 1) = lpDest - (lpSource + 5);
	memset(buffer + 5, 0x90, len - 5);		// nops
	d2memcpy(lpDest, lpSource, len);		// Patch in old code to new location
	d2memcpy(lpSource, (DWORD) buffer, len);// Route old location to new
	delete [] buffer;

	return TRUE;
}

BOOL DgFindFile(LPSTR szDest, LPCSTR szPath, LPCSTR szFileName, int nDestSize);

/////////////////////////////////////////////////////////////////////////////
// D2Findfile()
// --------------------------------------------------------------------------
// Searchs the nested directories where D2Hackir is at for the szFileName
// szDest is the first path matching szFileName
// Return Value is True if Successful or False if not
// If the file isn't found szDest is the Error Message
// -D'Flame 08-20-2002
// <<<<======== This function is totally modified by Dan_Goon (V0.80)
/////////////////////////////////////////////////////////////////////////////
BOOL EXPORT D2FindFile(LPSTR szDest, LPCSTR szFileName, int nDestSize)
{
	BOOL bRet = DgFindFile(szDest, si->PluginDirectory, szFileName, nDestSize);
	if(!bRet)
	{
		_snprintf(szDest, nDestSize, "File not found! : [%s]", szFileName);
	}
	return bRet;
}

BOOL DgFindFile(LPSTR szDest, LPCSTR szPath, LPCSTR szFileName, int nDestSize)
{
	_snprintf(szDest, nDestSize, "%s\\%s", szPath, szFileName);
	if(!_access(szDest, 0)) return TRUE;

	_finddata_t		fdPlugin;
	long			hFile;
	char			szTemp[_MAX_PATH];

	DGTRACE((DBGHDR "Module [%s] not found in D2HackIt folder.\n", szDest));

	sprintf(szTemp, "%s\\*.*", szPath);
	hFile = _findfirst(szTemp, &fdPlugin);		// Start gathering Dirs
	if(hFile < 0) return FALSE;

	while(1)
	{
		if(fdPlugin.attrib & _A_SUBDIR)
		{
			if(_stricmp(fdPlugin.name, ".") && _stricmp(fdPlugin.name, ".."))
			{
				sprintf(szTemp, "%s\\%s", szPath, fdPlugin.name);

				DGTRACE((DBGHDR "Searching in subfolder (%s)..\n", szTemp));

				if(DgFindFile(szDest, szTemp, szFileName, nDestSize))
				{
					DGTRACE((DBGHDR "Module File Found! (%s)..\n", szDest));
					_findclose(hFile);
					return TRUE;
				}
			}
		}

		if(_findnext(hFile, &fdPlugin)) break;
	}

	DGTRACE((DBGHDR "No subfolders!\n"));
	_findclose(hFile);
	return FALSE;
}

