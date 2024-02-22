#pragma once

#include <cstdint>
#pragma pack(1)

struct D2WaypointActTableStrc
{
	bool bTableInitialized;					//0x00
	int32_t nStartLevelId;						//0x01
	int32_t nEndLevelId;						//0x05
	char nWpNo;								//0x09
	char nLastWpNo;							//0x0A
};

struct D2WaypointCoordTableStrc
{
	int32_t nX;									//0x00
	int32_t nY;									//0x04
	int32_t nTextX;								//0x08
	int32_t nTextY;								//0x0C
	int32_t nClickX;							//0x10
	int32_t nClickY;							//0x14
};

struct D2WaypointDataStrc
{
	// Called "history" in the original game
	uint16_t nFlags[8];							//0x00
};

struct D2WaypointFlagTableStrc
{
	uint16_t nArrayId;							//0x00
	uint16_t nFlag;								//0x02
};

struct D2WaypointTableStrc
{
	int32_t nLevelId;							//0x00
	bool bActivated;						//0x04
};
#pragma pack()
