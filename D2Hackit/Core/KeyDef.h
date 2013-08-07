//////////////////////////////////////////////////////////////////////////
// KeyDef.h
//
// Retrieves key name by virtual key code.
//
// Written by Abin (abinn32@yahoo.com)
// 7/26/2004
//////////////////////////////////////////////////////////////////////////

#ifndef __KEYDEF_H__
#define __KEYDEF_H__

#include <windows.h>

DWORD D2GetKeyName(BYTE iVKCode, LPSTR lpszBuffer, DWORD dwMaxChars);

#endif // __KEYDEF_H__