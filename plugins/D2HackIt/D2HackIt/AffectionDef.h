///////////////////////////////////////////////////////////
// AffectionDef.h
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////
#ifndef __AFFECTIONDEF_H__
#define __AFFECTIONDEF_H__

#include <windows.h>

DWORD D2GetAffectionName(BYTE iAffectionID, LPTSTR lpszBuffer, DWORD dwMaxChars);
BOOL D2IsAura(BYTE iAffectionID);

#endif // __AFFECTIONDEF_H__
