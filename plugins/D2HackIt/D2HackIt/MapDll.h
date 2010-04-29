//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////

#pragma once

class MapDll
{
	struct MappedDllItem
	{
		MappedDllItem():mirroredImageBase(0), next(NULL){}
		DWORD mirroredImageBase;
		DWORD originalImageBase;
		DWORD imageSize;
		MappedDllItem *next;
		~MappedDllItem();
	};
public:
	MapDll(void);
	~MapDll(void);
	DWORD GetMappingAddress(DWORD addr);
private:
	MappedDllItem* FindModuleByAddress(DWORD addr);
	DWORD LoadMappingImage(LPCSTR lpFileName, LPVOID pOldDLLImage);
	static BOOL MapDLLFromImage(void *pDLLFileImage, void *pMemoryImage, void *pOldDLLImage);
	static BOOL FixupDLLImage(void *pMemoryImage, void* pOldDLLImage);
private:
	MappedDllItem *m_pMappedDllItems;
};

//extern DWORD __stdcall GlobalGetMappingAddress(DWORD addr);
//extern DWORD __stdcall GlobalGetMappingData(DWORD addr);
