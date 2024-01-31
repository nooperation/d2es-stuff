#pragma once 

#include <cstdint>

#pragma pack(1)

enum D2C_OverlayTypes
{
	OVERLAYTYPE_MODECHNG,		//overlay is deleted when unit mode changes
	OVERLAYTYPE_COUNTER,		//param 1 = counter. deletes overlay when counter ends
	OVERLAYTYPE_ONCE,			//overlay is deleted once animation reaches it's end
	OVERLAYTYPE_LOOP,			//loops, durrrrr
	OVERLAYTYPE_LOOPEX,			//dual overlay (param1 + param2), loops
	OVERLAYTYPE_STOP,			//freezes ate last frame. bugged
	OVERLAYTYPE_DELAYED,		//delayed loop, random frame delay
	OVERLAYTYPE_BONEARMOR,		//bone armor hardcode
	OVERLAYTYPE_AURA,			//doesn't replace overlays of this type. param1 = aura state. overlay is deleted when state is unset
	OVERLAYTYPE_AURAEX			//param1 = front overlay, param2 = back overlay, param3 = aura state, set overlay is state is set
};

struct D2OverlayTxt
{
	uint16_t wOverlay;					//0x00
	char szFilename[64];				//0x02
	uint16_t wVersion;					//0x42
	uint32_t dwFrames;					//0x44
	uint8_t nPreDraw;					//0x48
	uint8_t pad0x49[3];					//0x49
	uint32_t dw1ofN;					//0x4C
	uint8_t nDir;						//0x50
	uint8_t nOpen;						//0x51
	uint8_t nBeta;						//0x52
	uint8_t pad0x53;					//0x53
	uint32_t dwOffsetX;					//0x54
	uint32_t dwOffsetY;					//0x58
	uint32_t dwHeight[4];				//0x5C
	uint32_t dwAnimRate;				//0x6C
	uint32_t dwInitRadius;				//0x70
	uint32_t dwRadius;					//0x74
	uint32_t dwLoopWaitTime;			//0x78
	uint8_t nTrans;						//0x7C
	uint8_t nRed;						//0x7D
	uint8_t nGreen;						//0x7E
	uint8_t nBlue;						//0x7F
	uint8_t nNumDirections;				//0x80
	uint8_t nLocalBlood;				//0x81
	uint16_t pad0x82;					//0x82
};

#pragma pack()
