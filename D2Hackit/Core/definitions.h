#ifndef __DEFINITIONS_H__
#define __DEFINITIONS_H__

///////////////////////////////////////////////
// Originally Mousepad's Code
///////////////////////////////////////////////

///////////////////////////////////////////////
// And Ninjai D2BS Code
///////////////////////////////////////////////
#define D2NET_MAX_MSG_SIZE 512
#define MAX_MSG_SIZE 516

#include <cstdint>

enum eUIModes { UI_ON = 0, UI_OFF = 1, UI_TOGGLE = 2 };

struct UnitAny;
struct UnitItem;
struct UnitMonster;
struct Act;
struct Room;
struct RoomOther;
struct Drlg;
struct PresetUnit;
struct DrlgColl;
struct DrlgDS1Pre;
struct PresetType2info;
struct Inventory;

struct ObjectTxt {
	char szName[0x40];	// +00
	wchar_t wszName[0x40]; // +40
	BYTE _1[4]; // +c0
	BYTE nSelectable0; //+c4
	BYTE _2[0x87];
	BYTE nOrientation; //+14c
	BYTE _2b[0x19];
	BYTE nSubClass; // +166
	BYTE _3[0x11];
	BYTE nParm0; //+178
	BYTE _4[0x39];
	BYTE nPopulateFn; //+1b2
	BYTE nOperateFn; //+1b3
	BYTE _5[8];
	DWORD nAutoMap; // +1bb
};

struct ObjgroupTxt {
	DWORD nId[8]; //+00
	BYTE nDensity[8]; //+20
	BYTE nProb[8]; //+28
};

struct LevelTxt {
	BYTE _1[0xf5];
	char cszName[40]; //+f5
	char cszName2[40]; //+11d
	char szLvlDesc[41]; //+145
	wchar_t szName[40]; //+16e
	wchar_t szName2[40]; //+16e
	BYTE nObjGrp[8]; // unk
	BYTE nObjPrb[8]; // unk
	BYTE _1b[0x79];
};

/*
struct ItemTxt {
	wchar_t szName2[0x40]; //+00
	union {
		DWORD dwCode; // +80
		char szCode[4];
	};
	BYTE _2[0x70]; // +84
	WORD nLocaleTxtNo; // +f4
	BYTE _2a[0x28]; // +f6
	BYTE nType; // +11e
	BYTE _3[0x0d];
	BYTE fQuest; // +12a
};
*/
struct MonsterTxt {
	BYTE _1[0x6];
	WORD nLocaleTxtNo; // +06
	WORD flag; // +08
	WORD _1a;
	union {
		DWORD flag1; // +0c
		struct {
			BYTE flag1a; // +0c
			BYTE flag1b; // +0d
			BYTE flag1c[2]; // +0e
		};
	};
	BYTE _2[0x22]; // +10
	WORD velocity; // +32
	BYTE _2a[0x52]; // +34
	WORD tcs[3][4]; //+86
	BYTE _2b[0x52]; // +9e
	wchar_t szDescriptor[0x3c]; //+f0
	BYTE _3[0x1a0];
};

struct SuperuniqueTxt {
	WORD nIndex; //+00
	WORD nLocaleTxtNo; //+02
	DWORD _1[9]; //+04
	BYTE _1a[4]; //+28
	WORD tcs[4]; //+2c
};

struct Point {
	short X;
	short Y;
};

struct D2MSG {
	HWND hwnd;				// 0x00
	DWORD message;				// 0x04
	WPARAM wParam;				// 0x08
	union {
		LPARAM lParam;
		Point pt;
	};						// 0x0C
	DWORD _1;					// 0x10
	DWORD hwnd2;				// 0x14
	DWORD _2[2];				// 0x18
};

/*struct D2HANDLER {
	DWORD _1;					// 0x00
	DWORD Message;				// 0x04
	DWORD Handler;				// 0x08
};*/
// Third and final piece to the "Handler Table" experiment.  Didn't quite work
// This struct was one of the reasons.  Mainly, I could decide what the _1 was
// and if it should go at the beginning of the struct, or at the end.

struct BnetData
{
	DWORD Id;					//0x00
	DWORD Id2;					//0x04
	DWORD Unk0;					//0x08
	DWORD Unk1;					//0x0C
	DWORD Unk2;					//0x10
	DWORD Id3;					//0x14
	WORD Unk3;					//0x18
	char GameName[22];			//0x1A
	char GameIp[16];			//0x30
	DWORD Unk4[16];				//0x40
	DWORD Id4;					//0x80
	DWORD Unk6;					//0x84
	char AccountName[48];		//0x88
	char PlayerName[24];		//0xB8
	char RealmName[8];			//0xD0
	BYTE Unk7[273];				//0xD8
	BYTE CharType;				//0x1E9 00 - 06
	BYTE CharFlags;				//0x1EA LoD, Ladder, Hc Flags + 4 Unk
	BYTE MaxDifficulty;			//0x1EB
	BYTE Unk8[31];				//0x1EC
 	BYTE Difficulty;			//0x20B Works only when creating, not joining game
	DWORD *pUnk9;				//0x20C
	DWORD Unk10[3];				//0x210
	DWORD Unk11;				//0x21C
	DWORD Unk12;				//0x220
	WORD Unk13;					//0x224
	BYTE Unk14;					//0x226
	char RealmName2[24];		//0x227
	char GamePass[24];			//0x23F
	char GameDesc[256];			//0x257
	WORD Unk15;					//0x348
	BYTE Unk16;					//0x34A
	char CurChannel[32];		//0x34B
};

enum eMessageTypes { CHAT_NORMAL = 1, CHAT_WHISPERFROM, CHAT_3, CHAT_NONAME, CHAT_OVERHEAD, CHAT_WHISPERTO, CHAT_SCROLL };

struct ChatMsg {
   BYTE Identifier;        // 0x00  // should always be 0x26
   BYTE MessageType;       // 0x01  // 01 is standard chat message

                                    // 02 prints in the format "%s whispers: %s" where the first var is Name, and the
                                    // second is Message.

                                    // 04 bypasses the name checks, etc, and prints the message with color specified in
                                    // Color.

                                    // 05 creates an overhead struct and assigns it to the unit with id (UnitId)

                                    // 06 prints the "You whispered to %s:  %s, with the first var being Name, and the second
                                    // being Message.

                                    // 07 prints Message on a recipe scroll background.  Recipe scroll pane opens but lacks
                                    // buttons.
   BYTE Unk1;              // 0x02
   BYTE Style;             // 0x03  // 00 and 01 seem to print without sender, but 00 does some sort of checking, first
                                    // anything else prints as usual.
   DWORD UnitId;           // 0x04
   BYTE Color;             // 0x08
   BYTE Unk2;              // 0x09
   char Name[0x10];        // 0x0A
   char Message[0x100];    // 0x1A
};

struct Msg {
	BYTE Unk0;				//+00
   BYTE _1[3];                             //+01
	DWORD Unk1;				//+04
	DWORD Unk2;				//+08
	char Message[0x100];			//+0A //text msg 
};

struct Buffer
{
};

struct Quest
{
	Buffer *ptBuffer;			// 0x00
	DWORD Unk0;				// 0x04 // 60h * 2^3, 768
};

struct Waypoint
{
	BYTE flags;				// 0x00
};

struct PlrMsg
{
};

enum ePieceTokens {HD = 0, TR, LG, RA, LA, RH, LH, SH, S1, S2, S3, S4, S5, S6, S7, S8}; 

struct ObjectBinData {
   char Name[0x40];				// 0x00
   wchar_t wName[0x40];			// 0x40
   char Token[0x3];				// 0xC0
   BYTE SpawnMax;				// 0xC3
   BYTE Selectable[8];			// 0xC4
   DWORD TrapProb;				// 0xCC
   DWORD SizeX;				// 0xD0
   DWORD SizeY;				// 0xD4
   BYTE _1;					// 0xD8
   DWORD FrameCnt[8];			// 0xD9
   WORD FrameDelta[8];			// 0xF9
   BYTE CycleAnim[8];			// 0x109
   BYTE Lit[7];				// 0x111
   BYTE BlocksLight[8];			// 0x118
   BYTE Collision[8];			// 0x120
   BYTE IsAttackable;			// 0x128
   BYTE Start[8];				// 0x129
   BYTE OrderFlag[8];			// 0x131
   BYTE EnvEffect;				// 0x139
   BYTE IsDoor;				// 0x13A
   BYTE BlocksVis;				// 0x13B
   BYTE Orientation;			// 0x13C
   BYTE Trans;					// 0x13D
   BYTE PreOperate;				// 0x13E
   BYTE Mode[8];				// 0x13F
   short XOffset;				// 0x147
   short YOffset;				// 0x149
   bool Draw;					// 0x14B
   BYTE PieceTokens[0x10];		// 0x14F
   WORD _2;					// 0x15F
   BYTE TotalPieces;			// 0x161
   BYTE XSpace;				// 0x162
   BYTE YSpace;				// 0x163
   BYTE Red;					// 0x164
   BYTE Green;					// 0x165
   BYTE Blue;					// 0x166
   BYTE SubClass;				// 0x167
   long NameOffset;				// 0x168
   BYTE _3;					// 0x16C
   BYTE MonsterOK;				// 0x16D
   BYTE OperateRange;			// 0x16E
   BYTE ShrineFunction;			// 0x16F
   BYTE Act;					// 0x170 //this is a bitmask
   WORD _4;					// 0x171
   BYTE Restore;				// 0x173
   BYTE RestoreVirgins;			// 0x174
   BYTE _5[3];					// 0x175
   long Parm[8];				// 0x178
   /*BYTE Lockable;				// 0x198
   BYTE Gore;					// 0x199
   BYTE Sync;					// 0x19A
   BYTE Beta;					// 0x19B*/ // these don't seem to match as being here, for some reason.
   BYTE nTgtFX;				// 0x198
   BYTE nTgtFY;				// 0x199
   BYTE nTgtBX;				// 0x19A
   BYTE nTgtBY;				// 0x19B
   WORD Damage;				// 0x19C // temp
   WORD CollisionSubst;			// 0x19E // temp
   DWORD Left;					// 0x1A0
   DWORD Top;					// 0x1A4
   DWORD Width;				// 0x1A8
   DWORD Height;				// 0x1AC
   BYTE ClientFn;				// 0x1B0
   BYTE InitFn;				// 0x1B1
   BYTE PopulateFn;				// 0x1B2
   BYTE OperateFn;				// 0x1B3				
   // Leaving the rest undefined for now, until I get some more bin dumps.
};

// https://github.com/ThePhrozenKeep/D2MOO/blob/master/source/D2Common/include/DataTbls/ItemsTbls.h#L286

enum D2C_ItemInvPage
{
	INVPAGE_INVENTORY = 0,
	INVPAGE_EQUIP = 1,
	INVPAGE_TRADE = 2,
	INVPAGE_CUBE = 3,
	INVPAGE_STASH = 4,
	INVPAGE_BELT = 5,
	INVPAGE_NULL = 255
};
struct D2ItemsTxt
{
	char szFlippyFile[32];					//0x00
	char szInvFile[32];						//0x20
	char szUniqueInvFile[32];				//0x40
	char szSetInvFile[32];					//0x60
	union
	{
		uint32_t dwCode;					//0x80
		char szCode[4];						//0x80
	};
	uint32_t dwNormCode;					//0x84
	uint32_t dwUberCode;					//0x88
	uint32_t dwUltraCode;					//0x8C
	uint32_t dwAlternateGfx;				//0x90
	uint32_t dwPspell;						//0x94
	uint16_t wState;						//0x98
	uint16_t wCurseState[2];				//0x9A
	uint16_t wStat[3];						//0x9E
	uint32_t dwCalc[3];						//0xA4
	uint32_t dwLen;							//0xB0
	uint8_t nSpellDesc;						//0xB4
	uint8_t pad0xB5;						//0xB5
	uint16_t wSpellDescStr;					//0xB6
	uint32_t dwSpellDescCalc;				//0xB8
	uint32_t dwBetterGem;					//0xBC
	uint32_t dwWeapClass;					//0xC0
	uint32_t dwWeapClass2Hand;				//0xC4
	uint32_t dwTransmogrifyType;			//0xC8
	int32_t dwMinAc;						//0xCC
	int32_t dwMaxAc;						//0xD0
	uint32_t dwGambleCost;					//0xD4
	int32_t dwSpeed;						//0xD8
	uint32_t dwBitField1;					//0xDC
	uint32_t dwCost;						//0xE0
	uint32_t dwMinStack;					//0xE4
	uint32_t dwMaxStack;					//0xE8
	uint32_t dwSpawnStack;					//0xEC
	uint32_t dwGemOffset;					//0xF0
	uint16_t wNameStr;						//0xF4
	uint16_t wVersion;						//0xF6
	uint16_t wAutoPrefix;					//0xF8
	uint16_t wMissileType;					//0xFA
	uint8_t nRarity;						//0xFC
	uint8_t nLevel;							//0xFD
	uint8_t nMinDam;						//0xFE
	uint8_t nMaxDam;						//0xFF
	uint8_t nMinMisDam;						//0x100
	uint8_t nMaxMisDam;						//0x101
	uint8_t n2HandMinDam;					//0x102
	uint8_t n2HandMaxDam;					//0x103
	uint8_t nRangeAdder;					//0x104
	uint8_t unk0x105;						//0x105
	int16_t nStrBonus;						//0x106
	int16_t nDexBonus;						//0x108
	uint16_t wReqStr;						//0x10A
	uint16_t wReqDex;						//0x10C
	uint8_t nAbsorb;						//0x10E
	uint8_t nInvWidth;						//0x10F
	uint8_t nInvHeight;						//0x110
	uint8_t nBlock;							//0x111
	uint8_t nDurability;					//0x112
	uint8_t nNoDurability;					//0x113
	uint8_t nMissile;						//0x114
	uint8_t nComponent;						//0x115
	uint8_t nArmorComp[6];					//0x116
	uint8_t n2Handed;						//0x11C
	uint8_t nUseable;						//0x11D
	int16_t wType[2];						//0x11E
	uint8_t nSubType;						//0x122
	uint8_t unk0x123;						//0x123
	uint16_t wDropSound;					//0x124
	uint16_t wUseSound;						//0x126
	uint8_t nDropSfxFrame;					//0x128
	uint8_t nUnique;						//0x129
	uint8_t nQuest;							//0x12A
	uint8_t nQuestDiffCheck;				//0x12B
	uint8_t nTransparent;					//0x12C
	uint8_t nTransTbl;						//0x12D
	uint8_t pad0x12E;						//0x12E
	uint8_t nLightRadius;					//0x12F
	uint8_t nBelt;							//0x130
	uint8_t nAutoBelt;						//0x131
	uint8_t nStackable;						//0x132
	uint8_t nSpawnable;						//0x133
	uint8_t nSpellIcon;						//0x134
	uint8_t nDurWarning;					//0x135
	uint8_t nQuantityWarning;				//0x136
	uint8_t nHasInv;						//0x137
	uint8_t nGemSockets;					//0x138
	uint8_t nTransmogrify;					//0x139
	uint8_t nTmogMin;						//0x13A
	uint8_t nTmogMax;						//0x13B
	uint8_t nHitClass;						//0x13C
	uint8_t n1or2Handed;					//0x13D
	uint8_t nGemApplyType;					//0x13E
	uint8_t nLevelReq;						//0x13F
	uint8_t nMagicLevel;					//0x140
	int8_t nTransform;						//0x141
	int8_t nInvTrans;						//0x142
	uint8_t nCompactSave;					//0x143
	uint8_t nSkipName;						//0x144
	uint8_t nNameable;						//0x145
	uint8_t nVendorMin[17];					//0x146
	uint8_t nVendorMax[17];					//0x157
	uint8_t nVendorMagicMin[17];			//0x168
	uint8_t nVendorMagicMax[17];			//0x179
	uint8_t nVendorMagicLvl[17];			//0x18A
	uint8_t pad0x19B;						//0x19B
	uint32_t dwNightmareUpgrade;			//0x19C
	uint32_t dwHellUpgrade;					//0x1A0
	uint8_t nPermStoreItem;					//0x1A4
	uint8_t nMultibuy;						//0x1A5
	uint16_t pad0x1A6;						//0x1A6
};

struct MonsterBinData {
};

struct ItemBinData {
};

struct GlobalData {
};


struct PlayerData : GlobalData {
	char PlayerName[0x10];			// 0x00
 	Quest *ptQuestNormal;			// 0x10
 	Quest *ptQuestNightmare;		// 0x14
 	Quest *ptQuestHell;			// 0x18
 	Waypoint *ptWaypointNormal;		// 0x1c
	Waypoint *ptWaypointNightmare;		// 0x20
 	Waypoint *ptWaypointHell;		// 0x24
 	BYTE * Unk0;				// 0x28
	DWORD Unk1;				// 0x2C
 	DWORD Unk2;				// 0x30
 	DWORD *ptAreaUnit;			// 0x34
 	DWORD Unk3[25];				// 0x38
 	PlrMsg *ptPlrMsg;			// 0x9C
};

struct ItemData : GlobalData {
	ItemBinData * ptBinData;			// 0x00
	DWORD _1[0x10];						// 0x04
	BYTE ItemLocation;					// 0X44
	DWORD _1b[0x5];						// 0x48
	Inventory * ptOwnerInv;				// 0x5C
	DWORD _2;							// 0x60
	UnitItem * ptNextInvenItem;			// 0x64
};

struct MonsterData {
	BYTE _1[0x16];
	struct {
		BYTE fUnk:1;
		BYTE fNormal:1;
		BYTE fChamp:1;
		BYTE fBoss:1;
		BYTE fMinion:1;
	}; //+16
	BYTE _1aa;
	wchar_t szName[1]; //+18
	BYTE _1a[2]; // +1a
	BYTE anEnchants[9]; // +1c
	BYTE _2a; // +25
	WORD uniqueno; // +26
	BYTE _2[50*4-14];
	struct {
		DWORD _3[1];
		WORD uniqueno; //+04
		WORD _4[1];
		DWORD anEnchants[9]; //+08
	} *pEnchants; //+cc
};

struct TileData : GlobalData {
	/*
[20:54] <taylorjonl> 	DWORD			nWarpID;		//+00
[20:54] <taylorjonl> 	int				xSelect;		//+04
[20:54] <taylorjonl> 	int				ySelect;		//+08
[20:54] <taylorjonl> 	int				dxSelect;		//+0c
[20:54] <taylorjonl> 	int				dySelect;		//+10
[20:54] <taylorjonl> 	int				xExitWalk;		//+14
[20:54] <taylorjonl> 	int				yExitWalk;		//+18
[20:54] <taylorjonl> 	int				cxOffset;		//+1c
[20:54] <taylorjonl> 	int				cyOffset;		//+20
[20:54] <taylorjonl> 	int				nLitVersion;	//+24
[20:54] <taylorjonl> 	int				nTiles;			//+28
[20:54] <taylorjonl> 	int				chDirection;	//+2c	b/l/r
*/
};

struct ObjectData : GlobalData { //0x80 bytes in length
   ObjectBinData * ptBinData;
   DWORD _1;
   DWORD _2;
   DWORD _3;
   DWORD _4;
   DWORD _5;
   DWORD _6;
   DWORD _7;
   DWORD _8;
   DWORD _9;
   char parent[16];
};


struct Stats {

};

struct StatsList {
	DWORD _1[0x9];			// 0x00
	Stats * ptStats;		// 0x24
	WORD StatCount;		// 0x28
	WORD StatCount2;   		// 0x2A
	DWORD _2[2];			// 0x2C
	BYTE * _3;			// 0x34
	DWORD _4;				// 0x38
	StatsList * ptNext;		// 0x3C
};

struct DrlgLevel {
   Drlg * pDrlg;                  // 0x00
   int LevelNo;                   // 0x04
   DWORD _1[6];
   DWORD LevelX;					//1C
   DWORD LevelY;					//20
   DWORD LevelCX;					
   DWORD LevelCY;					
   RoomOther *pRoomOtherFirst;			//+30
   DWORD _3;                      // 0x34
   BYTE * ptMazeDef;              // 0x38
   DrlgDS1Pre *pRoom2FirstDS1Pre; // 0x3C
   DWORD _5[0x28];                // 0x40
   BYTE * ptLogicalRoomInfo;      // 0xE0
   DWORD _6[0x47];				// 0xE4
   DWORD _7[11];				//+200
   DrlgLevel *pNext;			//0x204
   DWORD _8[3];				//0x208
   BYTE * pnJungleDefs;           // 0x214
   // Leaving the rest undefined for now.
};
struct Drlg {
	DrlgLevel * pFirstLevel;       	// 0x00
	DWORD _1[2];			// 0x04
	Act * pAct;			// 0x0C
	int ActNum;			// 0x10
	DWORD _2[248];		//0x14
	DWORD nStaffTombLvl; //+3f4
	DWORD nBossTombLvl; //+3f8
	// Leaving the rest undefined for now.		
};

struct PresetUnit {
   DWORD nUnitType; //+00
   DWORD nTxtFileNo; //+04
   DWORD _1[1];
   DWORD xPos; //+0c position in relation to its room
   DWORD yPos; //+10 position in relation to its room
   DWORD _2[2];
   PresetUnit *pNext; //+1c
};

struct RoomTile {
	RoomOther *ptRoomOther;
	DWORD * nNum;
	DWORD _1[3];
	RoomTile *pNext;
};

struct RoomOther {
	DrlgLevel *ptDrlgLevel;       // 0x00
	DWORD xPos;                   // 0x04
	DWORD yPos;                   // 0x08
	DWORD xSize;				// 0x0C
	DWORD ySize;				// 0x10
	BYTE * _2;                    // 0x14
	DWORD _3[0x1];                // 0x18
	DWORD nPresetType;				//+1c   //1 for room1 type 2 for regular room
	PresetType2info * pPresetType2info; //+20
	DWORD _5;                     // 0x24
	BYTE * _6[0x10];              // 0x28
	DWORD _7[0x10];               // 0x68
	BYTE * _8;                    // 0xA8
	DWORD _9[0x3];                // 0xAC
	RoomOther * ptNext;           // 0xB8 //Only appears when DrlgRoom1 appears
	RoomOther * ptPrev;           // 0xBC //these 2 are sometimes rooms, sometimes not a room. Only appears when DrlgRoom1 appears
	RoomOther ** ptList;          // 0xC0 //rooms that are near you  //This all makes more sense now, when they are  close, the above is a linked list
	int nRoomList;                // 0xC4 //the number of rooms near you
	DWORD _10;                    // 0xC8 RoomTile *pRoomTiles; //+c8 Only appears when DrlgRoom1 appears
	PresetUnit *pPresetUnits;	// +cc Only appears when DrlgRoom1 appears //EXTREMELY IMPORTANT
	DWORD _12[0x4];               // 0xD0
	BYTE * _13;                   // 0xE0
	Room *pRoom;				// 0xE4 //Pointer to the room1 associated with this room2. Collision data resides in the room1 structure
	RoomOther *pNext;             // 0xE8
};

struct PresetType2info {
	DWORD roomNum;   
	DWORD _5[1];   
	DrlgDS1Pre *pDrlgDS1Pre;
};

struct DrlgDS1Pre {
	DWORD ds1roomNum;
	DWORD ds1used;
	//ds1fileinfo *pds1fileinfo;
	DWORD tmp1;
	//placed here because the actual ds1 file is not important
	DrlgDS1Pre *pNext;
	DWORD posX;
	DWORD posY;
	DWORD sizex;
	DWORD sizey;
	DWORD _1[13];
	DrlgDS1Pre *pPrevious;
};

struct Act {
	int ActNum;			                   // 0x00
	Room * ptFirstRoom;		                   // 0x04
	Drlg * ptMisc;			                   // 0x08
	DWORD _1[2];			                   // 0x0C
	BYTE * _2;			                   // 0x14
	//__fastcall int(*pfnRoomCallBack)(DWORD, DWORD);    // 0x18 act callback.
	DWORD pfnActCallBack;                              // 0x18
	DWORD _4[4];                                       // 0x1C
};

struct Coords {
   DWORD BeginXpos;              // 0x00
   DWORD BeginYpos;              // 0x04
   DWORD Xsize;                  // 0x08
   DWORD Ysize;                  // 0x0C
   DWORD _1[2];                  // 0x10
   DWORD _2[2];                  // 0x18
};

struct Room : Coords {
	BYTE * _3;                    // 0x20
	Room ** ptRoomList;           // 0x24
	int NumRooms;                 // 0x28
	UnitAny * ptFirstUnit;        // 0x2C
	UnitAny * ptOtherUnit;        // 0x30 ?
	DrlgColl *pColl;			// +34 --> discovered by woof
	RoomOther * ptRoomOther;      // 0x38
	DWORD Seed;                   // 0x3C
	DWORD _6[0x3];                // 0x40
	BYTE * _7;                    // 0x4C
	DWORD _8[3];                  // 0x50
	int nPlayerUnits;             // 0x5C
	DWORD _9[6];                  // 0x60
	Act * ptAct;                  // 0x78
	Room * ptNext;                // 0x7C
};

struct DrlgColl {
   DWORD            nPosGameX;
   DWORD            nPosGameY;
   DWORD            nSizeGameX;
   DWORD            nSizeGameY;
   DWORD            nPosRoomX;
   DWORD            nPosRoomY;
   DWORD            nSizeRoomX; 
   DWORD            nSizeRoomY;
   WORD *pMapStart;
   WORD *pMapEnd;   
   /* 0028 end of struct Collision 40 bytes */
};

struct Path 
{
	WORD wXOffset;				// 0x00
	WORD wX;				// 0x02
	WORD wYOffset;				// 0x04
	WORD wY;				// 0x06
	DWORD targetX;				// 0x08 
	DWORD targetY;				// 0x0C
	WORD sY;				// 0x10
	WORD sX;				// 0x12
	WORD sY2;				// 0x14
	WORD sX2;				// 0x16
	WORD sY3;				// 0x18
	WORD sX3;				// 0x1A
	Room * ptRoom; 				// 0x1C
	Room * ptRoom2;            		// 0x20
	DWORD _1[3];                            // 0x24
	UnitAny * ptUnit;       		// 0x30
	DWORD flags;            		// 0x34
	DWORD _2;               		// 0x38
	DWORD PathType;         		// 0x3C
	DWORD PrevPathType;     		// 0x40
	DWORD UnitSize;         		// 0x44
	DWORD CollisionPattern; 		// 0x48
	DWORD _3[3];            		// 0x4C
	UnitAny * ptTarget;     		// 0x58
	int TargetType;         		// 0x5C
	DWORD TargetId;         		// 0x60
	DWORD _4[0xF];                          // 0x64
	// Leaving the rest undefined for now
};

struct ObjPath
{
	Room * ptRoom;				// 0x00
	DWORD _1[2];				// 0x04
	DWORD Xpos;				// 0x0C
	DWORD Ypos;				// 0x10
	DWORD _2[5];				// 0x14
	BYTE * ptUnk;				// 0x28
	DWORD _3[5];				// 0x2C
};



struct Inventory {
	DWORD Signature;  			// 0x00  always 0x01020304
	BYTE * ptGame1C;              		// 0x04 //will be 0 for client
	UnitAny * ptOwner;			// 0x08
	UnitItem * pFirstItem;			// 0x0C
	UnitItem * pLastItem;			// 0x10
	BYTE * _1;				// 0x14
	DWORD _2;				// 0x18
	DWORD LeftItemUid;			// 0x1C
	UnitItem * CursorItem;			// 0x20
	DWORD OwnerUid;				// 0x24
	int NumItems;				// 0x28
	DWORD _3[0x2];				// 0x2C
	BYTE * _4;				// 0x34
	DWORD _5;					// 0x38
};

struct GfxInfo {
	DWORD Unks[0xF];
};

struct SkillInfo {
	WORD Id;				// 0x00
	BYTE bUnks[0x32];		// 0x02
	DWORD dwUnk;			// 0x34
};

struct Skill {				// 0x40 bytes long
	SkillInfo * pSkillInfo;	// 0x00
	Skill * pNextSkill;		// 0x04
	DWORD dwUnk1;			// 0x08
	DWORD dwUnk2[0x7];		// 0x0C
	DWORD SkillLevel;		// 0x28
	DWORD dwUnk3[2];		// 0x2C
	DWORD Flags;			// 0x34
	DWORD Charges;	    	// 0x38
	DWORD dwUnk5;		    // 0x3C
};

struct Info
{
	BYTE * ptGame_1C;              // 0x00
	Skill * ptFirstSkill;          // 0x04
	Skill * ptlSkill;             // 0x08
	Skill * ptrSkill;             // 0x0C
};

struct Light {
   DWORD _1[0x3];                // 0x00
   int nType;                    // 0x0C
   DWORD _2[0x7];                // 0x10
   DWORD fStaticValid;           // 0x2C
   int * pnStaticMap;            // 0x30

};

struct Game
{
};

struct Packet {
	long nTotal;				// 0x00
	long nUsed;				// 0x04
};

struct Roster {
	char Name[0x10];              // 0x00
	DWORD PlayerId;               // 0x10
	DWORD _1[0x2];                // 0x14
	DWORD ClassId;                // 0x1C
	WORD _2;                      // 0x20
	WORD _3;                      // 0x22
	DWORD LevelId;                // 0x24
	DWORD Xpos;                   // 0x28
	DWORD Ypos;                   // 0x2C
	DWORD _4;                     // 0x24
	BYTE * _5;                    // 0x34
	DWORD _6[0xB];                // 0x38
	WORD _7;                      // 0x64
	char Name2[0x10];             // 0x66
	WORD _8;                      // 0x76
	DWORD _9[0x2];                // 0x78
	Roster * ptNext;              // 0x80
};

// added needed DWORDS (spread)
struct RosterPet {
	DWORD ClassId;				//0x00
	DWORD PetType;				//0x04
	DWORD Id;					//0x08
	DWORD OwnerId;				//0x0C	Owners PlayerUnitId
	DWORD	_2;					//0x10
	DWORD	_3;					//0x14
	DWORD	_4;					//0x18
	DWORD	_5;					//0x1C
	DWORD	_6;					//0x20
	DWORD	_7;					//0x24
	DWORD	_8;					//0x28
	DWORD	_9;					//0x2C
	RosterPet *PetNext;			//0x30
};

//  Unit_All struct (basic)
struct UnitAny
{
	DWORD dwType;					// 0x00
	DWORD dwClassId;				// 0x04
	DWORD dwUnk1;					// 0x08
	DWORD dwId;					// 0x0C
	DWORD dwMode;					// 0x10
	union
	{
		PlayerData *ptPlayerData;
		ItemData *ptItemData;
		MonsterData *ptMonsterData;
		TileData *ptTileData;
		ObjectData * ptObjectData;
	};							// 0x14
	DWORD dwAct;               		// 0x18
	Act * ptAct;					// 0x1C
	DWORD dwSeed[2];				// 0x20
	DWORD dwUnk2;					// 0x28
	union 
	{
		Path * hPath;
		ObjPath *hOPath;
	};							// 0x2C
	DWORD dwUnk3[5];				// 0x30
	DWORD dwGfxFrame;				// 0x44
	DWORD FrameRemain;				// 0x48
	WORD FrameRate;           		// 0x4C
	WORD _1;                           // 0x4E
	BYTE * ptGfxUnk;				// 0x50
	GfxInfo * ptGfxInfo;			// 0x54
	DWORD dwUnk5;					// 0x58
	StatsList * ptStatsList;			// 0x5C
	Inventory * ptInventory;			// 0x60
	Light * ptLight;	       		// 0x64
	DWORD dwUnk6;					// 0x68
	bool bBusy;					// 0x6C
	DWORD dwUnk7[4];				// 0x70
	Game *ptGame;					// 0x80
	DWORD dwUnk8[2];				// 0x84
	WORD wX;						// 0x8C
	WORD wY;						// 0x8E
	DWORD dwUnk9[5];				// 0x90
	Msg * ptMsg;					// 0xA4
	Info * ptInfo;					// 0xA8
	DWORD dwUnk10[6];				// 0xAC
	DWORD dwFlags;					// 0xC4
	DWORD dwFlags2;				// 0xC8
	DWORD dwUnk11[3];				// 0xCC
	Packet * ptPacket;				// 0xD8
	DWORD dwUnk12;    				// 0xDC
	UnitAny * ptChangedNext;			// 0xE0
	UnitAny * ptRoomNext;			// 0xE4	
	UnitAny * ptListNext;      		// 0xE8
};

struct UnitPlayer : UnitAny
{
};

struct UnitItem : UnitAny
{
};

struct UnitMonster : UnitAny
{
};

struct UnitObject : UnitAny
{
};

struct CellBlockHeader {
	DWORD _1;					// 0x00
	DWORD width;				// 0x04
	DWORD height;				// 0x08
	// All I care about for now.
};

struct CellFile {
	DWORD version;				// 0x00
	DWORD dwFlags;				// 0x04
	DWORD _1;					// 0x08
	DWORD termination;			// 0x0C
	long directions;			// 0x10
	long frames;				// 0x14
	CellBlockHeader * Blocks[100];// 0x18 must realize there aren't going to be this many.  
							//      Its dynamic.  Its really frames*directions
};

struct ImageStruct {
	DWORD _1;					// 0x00
	CellFile * ptCellFile;		// 0x04
	int frame;				// 0x08
	DWORD _3;					// 0x0C
};

struct skill_t {
	char			name[64];
	WORD			skillID;
};

struct d2_WpId {
	int		oldId;
	int		Act;
	int		Wp;
};

// Table to convert oldWpId to Act/Waypoint
static d2_WpId WpId_Tbl[] =
{
	{0x01, 1, 1},{0x03, 1, 2},{0x04, 1, 3},{0x05, 1, 4},{0x06, 1, 5},{0x1b, 1, 6},{0x1d, 1, 7},{0x20, 1, 8},{0x23, 1, 9},
	{0x28, 2, 1},{0x30, 2, 2},{0x2a, 2, 3},{0x39, 2, 4},{0x2b, 2, 5},{0x2c, 2, 6},{0x34, 2, 7},{0x4a, 2, 8},{0x2e, 2, 9},
	{0x4b, 3, 1},{0x4c, 3, 2},{0x4d, 3, 3},{0x4e, 3, 4},{0x4f, 3, 5},{0x50, 3, 6},{0x51, 3, 7},{0x53, 3, 8},{0x65, 3, 9},
	{0x67, 4, 1},{0x6a, 4, 2},{0x6b, 4, 3},
	{0x6d, 5, 1},{0x6f, 5, 2},{0x70, 5, 3},{0x71, 5, 4},{0x73, 5, 5},{0x7b, 5, 6},{0x75, 5, 7},{0x76, 5, 8},{0x81, 5, 9},
};

struct d2_NpcMenu {
	DWORD		_1;			//0x00
	DWORD		_2;			//0x04
	DWORD		_3;			//0x08
	DWORD		_4;			//0x0C
    DWORD		clicked;	//0x10
	DWORD		_6;			//0x14
	DWORD		_7;			//0x18
	DWORD		_8;			//0x1C
	DWORD		_9;			//0x20
	DWORD		_10;		//0x24
	DWORD		_11;		//0x28
	DWORD		X;			//0x2C=X
	DWORD		Y;			//0x30=Y
	DWORD		_14;		//0x34
	DWORD		_15;		//0x38
	DWORD		_16;		//0x3C
	DWORD		_17;		//0x40
	DWORD		MenuId1;	//0x44
	DWORD		MenuId2;	//0x48
};



//---------------------------------------------------------------------------
// Skills
// Taken from Skills.txt
//---------------------------------------------------------------------------
// Obsoleted by lord2800 - See skills.h

/*#define SKILL_ATTACK                                                0
#define SKILL_KICK                                                  1
#define SKILL_THROW                                                 2
#define SKILL_UNSUMMON                                              3
#define SKILL_LEFT_HAND_THROW                                       4
#define SKILL_LEFT_HAND_SWING                                       5
#define SKILL_MAGIC_ARROW                                           6
#define SKILL_FIRE_ARROW                                            7
#define SKILL_INNER_SIGHT                                           8
#define SKILL_CRITICAL_STRIKE                                       9
#define SKILL_JAB                                                   10
#define SKILL_COLD_ARROW                                            11
#define SKILL_MULTIPLE_SHOT                                         12
#define SKILL_DODGE                                                 13
#define SKILL_POWER_STRIKE                                          14
#define SKILL_POISON_JAVELIN                                        15
#define SKILL_EXPLODING_ARROW                                       16
#define SKILL_SLOW_MISSILES                                         17
#define SKILL_AVOID                                                 18
#define SKILL_IMPALE                                                19
#define SKILL_LIGHTNING_BOLT                                        20
#define SKILL_ICE_ARROW                                             21
#define SKILL_GUIDED_ARROW                                          22
#define SKILL_PENETRATE                                             23
#define SKILL_CHARGED_STRIKE                                        24
#define SKILL_PLAGUE_JAVELIN                                        25
#define SKILL_STRAFE                                                26
#define SKILL_IMMOLATION_ARROW                                      27
#define SKILL_DOPPLEZON                                             28
#define SKILL_EVADE                                                 29
#define SKILL_FEND                                                  30
#define SKILL_FREEZING_ARROW                                        31
#define SKILL_VALKYRIE                                              32
#define SKILL_PIERCE                                                33
#define SKILL_LIGHTNING_STRIKE                                      34
#define SKILL_LIGHTNING_FURY                                        35
#define SKILL_FIRE_BOLT                                             36
#define SKILL_WARMTH                                                37
#define SKILL_CHARGED_BOLT                                          38
#define SKILL_ICE_BOLT                                              39
#define SKILL_FROZEN_ARMOR                                          40
#define SKILL_INFERNO                                               41
#define SKILL_STATIC_FIELD                                          42
#define SKILL_TELEKINESIS                                           43
#define SKILL_FROST_NOVA                                            44
#define SKILL_ICE_BLAST                                             45
#define SKILL_BLAZE                                                 46
#define SKILL_FIRE_BALL                                             47
#define SKILL_NOVA                                                  48
#define SKILL_LIGHTNING                                             49
#define SKILL_SHIVER_ARMOR                                          50
#define SKILL_FIRE_WALL                                             51
#define SKILL_ENCHANT                                               52
#define SKILL_CHAIN_LIGHTNING                                       53
#define SKILL_TELEPORT                                              54
#define SKILL_GLACIAL_SPIKE                                         55
#define SKILL_METEOR                                                56
#define SKILL_THUNDER_STORM                                         57
#define SKILL_ENERGY_SHIELD                                         58
#define SKILL_BLIZZARD                                              59
#define SKILL_CHILLING_ARMOR                                        60
#define SKILL_FIRE_MASTERY                                          61
#define SKILL_HYDRA                                                 62
#define SKILL_LIGHTNING_MASTERY                                     63
#define SKILL_FROZEN_ORB                                            64
#define SKILL_COLD_MASTERY                                          65
#define SKILL_AMPLIFY_DAMAGE                                        66
#define SKILL_TEETH                                                 67
#define SKILL_BONE_ARMOR                                            68
#define SKILL_SKELETON_MASTERY                                      69
#define SKILL_RAISE_SKELETON                                        70
#define SKILL_DIM_VISION                                            71
#define SKILL_WEAKEN                                                72
#define SKILL_POISON_DAGGER                                         73
#define SKILL_CORPSE_EXPLOSION                                      74
#define SKILL_CLAY_GOLEM                                            75
#define SKILL_IRON_MAIDEN                                           76
#define SKILL_TERROR                                                77
#define SKILL_BONE_WALL                                             78
#define SKILL_GOLEM_MASTERY                                         79
#define SKILL_RAISE_SKELETAL_MAGE                                   80
#define SKILL_CONFUSE                                               81
#define SKILL_LIFE_TAP                                              82
#define SKILL_POISON_EXPLOSION                                      83
#define SKILL_BONE_SPEAR                                            84
#define SKILL_BLOODGOLEM                                            85
#define SKILL_ATTRACT                                               86
#define SKILL_DECREPIFY                                             87
#define SKILL_BONE_PRISON                                           88
#define SKILL_SUMMON_RESIST                                         89
#define SKILL_IRONGOLEM                                             90
#define SKILL_LOWER_RESIST                                          91
#define SKILL_POISON_NOVA                                           92
#define SKILL_BONE_SPIRIT                                           93
#define SKILL_FIREGOLEM                                             94
#define SKILL_REVIVE                                                95
#define SKILL_SACRIFICE                                             96
#define SKILL_SMITE                                                 97
#define SKILL_MIGHT                                                 98
#define SKILL_PRAYER                                                99
#define SKILL_RESIST_FIRE                                           100
#define SKILL_HOLY_BOLT                                             101
#define SKILL_HOLY_FIRE                                             102
#define SKILL_THORNS                                                103
#define SKILL_DEFIANCE                                              104
#define SKILL_RESIST_COLD                                           105
#define SKILL_ZEAL                                                  106
#define SKILL_CHARGE                                                107
#define SKILL_BLESSED_AIM                                           108
#define SKILL_CLEANSING                                             109
#define SKILL_RESIST_LIGHTNING                                      110
#define SKILL_VENGEANCE                                             111
#define SKILL_BLESSED_HAMMER                                        112
#define SKILL_CONCENTRATION                                         113
#define SKILL_HOLY_FREEZE                                           114
#define SKILL_VIGOR                                                 115
#define SKILL_CONVERSION                                            116
#define SKILL_HOLY_SHIELD                                           117
#define SKILL_HOLY_SHOCK                                            118
#define SKILL_SANCTUARY                                             119
#define SKILL_MEDITATION                                            120
#define SKILL_FIST_OF_THE_HEAVENS                                   121
#define SKILL_FANATICISM                                            122
#define SKILL_CONVICTION                                            123
#define SKILL_REDEMPTION                                            124
#define SKILL_SALVATION                                             125
#define SKILL_BASH                                                  126
#define SKILL_SWORD_MASTERY                                         127
#define SKILL_AXE_MASTERY                                           128
#define SKILL_MACE_MASTERY                                          129
#define SKILL_HOWL                                                  130
#define SKILL_FIND_POTION                                           131
#define SKILL_LEAP                                                  132
#define SKILL_DOUBLE_SWING                                          133
#define SKILL_POLE_ARM_MASTERY                                      134
#define SKILL_THROWING_MASTERY                                      135
#define SKILL_SPEAR_MASTERY                                         136
#define SKILL_TAUNT                                                 137
#define SKILL_SHOUT                                                 138
#define SKILL_STUN                                                  139
#define SKILL_DOUBLE_THROW                                          140
#define SKILL_INCREASED_STAMINA                                     141
#define SKILL_FIND_ITEM                                             142
#define SKILL_LEAP_ATTACK                                           143
#define SKILL_CONCENTRATE                                           144
#define SKILL_IRON_SKIN                                             145
#define SKILL_BATTLE_CRY                                            146
#define SKILL_FRENZY                                                147
#define SKILL_INCREASED_SPEED                                       148
#define SKILL_BATTLE_ORDERS                                         149
#define SKILL_GRIM_WARD                                             150
#define SKILL_WHIRLWIND                                             151
#define SKILL_BERSERK                                               152
#define SKILL_NATURAL_RESISTANCE                                    153
#define SKILL_WAR_CRY                                               154
#define SKILL_BATTLE_COMMAND                                        155
#define SKILL_FIRE_HIT                                              156
#define SKILL_UNHOLYBOLT                                            157
#define SKILL_SKELETONRAISE                                         158
#define SKILL_MAGGOTEGG                                             159
#define SKILL_SHAMANFIRE                                            160
#define SKILL_MAGOTTUP                                              161
#define SKILL_MAGOTTDOWN                                            162
#define SKILL_MAGOTTLAY                                             163
#define SKILL_ANDRIALSPRAY                                          164
#define SKILL_JUMP                                                  165
#define SKILL_SWARM_MOVE                                            166
#define SKILL_NEST                                                  167
#define SKILL_QUICK_STRIKE                                          168
#define SKILL_VAMPIREFIREBALL                                       169
#define SKILL_VAMPIREFIREWALL                                       170
#define SKILL_VAMPIREMETEOR                                         171
#define SKILL_GARGOYLETRAP                                          172
#define SKILL_SPIDERLAY                                             173
#define SKILL_VAMPIREHEAL                                           174
#define SKILL_VAMPIRERAISE                                          175
#define SKILL_SUBMERGE                                              176
#define SKILL_FETISHAURA                                            177
#define SKILL_FETISHINFERNO                                         178
#define SKILL_ZAKARUMHEAL                                           179
#define SKILL_EMERGE                                                180
#define SKILL_RESURRECT                                             181
#define SKILL_BESTOW                                                182
#define SKILL_MISSILESKILL1                                         183
#define SKILL_MONTELEPORT                                           184
#define SKILL_PRIMELIGHTNING                                        185
#define SKILL_PRIMEBOLT                                             186
#define SKILL_PRIMEBLAZE                                            187
#define SKILL_PRIMEFIREWALL                                         188
#define SKILL_PRIMESPIKE                                            189
#define SKILL_PRIMEICENOVA                                          190
#define SKILL_PRIMEPOISONBALL                                       191
#define SKILL_PRIMEPOISONNOVA                                       192
#define SKILL_DIABLIGHT                                             193
#define SKILL_DIABCOLD                                              194
#define SKILL_DIABFIRE                                              195
#define SKILL_FINGERMAGESPIDER                                      196
#define SKILL_DIABWALL                                              197
#define SKILL_DIABRUN                                               198
#define SKILL_DIABPRISON                                            199
#define SKILL_POISONBALLTRAP                                        200
#define SKILL_ANDYPOISONBOLT                                        201
#define SKILL_HIREABLEMISSILE                                       202
#define SKILL_DESERTTURRET                                          203
#define SKILL_ARCANETOWER                                           204
#define SKILL_MONBLIZZARD                                           205
#define SKILL_MOSQUITO                                              206
#define SKILL_CURSEDBALLTRAPRIGHT                                   207
#define SKILL_CURSEDBALLTRAPLEFT                                    208
#define SKILL_MONFROZENARMOR                                        209
#define SKILL_MONBONEARMOR                                          210
#define SKILL_MONBONESPIRIT                                         211
#define SKILL_MONCURSECAST                                          212
#define SKILL_HELLMETEOR                                            213
#define SKILL_REGURGITATOREAT                                       214
#define SKILL_MONFRENZY                                             215
#define SKILL_QUEENDEATH                                            216
#define SKILL_SCROLL_OF_IDENTIFY                                    217
#define SKILL_BOOK_OF_IDENTIFY                                      218
#define SKILL_SCROLL_OF_TOWNPORTAL                                  219
#define SKILL_BOOK_OF_TOWNPORTAL                                    220
#define SKILL_RAVEN                                                 221
#define SKILL_PLAGUE_POPPY                                          222
#define SKILL_WEARWOLF                                              223
#define SKILL_SHAPE_SHIFTING                                        224
#define SKILL_FIRESTORM                                             225
#define SKILL_OAK_SAGE                                              226
#define SKILL_SUMMON_SPIRIT_WOLF                                    227
#define SKILL_WEARBEAR                                              228
#define SKILL_MOLTEN_BOULDER                                        229
#define SKILL_ARCTIC_BLAST                                          230
#define SKILL_CYCLE_OF_LIFE                                         231
#define SKILL_FERAL_RAGE                                            232
#define SKILL_MAUL                                                  233
#define SKILL_ERUPTION                                              234
#define SKILL_CYCLONE_ARMOR                                         235
#define SKILL_HEART_OF_WOLVERINE                                    236
#define SKILL_SUMMON_FENRIS                                         237
#define SKILL_RABIES                                                238
#define SKILL_FIRE_CLAWS                                            239
#define SKILL_TWISTER                                               240
#define SKILL_VINES                                                 241
#define SKILL_HUNGER                                                242
#define SKILL_SHOCK_WAVE                                            243
#define SKILL_VOLCANO                                               244
#define SKILL_TORNADO                                               245
#define SKILL_SPIRIT_OF_BARBS                                       246
#define SKILL_SUMMON_GRIZZLY                                        247
#define SKILL_FURY                                                  248
#define SKILL_ARMAGEDDON                                            249
#define SKILL_HURRICANE                                             250
#define SKILL_FIRE_TRAUMA                                           251
#define SKILL_CLAW_MASTERY                                          252
#define SKILL_PSYCHIC_HAMMER                                        253
#define SKILL_TIGER_STRIKE                                          254
#define SKILL_DRAGON_TALON                                          255
#define SKILL_SHOCK_FIELD                                           256
#define SKILL_BLADE_SENTINEL                                        257
#define SKILL_QUICKNESS                                             258
#define SKILL_FISTS_OF_FIRE                                         259
#define SKILL_DRAGON_CLAW                                           260
#define SKILL_CHARGED_BOLT_SENTRY                                   261
#define SKILL_WAKE_OF_FIRE_SENTRY                                   262
#define SKILL_WEAPON_BLOCK                                          263
#define SKILL_CLOAK_OF_SHADOWS                                      264
#define SKILL_COBRA_STRIKE                                          265
#define SKILL_BLADE_FURY                                            266
#define SKILL_FADE                                                  267
#define SKILL_SHADOW_WARRIOR                                        268
#define SKILL_CLAWS_OF_THUNDER                                      269
#define SKILL_DRAGON_TAIL                                           270
#define SKILL_LIGHTNING_SENTRY                                      271
#define SKILL_INFERNO_SENTRY                                        272
#define SKILL_MIND_BLAST                                            273
#define SKILL_BLADES_OF_ICE                                         274
#define SKILL_DRAGON_FLIGHT                                         275
#define SKILL_DEATH_SENTRY                                          276
#define SKILL_BLADE_SHIELD                                          277
#define SKILL_VENOM                                                 278
#define SKILL_SHADOW_MASTER                                         279
#define SKILL_ROYAL_STRIKE                                          280
#define SKILL_WAKE_OF_DESTRUCTION_SENTRY                            281
#define SKILL_IMP_INFERNO                                           282
#define SKILL_IMP_FIREBALL                                          283
#define SKILL_BAAL_TAUNT                                            284
#define SKILL_BAAL_CORPSE_EXPLODE                                   285
#define SKILL_BAAL_MONSTER_SPAWN                                    286
#define SKILL_CATAPULT_CHARGED_BALL                                 287
#define SKILL_CATAPULT_SPIKE_BALL                                   288
#define SKILL_SUCK_BLOOD                                            289
#define SKILL_CRY_HELP                                              290
#define SKILL_HEALING_VORTEX                                        291
#define SKILL_TELEPORT_2                                            292
#define SKILL_SELFRESURRECT                                         293
#define SKILL_VINE_ATTACK                                           294
#define SKILL_OVERSEER_WHIP                                         295
#define SKILL_BARBS_AURA                                            296
#define SKILL_WOLVERINE_AURA                                        297
#define SKILL_OAK_SAGE_AURA                                         298
#define SKILL_IMP_FIRE_MISSILE                                      299
#define SKILL_IMPREGNATE                                            300
#define SKILL_SIEGE_BEAST_STOMP                                     301
#define SKILL_MINIONSPAWNER                                         302
#define SKILL_CATAPULTBLIZZARD                                      303
#define SKILL_CATAPULTPLAGUE                                        304
#define SKILL_CATAPULTMETEOR                                        305
#define SKILL_BOLTSENTRY                                            306
#define SKILL_CORPSECYCLER                                          307
#define SKILL_DEATHMAUL                                             308
#define SKILL_DEFENSE_CURSE                                         309
#define SKILL_BLOOD_MANA                                            310
//#define SKILL_INFERNO_SENTRY                                        311
//#define SKILL_DEATH_SENTRY                                          312
#define SKILL_SENTRY_LIGHTNING                                      313
#define SKILL_FENRIS_RAGE                                           314
#define SKILL_BAAL_TENTACLE                                         315
#define SKILL_BAAL_NOVA                                             316
#define SKILL_BAAL_INFERNO                                          317
#define SKILL_BAAL_COLD_MISSILES                                    318*/

#endif // __DEFINITIONS_H__