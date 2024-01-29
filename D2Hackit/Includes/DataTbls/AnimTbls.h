#pragma once 

#include <cstdint>

#pragma pack(1)

struct D2AnimDataRecordStrc
{
	static const int MAX_FRAME_FLAGS = 144;
	char szAnimDataName[8];						//0x00
	uint32_t dwFrames;							//0x08
	int32_t dwAnimSpeed;						//0x0C
	uint8_t pFrameFlags[MAX_FRAME_FLAGS];		//0x10
};

struct D2AnimDataBucketStrc
{
	int32_t nbEntries;
	D2AnimDataRecordStrc aEntries[]; // Dynamically sized
};

struct D2AnimDataTableStrc
{
	void* pBinaryData;							//0x00
	D2AnimDataBucketStrc* pHashTableBucket[256];//0x04
	D2AnimDataRecordStrc tDefaultRecord;		//0x404
};

struct D2UnitStrc;
struct D2InventoryStrc;

#pragma pack()

