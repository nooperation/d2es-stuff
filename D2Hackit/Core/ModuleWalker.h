// ModuleWalker.h: interface for the CModuleWalker class.
// -------------------------------------------------------------------
// <stingxp@yahoo.com>
//////////////////////////////////////////////////////////////////////

#ifndef __MODULE_WALKER_H__
#define __MODULE_WALKER_H__

class CModuleWalker  
{
public:
	struct ModuleInfo {
		DWORD dwImageBase;
		DWORD dwImageSize;
		char szImageName[MAX_PATH];
	};
	static BOOL FindModuleByAddress(DWORD pid, DWORD addr, ModuleInfo *pmOutModuleInfo);
	static BOOL FindModuleByAddressNT(DWORD pid, DWORD addr, ModuleInfo *pmOutModuleInfo);
	static BOOL FindModuleByAddressToolhelp32(DWORD pid, DWORD addr, ModuleInfo *pmOutModuleInfo);
};

#endif // __MODULE_WALKER_H__
