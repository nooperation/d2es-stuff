//////////////////////////////////////////////////////////////////////
// IniFileHandlers.cpp
// -------------------------------------------------------------------
// Functions used for manipulating .ini files.
//
// <thohell@home.se>
//
// Modified by Dan_Goon
//////////////////////////////////////////////////////////////////////
#include "D2HackItServer.h"
#include <sys\types.h>
#include <sys\stat.h>


DWORD EXPORT GetIniFileName( LPCSTR lpHackName, LPSTR lpOutFileName, DWORD nBufferSize )
{
	bool				bFileFound = FALSE;
	char				lpFileName[MAX_PATH];
	DWORD				nFileNameSize;
	LinkedItem*			li;
	CLIENTINFOSTRUCT*	cds;

	sprintf( lpFileName, "%s\\%s.ini", si->PluginDirectory, lpHackName );

	if(_access(lpFileName, 0))
	{
		// Check in the dir of the loaded modules
		li = FindClientStructByName(lpHackName);
		if(li)
		{	
			cds = (CLIENTINFOSTRUCT*)li->GetData();
			if(cds)
			{
				sprintf( lpFileName, "%s\\%s.ini", cds->pDir, lpHackName );

				if(!_access(lpFileName, 0)) bFileFound = TRUE;
			}
		}

		if(!bFileFound)
		{
			sprintf(lpFileName, "Unable to open ini file: ÿc4%s", lpHackName);
			GamePrintError(lpFileName);
			return 0;
		}
	}

	nFileNameSize = strlen(lpFileName);

	if(lpOutFileName && nBufferSize)
	{
		strncpy(lpOutFileName, lpFileName, nBufferSize);

		if(nFileNameSize >= nBufferSize)	// if not enough buffer is supplied
		{
			lpOutFileName[nBufferSize - 1] = 0;
		}
		else
		{
			lpOutFileName[nFileNameSize] = 0;
		}
	}

	if(nFileNameSize) nFileNameSize++;	// add 1 for trailing zero
	return nFileNameSize;				// return value is read or required buffer size if not zero
}

DWORD EXPORT GetIniString(LPCSTR lpHackName, LPCSTR lpSectionName, LPCSTR lpKeyName, LPSTR lpOutBuffer, DWORD nBufferSize)
{
	char		lpFileName[MAX_PATH];
	BOOL		bSizeOnly = (BOOL)(lpOutBuffer == NULL);
	DWORD		nRead;

	if(!GetIniFileName( lpHackName, lpFileName, sizeof(lpFileName) ))
	{
		return 0;
	}

again:
	if(bSizeOnly)
	{
		nBufferSize = 4096;
		lpOutBuffer = new char[nBufferSize];

		if(!lpOutBuffer)
		{
			GamePrintError("ÿc1Buffer Allocation Failed");
			return 0;
		}
	}

	nRead = GetPrivateProfileString(lpSectionName, lpKeyName, "", lpOutBuffer, nBufferSize, lpFileName);

	if(bSizeOnly)
	{
		delete [] lpOutBuffer;
	}
	else if(nRead)
	{
		if(lpOutBuffer[nRead-1] == 0 || (nRead > 1 && lpOutBuffer[nRead-2] == 0))
		{
			bSizeOnly = TRUE;
			goto again;			// goto get required buffer size
		}

		if(nRead < nBufferSize) lpOutBuffer[nRead] = 0;
	}
	else
	{
		DGTRACE((DBGHDR "No Strings in Section [%s:%s] [%s]!\n", lpSectionName, lpKeyName, lpOutBuffer));
		lpOutBuffer[0] = 0;
	}

	if(nRead) nRead++;			// add 1 for trailing zero
	return nRead;				// return value is read or required buffer size if not zero
}


BOOL EXPORT SetHackProfileString(LPCSTR lpHackName, LPCSTR lpSectionName, LPCSTR lpKeyName, LPCSTR lpValue)
{
	char		lpFileName[MAX_PATH];

	if(!GetIniFileName( lpHackName, lpFileName, sizeof(lpFileName) ))
	{
		return FALSE;
	}

//	DGTRACE((DBGHDR "Writing Ini String : [%s]-[%s] to [%s]\n", lpSectionName, lpKeyName, lpValue));

	WritePrivateProfileString(lpSectionName, lpKeyName, lpValue, lpFileName);
	return TRUE;
}


DWORD EXPORT GetIniSectionNames( LPCSTR lpHackName, LPSTR lpOutBuffer, DWORD nBufferSize )
{
	char			lpFileName[MAX_PATH];
	struct _stat	stFileData;
	BOOL			bSizeOnly = (BOOL)(lpOutBuffer == NULL);
	DWORD			nRead;

	if(!GetIniFileName( lpHackName, lpFileName, sizeof(lpFileName) ))
	{
		return 0;
	}

	if( _stat(lpFileName, &stFileData) )
	{
		char tbf[256];
		sprintf(tbf, "Unable to find ini file: ÿc4%s", lpFileName);
		GamePrintError(tbf);
		return 0;
	}

again:
	if(bSizeOnly)
	{
		nBufferSize = stFileData.st_size;
		lpOutBuffer = new char[nBufferSize];
		if(!lpOutBuffer)
		{
			GamePrintError("ÿc1Buffer Allocation Failed");
			return 0;
		}
	}

	nRead = GetPrivateProfileSectionNames(lpOutBuffer, nBufferSize, lpFileName);

	DGTRACE((DBGHDR "nRead = %d for [%s]\n", nRead, lpHackName));

	if(bSizeOnly)
	{
		delete [] lpOutBuffer;
	}
	else if(nRead)
	{
		if(lpOutBuffer[nRead-1] == 0 || (nRead > 1 && lpOutBuffer[nRead-2] == 0))
		{
			bSizeOnly = TRUE;
			goto again;			// goto get required buffer size
		}

		if(nRead < nBufferSize)
		{
			lpOutBuffer[nRead] = 0;
			if(nRead + 1 < nBufferSize) lpOutBuffer[nRead+1] = 0;	// needs double zero termination
		}
	}
	else lpOutBuffer[0] = 0;

	if(nRead) nRead++;			// add 1 for trailing zero
	return nRead;				// return value is read or required buffer size if not zero
}


//////////////////////////////////////////////////////////////////////
// GetFingerPrint
// -------------------------------------------------------------------
// Reads szFingerPrintName from the [FingerprintData] section of the
// ini-file asociated with szHackName.
// 
// It breaks up the components of the string, does some basic sanity-
// check and calls GetMemoryAddressFromPattern() to store the address
// of the fingerprint, if found.
//////////////////////////////////////////////////////////////////////

BOOL EXPORT GetFingerprint(LPCSTR szHackName, LPCSTR szFingerprintName, FINGERPRINTSTRUCT &fps)
{
	char		lpBuffer[4096];
	int			i;
	int			nFields = 0;

	fps.AddressFound = 0;

	if(!szHackName || !szFingerprintName) goto find_direct;

	strcpy(fps.Name, szFingerprintName);

	if(!GetIniString(szHackName, "FingerprintData", szFingerprintName, lpBuffer, sizeof(lpBuffer)))
	{
		if(!psi->DontShowErrors)
		{
			sprintf(lpBuffer, "Can't find fingerprint for '%s' in '%s.ini'",
				szFingerprintName, szHackName);
			GamePrintError(lpBuffer);
		}

		return FALSE;
	}

	// Make sure we have 4 fields
	for (i=0; lpBuffer[i]; i++)
	{
		if (lpBuffer[i] == ',') nFields++;
	}

	if (nFields != 3)
	{ 
		sprintf(lpBuffer, "Fingerprint for '%s' in '%s.ini' is corrupt.",
			szFingerprintName, szHackName);
		GamePrintError(lpBuffer);
		return FALSE;
	}

	// Loop backwards to get fingerprint info
	for (; i!=0; i--)
	{
		if (lpBuffer[i] == ',')
		{
			lpBuffer[i] = 0;
			nFields--;
			switch (nFields)
			{
			case 2:
				strcpy(fps.FingerPrint, &lpBuffer[i+1]);
				break;
			case 1:
				fps.Offset = atoi(&lpBuffer[i+1]);
				break;
			case 0:
				fps.PatchSize = atoi(&lpBuffer[i+1]);
				break;
			}
		}
	}

	strcpy(fps.ModuleName, lpBuffer);

find_direct:
	if((fps.AddressFound = GetMemoryAddressFromPattern(fps.ModuleName, fps.FingerPrint, fps.Offset)) < 0x100)
	{
		if (psi->DontShowErrors) return FALSE;

		sprintf(lpBuffer, "Unable fo find location for '%s'.",
			szFingerprintName, szHackName);
		GamePrintError(lpBuffer);
		return FALSE;
	}

//	sprintf(lpBuffer, "Found '%s' at %.8x", szFingerprintName, fps.AddressFound);
//	GamePrintVerbose(lpBuffer);
	return TRUE;
}


//////////////////////////////////////////////////////////////////////
// GetHackProfileString
// -------------------------------------------------------------------
// Reads a whole profile section using GetPrivateProfileString().
// Remember that this function uses 'new char[]' to create a return
// value the application. This means it should be unitialized when 
// calling this functions. You also need to manually delete it when 
// you are done with the data if you plan on using the variable again.
//
// Failing to do this *WILL* result in memory leaks!
//
// Update: The string will be deleted on ServerStop() if you relie on
// this then you must check for the server version number v0.58 and up
//
//////////////////////////////////////////////////////////////////////
//#########################################################################
//	The Function below will be obsoleted !!!!!!!
//  =============================================
//	This is only available in temporal period for backward compatibility.
//	Do not use this function for new module design.
//#########################################################################
LPSTR EXPORT GetHackProfileString(LPCSTR lpHackName, LPCSTR lpSectionName, LPCSTR lpKeyName)
{
	DWORD	dwDummy;
	return GetHackProfileStringEx( lpHackName, lpSectionName, lpKeyName, dwDummy );
}

char	szDummyReturn = 0;

//#########################################################################
//	The Function below will be obsoleted !!!!!!!
//  =============================================
//	This is only available in temporal period for backward compatibility.
//	Do not use this function for new module design.
//#########################################################################
LPSTR EXPORT GetHackProfileStringEx(LPCSTR lpHackName, LPCSTR lpSectionName, LPCSTR lpKeyName, DWORD& cSize)
{
	LPSTR		lpReturnedString = NULL;

	// Get required buffer size
	cSize = GetIniString( lpHackName, lpSectionName, lpKeyName, lpReturnedString, 0);
	if(cSize == 0)
	{
		// This is for old&bad modules who checks only returned string value
		// and do not check if cSize==0 nor lpReturnedString == NULL!
		return &szDummyReturn;
	}

	lpReturnedString = new char[cSize];
	GetIniString( lpHackName, lpSectionName, lpKeyName, lpReturnedString, cSize);

	szBufferList.AddItem( lpReturnedString );	// Buffer used to delete this string
												// when Server exits
//	DGTRACE((DBGHDR "[%s] returns (%d) bytes : [%s]\n", lpHackName, cSize, lpReturnedString));
	return lpReturnedString;
}


//////////////////////////////////////////////////////////////////////
// GetHackProfileSectionNames()
// -------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////
//#########################################################################
//	The Function below will be obsoleted !!!!!!!
//  =============================================
//	This is only available in temporal period for backward compatibility.
//	Do not use this function for new module design.
//#########################################################################
LPSTR EXPORT GetHackProfileSectionNames( LPCSTR lpHackName, DWORD& cSize )
{
	char		tbf[128];
	LPSTR		lpReturnedString = NULL;

	_snprintf(tbf, sizeof(tbf), "[%s] calls 'GetHackProfileSectionNames' - this will be obsoleted", lpHackName);
	GamePrintVerbose(tbf);

	// Get required buffer size
	cSize = GetIniSectionNames( lpHackName, lpReturnedString, 0 );

	if(cSize == 0)
	{
		// This is for old&bad modules who checks only returned string value
		// and do not check if cSize==0 nor lpReturnedString == NULL!
		return &szDummyReturn;
	}

	lpReturnedString = new char[cSize];
	GetIniSectionNames( lpHackName, lpReturnedString, cSize );

	szBufferList.AddItem( lpReturnedString );		// Buffer used to delete this string
													// when Server exits
	DGTRACE((DBGHDR "GHPN:[%s] returns (%d) bytes : [%s]\n", lpHackName, cSize, lpReturnedString));
	return lpReturnedString;
}


BOOL EXPORT FreeHackProfileString(LPCSTR lpString)
{
	LinkedItem*	li;

	DGTRACE((DBGHDR "FreeHackProfileString is called for [%s]\n", lpString));

	if(!lpString) return FALSE;
	for(li = szBufferList.GetFirstItem(); li; li=szBufferList.GetNextItem(li))
	{
		if(lpString == (LPCSTR)li->GetData())
		{
			szBufferList.RemoveItem(li);
			return TRUE;
		}
	}
	return FALSE;
}

