//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////

#include "D2HackItServer.h"
#include "ModuleWalker.h"
#include "MapDll.h"

#define NTSIGNATURE(ptr) ((LPVOID)((BYTE *)(ptr) + ((PIMAGE_DOS_HEADER)(ptr))->e_lfanew))
#define SIZE_OF_NT_SIGNATURE (sizeof(DWORD))
#define PEFHDROFFSET(ptr) ((LPVOID)((BYTE *)(ptr)+((PIMAGE_DOS_HEADER)(ptr))->e_lfanew+SIZE_OF_NT_SIGNATURE))
#define OPTHDROFFSET(ptr) ((LPVOID)((BYTE *)(ptr)+((PIMAGE_DOS_HEADER)(ptr))->e_lfanew+SIZE_OF_NT_SIGNATURE+sizeof(IMAGE_FILE_HEADER)))
#define SECHDROFFSET(ptr) ((LPVOID)((BYTE *)(ptr)+((PIMAGE_DOS_HEADER)(ptr))->e_lfanew+SIZE_OF_NT_SIGNATURE+sizeof(IMAGE_FILE_HEADER)+sizeof(IMAGE_OPTIONAL_HEADER)))
#define RVATOVA(base,offset) ((LPVOID)((DWORD)(base)+(DWORD)(offset)))
#define VATORVA(base,offset) ((LPVOID)((DWORD)(offset)-(DWORD)(base)))

#pragma pack(push,1)

typedef struct {
	DWORD	dwPageRVA;
	DWORD	dwBlockSize;
} IMAGE_FIXUP_BLOCK, *PIMAGE_FIXUP_BLOCK;

typedef struct {
	WORD	offset:12;
	WORD	type:4;
} IMAGE_FIXUP_ENTRY, *PIMAGE_FIXUP_ENTRY;

#pragma pack(pop)

MapDll::MapDll(void)
			:m_pMappedDllItems(NULL)
{
}

MapDll::~MapDll(void)
{
	MappedDllItem *p = m_pMappedDllItems;
	while (p)
	{
		MappedDllItem *p1 = p->next;
		delete p;
		p = p1;
	}
}

MapDll::MappedDllItem::~MappedDllItem()
{
	if(mirroredImageBase) VirtualFree((LPVOID)mirroredImageBase, 0, MEM_RELEASE);
}

DWORD __stdcall GlobalGetMappingAddress(DWORD addr)
{
	static MapDll md;
	return md.GetMappingAddress(addr);
}

DWORD __stdcall GlobalGetMappingData(DWORD addr)
{
	return *(LPDWORD)GlobalGetMappingAddress(addr);
}

DWORD MapDll::GetMappingAddress(DWORD addr)
{
	if (IsBadReadPtr((LPVOID)addr, sizeof(DWORD))) return addr;

	MappedDllItem* pdi = FindModuleByAddress(addr);
	if(pdi != NULL)
		return (pdi->mirroredImageBase + (addr-pdi->originalImageBase));
	
	
	CModuleWalker::ModuleInfo mi;
	if(!CModuleWalker::FindModuleByAddress(GetCurrentProcessId(), addr, &mi))
		return addr;
	
	DWORD mirroredImageBase = LoadMappingImage(mi.szImageName, (LPVOID)mi.dwImageBase);
	if(mirroredImageBase)
	{
		MappedDllItem *pNewModule = new MappedDllItem;
		pNewModule->mirroredImageBase = mirroredImageBase;
		pNewModule->originalImageBase = mi.dwImageBase;
		pNewModule->imageSize = mi.dwImageSize;
		pNewModule->next = m_pMappedDllItems;
		m_pMappedDllItems = pNewModule;
	}
	
	return mirroredImageBase ? (mirroredImageBase + (addr-mi.dwImageBase)) : addr;
}

MapDll::MappedDllItem* MapDll::FindModuleByAddress(DWORD addr)
{
	for (MappedDllItem *p = m_pMappedDllItems; p != NULL; p = p->next)
	{
		if(p->originalImageBase < addr && p->originalImageBase+p->imageSize > addr)
		{
			return p;
		}
	}
	return NULL;
}

DWORD MapDll::LoadMappingImage(LPCSTR lpFileName, LPVOID pOldDLLImage)
{
	HANDLE hfile=CreateFileA(lpFileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
	if(hfile==INVALID_HANDLE_VALUE) return 0;

	HANDLE hmapping;
	hmapping=CreateFileMapping(hfile, NULL, PAGE_READONLY, 0, 0, NULL);

	CloseHandle(hfile);


	void *baseaddr;
	baseaddr=MapViewOfFile(hmapping,FILE_MAP_READ,0,0,0);
	if(baseaddr==NULL) {
		CloseHandle(hmapping);
		return 0;
	}
	PIMAGE_OPTIONAL_HEADER poh;

	poh=(PIMAGE_OPTIONAL_HEADER) OPTHDROFFSET(baseaddr);

	LPVOID pMemoryImage = VirtualAlloc(NULL, poh->SizeOfImage, MEM_COMMIT, PAGE_READWRITE);
	if(pMemoryImage)
	{
		if(!MapDLLFromImage(baseaddr, pMemoryImage, pOldDLLImage) || !FixupDLLImage(pMemoryImage, pOldDLLImage))
		{
			VirtualFree(pMemoryImage, 0, MEM_RELEASE);
			pMemoryImage = NULL;
		}
	}

	UnmapViewOfFile(baseaddr);
	CloseHandle(hmapping);

	return (DWORD)pMemoryImage;
}

BOOL MapDll::MapDLLFromImage(void *pDLLFileImage, void *pMemoryImage, void *pOldDLLImage)
{
	PIMAGE_FILE_HEADER pfh;
	int nSectionCount;

	pfh=(PIMAGE_FILE_HEADER) PEFHDROFFSET(pDLLFileImage);
	nSectionCount=pfh->NumberOfSections;

	PIMAGE_OPTIONAL_HEADER poh;
	DWORD hdrlen;

	poh=(PIMAGE_OPTIONAL_HEADER) OPTHDROFFSET(pDLLFileImage);
	hdrlen=poh->SizeOfHeaders;

	memcpy(pMemoryImage, pOldDLLImage, poh->SizeOfImage);
	memcpy(pMemoryImage,pDLLFileImage,hdrlen);

	int i;
	PIMAGE_SECTION_HEADER psh;

	psh=(PIMAGE_SECTION_HEADER) SECHDROFFSET(pDLLFileImage);
	for(i=0;i<nSectionCount;i++) {
		void *secMemAddr, *secFileAddr;

		secMemAddr  = (char *)pMemoryImage + psh->VirtualAddress;
		secFileAddr = (char *)pDLLFileImage + psh->PointerToRawData;

		// only recovery code section here
		// can check more section if neccessary
		if((psh->Characteristics & IMAGE_SCN_MEM_WRITE)==0)//IMAGE_SCN_MEM_EXECUTE)
		{
			memcpy(secMemAddr,secFileAddr,psh->SizeOfRawData);
		}

		psh++;
	}

	return TRUE;
}

BOOL MapDll::FixupDLLImage(void *pMemoryImage, void* pOldDLLImage)
{
	PIMAGE_OPTIONAL_HEADER   poh;
	PIMAGE_SECTION_HEADER    psh;
	poh = (PIMAGE_OPTIONAL_HEADER)OPTHDROFFSET (pMemoryImage);
	psh = (PIMAGE_SECTION_HEADER)SECHDROFFSET (pMemoryImage);


	int nDirCount;
	nDirCount=poh->NumberOfRvaAndSizes;
	if(nDirCount<16) return FALSE;

	//-----------------------------------------------
	// Process import table
	//-----------------------------------------------
	if(poh->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size!=0)
	{
		PIMAGE_IMPORT_DESCRIPTOR pid;
		pid=(IMAGE_IMPORT_DESCRIPTOR *)RVATOVA(pMemoryImage,poh->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

		PIMAGE_IMPORT_DESCRIPTOR pid2;
		pid2=(IMAGE_IMPORT_DESCRIPTOR *)RVATOVA(pOldDLLImage,poh->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

		//---------------------------------------------------
		// For all imported DLLs
		// OriginalFirstThunk is always zero for image generated by borland's linker
		//---------------------------------------------------
		while( 1 )
		{
			if ( (pid->TimeDateStamp==0 ) && (pid->Name==0) )
				break;

			char *svDllName;
			svDllName=(char *) RVATOVA(pMemoryImage,pid->Name);

			HMODULE hDll;
			hDll=GetModuleHandleA(svDllName);
			if(hDll==NULL) hDll=LoadLibraryA(svDllName);

			if(hDll==NULL)
				return FALSE;


			pid->ForwarderChain=pid2->ForwarderChain;

			pid->TimeDateStamp=pid2->TimeDateStamp;

			//-------------------------------------------------------------
			// Fill in Import Address Table
			//-------------------------------------------------------------
			PIMAGE_THUNK_DATA ptd_in,ptd_out;
			ptd_in=(PIMAGE_THUNK_DATA) RVATOVA(pMemoryImage, pid->OriginalFirstThunk);
			ptd_out=(PIMAGE_THUNK_DATA) RVATOVA(pMemoryImage, pid->FirstThunk);
			if( pid->OriginalFirstThunk == 0 )
			{
				if( pid->FirstThunk == 0 )
					break;

				ptd_in = ptd_out;
			}

			while(ptd_in->u1.Function!=NULL)
			{
				FARPROC func;

				if(ptd_in->u1.Ordinal & 0x80000000) {
					// Ordinal
					func=GetProcAddress(hDll,MAKEINTRESOURCEA(ptd_in->u1.Ordinal));
				} else {
					// Function name
					PIMAGE_IMPORT_BY_NAME pibn;
					pibn=(PIMAGE_IMPORT_BY_NAME) RVATOVA(pMemoryImage,ptd_in->u1.AddressOfData);
					func=GetProcAddress(hDll,(char *)pibn->Name);
				}

				if(func==NULL)
					return FALSE;

				*(FARPROC*)&ptd_out->u1.Function = func;

				ptd_in++;
				ptd_out++;
			}

			pid++;
			pid2++;
		}
	}


	//-------------------------------------------------------------
	// Process relocation tables if necessary
	//-------------------------------------------------------------


	//-------------------------------------------------------------
	// Calculate fixup delta
	//-------------------------------------------------------------
	DWORD delta;
	delta=(DWORD)pOldDLLImage - (DWORD)poh->ImageBase;

	if((delta!=0) && (poh->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size!=0))
	{
		PIMAGE_FIXUP_BLOCK pfb;
		pfb=(PIMAGE_FIXUP_BLOCK)RVATOVA(pMemoryImage,poh->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);

		PIMAGE_FIXUP_BLOCK pfb2;
		pfb2=(PIMAGE_FIXUP_BLOCK)RVATOVA(pOldDLLImage,poh->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);

		//-------------------------------------------------------------
		// For each fixup block
		//-------------------------------------------------------------
		while(pfb->dwPageRVA!=0)
		{
			PIMAGE_FIXUP_ENTRY pfe;
			int i,count;

			count=(pfb->dwBlockSize-sizeof(IMAGE_FIXUP_BLOCK))/sizeof(IMAGE_FIXUP_ENTRY);
			pfe=(PIMAGE_FIXUP_ENTRY)((char *)pfb + sizeof(IMAGE_FIXUP_BLOCK));

			PIMAGE_FIXUP_ENTRY pfe2;
			pfe2=(PIMAGE_FIXUP_ENTRY)((char *)pfb2 + sizeof(IMAGE_FIXUP_BLOCK));

			//-------------------------------------------------------------
			// For each fixup entry, fill with checking image data
			//-------------------------------------------------------------
			for(i=0;i<count;i++)
			{
				void *fixaddr;

				fixaddr=RVATOVA(pMemoryImage,pfb->dwPageRVA + pfe->offset);
				void *fixaddr2;
				fixaddr2=RVATOVA(pOldDLLImage,pfb2->dwPageRVA + pfe2->offset);
				switch(pfe->type)
				{
				case IMAGE_REL_BASED_ABSOLUTE:
					break;
				case IMAGE_REL_BASED_HIGH:
				case IMAGE_REL_BASED_LOW:
					*((WORD *)fixaddr) = *((WORD *)fixaddr2);
					break;
				case IMAGE_REL_BASED_HIGHLOW:
					if( *((DWORD *)fixaddr) != *((DWORD *)fixaddr2))
						*((DWORD *)fixaddr) += delta;
					break;
				case IMAGE_REL_BASED_HIGHADJ:
					*((WORD *)fixaddr) = *((WORD *)fixaddr2);
					pfe++;
					pfe2++;
					break;
				default:
					return FALSE;
				}

				pfe++;
				pfe2++;
			}

			pfb=(PIMAGE_FIXUP_BLOCK)((char *)pfb + pfb->dwBlockSize);
			pfb2=(PIMAGE_FIXUP_BLOCK)((char *)pfb2 + pfb2->dwBlockSize);
		}
	}
	else
	{
		// Oh shit
		if( delta !=0 && poh->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size == 0 )
			return FALSE;
	}

	return TRUE;
}
