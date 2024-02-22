#pragma once

#include <cstdint>

#include "D2Inventory.h"

struct D2CellFileStrc; // From D2CMP / D2Gfx
struct D2ActiveRoomStrc;
struct D2SkillsTxt;

#pragma pack(1)

enum D2C_SkillCastType
{
	SKCASTTYPE_SHIFTLEFT = 0xE5,
	SKCASTTYPE_TARGETLEFT = 0xC5,
	SKCASTTYPE_SHIFTRIGHT = 0x66,
	SKCASTTYPE_RIGHT = 0x46
};

enum D2C_AuraFilters
{
	AURAFILTER_FINDPLAYER = 0x01,			//Targets Players Units
	AURAFILTER_FINDMONSTER = 0x02,			//Targets Monster Units
	AURAFILTER_FINDUNDEAD = 0x04,			//Targets Undead Monsters Units
	AURAFILTER_FINDMISSILE = 0x08,			//Targets Missiles Units			
	AURAFILTER_FINDOBJECT = 0x10,			//Target Objects Units
	AURAFILTER_FINDITEM = 0x20,				//Targets Items Units
	AURAFILTER_FINDISATT = 0x80,			//Targets Units with IsAtt (Monstats2)
	AURAFILTER_IGNINTOWN = 0x100,			//Ignore Units in Town
	AURAFILTER_USELOS = 0x200,				//Filter Uses Line of Sight
	AURAFILTER_FINDISSEL = 0x400,			//Targets Selectable Units
	AURAFILTER_FINDCORPSE = 0x1000,			//Targets Units Corpses
	AURAFILTER_IGNINTOWN2 = 0x2000,			//Ignore Units in Town
	AURAFILTER_IGNBOSS = 0x4000,			//Ignore Special Units (boss in monstats)
	AURAFILTER_IGNALLY = 0x8000,			//Ignore Allied Units
	AURAFILTER_FINDALLY = 0x10000,			//Targets Allied Units
	AURAFILTER_IGNPRIME = 0x40000,			//Ignore Prime Evils
	AURAFILTER_IGNJUSTHIT = 0x80000			//Ignore Units with Justhit State
};

enum D2C_SkillFlags
{
	SKFLAG_PASSIVE = 0x00000001,
	SKFLAG_RSKILLONLY = 0x00000002,
	SKFLAG_TARGETABLE = 0x00000004,
	SKFLAG_NEEDCORPSE = 0x00000008,
	SKFLAG_INTOWN = 0x00000010,
	SKFLAG_PHYSICAL = 0x00000020,
	SKFLAG_MAGICAL = 0x00000040,
	SKFLAG_FIRE = 0x00000080,
	SKFLAG_COLD = 0x00000100,
	SKFLAG_LTNG = 0x00000200,
	SKFLAG_POIS = 0x00000400,
	SKFLAG_KNOCKBACK = 0x00000800,
	SKFLAG_STUN = 0x00001000,
	SKFLAG_RADIUSEFFECT = 0x00002000,
	SKFLAG_MELEE = 0x00004000,
	SKFLAG_SUMMON = 0x00008000,
	SKFLAG_PARTYCAST = 0x00010000,
	SKFLAG_GUIDED = 0x00020000
};

enum D2C_SkillUseStates
{
	SKILLUSTATE_USABLE = 0,
	SKILLUSTATE_NOMANA = 1,
	SKILLUSTATE_NOQUANTITY = 2,
	SKILLUSTATE_DISABLED = 3,
	SKILLUSTATE_SHAPERESTRICT = 4,
	SKILLUSTATE_PASSIVE = 5,
	SKILLUSTATE_AURA = 6,
	SKILLUSTATE_NOLEVEL = 7,
	SKILLUSTATE_BLOCKED = 8,
};

struct D2SkillCalcStrc
{
	D2UnitStrc* pUnit;						//0x00
	int32_t nSkillId;							//0x04
	int32_t nSkillLevel;						//0x08
};

struct D2SkillStrc
{

	D2SkillsTxt* pSkillsTxt;				//0x00
	D2SkillStrc* pNextSkill;				//0x04
	uint32_t dwSkillMode;						//0x08
	uint32_t dwFlags;							//0x0C D2C_SkillFlags
	uint32_t unk0x10[2];						//0x10 - not used?

	union
	{
		uint32_t dwTargetInfo;					//0x18
		uint32_t nXpos;						//0x18
		uint32_t nPar1;						//0x18
	};
	union
	{
		uint32_t dwTargetType;					//0x1C
		uint32_t nYpos;						//0x1C
		uint32_t nPar2;						//0x1C
	};
	union
	{
		uint32_t dwTargetGUID;					//0x20
		uint32_t nPar3;						//0x20
	};

	uint32_t nPar4;							//0x24
	int32_t nSkillLevel;						//0x28
	uint32_t nLevelBonus;						//0x2C
	int32_t nQuantity;							//0x30
	uint32_t nOwnerGUID;						//0x34 -1 = Native Skill
	int32_t nCharges;							//0x38
};

struct D2SkillListStrc
{
	void* pMemPool;							//0x00
	D2SkillStrc* pFirstSkill;				//0x04
	D2SkillStrc* pLeftSkill;				//0x08
	D2SkillStrc* pRightSkill;				//0x0C
	D2SkillStrc* pUsedSkill;				//0x10
	uint32_t unk014;						//0x14
};

struct D2SkillTreeChartStrc
{
	D2CellFileStrc* pCellFile;				//0x00
	char* szFileName;						//0x04
	uint32_t unk0x08[6];						//0x08
	uint16_t unk0x20;							//0x20
};



#pragma pack()
