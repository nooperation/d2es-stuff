//////////////////////////////////////////////////////////////////////
// OtherExportedFunctions.cpp
// -------------------------------------------------------------------
// Misc. stuff that doesn't really go anywhere else.
//
// <thohell@home.se>
//////////////////////////////////////////////////////////////////////
#include "D2HackItServer.h"

PTHISGAMESTRUCT EXPORT GetThisgameStruct(void)
{
	return thisgame;
}

HWND EXPORT FindMyD2WndHandle()
{
	return g_hD2;
}

extern DWORD g_dwClientVer;

PSERVERINFO EXPORT GetServerInfo(DWORD dwVersion, LPCSTR szModule)
{
	char tbf[256];

	DGTRACE((DBGHDR "GetServerInfo(.%d.%.2d, %s)\n", LOWORD(dwVersion), HIWORD(dwVersion), szModule));

	if(dwVersion < __SERVERVERSION__)	// if calling module is older than this server
	{
		_snprintf(tbf, sizeof(tbf), "[%s] is built on D2HackIt Ver.%d.%.2d. (Server = Ver.%d.%.2d)",
			szModule,
			LOWORD(dwVersion), HIWORD(dwVersion), 
			LOWORD(__SERVERVERSION__), HIWORD(__SERVERVERSION__));
		GamePrintVerbose(tbf);
	}
	else if(dwVersion > __SERVERVERSION__)	// if calling module is newer than this server
	{
		// This is warning !!!
		_snprintf(tbf, sizeof(tbf), "ÿc1[%s] is built on D2HackIt Ver.%d.%.2d. But your D2HackIt is Ver.%d.%.2d",
			szModule,
			LOWORD(dwVersion), HIWORD(dwVersion), 
			LOWORD(__SERVERVERSION__), HIWORD(__SERVERVERSION__));
		GamePrintInfo(tbf);
	}

	g_dwClientVer = dwVersion;

	return si;
}

BOOL EXPORT GameSaveAndExit(void)
{
	if(psi->GameSaveAndExitLocation)
	{
		((VOID (PRIVATE *)())psi->GameSaveAndExitLocation)();
		return TRUE;
	}

	GamePrintError("GameSaveAndExit not available");
	return FALSE;
}

DWORD FormatPacket(BOOL bReceived, const BYTE *aPacket, DWORD aLen, LPSTR lpszBuffer, DWORD dwMaxChars, BOOL bUseColor)
{
	if (lpszBuffer == NULL)
		return 0;

	lpszBuffer[0] = '\0';
	if (aPacket == NULL || aLen == 0)
		return 0;

	char sz[64] = "";

	if (bUseColor)
	{
		if (bReceived)
			sprintf(sz, "ÿc4R ÿc2%3dÿc4:ÿc0", aLen);
		else
			sprintf(sz, "ÿc4S ÿc2%3dÿc4:ÿc0", aLen);
	}
	else
	{
		if (bReceived)
			sprintf(sz, "R %3d:", aLen);
		else
			sprintf(sz, "S %3d:", aLen);
	}
	

	DWORD dwCopied = strlen(sz);
	if (dwCopied > dwMaxChars)
		return 0;

	strcat(lpszBuffer, sz);

	for (DWORD i = 0; i < aLen && dwCopied + 3 <= dwMaxChars; i++)
	{
		sprintf(sz, "  %02X", aPacket[i]);
		strcat(lpszBuffer, sz);
		dwCopied += 3;
	}

	return dwCopied;
}

BOOL EXPORT GamePrintPacket(BOOL bReceived, const BYTE *aPacket, DWORD aLen)
{
	char szBuffer[1024] = "";
	if (!FormatPacket(bReceived, aPacket, aLen, szBuffer, 1023, TRUE))
		return FALSE;

	return GamePrintInfo(szBuffer);
}