#pragma once
#include <windows.h>
#include "definitions.h"

struct MH_DrlgRoom2;
struct MH_DrlgLevel;
struct MH_DrlgMisc;
struct MH_DrlgAct;
struct MH_UnitAny;

struct D2Seed {
	DWORD d1, d2;
};

struct MH_RoomTile {
	MH_DrlgRoom2* pRoom2; //+00
	DWORD* nNum; //+04
	DWORD _1[3];
	RoomTile* pNext; //+14
};

struct MH_DrlgRoom1 {
	DWORD xpos1; //+00
	DWORD ypos1; //+04
	DWORD xsize1; //+08
	DWORD ysize1; //+0c
	DWORD xpos2; //+10
	DWORD ypos2; //+14
	DWORD xsize2; //+18
	DWORD ysize2; //+1c
	DWORD _1[1];
	MH_DrlgRoom1** paRoomsNear; //+24
	DWORD nRoomsNear; //+28;
	UnitAny* pUnitFirst; //+2c
	DWORD _2[2];
	MH_DrlgRoom2* pRoom2; //+38
	D2Seed seed; //+3c
	DWORD _3[14];
	MH_DrlgRoom1* pNext; // +7c
};

struct MH_DrlgRoom2 {
	DrlgLevel* pDrlgLevel; //+00
	DWORD xPos; //+04
	DWORD yPos; //+08
	DWORD xPos1; // +0c
	DWORD yPos1; // +10
	DWORD _1[2];
	DWORD nPresetType; //+1c
	DWORD* nPresetType2No; //+20
	DWORD _2[39];
	MH_DrlgRoom2** paRoomsNear; //+c0
	DWORD nRoomsNear; //+c4
	RoomTile* pRoomTiles; //+c8
	PresetUnit* pPresetUnits; //+cc
	DWORD _3[1];
	D2Seed seed; //+d4
	DWORD _3b[2];
	MH_DrlgRoom1* pRoom1; //+e4
	MH_DrlgRoom2* pNext; //+e8
};

struct MH_DrlgLevel {
	MH_DrlgMisc* pDrlgMisc; //+00
	DWORD nLevelNo; //+04
	DWORD _1[10];
	MH_DrlgRoom2* pRoom2First; //+30
};

struct MH_DrlgMisc {
	MH_DrlgLevel* pLevelFirst; //+00
	DWORD _1[2];
	MH_DrlgAct* pDrlgAct; //+0c
	DWORD _2[250];
	DWORD nStaffTombLvl; // +3f8
	DWORD nBossTombLvl; //+3fc
};

struct MH_DrlgMisc_109 {
	MH_DrlgLevel* pLevelFirst; //+00
	DWORD _1[2];
	MH_DrlgAct* pDrlgAct; //+0c
	DWORD _2[249];
	DWORD nStaffTombLvl; //+3f4
	DWORD nBossTombLvl; //+3f8
};

struct MH_DrlgAct {
	DWORD _1;
	MH_DrlgRoom1* pRoom1; //+04
	MH_DrlgMisc* pDrlgMisc; //+08
};

struct MH_DrlgAct_109 {
	DWORD _1[2];
	MH_DrlgMisc* pDrlgMisc; //+08
};

struct MH_AutomapCell {
	DWORD fSaved; //+00
	WORD nCellNo; //+04
	WORD xPixel; //+06
	WORD yPixel; //+08
	WORD _1[1];
	MH_AutomapCell* pLess; //+0c
	MH_AutomapCell* pMore; //+10
};

struct MH_AutomapLayer {
	DWORD nLayerNo;
	DWORD fSaved;
	MH_AutomapCell* pFloors; //+08
	MH_AutomapCell* pWalls; //+0c
	MH_AutomapCell* pObjects; //+10
	MH_AutomapCell* pExtras; //+14
};

struct MH_AutomapCellNode {
	DWORD _1[4];
	MH_AutomapCell* pAutomapCell; // +10
	DWORD _2;
	MH_AutomapCellNode* pNext; // +18
};
