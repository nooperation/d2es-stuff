/*
  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//define the functions that call ingame funcs
//util functions found in utilfuncs.cpp

// d2 function libraries:
#include "d2ptrs.h"

////////////////////////////////////////////////////////////
// Originally Mousepad & D2BS Code
////////////////////////////////////////////////////////////
DWORD GetDllOffset(char *dll, int offset);
DWORD GetDllOffset(int num);

void DefineOffsets()
{
	DWORD *p = (DWORD *)&D2COMMON_START;
	do {
		*p++ = GetDllOffset(*p);
	} while (p <= (DWORD *)&D2COMMON_END);

	p = (DWORD *)&D2CLIENT_START;
	do {
		*p++ = GetDllOffset(*p);
	} while (p <= (DWORD *)&D2CLIENT_END);

	p = (DWORD *)&D2OTHER_START;
	do {
		*p++ = GetDllOffset(*p);
	} while (p <= (DWORD *)&D2OTHER_END);
}

/************************************************************/
DWORD GetDllOffset(char *dll, int offset)
{
	HMODULE hmod = GetModuleHandle(dll);

	// restore mousepad's old code...
	//if (!hmod)
	//	hmod = LoadLibrary(dll);

	if (!hmod) return 0;
	
	if (offset < 0) {
		return (DWORD)GetProcAddress(hmod, (LPCSTR)-offset);
	}
	return ((DWORD)hmod)+offset;
}

DWORD GetDllOffset(int num)
{
	static char *dlls[] = {"D2CLIENT.DLL", "D2COMMON.DLL", "D2GFX.DLL", "D2LANG.DLL", 
			       "D2WIN.DLL", "D2NET.DLL", "D2GAME.DLL", "D2LAUNCH.DLL", "FOG.DLL", "BNCLIENT.DLL",
					"STORM.DLL"};
	return GetDllOffset(dlls[num&0xff], num>>8);
}

