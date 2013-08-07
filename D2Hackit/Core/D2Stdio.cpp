///////////////////////////////////////////////////////////
// D2Stdio.cpp
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////
#include <stdio.h>
#include <string.h>
#include "D2Hackit.h"

BOOL EXPORT GameErrorf(LPCSTR lpszFormat, ...)
{
	if (lpszFormat == NULL || lpszFormat[0] == '\0')
		return FALSE;

	char szString[256] = "";
	va_list args;
	va_start(args, lpszFormat);
	_vsnprintf(szString, 255, lpszFormat, args);
	va_end(args);
	return GamePrintError(szString);
}

BOOL EXPORT GameInfof(LPCSTR lpszFormat, ...)
{
	if (lpszFormat == NULL || lpszFormat[0] == '\0')
		return FALSE;

	TCHAR szString[256] = "";
	va_list args;
	va_start(args, lpszFormat);
	_vsnprintf(szString, 255, lpszFormat, args);
	va_end(args);
	return GamePrintInfo(szString);
}

BOOL EXPORT GameStringf(LPCSTR lpszFormat, ...)
{
	if (lpszFormat == NULL || lpszFormat[0] == '\0')
		return FALSE;

	TCHAR szString[256] = "";
	va_list args;
	va_start(args, lpszFormat);
	_vsnprintf(szString, 255, lpszFormat, args);
	va_end(args);
	return GamePrintString(szString);
}

BOOL EXPORT GameCommandf(LPCSTR lpszFormat, ...)
{
	if (lpszFormat == NULL || lpszFormat[0] == '\0')
		return FALSE;

	TCHAR szString[256] = "";
	va_list args;
	va_start(args, lpszFormat);
	_vsnprintf(szString, 255, lpszFormat, args);
	va_end(args);
	return GameCommandLine(szString);
}