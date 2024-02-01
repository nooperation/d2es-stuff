#pragma once

#include <cstdint>
#include "CommonDefinitions.h"


struct D2UnitStrc;
struct D2ActiveRoomStrc;
struct D2MonStatsTxt;

#pragma pack(1)

enum D2InactiveMonsterFlags
{
	INACTIVEMONSTERFLAG_TYPE_OTHER = 0x00000001,
	INACTIVEMONSTERFLAG_TYPE_CHAMPION = 0x00000002,
	INACTIVEMONSTERFLAG_DEAD = 0x00000004,
	INACTIVEMONSTERFLAG_MINIONRELATED1 = 0x00000008,
	INACTIVEMONSTERFLAG_MINIONRELATED2 = 0x00000010,
	INACTIVEMONSTERFLAG_TYPE_MINION = 0x00000020,
	INACTIVEMONSTERFLAG_UNKSUMMONERFLAG = 0x00000040,
	INACTIVEMONSTERFLAG_ALIGNMENT_NEUTRAL = 0x00000080,
	INACTIVEMONSTERFLAG_ALIGNMENT_GOOD = 0x00000100,
	INACTIVEMONSTERFLAG_ALIGNMENT_EVIL = 0x00000200,
	INACTIVEMONSTERFLAG_NODEINDEXUNEQUAL11 = 0x00000400,
	INACTIVEMONSTERFLAG_TYPE_SUPERUNIQUE = 0x00000800,
};

enum D2C_MonTypeFlags
{
	MONTYPEFLAG_OTHER = 0x01,
	MONTYPEFLAG_SUPERUNIQUE = 0x02,
	MONTYPEFLAG_CHAMPION = 0x04,
	MONTYPEFLAG_UNIQUE = 0x08,
	MONTYPEFLAG_MINION = 0x10,
	MONTYPEFLAG_POSSESSED = 0x20,
	MONTYPEFLAG_GHOSTLY = 0x40,
	MONTYPEFLAG_MULTISHOT = 0x80
};

enum D2C_MonModes
{
	MONMODE_DEATH, 		//0x00 Death DT
	MONMODE_NEUTRAL, 	//0x01 Neutral NU
	MONMODE_WALK, 		//0x02 Walk WL
	MONMODE_GETHIT, 	//0x03 Get Hit GH
	MONMODE_ATTACK1, 	//0x04 Melee Attack A1
	MONMODE_ATTACK2, 	//0x05 Melee Attack A2
	MONMODE_BLOCK, 		//0x06 Block BL
	MONMODE_CAST, 		//0x07 Spell Cast SC
	MONMODE_SKILL1, 	//0x08 Special S1
	MONMODE_SKILL2, 	//0x09 Special S2
	MONMODE_SKILL3, 	//0x0A Special S3
	MONMODE_SKILL4, 	//0x0B Special S4
	MONMODE_DEAD, 		//0x0C Dead DD
	MONMODE_KNOCKBACK, 	//0x0D Knockback KB
	MONMODE_SEQUENCE, 	//0x0E Sequence SQ
	MONMODE_RUN,		//0x0F Run RN
	NUMBER_OF_MONMODES
};

enum D2C_MercenaryClasses
{
	MERC_A1 = 0x10F, 	//Monstats Id Act 1 Mercenary
	MERC_A2 = 0x152, 	//Monstats Id Act 2 Mercenary
	MERC_A3 = 0x167, 	//Monstats Id Act 3 Mercenary
	MERC_A5 = 0x231		//Monstats Id Act 5 Mercenary
};

enum D2C_PetWarpMode
{
	PETMOVE_OWNERROOM,
	PETMOVE_OWNERPOS,
	PETMOVE_TOWNIDLE,
	PETMOVE_TELE,
	PETMOVE_SCATTER,
	PETMOVE_IDLE
};

enum D2C_MonUMods
{
	MONUMOD_NONE, 					//none
	MONUMOD_RNDNAME, 				//randomized name
	MONUMOD_HPMULTIPLY, 			//adds life%
	MONUMOD_LIGHT, 					//increased light radius
	MONUMOD_LEVELADD, 				//adds three mlvl
	MONUMOD_EXTRASTRONG, 			//extra strong, dmg
	MONUMOD_EXTRAFAST, 				//extra fast, faster
	MONUMOD_CURSED, 				//amplify damage on hit
	MONUMOD_MAGICRESISTANT, 		//increased elemental resists
	MONUMOD_FIRECHANT, 				//fire enchanted
	MONUMOD_POISDEATH, 				//poison cloud on death
	MONUMOD_WORMDEATH, 				//releases worms and insects on death (Duriel)
	MONUMOD_BRAVENDEATH, 			//only works for Blood raven
	MONUMOD_IGNOREAC, 				//Ignore defense
	MONUMOD_SPCDAMAGE, 				//unknown
	MONUMOD_KILLMINIONSDEATH, 		//all minions of the boss die when the boss is killed
	MONUMOD_CHAMPMODS, 				//related to champions mods
	MONUMOD_LIGHTCHANT, 			//lightning enchanted
	MONUMOD_COLDCHANT, 				//cold enchanted
	MONUMOD_UNUSEDMERCMOD, 			//old unused mercenary dmg mod, buggy
	MONUMOD_CHARGEDBOLTS, 			//charged bolts when struck (scarabs)
	MONUMOD_TEMPSUMMON, 			//used by summons that that are destroyed after a set time
	MONUMOD_QUESTMOD, 				//related to quests
	MONUMOD_POISONFIELD, 			//the boss is dealing poison damage in an aura-like method
	MONUMOD_THIEF, 					//unused, thief mod
	MONUMOD_MANABURN, 				//mana burn
	MONUMOD_TELEHEAL, 				//teleports randomly and heal
	MONUMOD_SPECTRALHIT, 			//spectral hit, elemental damage
	MONUMOD_STONESKIN, 				//physical resistance
	MONUMOD_MULTISHOT, 				//multi-shot
	MONUMOD_AURACHANT, 				//aura enchanted, random aura when eqquipped (from aura table)
	MONUMOD_CORPSEBOOMDEATH, 		//explosion of the corpse on death
	MONUMOD_FIREBOOMDEATH, 			//fire explosion on death
	MONUMOD_FREEZINDEATH, 			//freeze you on death
	MONUMOD_SELFRESURRECT, 			//self-resurrect for reanimated-horde
	MONUMOD_ICESHATTERDEATH, 		//ice shatter on death
	MONUMOD_CHAMPSTONED, 			//slowed down but stronger, for champions
	MONUMOD_CHAMPSTATS, 			//champs stats related
	MONUMOD_CHAMPCURSEIMMUNE, 		//champion cannot be cursed
	MONUMOD_CHAMPSTATS2, 			//related to champ stats
	MONUMOD_PAINWORMDEATH, 			//summons a pain work on death
	MONUMOD_ALWAYSRUNAI, 			//unknown
	MONUMOD_NOVADEATH				//nova on death
};


enum D2MapAIActions
{
	AIACTION_MODE_NU = 1,
	AIACTION_MODE_S1 = 4,
	AIACTION_MODE_S2 = 5,
};

struct D2HirelingInitStrc
{
	int32_t nId;								//0x00
	int32_t nLevel;								//0x04
	int32_t nHitpoints;							//0x08
	int32_t nStrength;							//0x0C
	int32_t nDexterity;							//0x10
	int32_t nGold;								//0x14
	int32_t nExperience;						//0x18
	int32_t nDefense;							//0x1C
	int32_t nMinDamage;							//0x20
	int32_t nMaxDamage;							//0x24
	int32_t nShare;								//0x28
	int32_t nResist;							//0x2C
	int32_t nHireDesc;							//0x30
	int32_t field_34;							//0x34
	int16_t nFlags;							//0x38
};
#pragma pack()
