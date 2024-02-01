#pragma once 

#include <cstdint>

#pragma pack(1)

struct D2ArenaTxt
{
	union
	{
		struct
		{
			int32_t dwSuicide;					//0x00
			int32_t dwPlayerKill;				//0x04
			int32_t dwPlayerKillPercent;		//0x08
			int32_t dwMonsterKill;				//0x0C
			int32_t dwPlayerDeath;				//0x10
			int32_t dwPlayerDeathPercent;		//0x14
			int32_t dwMonsterDeath;				//0x18
		};
		int32_t dwScores[7];
	};
};

struct D2CharItemStrc
{
	uint32_t dwItemCode;				//0x00
	uint8_t nBodyLoc;					//0x04
	int8_t nItemCount;					//0x05
	uint16_t pad0x06;					//0x06
};

struct D2CharTemplateTxt
{
	char szName[30];					//0x00
	uint8_t nClass;						//0x1E
	uint8_t nLevel;						//0x1F
	uint8_t nAct;						//0x20
	uint8_t nStr;						//0x21
	uint8_t nDex;						//0x22
	uint8_t nInt;						//0x23
	uint8_t nVit;						//0x24
	uint8_t nMana;						//0x25
	uint8_t nHitpoints;					//0x26
	uint8_t nManaRegenBonus;			//0x27
	uint8_t nVelocity;					//0x28
	uint8_t nAttackRate;				//0x29
	uint8_t nOtherRate;					//0x2A
	uint8_t pad0x2B;					//0x2B
	uint32_t dwRightSkill;				//0x2C
	uint32_t dwSkill[9];				//0x30
	uint32_t dwSkillLevel[9];			//0x54
	D2CharItemStrc pItems[15];			//0x78
};

#pragma pack()
