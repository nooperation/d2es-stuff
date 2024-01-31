#pragma once 

#include <cstdint>

#pragma pack(1)

struct D2ArmTypeTxt
{
	char szName[32];					//0x00
	char szToken[20];					//0x20
};

struct D2PlrModeTypeTxt
{
	char szName[32];					//0x00
	char szToken[20];					//0x20
};

struct D2PlrModeDataTbl								//sgptDataTable + 0x10D4
{
	int nPlrModeTypeTxtRecordCount;					//0x00
	D2PlrModeTypeTxt* pPlrModeTypeTxt;				//0x04
	D2PlrModeTypeTxt* pPlayerType;					//0x08
	D2PlrModeTypeTxt* pPlayerMode;					//0x0C
};

struct D2MonModeTxt
{
	char szName[32];					//0x00
	uint32_t dwToken;					//0x20
	uint8_t nDTDir;						//0x24
	uint8_t nNUDir;						//0x25
	uint8_t nWLDir;						//0x26
	uint8_t nGHDir;						//0x27
	uint8_t nA1Dir;						//0x28
	uint8_t nA2Dir;						//0x29
	uint8_t nBLDir;						//0x2A
	uint8_t nSCDir;						//0x2B
	uint8_t nS1Dir;						//0x2C
	uint8_t nS2Dir;						//0x2D
	uint8_t nS3Dir;						//0x2E
	uint8_t nS4Dir;						//0x2F
	uint8_t nDDDir;						//0x30
	uint8_t nKBDir;						//0x31
	uint8_t nSQDir;						//0x32
	uint8_t nRNDir;						//0x33
};

struct D2MonModeDataTbl								//sgptDataTable + 0x10B0
{
	int nMonModeTxtRecordCount;						//0x00
	D2MonModeTxt* pMonModeTxt;						//0x04
	D2MonModeTxt* pMonMode[2];						//0x08
};

struct D2ObjModeTypeTxt
{
	char szName[32];					//0x00
	char szToken[20];					//0x20
};

struct D2ObjModeDataTbl								//sgptDataTable + 0x10C4
{
	int nObjModeTypeTxtRecordCount;					//0x00
	D2ObjModeTypeTxt* pObjModeTypeTxt;				//0x04
	D2ObjModeTypeTxt* pObjType;						//0x08
	D2ObjModeTypeTxt* pObjMode;						//0x0C
};


struct D2CompositTxt
{
	char szName[32];						//0x00
	char szToken[20];						//0x20
};

struct D2CompCodeTxt
{
	uint32_t dwCode;						//0x00
};

#pragma pack()
