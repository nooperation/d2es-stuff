/////////////////////////////////////////////////////////////////////
// I believe these are Mousepad's code
//////////////////////////////////////////////////////////////////////
#ifndef __D2EXPFUNCS
#define __D2EXPFUNCS

#include "definitions.h"

#define D2EXPFUNC(d1,v1,t1,t2) typedef t1 d1##_##v1##_t t2; extern d1##_##v1##_t *d1##_##v1;
#define D2EXPVAR(d1,v1,t1)     typedef t1 d1##_##v1##_t;    extern d1##_##v1##_t *p_##d1##_##v1;
#define D2EXPASM(d1,v1)        extern DWORD d1##_##v1;


/*  D2Client.dll  */

// internal functions:
D2EXPFUNC(D2CLIENT, PrintChatMessage, void __fastcall, (wchar_t * Message, DWORD color));
D2EXPFUNC(D2CLIENT, PrintMessage, void __fastcall, (ChatMsg * Message));
//D2EXPFUNC(D2CLIENT, PrintMessage, void __fastcall, (ptMsg * Message));
D2EXPFUNC(D2CLIENT, GetPlayerUnit, UnitPlayer * __fastcall, (void));
D2EXPFUNC(D2CLIENT, GetGameStatus, BOOL __fastcall, (void));
D2EXPFUNC(D2CLIENT, Move, void __fastcall, (DWORD id, UnitPlayer *player, DWORD x, DWORD y, DWORD Unk /*always 0*/));
D2EXPFUNC(D2CLIENT, GetUnit, UnitAny * __fastcall, (DWORD id, DWORD type));
D2EXPFUNC(D2CLIENT, GetMonsterOwner, DWORD __fastcall, (DWORD monnum));
D2EXPFUNC(D2CLIENT, QuitGame, void __stdcall, (void));			//quits the d2 game
D2EXPFUNC(D2CLIENT, ExitGame, void __stdcall, (DWORD flags));  //exits diablo2
D2EXPFUNC(D2CLIENT, GetGameType, int __fastcall, (void)); //returns 0 for classic, 1 for exp
D2EXPFUNC(D2CLIENT, WeaponSwitch, void __fastcall, (void));
D2EXPFUNC(D2CLIENT, GetMouseX, unsigned short __fastcall, (void));
D2EXPFUNC(D2CLIENT, GetMouseY, unsigned short __fastcall, (void));
//D2EXPFUNC(D2CLIENT, ScreenToMapCoords, void __fastcall, (int * ptX, int * ptY));
D2EXPFUNC(D2CLIENT, usePotion, void __fastcall, (UnitPlayer *player, Inventory *inv, DWORD MeOrMerc /*0 or 0x8000*/, DWORD slot));
D2EXPFUNC(D2CLIENT, GetNameFromUnit, wchar_t * __fastcall, (UnitAny *unit)); //0x6FB297F0
D2EXPFUNC(D2CLIENT, LoadUIImage, CellFile * __fastcall, (char * uiname));
D2EXPFUNC(D2CLIENT, UnloadUIImage, void __fastcall, (CellFile * UiImage));
D2EXPFUNC(D2CLIENT, KeyPressHandler, void __stdcall, (D2MSG * ptMsg));
D2EXPFUNC(D2CLIENT, ChatPressHandler, void __stdcall, (D2MSG * ptMsg));
D2EXPFUNC(D2CLIENT, ToggleChatBox, void __stdcall, (void));
D2EXPFUNC(D2CLIENT, ManipUI, void __fastcall, (int uiNum, int uiState, DWORD _1));
D2EXPFUNC(D2CLIENT, GetUIState, int __fastcall, (int uiNum));
D2EXPFUNC(D2CLIENT, ClickWaypointUI, void __fastcall, (DWORD d2handle, DWORD UnkAddr, DWORD ptrClickMsg));
D2EXPFUNC(D2CLIENT, ClickWaypointPageUI, void __fastcall, (DWORD d2handle, DWORD UnkAddr, DWORD ptrClickMsg));
D2EXPFUNC(D2CLIENT, ClickNpcMenuUI, void __fastcall, (DWORD d2handle, DWORD UnkAddr, DWORD ptrClickMsg));
D2EXPFUNC(D2CLIENT, ClickTradeUIdown, void __fastcall, (DWORD d2handle, DWORD UnkAddr, DWORD ptrClickMsg));
D2EXPFUNC(D2CLIENT, ClickTradeUIup, void __fastcall, (DWORD d2handle, DWORD UnkAddr, DWORD ptrClickMsg));
D2EXPFUNC(D2CLIENT, ClickStashUIdown, void __fastcall, (DWORD d2handle, DWORD UnkAddr, DWORD ptrClickMsg));
D2EXPFUNC(D2CLIENT, ClickStashUIup, void __fastcall, (DWORD d2handle, DWORD UnkAddr, DWORD ptrClickMsg));
D2EXPFUNC(D2CLIENT, SetSkill, void __fastcall, (DWORD Fkey, DWORD unk));
D2EXPFUNC(D2CLIENT, GetMouseXOffset, int __fastcall, (void));
D2EXPFUNC(D2CLIENT, GetMouseYOffset, int __fastcall, (void));
D2EXPFUNC(D2CLIENT, MouseDownHandler, void __stdcall, (D2MSG * ptMsg));
D2EXPFUNC(D2CLIENT, MouseUpHandler, void __stdcall, (D2MSG * ptMsg));
D2EXPFUNC(D2CLIENT, RepeatMouseUpHandler, void __stdcall, (D2MSG * ptMsg));
D2EXPFUNC(D2CLIENT, MouseClick, int __fastcall, (int ClickType, unsigned short MouseX, unsigned short MouseY, BYTE ClickFlag));
D2EXPFUNC(D2CLIENT, DrawRectangle, void __fastcall, (DWORD Xpos, DWORD YPos, DWORD Width, DWORD Height, DWORD Color, DWORD TransparencyLevel));
D2EXPFUNC(D2CLIENT, DrawClientScreen, void __fastcall, (void));
D2EXPFUNC(D2CLIENT, GetSelectedUnit, UnitAny * __fastcall, (void));
D2EXPFUNC(D2CLIENT, SetSelectedUnit, void __fastcall, (UnitAny * ptSelectedUnit));
D2EXPFUNC(D2CLIENT, ClearSelectedUnit, void __fastcall, (void));
D2EXPFUNC(D2CLIENT, UnitTestSelect, bool __fastcall, (UnitAny * ptUnit, DWORD _1, DWORD _2, bool _3));
D2EXPFUNC(D2CLIENT, GetNextPlayer, UnitAny * __fastcall, (UnitAny* pla));


// variables:
//D2EXPVAR(D2CLIENT, nArea, int, 0x115EF0);
D2EXPVAR(D2CLIENT, nDiff, int);
D2EXPVAR(D2CLIENT, dwMsgId, DWORD);
D2EXPVAR(D2CLIENT, bAlwaysRun, int);
D2EXPVAR(D2CLIENT, slotsWithPotion, int);
D2EXPVAR(D2CLIENT, bWeapSwitch, int);
D2EXPVAR(D2CLIENT, wLastChatMessage, wchar_t);
D2EXPVAR(D2CLIENT, MessageTimer, DWORD);
D2EXPVAR(D2CLIENT, ClickWpId, DWORD);
D2EXPVAR(D2CLIENT, NpcMenu, d2_NpcMenu *);
D2EXPVAR(D2CLIENT, ClickTrade, DWORD);
D2EXPVAR(D2CLIENT, d2MouseXa, DWORD);
D2EXPVAR(D2CLIENT, d2MouseYa, DWORD);
D2EXPVAR(D2CLIENT, d2MouseXb, DWORD);
D2EXPVAR(D2CLIENT, d2MouseYb, DWORD);
D2EXPVAR(D2CLIENT, F13Skill, DWORD);
D2EXPVAR(D2CLIENT, F13SkillHand, DWORD);
D2EXPVAR(D2CLIENT, ScreenYOffset, DWORD);
D2EXPVAR(D2CLIENT, ScreenXOffset, DWORD);
D2EXPVAR(D2CLIENT, ClientScreenWidth, int);
D2EXPVAR(D2CLIENT, ClientScreenHeight, int);
D2EXPVAR(D2CLIENT, HasHighlight, bool);
D2EXPVAR(D2CLIENT, HighlightUnitType, int);
D2EXPVAR(D2CLIENT, HighlightUnitId, int);
//D2EXPVAR(D2CLIENT, FirstHandler, D2HANDLER);
D2EXPVAR(D2CLIENT, IsPaused, bool);
D2EXPVAR(D2CLIENT, PlayerUnit, UnitPlayer *);
D2EXPVAR(D2CLIENT, FirstRosterPet, RosterPet *);
D2EXPVAR(D2CLIENT, FirstRosterUnit, Roster *);

//redefine some stuff:
#define D2CLIENT_HasHighlight *p_D2CLIENT_HasHighlight
#define D2CLIENT_HighlightUnitType *p_D2CLIENT_HighlightUnitType
#define D2CLIENT_HighlightUnitId *p_D2CLIENT_HighlightUnitId

// asm pointers:
D2EXPASM(D2CLIENT, CallStartGame);  
D2EXPASM(D2CLIENT, ScreenPrint);
D2EXPASM(D2CLIENT, CallMouseDownHandler);
D2EXPASM(D2CLIENT, CallMouseUpHandler);
D2EXPASM(D2CLIENT, CallRepeatMouseUpHandler);
D2EXPASM(D2CLIENT, UIClickHandler);
D2EXPASM(D2CLIENT, CallKeyHandler);
D2EXPASM(D2CLIENT, CallChatHandler);
D2EXPASM(D2CLIENT, CallGamePlayerInfoHandler);
D2EXPASM(D2CLIENT, CallGameReceivePacketHandler);
D2EXPFUNC(D2CLIENT, GetMonsterTxt, MonsterTxt * __fastcall, (DWORD monno))

/*  D2Common.dll  */

// exports:
D2EXPFUNC(D2COMMON, GetActFromLevelId, int __stdcall, (int LevelId));												// #10001
D2EXPFUNC(D2COMMON, GetRoomOtherFromRoom, RoomOther * __fastcall, (Room * ptRoom));									// #10007
D2EXPFUNC(D2COMMON, GetCellNum, int __fastcall, (int unk, TileData * ptTileData, int ActNum, Act * ptAct));				// #10011
D2EXPFUNC(D2COMMON, GetFirstRoomUnit, UnitAny * __stdcall, (Room * ptRoom));										// #10029
D2EXPFUNC(D2COMMON, GetRoomCoords, void __stdcall, (Room * ptRoom, Coords * ptCoords));								// #10034
D2EXPFUNC(D2COMMON, GetRoomsList, void * __fastcall, (Room * ptRoom, Room *** RoomsList, int * RoomCount)); 				// #10035
D2EXPFUNC(D2COMMON, GetFirstActRoom, Room * __stdcall, (Act * ptAct));												// #10056 
D2EXPFUNC(D2COMMON, GetLevelIdFromRoom, int __stdcall, (Room * hRoom));											// #10057
D2EXPFUNC(D2COMMON, AddRoomData, void __stdcall, (Act * ptAct, int LevelId, int Xpos, int Ypos, UnitPlayer * ptPlayer));		// #10063
D2EXPFUNC(D2COMMON, RemoveRoomData, void __stdcall, (Act * ptAct, int LevelId, int Xpos, int Ypos, UnitPlayer * ptPlayer));	// #10064
D2EXPFUNC(D2COMMON, IsInTown, bool __stdcall, (Room * ptRoom));													// #10082
D2EXPFUNC(D2COMMON, GetTownLevelId, int __stdcall, (int Act)); 													// #10085
D2EXPFUNC(D2COMMON, IsTownLevel, bool __stdcall, (int LevelId));													// #10086
D2EXPFUNC(D2COMMON, GetMiscFromAct, Drlg * __stdcall, (Act * ptAct));												// #10088
D2EXPFUNC(D2COMMON, AbsScreenToMapCoords, void __stdcall, (long * ptAbsMouseX, long * ptAbsMouseY));					// #10108
D2EXPFUNC(D2COMMON, MapToAbsScreenCoords, void __stdcall, (long * ptXpos, long * ptYpos));							// #10111
D2EXPFUNC(D2COMMON, GetUnitX, int __stdcall, (Path * hPath));													// #10162
D2EXPFUNC(D2COMMON, GetUnitY, int __stdcall, (Path * hPath));													// #10163
D2EXPFUNC(D2COMMON, GetPathTargetY, DWORD __stdcall, (Path * hPath));												// #10165
D2EXPFUNC(D2COMMON, GetRoomFromPath, Room * __stdcall, (Path * hPath));     										// #10166
D2EXPFUNC(D2COMMON, GetTargetFromPath, Path * __stdcall, (Path * hPath, int * TargetType, DWORD * UnitId));				// #10171
D2EXPFUNC(D2COMMON, GetTargetXFromPath, int __stdcall, (Path * hPath));											// #10175
D2EXPFUNC(D2COMMON, GetTargetYFromPath, int __stdcall, (Path * hPath));											// #10176
D2EXPFUNC(D2COMMON, GetTargetIdFromPath, int __stdcall, (Path * hPath));											// #10180
D2EXPFUNC(D2COMMON, InventoryItem, bool __stdcall, (Inventory * ptInventory, UnitItem * ptItem));				 		// #10243
D2EXPFUNC(D2COMMON, GetCursorItem, UnitItem * __stdcall, (Inventory * ptInventory)); 								// #10262
D2EXPFUNC(D2COMMON, GetFirstInventoryItem, UnitItem * __stdcall, (Inventory * ptInventory));							// #10277
D2EXPFUNC(D2COMMON, GetNextInventoryItem, UnitItem * __stdcall, (UnitItem * ptItem)); 								// #10304
D2EXPFUNC(D2COMMON, IsItem, UnitItem * __stdcall, (UnitAny * ptUnit)); 											// #10305
D2EXPFUNC(D2COMMON, GetItemId, DWORD __stdcall, (UnitItem * ptItem));  											// #10306
D2EXPFUNC(D2COMMON, GetItemNodePage, int __stdcall, (UnitItem * ptItem));											// #10307
D2EXPFUNC(D2COMMON, GetStartSkill, Skill * __stdcall, (UnitPlayer * ptPlayer));										// #10320
D2EXPFUNC(D2COMMON, GetLeftSkill, Skill * __stdcall, (UnitPlayer * ptPlayer));										// #10321
D2EXPFUNC(D2COMMON, GetRightSkill, Skill * __stdcall, (UnitPlayer * ptPlayer));										// #10322
D2EXPFUNC(D2COMMON, GetCurrentSkill, Skill * __stdcall, (UnitPlayer * ptPlayer));									// #10323
D2EXPFUNC(D2COMMON, GetStashGoldLimit, int __stdcall, (UnitAny * ptUnit));											// #10339
D2EXPFUNC(D2COMMON, GetRoomFromUnit, Room * __stdcall, (UnitAny * ptUnit)); 		       							// #10342
D2EXPFUNC(D2COMMON, IsUnitInRoom, bool __stdcall, (Room * ptRoom, UnitAny * hUnit));									// #10389
D2EXPFUNC(D2COMMON, IsUnitObjectDoor, bool __stdcall, (UnitObject * ptUnit));										// #10392
D2EXPFUNC(D2COMMON, GetObjectBinData, ObjectBinData * __stdcall, (UnitObject * ptObject));								// #10394
D2EXPFUNC(D2COMMON, GetPtData, GlobalData * __stdcall, (UnitAny * ptUnit)); 										// #10424
D2EXPFUNC(D2COMMON, GetUnitState, BOOL __stdcall, (UnitAny *ptUnit, DWORD stateno))									// #10487
D2EXPFUNC(D2COMMON, GetBaseStat, int __stdcall, (UnitAny * ptUnit, int StatId, int StatTblIndex));	 					// #10519
D2EXPFUNC(D2COMMON, GetAddedStat, int __stdcall, (UnitAny * ptUnit, int StatId, int StatTblIndex)); 					// #10520
D2EXPFUNC(D2COMMON, GetStatValue3, int __stdcall, (UnitAny * ptUnit, int StatId, int StatTblIndex)); 					// #10521
D2EXPFUNC(D2COMMON, GetMaxHealth, int __stdcall, (UnitAny * ptUnit)); 												// #10564
D2EXPFUNC(D2COMMON, GetMaxMana, int __stdcall, (UnitAny * ptUnit)); 												// #10565
D2EXPFUNC(D2COMMON, GetMaxStamina, int __stdcall, (UnitAny * ptUnit)); 											// #10566
D2EXPFUNC(D2COMMON, GetItemCode, char * __stdcall, (int itemid)); 												// #10600
D2EXPFUNC(D2COMMON, GetExperienceForLevel, int __stdcall, (int PlayerClass, int TargetLevel));
D2EXPFUNC(D2COMMON, GetItemQuality, int __stdcall, (UnitAny * ptUnit)); 							// #10628
D2EXPFUNC(D2COMMON, GetItemPage, int __stdcall, (UnitItem * ptItem));												// #10719
D2EXPFUNC(D2COMMON, TwoHandedItemCheck, bool __stdcall, (UnitItem * ptItem));										// #10765
D2EXPFUNC(D2COMMON, GetNumberOfUnidItems, int __stdcall, (UnitPlayer * ptPlayer));									// #10776
D2EXPFUNC(D2COMMON, GetMaxItemQuantity, int __stdcall, (UnitItem * ptItem));										// #10795
D2EXPFUNC(D2COMMON, GetMaxItemSockets, int __stdcall, (UnitItem * ptItem));											// #10815
D2EXPFUNC(D2COMMON, PrintOverhead, Msg * __stdcall, (DWORD Unk/*always 0*/ , char * Message,  DWORD dwMsgId));				// #10892
D2EXPFUNC(D2COMMON, GetUnitSkill, Skill * __fastcall, (UnitPlayer * ptPlayer, WORD SkillId, DWORD Flags));				// #10949
D2EXPFUNC(D2COMMON, GetSkillFlags, DWORD * __stdcall, (Skill * hSkill));											// #10957
D2EXPFUNC(D2COMMON, GetSkillType, short __stdcall, (Skill * ptSkill));												// #10963

D2EXPFUNC(D2COMMON, GetItemTxt, ItemTxt * __stdcall, (DWORD itemno))
D2EXPFUNC(D2COMMON, GetObjectTxt, ObjectTxt * __stdcall, (DWORD objno))
D2EXPFUNC(D2COMMON, GetLevelTxt, LevelTxt * __stdcall, (DWORD levelno))
D2EXPFUNC(D2COMMON, GetSuperuniqueTxt, SuperuniqueTxt * __stdcall, (DWORD monno))



/*  D2Gfx.dll  */

// exports:
D2EXPFUNC(D2GFX, PrintImage, void __stdcall, (void * ptStruct, int Xpos, int Ypos, DWORD layer, DWORD _2, DWORD _3));
D2EXPFUNC(D2GFX, GetWindowHandle, HWND __stdcall, (void));
D2EXPFUNC(D2GFX, DrawRectangle, void __stdcall, (int X1, int Y1, int X2, int Y2, int color, int Transparency));
D2EXPFUNC(D2GFX, DrawRectangle2, void __stdcall, (int X1, int Y1, int X2, int Y2, int color, int Transparency));

/*  D2Win.dll  */

// exports:
D2EXPFUNC(D2WIN, DrawText, void __fastcall, (wchar_t * pszwText, int x, int y, int nColor, int unknown));
D2EXPFUNC(D2WIN, GetTextWidth, int __fastcall, (wchar_t * pszwText));
D2EXPFUNC(D2WIN, GetNTextWidth, int __fastcall, (wchar_t * pszwText, int nChar));
D2EXPFUNC(D2WIN, GetFontHeight, BYTE __fastcall, (void));
D2EXPFUNC(D2WIN, DrawTextFont, DWORD __fastcall, (int fontnum));

D2EXPFUNC(D2WIN, MouseHandler, void __stdcall, (D2MSG * ptMsg));

D2EXPVAR(D2WIN, CurrentFont, DWORD);
//D2EXPVAR(D2WIN, FirstHandler, D2HANDLER);

/* bnclient.dll */

D2EXPVAR(BNCLIENT, fnLoadLibraryA, DWORD*)
// internal functions
D2EXPFUNC(BNCLIENT, ChatSrvSay, void __fastcall, (char *pszMessage));

/*  Others:  */
D2EXPFUNC(D2LAUNCH, UIEnterBnet, void __stdcall, (void));
D2EXPFUNC(D2LAUNCH, UIEnterMainMenu, void __stdcall, (void));
D2EXPFUNC(D2LAUNCH, EnterBnet, int __stdcall, (void * arg));

D2EXPASM(D2LAUNCH, CallEnterBnet1);
D2EXPASM(D2LAUNCH, CallEnterBnet2);

D2EXPASM(D2LAUNCH, CallEndGame);

D2EXPFUNC(D2CLIENT, CallStartEndGame, void __stdcall, (void))

extern Roster **p_D2CLIENT_FirstRosterUnit;
extern RosterPet **p_D2CLIENT_FirstRosterPet;
extern int *p_D2CLIENT_HighlightUnitType;
extern int *p_D2CLIENT_HighlightUnitId;
extern int *p_D2CLIENT_nDiff;
extern BnetData **p_D2LAUNCH_bnData;
extern DWORD *p_D2CLIENT_dwMsgId;
extern int *p_D2CLIENT_bAlwaysRun;
extern int *p_D2CLIENT_slotsWithPotion;
extern int *p_D2CLIENT_bWeapSwitch;
extern int *p_D2CLIENT_ClientScreenWidth;
extern int *p_D2CLIENT_ClientScreenHeight;
extern d2_NpcMenu **p_D2CLIENT_NpcMenu;

#endif