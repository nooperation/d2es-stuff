/////////////////////////////////////////////////////////////////
// Constants.h
//
// Diablo II game constants definitions.
//
// Written by Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////

#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

//////////////////////////////////////////////////////////////////////
// Diablo 2 Fonts
//////////////////////////////////////////////////////////////////////
#define D2FONT_SMALL		6	// Small font
#define D2FONT_THIN			4	// Thin font
#define D2FONT_NORMAL		0	// Normal font
#define D2FONT_SMOOTH		13	// Smooth font
#define D2FONT_BIGSMOOTH	5	// Big smooth font
#define D2FONT_CAPS			1	// Capital font
#define D2FONT_ROUGH		7	// Rough Font
#define D2FONT_BIGROUGH		2	// Big rough font
#define D2FONT_BIGBIGROUGH	3	// Big big rough font
#define D2FONT_SMOKE		11	// Smoke font

//////////////////////////////////////////////////////////////////////
// Game Unit Type Definition (v2.00 by Abin)
//////////////////////////////////////////////////////////////////////
enum {  UNIT_TYPE_PLAYER = 0,	// Player
		UNIT_TYPE_MONSTER,		// Monster or Npc
		UNIT_TYPE_OBJECT,		// Static object(wp, stash, etc)
		UNIT_TYPE_MISSILE,		// Missle
		UNIT_TYPE_ITEM,			// Item
		UNIT_TYPE_ROOMTILE };	// Room tile


//////////////////////////////////////////////////////////////////////
// Mercenary Class ID's
//////////////////////////////////////////////////////////////////////
#define MERC_A1		0x010f
#define MERC_A2		0x0152
#define MERC_A3		0x0167
#define MERC_A5		0x0231

//////////////////////////////////////////////////////////////////////
// Chicken Types
//////////////////////////////////////////////////////////////////////
enum {  CHICKEN_POTION = 0, // Drink potion
		CHICKEN_TOWN,		// Back to town
		CHICKEN_QUIT };		// Quit game

//////////////////////////////////////////////////////////////////////
// Unit Stat ID Definition (Partial)
//////////////////////////////////////////////////////////////////////
#define STAT_STRENGTH				0		// str
#define	STAT_ENERGY					1		// energy
#define	STAT_DEXTERITY				2		// dexterity
#define	STAT_VITALITY				3		// vitality
#define	STAT_HP						6		// life
#define	STAT_MAXHP					7		// max life
#define	STAT_MANA					8		// mana
#define	STAT_MAXMANA				9		// max mana
#define	STAT_STAMINA				10		// stamina
#define	STAT_MAXSTAMINA				11		// max stamina
#define	STAT_LEVEL					12		// level
#define	STAT_GOLD					14		// gold
#define	STAT_GOLDBANK				15		// stash gold
#define	STAT_TOBLOCK				20		// to block
#define	STAT_DAMAGEREDUCTION		34		// damage reduction
#define	STAT_MAGICDAMAGEREDUCTION	35		// magic damage reduction
#define	STAT_DAMAGERESIST			36		// physic resist
#define	STAT_MAGICRESIST			37		// magic resist
#define	STAT_MAXMAGICRESIST			38		// max magic resist
#define	STAT_FIRERESIST				39		// fire resist
#define	STAT_MAXFIRERESIST			40		// max fire resist
#define	STAT_LIGHTNINGRESIST		41		// lightning resist
#define	STAT_MAXLIGHTNINGRESIST		42		// max lightning resist
#define	STAT_COLDRESIST				43		// cold resist
#define	STAT_MAXCOLDRESIST			44		// max cold resist
#define	STAT_POISONRESIST			45		// poison resist
#define	STAT_MAXPOISONRESIST		46		// max poison resist
#define	STAT_VELOCITYPERCENT		67		// effective run/walk
#define STAT_AMMOQUANTITY			70		// ammo quantity(arrow/bolt/throwing)
#define STAT_DURABILITY				72		// item durability
#define STAT_MAXDURABILITY			73		// max item durability
#define	STAT_MAGICFIND				80		// magic find (MF)
#define	STAT_IAS					93		// IAS
#define	STAT_FASTERRUNWALK			96		// faster run/walk
#define	STAT_FASTERHITRECOVERY		99		// faster hit recovery
#define	STAT_FASTERBLOCK			102		// faster block rate
#define	STAT_FASTERCAST				105		// faster cast rate
#define	STAT_CRUSHINGBLOW			136		// crushing blow
#define	STAT_DEADLYSTRIKE			141		// deadly strike
#define	STAT_FIREABSORBPERCENT		142		// fire absorb %
#define	STAT_FIREABSORB				143		// fire absorb
#define	STAT_LIGHTNINGABSORBPERCENT	144		// lightning absorb %
#define	STAT_LIGHTNINGABSORB		145		// lightning absorb
#define	STAT_COLDABSORBPERCENT		148		// cold absorb %
#define	STAT_COLDABSORB				149		// cold absorb

//////////////////////////////////////////////////////////////////////
// Unit Affection ID Definition
//////////////////////////////////////////////////////////////////////
#define AFFECT_NONE                                                  0
#define AFFECT_FREEZE                                                1
#define AFFECT_POISON                                                2
#define AFFECT_RESISTFIRE                                            3
#define AFFECT_RESISTCOLD                                            4
#define AFFECT_RESISTLIGHT                                           5
#define AFFECT_RESISTMAGIC                                           6
#define AFFECT_PLAYERBODY                                            7
#define AFFECT_RESISTALL                                             8
#define AFFECT_AMPLIFYDAMAGE                                         9
#define AFFECT_FROZENARMOR                                           10
#define AFFECT_COLD                                                  11
#define AFFECT_INFERNO                                               12
#define AFFECT_BLAZE                                                 13
#define AFFECT_BONEARMOR                                             14
#define AFFECT_CONCENTRATE                                           15
#define AFFECT_ENCHANT                                               16
#define AFFECT_INNERSIGHT                                            17
#define AFFECT_SKILL_MOVE                                            18
#define AFFECT_WEAKEN                                                19
#define AFFECT_CHILLINGARMOR                                         20
#define AFFECT_STUNNED                                               21
#define AFFECT_SPIDERLAY                                             22
#define AFFECT_DIMVISION                                             23
#define AFFECT_SLOWED                                                24
#define AFFECT_FETISHAURA                                            25
#define AFFECT_SHOUT                                                 26
#define AFFECT_TAUNT                                                 27
#define AFFECT_CONVICTION                                            28
#define AFFECT_CONVICTED                                             29
#define AFFECT_ENERGYSHIELD                                          30
#define AFFECT_VENOMCLAWS                                            31
#define AFFECT_BATTLEORDERS                                          32
#define AFFECT_MIGHT                                                 33
#define AFFECT_PRAYER                                                34
#define AFFECT_HOLYFIRE                                              35
#define AFFECT_THORNS                                                36
#define AFFECT_DEFIANCE                                              37
#define AFFECT_THUNDERSTORM                                          38
#define AFFECT_LIGHTNINGBOLT                                         39
#define AFFECT_BLESSEDAIM                                            40
#define AFFECT_VIGOR	                                                41
#define AFFECT_CONCENTRATION                                         42
#define AFFECT_HOLYFREEZE                                            43
#define AFFECT_HOLYFREEZECHILL                                       44
#define AFFECT_CLEANSING                                             45
#define AFFECT_HOLYSHOCK                                             46
#define AFFECT_SANCTUARY                                             47
#define AFFECT_MEDITATION                                            48
#define AFFECT_FANATICISM                                            49
#define AFFECT_REDEMPTION                                            50
#define AFFECT_BATTLECOMMAND                                         51
#define AFFECT_PREVENTHEAL                                           52
#define AFFECT_CONVERSION                                            53
#define AFFECT_UNINTERRUPTABLE                                       54
#define AFFECT_IRONMAIDEN                                            55
#define AFFECT_TERROR                                                56
#define AFFECT_ATTRACT                                               57
#define AFFECT_LIFETAP                                               58
#define AFFECT_CONFUSE                                               59
#define AFFECT_DECREPIFY                                             60
#define AFFECT_LOWERRESIST                                           61
#define AFFECT_OPENWOUNDS                                            62
#define AFFECT_DOPPLEZON                                             63
#define AFFECT_CRITICALSTRIKE                                        64
#define AFFECT_DODGE                                                 65
#define AFFECT_AVOID                                                 66
#define AFFECT_PENETRATE                                             67
#define AFFECT_EVADE                                                 68
#define AFFECT_PIERCE                                                69
#define AFFECT_WARMTH                                                70
#define AFFECT_FIREMASTERY                                           71
#define AFFECT_LIGHTNINGMASTERY                                      72
#define AFFECT_COLDMASTERY                                           73
#define AFFECT_SWORDMASTERY                                          74
#define AFFECT_AXEMASTERY                                            75
#define AFFECT_MACEMASTERY                                           76
#define AFFECT_POLEARMMASTERY                                        77
#define AFFECT_THROWINGMASTERY                                       78
#define AFFECT_SPEARMASTERY                                          79
#define AFFECT_INCREASEDSTAMINA                                      80
#define AFFECT_IRONSKIN                                              81
#define AFFECT_INCREASEDSPEED                                        82
#define AFFECT_NATURALRESISTANCE                                     83
#define AFFECT_FINGERMAGECURSE                                       84
#define AFFECT_NOMANAREGEN                                           85
#define AFFECT_JUSTHIT                                               86
#define AFFECT_SLOWMISSILES                                          87
#define AFFECT_SHIVERARMOR                                           88
#define AFFECT_BATTLECRY                                             89
#define AFFECT_BLUE                                                  90
#define AFFECT_RED                                                   91
#define AFFECT_DEATH_DELAY                                           92
#define AFFECT_VALKYRIE                                              93
#define AFFECT_FRENZY                                                94
#define AFFECT_BERSERK                                               95
#define AFFECT_REVIVE                                                96
#define AFFECT_ITEMFULLSET                                           97
#define AFFECT_SOURCEUNIT                                            98
#define AFFECT_REDEEMED                                              99
#define AFFECT_HEALTHPOT                                             100
#define AFFECT_HOLYSHIELD                                            101
#define AFFECT_JUST_PORTALED                                         102
#define AFFECT_MONFRENZY                                             103
#define AFFECT_CORPSE_NODRAW                                         104
#define AFFECT_ALIGNMENT                                             105
#define AFFECT_MANAPOT                                               106
#define AFFECT_SHATTER                                               107
#define AFFECT_SYNC_WARPED                                           108
#define AFFECT_CONVERSION_SAVE                                       109
#define AFFECT_PREGNANT                                              110
#define AFFECT_111                                                   111
#define AFFECT_RABIES                                                112
#define AFFECT_DEFENSE_CURSE                                         113
#define AFFECT_BLOOD_MANA                                            114
#define AFFECT_BURNING                                               115
#define AFFECT_DRAGONFLIGHT                                          116
#define AFFECT_MAUL                                                  117
#define AFFECT_CORPSE_NOSELECT                                       118
#define AFFECT_SHADOWWARRIOR                                         119
#define AFFECT_FERALRAGE                                             120
#define AFFECT_SKILLDELAY                                            121
#define AFFECT_PROGRESSIVE_DAMAGE                                    122
#define AFFECT_PROGRESSIVE_STEAL                                     123
#define AFFECT_PROGRESSIVE_OTHER                                     124
#define AFFECT_PROGRESSIVE_FIRE                                      125
#define AFFECT_PROGRESSIVE_COLD                                      126
#define AFFECT_PROGRESSIVE_LIGHTNING                                 127
#define AFFECT_SHRINE_ARMOR                                          128
#define AFFECT_SHRINE_COMBAT                                         129
#define AFFECT_SHRINE_RESIST_LIGHTNING                               130
#define AFFECT_SHRINE_RESIST_FIRE                                    131
#define AFFECT_SHRINE_RESIST_COLD                                    132
#define AFFECT_SHRINE_RESIST_POISON                                  133
#define AFFECT_SHRINE_SKILL                                          134
#define AFFECT_SHRINE_MANA_REGEN                                     135
#define AFFECT_SHRINE_STAMINA                                        136
#define AFFECT_SHRINE_EXPERIENCE                                     137
#define AFFECT_FENRIS_RAGE                                           138
#define AFFECT_WOLF                                                  139
#define AFFECT_BEAR                                                  140
#define AFFECT_BLOODLUST                                             141
#define AFFECT_CHANGECLASS                                           142
#define AFFECT_ATTACHED                                              143
#define AFFECT_HURRICANE                                             144
#define AFFECT_ARMAGEDDON                                            145
#define AFFECT_INVIS                                                 146
#define AFFECT_BARBS                                                 147
#define AFFECT_WOLVERINE                                             148
#define AFFECT_OAKSAGE                                               149
#define AFFECT_VINE_BEAST                                            150
#define AFFECT_CYCLONEARMOR                                          151
#define AFFECT_CLAWMASTERY                                           152
#define AFFECT_CLOAK_OF_SHADOWS                                      153
#define AFFECT_RECYCLED                                              154
#define AFFECT_WEAPONBLOCK                                           155
#define AFFECT_CLOAKED                                               156
#define AFFECT_BURSTOFSPEED                                          157
#define AFFECT_BLADESHIELD                                           158
#define AFFECT_FADE                                                  159

#define AFFECT_ES_BARBTHUNDERSTORM									 215
#define AFFECT_ES_SHOCKARMOR										  10

//////////////////////////////////////////////////////////////////////
// Monster Types
//////////////////////////////////////////////////////////////////////
enum {
	MONSTER_UNKNOWN = 0,	// Unknown type
	MONSTER_NORMAL,			// Normal
	MONSTER_CHAMPION,		// Champion
	MONSTER_BOSS,			// Boss
	MONSTER_MINION,			// Minion
};

//////////////////////////////////////////////////////////////////////
// Npc Interaction Types
//////////////////////////////////////////////////////////////////////
enum {	NPC_NORMAL = 0, // Just interact
		NPC_TRADE,		// Interact and open store
		NPC_GAMBLE };	// Interact and open gamble window

//////////////////////////////////////////////////////////////////////
// Game UI Flags(From D2JSP)
//////////////////////////////////////////////////////////////////////
#define UI_INVENTORY			0x01
#define UI_CHARACTER			0x02
#define UI_MINISKILL			0x03
#define UI_SKILL				0x04
#define UI_CHAT					0x05
#define UI_NPCMENU				0x08	
#define UI_MAINMENU				0x09
#define UI_AUTOMAP				0x0a
#define UI_CONFIG				0x0b
#define UI_NPCSHOP				0x0c
#define UI_ALTDOWN				0x0d			
#define UI_ANVIL				0x0e
#define UI_QUEST				0x0f
#define UI_QUESTLOG				0x11
#define UI_STATUSAREA			0x12
#define UI_WPMENU				0x14
#define UI_MINIPANEL			0x15
#define UI_PARTY				0x16
#define UI_TRADE				0x17
#define UI_MSGS					0x18
#define UI_STASH				0x19
#define UI_CUBE					0x1a
#define UI_BELT					0x1f
#define UI_HELP					0x21
#define UI_MERC					0x24
#define UI_SCROLL				0x25

//////////////////////////////////////////////////////////////////////
// Character Storage Types
//////////////////////////////////////////////////////////////////////	
enum {  STORAGE_NONE = 0,      // Not exist
		STORAGE_INVENTORY,     // Inventory
		STORAGE_STASH,         // Stash
		STORAGE_CUBE };        // Cube

#define INV_ROW		9 
#define INV_COL		11  
#define STASH_ROW	10
#define STASH_COL	10
#define CUBE_ROW	8
#define CUBE_COL	10
#define BELT_ROW	4
#define BELT_COL	4

//////////////////////////////////////////////////////////////////////
// Character Equip Locations
//////////////////////////////////////////////////////////////////////
enum {  EQUIP_NONE = 0,			// Not equipped
		EQUIP_HEAD,				// Head
		EQUIP_AMULET,			// Amulet
		EQUIP_BODY,				// Body armor
		EQUIP_RIGHT_PRIMARY,	// Right primary slot
		EQUIP_LEFT_PRIMARY,		// Left primary slot
		EQUIP_RIGHT_RING,		// Right ring
		EQUIP_LEFT_RING,		// Left ring
		EQUIP_BELT,				// Belt
		EQUIP_FEET,				// Feet
		EQUIP_GLOVES,			// Gloves
		EQUIP_RIGHT_SECONDARY,	// Right secondary slot
		EQUIP_LEFT_SECONDARY,	// Left secondary slot
};

///////////////////////////////////////////////////////////////////////
// D2 Message Definition
///////////////////////////////////////////////////////////////////////
enum {

	DM_VOID	= 0,		// Placeholder

	///////////////////////////////////////////////////////////////////////
	// Unit Messages
	///////////////////////////////////////////////////////////////////////
	UM_SPAWN,			// Unit spawns, wParam: 0, lParam: lower-word position x, higher-word position y
	UM_DESTROY,			// Unit is gone from current player's vision, wParam: 0, lParam: 0
	UM_AFFECT,			// Unit is affected by some states, wParam: 0-initiative, 1-passive, lParam: affection id
	UM_DISAFFECT,		// Unit is disaffected by some states, wParam: 0-initiative, 1-passive, lParam: affection id
	UM_STARTMOVE,		// Unit starts to move, wParam: 0, lParam: lower-word destination x, higher-word destination y
	UM_ENDMOVE,			// Unit ends moving, wParam: 0, lParam: lower-word current x, higher-word current y
	UM_HPPERCENT,		// Unit HP percentage changed, wParam: current HP percent, lParam: 0
	UM_ATTACK,			// Unit attacks, wParam: address of victim unit struct, lParam: lower-word position x, higher-word position y
	UM_UNDERATTACK,		// Unit under attack, wParam: address of attacker unit struct, lParam: lower-word position x, higher-word position y
	UM_DEATH,			// Unit dies, wParam: unit ID, lParam: 0
	UM_CORPSE,			// Unit corpse appears, wParam: unit ID, lParam: lower-word position x, higher-word position y
	UM_REVIVED,			// Unit is revived(e.g. fallen by shaman), wParam: 1-revived by others, 0-revived by itself,  lParam: lower-word position x, higher-word position y
	UM_CAVENOTIFY,		// Cave/entrance/stairs notification, wParam: cave type, lParam: lower-word position x, higher-word position y
	UM_ITEMEVENT, // ++ B

	///////////////////////////////////////////////////////////////////////
	// This-Player Messages
	///////////////////////////////////////////////////////////////////////
	PM_AFFECT,			// Player is affected by some states, wParam: 0-initiative, 1-passive, lParam: affection id
	PM_DISAFFECT,		// Player is disaffected by some states, wParam: 0-initiative, 1-passive, lParam: affection id
	PM_SPELLCHARGES,	// Player's spell charges changed (spells those can only be used for a particular times, e.g., Tome of town portal 20 charges), wParam: spell ID, lParam: charges left
	PM_SPELLSELECT,		// Player selected a spell , wParam: 1-lefthand, 0-righthand, lParam: new spell ID
	PM_LEVEL,			// Player's character level retrieved, wParam: 0, lParam: character level
	PM_HPUP,			// Player HP increases, wParam: original HP, lParam: current HP
	PM_HPDOWN,			// Player HP decreases, wParam: original HP, lParam: current HP
	PM_MANAUP,			// Player mana increases, wParam: original mana, lParam: current mana
	PM_MANADOWN,		// Player mana decreases, wParam: original mana, lParam: current mana
	PM_DEATH,			// Player dies, wParam: 0, lParam: 0
	PM_CORPSE,			// Player's corpse appears, wParam: corpse ID, lParam: lower-word position x, higher-word position y
	PM_CORPSEPICKEDUP,	// Player's corpse picked up, wParam: 0, lParam: 0
	PM_HOSTILE,			// Player is hostiled by other players, wParam: hostiler ID, lParam: hostiler count
	PM_UNHOSTILE,		// Player is unhostiled by other players, wParam: un-hostiler ID, lParam: hostiler count
	PM_MAPBLINK,		// The map blinked (because of WP, TP, Teleport, etc), wParam: act, lParam: lower-word position x, higher-word position y
	PM_MAPCHANGE,		// The player has entered a new area, wParam: old area ID, lParam: new area ID 
	PM_ENTERTOWN,		// The player has entered town (safe areas), wParam: old area ID, lParam: new area ID
	PM_LEAVETOWN,		// The player has left town (unsafe areas), wParam: old area ID, lParam: new area ID
	PM_ACTCHANGE,		// Current act changed, wParam: original act, lParam: current act
	PM_MOVECOMPLETE,	// The player has completed a moving action, wParam: 0-failed, 1-succeeded, lParam: 0
	PM_WEAPONSWITCH,	// The player's weapon switched, wParam: 0, lParam: 0
	PM_TOWNFAIL,		// The "Back to town" operation has failed, wParam: 0, lParam: 0
	PM_INVENTORYFULL,	// Player's inventory is full, wParam: 0, lParam: 0
	PM_EQUIPEVENT,		// Player equipped/unequipped an item, wParam: event type, lParam: address of an ITEM struct
	PM_WPOPEN,			// The WP has been opened, wParam: act, lParam: map ID
	PM_STASHOPEN,		// The stash is opened, wParam: act, lParam: 0
	PM_GAMEREADY,		// Game is ready for operations, wParam: 0, lParam: 0
	PM_INVITATION,		// Other player invited me to party, wParam: other player's ID, lParam: 0
	PM_JOINPARTY,		// Player joined a party, wParam: invitation player's ID, lParam: 0
	PM_UICLOSED,		// All previously opened UI's have been closed, wParam: 0, lParam: 0		
	PM_WPFAIL,			// A previous call to "TakeWaypoint" has failed
	PM_NPCSESSION,		// Indicates results of a previous call to "StartNpcSession". wParam: 1-Succeeded, 0-Failed, lParam: npc ID

	///////////////////////////////////////////////////////////////////////
	// Available for Application Defined Messages
	///////////////////////////////////////////////////////////////////////
	DM_APP
};


///////////////////////////////////////////////////
// Map Definition
///////////////////////////////////////////////////
#define MAP_UNKNOWN								0x00

///////////////////////////////////////////////////
// Act 1 Maps
///////////////////////////////////////////////////
#define MAP_A1_ROGUE_ENCAMPMENT					0x01
#define MAP_A1_BLOOD_MOOR						0x02
#define MAP_A1_COLD_PLAINS						0x03
#define MAP_A1_STONY_FIELD						0x04
#define MAP_A1_DARK_WOOD						0x05
#define MAP_A1_BLACK_MARSH						0x06
#define MAP_A1_TAMOE_HIGHLAND					0x07
#define MAP_A1_DEN_OF_EVIL						0x08
#define MAP_A1_CAVE_LEVEL_1						0x09 // Cave in Cold plains
#define MAP_A1_UNDERGROUND_PASSAGE_LEVEL_1		0x0a
#define MAP_A1_HOLE_LEVEL_1						0x0b
#define MAP_A1_PIT_LEVEL_1						0x0c
#define MAP_A1_CAVE_LEVEL_2						0x0d // Cave in Cold plains
#define MAP_A1_UNDERGROUND_PASSAGE_LEVEL_2		0x0e
#define MAP_A1_HOLE_LEVEL_2						0x0f
#define MAP_A1_PIT_LEVEL_2						0x10
#define MAP_A1_BURIAL_GROUNDS					0x11
#define MAP_A1_CRYPT							0x12
#define MAP_A1_MAUSOLEUM						0x13
#define MAP_A1_FORGOTTEN_TOWER					0x14
#define MAP_A1_TOWER_CELLAR_LEVEL_1				0x15
#define MAP_A1_TOWER_CELLAR_LEVEL_2				0x16
#define MAP_A1_TOWER_CELLAR_LEVEL_3				0x17
#define MAP_A1_TOWER_CELLAR_LEVEL_4				0x18
#define MAP_A1_TOWER_CELLAR_LEVEL_5				0x19
#define MAP_A1_MONASTERY_GATE					0x1a
#define MAP_A1_OUTER_CLOISTER					0x1b
#define MAP_A1_BARRACKS							0x1c
#define MAP_A1_JAIL_LEVEL_1						0x1d
#define MAP_A1_JAIL_LEVEL_2						0x1e
#define MAP_A1_JAIL_LEVEL_3						0x1f
#define MAP_A1_INNER_CLOISTER					0x20
#define MAP_A1_INNER_CLOISTER_2					0x21
#define MAP_A1_CATACOMBS_LEVEL_1				0x22
#define MAP_A1_CATACOMBS_LEVEL_2				0x23
#define MAP_A1_CATACOMBS_LEVEL_3				0x24
#define MAP_A1_CATACOMBS_LEVEL_4				0x25
#define MAP_A1_TRISTRAM							0x26
#define MAP_A1_THE_SECRET_COW_LEVEL				0x27


///////////////////////////////////////////////////
// Act 2 Maps
///////////////////////////////////////////////////
#define MAP_A2_LUT_GHOLEIN						0x28
#define MAP_A2_ROCKY_WASTE						0x29
#define MAP_A2_DRY_HILLS						0x2a
#define MAP_A2_FAR_OASIS						0x2b
#define MAP_A2_LOST_CITY						0x2c
#define MAP_A2_VALLEY_OF_SNAKES					0x2d
#define MAP_A2_CAYON_OF_THE_MAGI				0x2e
#define MAP_A2_SEWERS_LEVEL_1					0x2f
#define MAP_A2_SEWERS_LEVEL_2					0x30
#define MAP_A2_SEWERS_LEVEL_3					0x31
#define MAP_A2_HAREM_LEVEL_1					0x32
#define MAP_A2_HAREM_LEVEL_2					0x33
#define MAP_A2_PALACE_CELLAR_LEVEL_1			0x34
#define MAP_A2_PALACE_CELLAR_LEVEL_2			0x35
#define MAP_A2_PALACE_CELLAR_LEVEL_3			0x36
#define MAP_A2_STONY_TOMB_LEVEL_1				0x37
#define MAP_A2_HALLS_OF_THE_DEAD_LEVEL_1		0x38
#define MAP_A2_HALLS_OF_THE_DEAD_LEVEL_2		0x39
#define MAP_A2_CLAW_VIPER_TEMPLE_LEVEL_1		0x3a
#define MAP_A2_STONY_TOMB_LEVEL_2				0x3b
#define MAP_A2_HALLS_OF_THE_DEAD_LEVEL_3		0x3c
#define MAP_A2_CLAW_VIPER_TEMPLE_LEVEL_2		0x3d
#define MAP_A2_MAGGOT_LAIR_LEVEL_1				0x3e
#define MAP_A2_MAGGOT_LAIR_LEVEL_2				0x3f
#define MAP_A2_MAGGOT_LAIR_LEVEL_3				0x40
#define MAP_A2_ANCIENT_TUNNELS					0x41
#define MAP_A2_TAL_RASHAS_TOMB_1				0x42
#define MAP_A2_TAL_RASHAS_TOMB_2				0x43
#define MAP_A2_TAL_RASHAS_TOMB_3				0x44
#define MAP_A2_TAL_RASHAS_TOMB_4				0x45
#define MAP_A2_TAL_RASHAS_TOMB_5				0x46
#define MAP_A2_TAL_RASHAS_TOMB_6				0x47
#define MAP_A2_TAL_RASHAS_TOMB_7				0x48
#define MAP_A2_TAL_RASHAS_CHAMBER				0x49
#define MAP_A2_ARCANE_SANCTUARY					0x4a


///////////////////////////////////////////////////
// Act 3 Maps
///////////////////////////////////////////////////
#define MAP_A3_KURAST_DOCKS						0x4b
#define MAP_A3_SPIDER_FOREST					0x4c
#define MAP_A3_GREAT_MARSH						0x4d
#define MAP_A3_FLAYER_JUNGLE					0x4e
#define MAP_A3_LOWER_KURAST						0x4f
#define MAP_A3_KURAST_BAZAAR					0x50
#define MAP_A3_UPPER_KURAST						0x51
#define MAP_A3_KURAST_CAUSEWAY					0x52
#define MAP_A3_TRAVINCAL						0x53
#define MAP_A3_ARCHNID_LAIR						0x54
#define MAP_A3_SPIDER_CAVERN					0x55
#define MAP_A3_SWAMPY_PIT_LEVEL_1				0x56
#define MAP_A3_SWAMPY_PIT_LEVEL_2				0x57
#define MAP_A3_FLAYER_DUNGEON_LEVEL_1			0x58
#define MAP_A3_FLAYER_DUNGEON_LEVEL_2			0x59
#define MAP_A3_SWAMPY_PIT_LEVEL_3				0x5a
#define MAP_A3_FLAYER_DUNGEON_LEVEL_3			0x5b
#define MAP_A3_SEWERS_LEVEL_1					0x5c
#define MAP_A3_SEWERS_LEVEL_2					0x5d
#define MAP_A3_RUINED_TEMPLE					0x5e
#define MAP_A3_DISUSED_FANE						0x5f
#define MAP_A3_FORGOTTEN_RELIQUARY				0x60
#define MAP_A3_FORGOTTEN_TEMPLE					0x61
#define MAP_A3_RUINED_FANE						0x62
#define MAP_A3_DISUSED_RELIQUARY				0x63
#define MAP_A3_DURANCE_OF_HATE_LEVEL_1			0x64
#define MAP_A3_DURANCE_OF_HATE_LEVEL_2			0x65
#define MAP_A3_DURANCE_OF_HATE_LEVEL_3			0x66

///////////////////////////////////////////////////
// Act 4 Maps
///////////////////////////////////////////////////
#define MAP_A4_THE_PANDEMONIUM_FORTRESS			0x67
#define MAP_A4_OUTER_STEPPES					0x68
#define MAP_A4_PLAINS_OF_DESPAIR				0x69
#define MAP_A4_CITY_OF_THE_DAMNED				0x6a
#define MAP_A4_RIVER_OF_FLAME					0x6b
#define MAP_A4_THE_CHAOS_SANCTUARY				0x6c

///////////////////////////////////////////////////
// Act 5 Maps
///////////////////////////////////////////////////
#define MAP_A5_HARROGATH						0x6d
#define MAP_A5_THE_BLOODY_FOOTHILLS				0x6e
#define MAP_A5_FRIGID_HIGHLANDS					0x6f
#define MAP_A5_ARREAT_PLATEAU					0x70
#define MAP_A5_CRYSTALLINE_PASSAGE				0x71
#define MAP_A5_FROZEN_RIVER						0x72
#define MAP_A5_GLACIAL_TRAIL					0x73
#define MAP_A5_DRIFTER_CAVERN					0x74
#define MAP_A5_FROZEN_TUNDRA					0x75
#define MAP_A5_THE_ANCIENTS_WAY					0x76
#define MAP_A5_ICY_CELLAR						0x77
#define MAP_A5_ARREAT_SUMMIT					0x78
#define MAP_A5_NIHLATHAKS_TEMPLE				0x79
#define MAP_A5_HALLS_OF_ANGUISH					0x7a
#define MAP_A5_HALLS_OF_PAIN					0x7b
#define MAP_A5_HALLS_OF_VAUGHT					0x7c
#define MAP_A5_ABADDON							0x7d
#define MAP_A5_PIT_OF_ACHERON					0x7e
#define MAP_A5_INFERNAL_PIT						0x7f
#define MAP_A5_WORLDSTONE_KEEP_LEVEL_1			0x80
#define MAP_A5_WORLDSTONE_KEEP_LEVEL_2			0x81
#define MAP_A5_WORLDSTONE_KEEP_LEVEL_3			0x82
#define MAP_A5_THRONE_OF_DESTRUCTION			0x83
#define MAP_A5_THE_WORLDSTONE_CHAMBER			0x84


////////////////////////////////////////////////////////////////////
// Player Mode Definition, returned by "me->GetMode();"
// Added v2.00 by Abin
////////////////////////////////////////////////////////////////////
enum {	MODE_DEATH = 0,				// death
		MODE_STAND_OUTTOWN,			// standing outside town
		MODE_WALK_OUTTOWN,			// walking outside town
		MODE_RUN,					// running
		MODE_BEING_HIT,				// being hit
		MODE_STAND_INTOWN,			// standing inside town
		MODE_WALK_INTOWN,			// walking outside town
		MODE_ATTECK1,				// attacking 1
		MODE_ATTACK2,				// attacking 2
		MODE_BLOCK,					// blocking
		MODE_CAST,					// casting spell
		MODE_THROW,					// throwing
		MODE_KICK,					// kicking (assassin)
		MODE_USESKILL1,				// using skill 1
		MODE_USESKILL2,				// using skill 2
		MODE_USESKILL3,				// using skill 3
		MODE_USESKILL4,				// using skill 4
		MODE_DEAD,					// dead
		MODE_SEQUENCE,				// sequence
		MODE_BEING_KNOCKBACK };		// being knocked back


////////////////////////////////////////////////////////////////////
// Item Related
////////////////////////////////////////////////////////////////////
enum 
{ 
	POTION_UNKNOWN = 0,
	POTION_RED,
	POTION_BLUE,
	POTION_PURPLE,
	POTION_ANTIDOTE,
	POTION_STAMINA,
	POTION_THAWING,
	POTION_SPECIAL,
	POTION_END
};

#define	ITEM_MESSAGEID_DROP				0x9C // drop/pick from belt/ground
#define	ITEM_MESSAGEID_OPERATE			0x9D // pick/equip
	
#define	ITEM_ACTION_NEW_GROUND			0x00 //9c
#define	ITEM_ACTION_PICK_UP				0x01 //9c
#define	ITEM_ACTION_DROP				0x02 //9c
#define	ITEM_ACTION_OLD_GROUND			0x03 //9c
#define	ITEM_ACTION_TO_STORAGE			0x04 //9c
#define	ITEM_ACTION_FROM_STORAGE		0x05 //9d
#define	ITEM_ACTION_TO_EQUIP			0x06 //9d
#define	ITEM_ACTION_FROM_EQUIP			0x08 //9d
#define	ITEM_ACTION_SWITCH_EQUIP		0x09 //9d
#define	ITEM_ACTION_TO_STORE			0x0B //9c
#define	ITEM_ACTION_FROM_STORE			0x0C //9c
#define	ITEM_ACTION_SWITCH_STORAGE		0x0d //9c
#define	ITEM_ACTION_TO_BELTSLOT			0x0E //9c
#define	ITEM_ACTION_FROM_BELTSLOT		0x0F //9c
#define	ITEM_ACTION_SWITCH_BELTSLOT		0x10 //9c
#define ITEM_ACTION_TO_CURSOR			0x12 //9c
#define ITEM_ACTION_SHIFT_BELTSLOT		0x15 //9d
	
#define	ITEM_ITEMTYPE_HELM				0x00
#define	ITEM_ITEMTYPE_ARMOR				0x01
#define	ITEM_ITEMTYPE_WEAPON			0x05
#define	ITEM_ITEMTYPE_BOW				0x06
#define	ITEM_ITEMTYPE_SHIELD			0x07
#define	ITEM_ITEMTYPE_EXPANSION			0x0A // heads
#define	ITEM_ITEMTYPE_OTHER				0x10

// Drop locations
#define	ITEM_LOCAIION_STORED			0x00
#define	ITEM_LOCATION_WORN				0x01
#define	ITEM_LOCATION_BELT				0x02
#define	ITEM_LOCATION_GROUND			0x03
#define	ITEM_LOCATION_TOHAND			0x04

#define	ITEM_LEVEL_INFERIOR				0x01
#define	ITEM_LEVEL_NORMAL				0x02
#define	ITEM_LEVEL_SUPERIOR				0x03
#define	ITEM_LEVEL_MAGIC				0x04
#define	ITEM_LEVEL_SET					0x05
#define	ITEM_LEVEL_RARE					0x06
#define	ITEM_LEVEL_UNIQUE				0x07
#define	ITEM_LEVEL_CRAFT				0x08

// No idea why they named it ITEM_LEVEL_* for 
//   item quality.
#define	ITEM_QUALITY_INFERIOR			ITEM_LEVEL_INFERIOR
#define	ITEM_QUALITY_NORMAL				ITEM_LEVEL_NORMAL
#define	ITEM_QUALITY_SUPERIOR			ITEM_LEVEL_SUPERIOR
#define	ITEM_QUALITY_MAGIC				ITEM_LEVEL_MAGIC
#define	ITEM_QUALITY_SET				ITEM_LEVEL_SET
#define	ITEM_QUALITY_RARE				ITEM_LEVEL_RARE
#define	ITEM_QUALITY_UNIQUE				ITEM_LEVEL_UNIQUE
#define	ITEM_QUALITY_CRAFT				ITEM_LEVEL_CRAFT

#define	ITEM_INFERIORTYPE_CRUDE			0x00
#define	ITEM_INFERIORTYPE_CRACKED		0x01
#define	ITEM_INFERIORTYPE_DAMAGED		0x02
#define	ITEM_INFERIORTYPE_LOWQUALITY	0x04
#define	ITEM_SUPERIOR_AR				0x00
#define	ITEM_SUPERIOR_MAXDMG			0x01
#define	ITEM_SUPERIOR_AC				0x02
#define	ITEM_SUPERIOR_AR_MAXDMG			0x03
#define	ITEM_SUPERIOR_DUR				0x04
#define	ITEM_SUPERIOR_DUR_AR			0x05
#define	ITEM_SUPERIOR_DUR_MAXDMG		0x06
#define	ITEM_SUPERIOR_DUR_AC			0x07

// Gold sizes
#define	ITEM_GOLDSIZE_12BIT				0x00
#define	ITEM_GOLDSIZE_32BIT				0x01
#define	ITEM_UNSET						0xFF

///////////////////////////////////////////////////////////////
//    Common Spells
///////////////////////////////////////////////////////////////

#define D2S_INVALID				0xffff // should never happen
#define D2S_ATTACK				0x0000
#define D2S_KICK				0x0001
#define D2S_THROW				0x0002
#define D2S_UNSUMMON			0x0003
#define D2S_LEFTTHROW			0x0004
#define D2S_LEFTSWING			0x0005
#define D2S_TOMEOFIDENTIFY		0x00da
#define D2S_SCROLLOFIDENTIFY	0x00d9
#define D2S_TOMEOFTOWNPORTAL	0x00dc
#define D2S_SCROLLOFTOWNPORTAL	0x00db

///////////////////////////////////////////////////////////////
//    Amazon Spells
///////////////////////////////////////////////////////////////

// Javelin and Spear spells
#define D2S_JAB					0x000a
#define D2S_IMPALE				0x0013
#define D2S_FEND				0x001e
#define D2S_POWERSTRIKE			0x000e
#define D2S_CHARGEDSTRIKE		0x0018
#define D2S_LIGHTNINGSTRIKE		0x0022
#define D2S_POISONJAVELIN		0x000f
#define D2S_LIGHTNINGBOLT		0x0014
#define D2S_PLAGUEJAVELIN		0x0019
#define D2S_LIGHTNINGFURY		0x0023

// Passive and Magic spells
#define D2S_INNERSIGHT			0x0008
#define D2S_SLOWMISSILES		0x0011
#define D2S_DECOY				0x001c
#define D2S_VALKYRIE			0x0020
#define D2S_DODGE				0x000d
#define D2S_AVOID				0x0012
#define D2S_EVADE				0x001d
#define D2S_CRITICALSTRIKE		0x0009
#define D2S_PENETRATE			0x0017
#define D2S_PIERCE				0x0021

// Bow and Crossbow spells
#define D2S_COLDARROW			0x000b
#define D2S_ICEARROW			0x0015
#define D2S_FREEZINGARROW		0x001f
#define D2S_MAGICARROW			0x0006
#define D2S_MULTIPLESHOT		0x000c
#define D2S_GUIDEDARROW			0x0016
#define D2S_STRAFE				0x001a
#define D2S_FIREARROW			0x0007
#define D2S_EXPLODINGARROW		0x0010
#define D2S_IMMOLATIONARROW		0x001b


///////////////////////////////////////////////////////////////
//    Assassin Spells
///////////////////////////////////////////////////////////////

// Martial Arts
#define D2S_FISTSOFFIRE			0x0103
#define D2S_CLAWSOFTHUNDER		0x010d
#define D2S_BLADESOFICE			0x0112
#define D2S_TIGERSTRIKE			0x00fe
#define D2S_COBRASTRIKE			0x0109
#define D2S_PHOENIXSTRIKE		0x0118
#define D2S_DRAGONTALON			0x00ff
#define D2S_DRAGONCLAW			0x0104
#define D2S_DRAGONTAIL			0x010e
#define D2S_DRAGONFLIGHT		0x0113

// Shadow Disciplines
#define D2S_BURSTOFSPEED		0x0102
#define D2S_FADE				0x010b
#define D2S_VENOM				0x0116
#define D2S_CLAWMASTERY			0x00fc
#define D2S_WEAPONBLOCK			0x0107
#define D2S_SHADOWWARRIOR		0x010c
#define D2S_SHADOWMASTER		0x0117
#define D2S_PSYCHICHAMMER		0x00fd
#define D2S_CLOAKOFSHADOWS		0x0108
#define D2S_MINDBLAST			0x0111

// Traps
#define D2S_SHOCKWEB			0x0100
#define D2S_CHARGEDBOLTSENTRY	0x0105
#define D2S_LIGHTNINGSENTRY		0x010f
#define D2S_DEATHSENTRY			0x0114
#define D2S_FIREBLAST			0x00fb
#define D2S_WAKEOFFIRE			0x0106
#define D2S_WAKEOFINFERNO		0x0110
#define D2S_BLADESENTINEL		0x0101
#define D2S_BLADEFURY			0x010a
#define D2S_BLADESHIELD			0x0115


///////////////////////////////////////////////////////////////
//    Barbarian Spells
///////////////////////////////////////////////////////////////

// Warcries
#define D2S_HOWL				0x0082
#define D2S_TAUNT				0x0089
#define D2S_BATTLECRY			0x0092
#define D2S_WARCRY				0x009a
#define D2S_SHOUT				0x008a
#define D2S_BATTLEORDERS		0x0095
#define D2S_BATTLECOMMAND		0x009b
#define D2S_FINDPOTION			0x0083
#define D2S_FINDITEM			0x008e
#define D2S_GRIMWARD			0x0096

// Combat Masteries
#define D2S_SWORDMASTERY		0x007f
#define D2S_POLEARMMASTERY		0x0086
#define D2S_INCREASEDSTAMINA	0x008d
#define D2S_INCREASEDSPEED		0x0094
#define D2S_AXEMASTERY			0x0080
#define D2S_THROWINGMASTERY		0x0087
#define D2S_MACEMASTERY			0x0081
#define D2S_SPEARMASTERY		0x0088
#define D2S_IRONSKIN			0x0091
#define D2S_NATURALRESISTANCE	0x0099

#define D2S_ES_BARBTHUNDERSTORM 0x0182

// Combat spells
#define D2S_LEAP				0x0084
#define D2S_LEAPATTACK			0x008f
#define D2S_WHIRLWIND			0x0097
#define D2S_BASH				0x007e
#define D2S_STUN				0x008b
#define D2S_CONCENTRATE			0x0090
#define D2S_BERSERK				0x0098
#define D2S_DOUBLESWING			0x0085
#define D2S_DOUBLETHROW			0x008c
#define D2S_FRENZY				0x0093


///////////////////////////////////////////////////////////////
//    Druid Spells
///////////////////////////////////////////////////////////////

// Elemental
#define D2S_FIRESTORM			0x00e1
#define D2S_MOLTENBOULDER		0x00e5
#define D2S_FISSURE				0x00ea
#define D2S_VOLCANO				0x00f4
#define D2S_ARMAGEDDON			0x00f9
#define D2S_TWISTER				0x00f0
#define D2S_TORNADO				0x00f5
#define D2S_HURRICANE			0x00fa
#define D2S_ARCTICBLAST			0x00e6
#define D2S_CYCLONEARMOR		0x00eb

// Shape Shifting
#define D2S_WEREWOLF			0x00df
#define D2S_FERALRAGE			0x00e8
#define D2S_RABIES				0x00ee
#define D2S_FURY				0x00f8
#define D2S_LYCANTHROPY			0x00e0
#define D2S_FIRECLAWS			0x00ef
#define D2S_HUNGER				0x00f2
#define D2S_WEREBEAR			0x00e4
#define D2S_MAUL				0x00e9
#define D2S_SHOCKWAVE			0x00f3

// Summoning
#define D2S_OAKSAGE				0x00e2
#define D2S_HEARTOFWOLVERINE	0x00ec
#define D2S_SPIRITOFBARBS		0x00f6
#define D2S_RAVEN				0x00dd
#define D2S_SUMMONSPIRITWOLF	0x00e3
#define D2S_SUMMONDIREWOLF		0x00ed
#define D2S_SUMMONGRIZZLY		0x00f7
#define D2S_POISONCREEPER		0x00de
#define D2S_CARRIONVINE			0x00e7
#define D2S_SOLARCREEPER		0x00f1


///////////////////////////////////////////////////////////////
//    Necromancer Spells
///////////////////////////////////////////////////////////////

// Summoning Spells
#define D2S_SKELETONMASTERY		0x0045
#define D2S_GOLEMMASTERY		0x004f
#define D2S_SUMMONRESIST		0x0059
#define D2S_CLAYGOLEM			0x004b
#define D2S_BLOODGOLEM			0x0055
#define D2S_IRONGOLEM			0x005a
#define D2S_FIREGOLEM			0x005e
#define D2S_RAISESKELETON		0x0046
#define D2S_RAISESKELETALMAGE	0x0050
#define D2S_REVIVE				0x005f

// Poison and Bone Spells
#define D2S_POISONDAGGER		0x0049
#define D2S_POISONEXPLOSION		0x0053
#define D2S_POISONNOVA			0x005c
#define D2S_TEETH				0x0043
#define D2S_CORPOSEEXPLOSION	0x004a
#define D2S_BONESPEAR			0x0054
#define D2S_BONESPIRIT			0x005d
#define D2S_BONEARMOR			0x0044
#define D2S_BONEWALL			0x004e
#define D2S_BONEPRISON			0x0058

// Curses
#define D2S_DIMVISION			0x0047
#define D2S_CONFUSE				0x0051
#define D2S_ATTRACT				0x0056
#define D2S_AMPLIFYDAMAGE		0x0042
#define D2S_IRONMAIDEN			0x004c
#define D2S_LIFETAP				0x0052
#define D2S_LOWERRESIST			0x005b
#define D2S_WEAKEN				0x0048
#define D2S_TERROR				0x004d
#define D2S_DECREPIFY			0x0057


///////////////////////////////////////////////////////////////
//    Paladin Spells
///////////////////////////////////////////////////////////////

// Defensive Auras
#define D2S_PRAYER				0x0063
#define D2S_CLEANSING			0x006d
#define D2S_MEDITATION			0x0078
#define D2S_DEFIANCE			0x0068
#define D2S_VIGOR				0x0073
#define D2S_REDEMPTION			0x007c
#define D2S_RESISTFIRE			0x0064
#define D2S_RESISTCOLD			0x0069
#define D2S_RESISTLIGHTNING		0x006e
#define D2S_SALVATION			0x007d

// Offensive Auras
#define D2S_MIGHT				0x0062
#define D2S_BLESSEDAIM			0x006c
#define D2S_CONCENTRATION		0x0071
#define D2S_FANATICISM			0x007a
#define D2S_HOLYFIRE			0x0066
#define D2S_HOLYFREEZE			0x0072
#define D2S_HOLYSHOCK			0x0076
#define D2S_THORNS				0x0067
#define D2S_SANCTUARY			0x0077
#define D2S_CONVICTION			0x007b

// Combat spells
#define D2S_SACRIFICE			0x0060
#define D2S_ZEAL				0x006a
#define D2S_VENGEANCE			0x006f
#define D2S_CONVERSION			0x0074
#define D2S_HOLYBOLT			0x0065
#define D2S_BLESSEDHAMMER		0x0070
#define D2S_FISTOFTHEHEAVENS	0x0079
#define D2S_SMITE				0x0061
#define D2S_CHARGE				0x006b
#define D2S_HOLYSHIELD			0x0075


///////////////////////////////////////////////////////////////
//    Sorceress Spells
///////////////////////////////////////////////////////////////

// Cold Spells
#define D2S_FROSTNOVA			0x002c
#define D2S_BLIZZARD			0x003b
#define D2S_FROZENORB			0x0040
#define D2S_ICEBOLT				0x0027
#define D2S_ICEBLAST			0x002d
#define D2S_GLACIALSPIKE		0x0037
#define D2S_COLDMASTERY			0x0041
#define D2S_FROZENARMOR			0x0028
#define D2S_SHIVERARMOR			0x0032
#define D2S_CHILLINGARMOR		0x003c

// Lightning Spells
#define D2S_ES_SHOCKARMOR		0x0028

#define D2S_STATICFIELD			0x002a
#define D2S_NOVA				0x0030
#define D2S_THUNDERSTORM		0x0039
#define D2S_CHARGEDBOLT			0x0026
#define D2S_LIGHTNING			0x0031
#define D2S_CHAINLIGHTNING		0x0035
#define D2S_LIGHTNINGMASTERY	0x003f
#define D2S_TELEKINESIS			0x002b
#define D2S_TELEPORT			0x0036
#define D2S_ENERGYSHIELD		0x003a


// Fire Spells
#define D2S_INFERNO				0x0029
#define D2S_BLAZE				0x002e
#define D2S_FIREWALL			0x0033
#define D2S_FIREBOLT			0x0024
#define D2S_FIREBALL			0x002f
#define D2S_METEOR				0x0038
#define D2S_FIREMASTERY			0x003d
#define D2S_WARMTH				0x0025
#define D2S_ENCHANT				0x0034
#define D2S_HYDRA				0x003e


#endif // __CONSTANTS_H__