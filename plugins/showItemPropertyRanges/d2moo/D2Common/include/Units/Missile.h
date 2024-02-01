#pragma once

#include <cstdint>
#include "MissileStream.h"
#include "../Path/Path.h"

#pragma pack(1)
//TODO: Redo Header defs when .cpp is done

struct D2UnkMissileCalcStrc
{
	int32_t field_0;
	int32_t field_4;
	int32_t field_8;
	int32_t field_C;
	int32_t field_10;
	int32_t field_14;
	int32_t field_18;
	int32_t field_1C;
	D2SeedStrc pSeed;
};

struct D2MissileCalcStrc
{
	D2UnitStrc* pMissile;					//0x00
	D2UnitStrc* pOwner;						//0x04
	int32_t nMissileId;							//0x08
	int32_t nMissileLevel;						//0x0C
};

struct D2MissileDamageDataStrc
{
	int32_t nFlags;								//0x00
	int32_t nMinDamage;							//0x04
	int32_t nMaxDamage;							//0x08
	int32_t nFireMinDamage;						//0x0C
	int32_t nFireMaxDamage;						//0x10
	int32_t nFireLength;						//0x14
	int32_t nLightMinDamage;					//0x18
	int32_t nLightMaxDamage;					//0x1C
	int32_t nMagicMinDamage;					//0x20
	int32_t nMagicMaxDamage;					//0x24
	int32_t nColdMinDamage;						//0x28
	int32_t nColdMaxDamage;						//0x2C
	int32_t nColdLength;						//0x30
	int32_t nPoisonMinDamage;					//0x34
	int32_t nPoisonMaxDamage;					//0x38
	int32_t nPoisonLength;						//0x3C
	int32_t nPoisonCount;						//0x40
	int32_t nLifeDrainMinDamage;				//0x44
	int32_t nLifeDrainMaxDamage;				//0x48
	int32_t nManaDrainMinDamage;				//0x4C
	int32_t nManaDrainMaxDamage;				//0x50
	int32_t nStaminaDrainMinDamage;				//0x54
	int32_t nStaminaDrainMaxDamage;				//0x58
	int32_t nStunLength;						//0x5C
	int32_t nBurningMin;						//0x60
	int32_t nBurningMax;						//0x64
	int32_t nBurnLength;						//0x68
	int32_t nDemonDamagePercent;				//0x6C
	int32_t nUndeadDamagePercent;				//0x70
	int32_t nDamageTargetAC;					//0x74
	int32_t nDamagePercent;						//0x78
};

struct D2MissileDataStrc
{
	uint32_t unk0x00;							//0x00 - some type of coords, see D2Common.11128, D2Common.11131 - D2Common.11134
	int16_t nStreamMissile;					//0x04
	int16_t nStreamRange;						//0x06
	int16_t nActivateFrame;					//0x08
	int16_t nSkill;							//0x0A
	int16_t nLevel;							//0x0C
	int16_t nTotalFrames;						//0x0E
	int16_t nCurrentFrame;					//0x10
	uint16_t unk0x12;							//0x12
	uint32_t fFlags;							//0x14
	int32_t nOwnerType;							//0x18
	uint32_t dwOwnerGUID;						//0x1C
	int32_t nHomeType;							//0x20
	uint32_t dwHomeGUID;						//0x24
	union
	{
		struct
		{
			int32_t nStatus;					//0x28 - used only by heatseeking projectiles
			D2PathPointStrc pCoords;		//0x2C - same, these are not coords, they are missile streams etc, see D2Common.11213 & D2Common.11214
		};

		D2CoordStrc pTargetCoords;			//0x28
	};
	D2MissileStreamStrc* pStream;			//0x30
};

struct D2MissileStrc
{
	uint32_t dwFlags;							//0x00
	D2UnitStrc* pOwner;							//0x04
	D2UnitStrc* pOrigin;						//0x08
	D2UnitStrc* pTarget;						//0x0C
	int32_t nMissile;							//0x10
	int32_t nX;									//0x14
	int32_t nY;									//0x18
	int32_t nTargetX;							//0x1C
	int32_t nTargetY;							//0x20
	int32_t nGfxArg;							//0x24
	int32_t nVelocity;							//0x28
	int32_t nSkill;								//0x2C
	int32_t nSkillLevel;						//0x30
	int32_t nLoops;								//0x34
	uint32_t unk0x38;							//0x38
	uint32_t unk0x3C;							//0x3C
	int32_t nFrame;								//0x40
	int32_t nActivateFrame;						//0x44
	int32_t nAttBonus;							//0x48
	int32_t nRange;								//0x4C
	int32_t nLightRadius;						//0x50
	void * pInitFunc;					//0x54
	int32_t pInitArgs;							//0x58
};
#pragma pack()
