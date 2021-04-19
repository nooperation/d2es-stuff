#ifndef __D2PTRS_H__
#define __D2PTRS_H__

#include <windows.h>
#include "definitions.h"

///////////////////////////////////////////////
// Originally Mousepad's Code
///////////////////////////////////////////////

////////////////////////////////////////////////////////////////
// Correct(as of 9/05, 2004): Ninjai D2BS's Code
////////////////////////////////////////////////////////////////
enum {DLLNO_D2CLIENT, DLLNO_D2COMMON, DLLNO_D2GFX, DLLNO_D2LANG, DLLNO_D2WIN, DLLNO_D2NET, DLLNO_D2GAME, DLLNO_D2LAUNCH, DLLNO_FOG, DLLNO_BNCLIENT, DLLNO_STORM};

#define DLLOFFSET(a1,b1) ((DLLNO_##a1)|((b1)<<8))

#define D2FUNCPTR(d1,v1,t1,t2,o1) typedef t1 d1##_##v1##_t t2; d1##_##v1##_t *d1##_##v1 = (d1##_##v1##_t *)DLLOFFSET(d1,o1);
#define D2VARPTR(d1,v1,t1,o1)     typedef t1 d1##_##v1##_t;    d1##_##v1##_t *p_##d1##_##v1 = (d1##_##v1##_t *)DLLOFFSET(d1,o1);
#define D2ASMPTR(d1,v1,o1)        DWORD d1##_##v1 = DLLOFFSET(d1,o1);


#define D2CLIENT_START D2CLIENT_UnloadUIImage
// functions:
// note, try to keep this in order of offset.  Thanks
// -Ninjai
D2FUNCPTR(D2CLIENT, UnloadUIImage, void __fastcall, (CellFile * UiImage), 0x1140);
D2FUNCPTR(D2CLIENT, MouseDownHandler, void __stdcall, (D2MSG * ptMsg), 0x8C10);
D2FUNCPTR(D2CLIENT, RepeatMouseUpHandler, void __stdcall, (D2MSG * ptMsg), 0x8CC0);
D2FUNCPTR(D2CLIENT, MouseUpHandler, void __stdcall, (D2MSG * ptMsg), 0x9100);
D2FUNCPTR(D2CLIENT, GetGameStatus, BOOL __fastcall, (void), 0x9440);
D2FUNCPTR(D2CLIENT, GetGameType, int __fastcall, (void), 0xC080); //returns 0 for classic, 1 for exp
D2FUNCPTR(D2CLIENT, QuitGame, void __stdcall, (void), 0xC2F0);			//quits the d2 game
D2FUNCPTR(D2CLIENT, GetNextPlayer, UnitAny * __fastcall, (UnitAny* pla), 0xF5B0);
D2FUNCPTR(D2CLIENT, GetMonsterOwner, DWORD __fastcall, (DWORD monnum), 0x11B00)
//D2FUNCPTR(D2CLIENT, PrintMessage, void __fastcall, (ptMsg * Message), 0x13890);
D2FUNCPTR(D2CLIENT, GetMouseXOffset, int __fastcall, (void), 0x15890);
D2FUNCPTR(D2CLIENT, GetMouseYOffset, int __fastcall, (void), 0x158A0);
D2FUNCPTR(D2CLIENT, GetSelectedUnit, UnitAny * __fastcall, (void), 0x15A20);
D2FUNCPTR(D2CLIENT, SetSelectedUnit, void __fastcall, (UnitAny * ptSelectedUnit), 0x15AE0);
D2FUNCPTR(D2CLIENT, ClearSelectedUnit, void __fastcall, (void), 0x15D40);
//D2FUNCPTR(D2CLIENT, ScreenToMapCoords, void __fastcall, (int * ptX, int * ptY), 0x158B0);
D2FUNCPTR(D2CLIENT, ChatPressHandler, void __stdcall, (D2MSG * ptMsg), 0x32890);
D2FUNCPTR(D2CLIENT, ToggleChatBox, void __stdcall, (void), 0x337B0);
D2FUNCPTR(D2CLIENT, KeyPressHandler, void __stdcall, (D2MSG * ptMsg), 0x35A00);
D2FUNCPTR(D2CLIENT, WeaponSwitch, void __fastcall, (void), 0x4D2B0);
D2FUNCPTR(D2CLIENT, usePotion, void __fastcall, (UnitPlayer *player, Inventory *inv, DWORD MeOrMerc /*0 or 0x8000*/, DWORD slot), 0x618A0);
D2FUNCPTR(D2CLIENT, PrintChatMessage, void __fastcall, (wchar_t * Message, DWORD color), 0x7C600);
D2FUNCPTR(D2CLIENT, PrintMessage, void __fastcall, (ChatMsg * Message), 0x7E120);
D2FUNCPTR(D2CLIENT, LoadUIImage, CellFile * __fastcall, (char * uiname), 0x80100);
D2FUNCPTR(D2CLIENT, ManipUI, void __fastcall, (int uiNum, int uiState, DWORD _1), 0x83260);
D2FUNCPTR(D2CLIENT, GetUIState, int __fastcall, (int uiNum), 0x83230);
D2FUNCPTR(D2CLIENT, ClickWaypointUI, void __fastcall, (DWORD d2handle, DWORD UnkAddr, DWORD ptrClickMsg), 0x865C0);
D2FUNCPTR(D2CLIENT, ClickWaypointPageUI, void __fastcall, (DWORD d2handle, DWORD UnkAddr, DWORD ptrClickMsg), 0x867A0);
D2FUNCPTR(D2CLIENT, UnitTestSelect, bool __fastcall, (UnitAny * ptUnit, DWORD _1, DWORD _2, bool _3), 0x886F0);
D2FUNCPTR(D2CLIENT, ClickNpcMenuUI, void __fastcall, (DWORD d2handle, DWORD UnkAddr, DWORD ptrClickMsg), 0x35DC0);
D2FUNCPTR(D2CLIENT, ClickTradeUIdown, void __fastcall, (DWORD d2handle, DWORD UnkAddr, DWORD ptrClickMsg), 0x4A1C0);
D2FUNCPTR(D2CLIENT, ClickTradeUIup, void __fastcall, (DWORD d2handle, DWORD UnkAddr, DWORD ptrClickMsg), 0x4A850);
D2FUNCPTR(D2CLIENT, ClickStashUIdown, void __fastcall, (DWORD d2handle, DWORD UnkAddr, DWORD ptrClickMsg), 0x4BAD0);
D2FUNCPTR(D2CLIENT, ClickStashUIup, void __fastcall, (DWORD d2handle, DWORD UnkAddr, DWORD ptrClickMsg), 0x4C030);
D2FUNCPTR(D2CLIENT, SetSkill, void __fastcall, (DWORD Fkey, DWORD unk) , 0x7A130);
D2FUNCPTR(D2CLIENT, DrawClientScreen, void __fastcall, (void), 0x5F480);
D2FUNCPTR(D2CLIENT, GetUnit, UnitAny * __fastcall, (DWORD id, DWORD type), 0x869F0);
D2FUNCPTR(D2CLIENT, GetPlayerUnit, UnitPlayer * __fastcall, (void), 0x883D0);

D2FUNCPTR(D2CLIENT, GetNameFromUnit, wchar_t * __fastcall, (UnitAny *unit), 0x897F0); //0x6FB297F0
D2FUNCPTR(D2CLIENT, MouseClick, int __fastcall, (int ClickType, unsigned short MouseX, unsigned short MouseY, BYTE ClickFlag), 0xA9920);
D2FUNCPTR(D2CLIENT, Move, void __fastcall, (DWORD id, UnitPlayer *player, DWORD x, DWORD y, DWORD Unk /*always 0*/), 0xAEA90);
D2FUNCPTR(D2CLIENT, GetMouseX, unsigned short __fastcall, (void), 0xB7BC0);
D2FUNCPTR(D2CLIENT, GetMouseY, unsigned short __fastcall, (void), 0xB7BD0);
D2FUNCPTR(D2CLIENT, DrawRectangle, void __fastcall, (DWORD Xpos, DWORD YPos, DWORD Width, DWORD Height, DWORD Color, DWORD TransparencyLevel), 0xBB0F0);
D2FUNCPTR(D2CLIENT, ExitGame, void __stdcall, (DWORD flags), 0xC06B3);  //exits diablo2

//D2FUNCPTR(D2CLIENT, RecvCommand07, void __fastcall, (BYTE *cmdbuf), 0x6FAB1C80)
//D2FUNCPTR(D2CLIENT, RecvCommand08, void __fastcall, (BYTE *cmdbuf), 0x6FAB1CF0)
//D2FUNCPTR(D2CLIENT, RevealAutomapRoom, void __fastcall, (Room *room, DWORD clipflag, AutomapLayer *layer), 0x6FAC6E70)

// variables
//D2VARPTR(D2CLIENT, nArea, int, 0x115EF0);
D2VARPTR(D2CLIENT, nDiff, int, 0x10795C);
D2VARPTR(D2CLIENT, dwMsgId, DWORD, 0x119EF6);
D2VARPTR(D2CLIENT, bAlwaysRun, int, 0x1079B8);
D2VARPTR(D2CLIENT, slotsWithPotion, int, 0x115EBC);
D2VARPTR(D2CLIENT, bWeapSwitch, int, 0x11594C);
D2VARPTR(D2CLIENT, wLastChatMessage, wchar_t, 0x111A58);
D2VARPTR(D2CLIENT, MessageTimer, DWORD, 0x112D08);
D2VARPTR(D2CLIENT, ClickWpId, DWORD, 0x11A9A5);
D2VARPTR(D2CLIENT, NpcMenu, d2_NpcMenu *, 0x113644);
D2VARPTR(D2CLIENT, ClickTrade, DWORD, 0x115BA8);
D2VARPTR(D2CLIENT, d2MouseXa, DWORD, 0x121AE4);
D2VARPTR(D2CLIENT, d2MouseYa, DWORD, 0x121AE8);
D2VARPTR(D2CLIENT, d2MouseXb, DWORD, 0x11388C);
D2VARPTR(D2CLIENT, d2MouseYb, DWORD, 0x113894);
D2VARPTR(D2CLIENT, F13Skill, DWORD, 0x119D20);
D2VARPTR(D2CLIENT, F13SkillHand, DWORD, 0x119888);
D2VARPTR(D2CLIENT, HasHighlight, bool, 0x10B9E8);
D2VARPTR(D2CLIENT, HighlightUnitType, int, 0x10B9CC);
D2VARPTR(D2CLIENT, HighlightUnitId, int, 0x10B9D0);
D2VARPTR(D2CLIENT, ghRoom, Room *, 0x10B9F8);
D2VARPTR(D2CLIENT, ScreenYOffset, DWORD, 0x10B9B8);
D2VARPTR(D2CLIENT, ScreenXOffset, DWORD, 0x10B9BC);
D2VARPTR(D2CLIENT, ClientScreenWidth, int, 0xFA704);
D2VARPTR(D2CLIENT, ClientScreenHeight, int, 0xFA700);
/*D2VARPTR(D2CLIENT, FirstHandlerTable, D2HANDLER, 0xD5818);
#define NUM_FIRSTHANDLERS 10
D2VARPTR(D2CLIENT, SecondHandlerTable, D2HANDLER, 0xD87F0);
D2VARPTR(D2CLIENT, ThirdHandlerTable, D2HANDLER, 0x*/
// Was experimenting with this, but they don't seem to align too well.
// Also, there are too many of them, some with duplicate message ids.
// I'll have to look into it a little later.
D2VARPTR(D2CLIENT, IsPaused, bool, 0x11C23C);
D2VARPTR(D2CLIENT, FirstRosterPet, RosterPet *, 0x10B9A0);
D2VARPTR(D2CLIENT, FirstRosterUnit, Roster *, 0x10B994);
D2VARPTR(D2CLIENT, PlayerUnit, UnitPlayer *, 0x11C200);

// asm pointers:
D2ASMPTR(D2CLIENT, CallGameReceivePacketHandler, 0x15123);
D2ASMPTR(D2CLIENT, ScreenPrint, 0x81FF7);	// This is a call opcode, so patch at +1 with &MyFunc - (ScreenPrint+1+4);
									// Add 1 for opcode length, then 4 for opcode payload.  This will return
									// the right offset relative to the next instruction in line.
D2ASMPTR(D2CLIENT, CallGamePlayerInfoHandler, 0x883C4);
D2ASMPTR(D2CLIENT, CallStartGame, 0xD490);  
D2ASMPTR(D2CLIENT, CallMouseDownHandler, 0xD5820);   
D2ASMPTR(D2CLIENT, CallRepeatMouseUpHandler, 0xD582C);
D2ASMPTR(D2CLIENT, CallMouseUpHandler, 0xD588C);
D2ASMPTR(D2CLIENT, UIClickHandler, 0xD582C);
D2ASMPTR(D2CLIENT, CallKeyHandler, 0xD8C2C);
D2ASMPTR(D2CLIENT, CallChatHandler, 0xD82F8);

#define D2CLIENT_END D2CLIENT_CallChatHandler

#define D2COMMON_START D2COMMON_GetActFromLevelId

D2FUNCPTR(D2COMMON, GetActFromLevelId, int __stdcall, (int LevelId), -0x2711);							// #10001
D2FUNCPTR(D2COMMON, GetRoomOtherFromRoom, RoomOther * __fastcall, (Room * ptRoom), -0x2717);					// #10007
D2FUNCPTR(D2COMMON, GetCellNum, int __fastcall, (int unk, TileData * ptTileData, int ActNum, Act * ptAct), -0x271B);		// #10011
D2FUNCPTR(D2COMMON, GetFirstRoomUnit, UnitAny * __stdcall, (Room * ptRoom), -0x272D);						// #10029
D2FUNCPTR(D2COMMON, GetRoomCoords, void __stdcall, (Room * ptRoom, Coords * ptCoords), -0x2732);				// #10034
D2FUNCPTR(D2COMMON, GetRoomsList, void * __fastcall, (Room * ptRoom, Room *** RoomsList, int * RoomCount), -0x2733); 		// #10035
D2FUNCPTR(D2COMMON, GetFirstActRoom, Room * __stdcall, (Act * ptAct), -0x2748);							// #10056 
D2FUNCPTR(D2COMMON, GetLevelIdFromRoom, int __stdcall, (Room * hRoom), -0x2749);						// #10057
D2FUNCPTR(D2COMMON, AddRoomData, void __stdcall, (Act * ptAct, int LevelId, int Xpos, int Ypos, UnitPlayer * ptPlayer), -0x274F);	// #10063
D2FUNCPTR(D2COMMON, RemoveRoomData, void __stdcall, (Act * ptAct, int LevelId, int Xpos, int Ypos, UnitPlayer * ptPlayer), -0x2750);	// #10064
D2FUNCPTR(D2COMMON, IsInTown, bool __stdcall, (Room * ptRoom), -0x2762);							// #10082
D2FUNCPTR(D2COMMON, GetTownLevelId, int __stdcall, (int Act), -0x2765); 							// #10085
D2FUNCPTR(D2COMMON, IsTownLevel, bool __stdcall, (int LevelId), -0x2766);							// #10086
D2FUNCPTR(D2COMMON, GetMiscFromAct, Drlg * __stdcall, (Act * ptAct), -0x2768);							// #10088
D2FUNCPTR(D2COMMON, AbsScreenToMapCoords, void __stdcall, (long * ptAbsMouseX, long * ptAbsMouseY), -0x277C);			// #10108
D2FUNCPTR(D2COMMON, MapToAutomapCoords, void __stdcall, (long * ptXpos, long * ptYPos), -0x277E);				// #10110
D2FUNCPTR(D2COMMON, MapToAbsScreenCoords, void __stdcall, (long * ptXpos, long * ptYpos), -0x277F);				// #10111
D2FUNCPTR(D2COMMON, GetUnitX, int __stdcall, (Path * hPath), -0x27B2);								// #10162
D2FUNCPTR(D2COMMON, GetUnitY, int __stdcall, (Path * hPath), -0x27B3);								// #10163
D2FUNCPTR(D2COMMON, GetPathTargetY, DWORD __stdcall, (Path * hPath), -0x27B5);							// #10165
D2FUNCPTR(D2COMMON, GetRoomFromPath, Room * __stdcall, (Path * hPath), -0x27B6);     						// #10166
D2FUNCPTR(D2COMMON, GetTargetFromPath, Path * __stdcall, (Path * hPath, int * TargetType, DWORD * UnitId), -0x27BB);		// #10171
D2FUNCPTR(D2COMMON, GetTargetXFromPath, int __stdcall, (Path * hPath), -0x27BF);						// #10175
D2FUNCPTR(D2COMMON, GetTargetYFromPath, int __stdcall, (Path * hPath), -0x27C0);						// #10176
D2FUNCPTR(D2COMMON, GetTargetIdFromPath, int __stdcall, (Path * hPath), -0x27C4);						// #10180
D2FUNCPTR(D2COMMON, InventoryItem, bool __stdcall, (Inventory * ptInventory, UnitItem * ptItem), -0x2803);	 		// #10243
D2FUNCPTR(D2COMMON, GetCursorItem, UnitItem * __stdcall, (Inventory * ptInventory), -0x2816); 					// #10262
D2FUNCPTR(D2COMMON, GetFirstInventoryItem, UnitItem * __stdcall, (Inventory * ptInventory), -0x2825);				// #10277
D2FUNCPTR(D2COMMON, GetNextInventoryItem, UnitItem * __stdcall, (UnitItem * ptItem), -0x2840); 					// #10304
D2FUNCPTR(D2COMMON, IsItem, UnitItem * __stdcall, (UnitAny * ptUnit), -0x2841); 						// #10305
D2FUNCPTR(D2COMMON, GetItemId, DWORD __stdcall, (UnitItem * ptItem), -0x2842);  						// #10306
D2FUNCPTR(D2COMMON, GetItemNodePage, int __stdcall, (UnitItem * ptItem), -0x2843);						// #10307
D2FUNCPTR(D2COMMON, GetStartSkill, Skill * __stdcall, (UnitPlayer * ptPlayer), -0x2850);					// #10320
D2FUNCPTR(D2COMMON, GetLeftSkill, Skill * __stdcall, (UnitPlayer * ptPlayer), -0x2851);						// #10321
D2FUNCPTR(D2COMMON, GetRightSkill, Skill * __stdcall, (UnitPlayer * ptPlayer), -0x2852);					// #10322
D2FUNCPTR(D2COMMON, GetCurrentSkill, Skill * __stdcall, (UnitPlayer * ptPlayer), -0x2853);					// #10323
D2FUNCPTR(D2COMMON, GetStashGoldLimit, int __stdcall, (UnitAny * ptUnit), -0x2863);						// #10339
D2FUNCPTR(D2COMMON, GetRoomFromUnit, Room * __stdcall, (UnitAny * ptUnit), -0x2866); 		       				// #10342
D2FUNCPTR(D2COMMON, IsUnitInRoom, bool __stdcall, (Room * ptRoom, UnitAny * hUnit), -0x2895);					// #10389
D2FUNCPTR(D2COMMON, IsUnitObjectDoor, bool __stdcall, (UnitObject * ptUnit), -0x2898);						// #10392
D2FUNCPTR(D2COMMON, GetObjectBinData, ObjectBinData * __stdcall, (UnitObject * ptObject), -0x289A);				// #10394
D2FUNCPTR(D2COMMON, GetPtData, GlobalData * __stdcall, (UnitAny * ptUnit), -0x28B8); 						// #10424
D2FUNCPTR(D2COMMON, GetUnitState, BOOL __stdcall, (UnitAny *ptUnit, DWORD stateno), -0x28F7)					// #10487
D2FUNCPTR(D2COMMON, GetBaseStat, int __stdcall, (UnitAny * ptUnit, int StatId, int StatTblIndex), -0x2917);	 		// #10519
D2FUNCPTR(D2COMMON, GetAddedStat, int __stdcall, (UnitAny * ptUnit, int StatId, int StatTblIndex), -0x2918); 			// #10520
D2FUNCPTR(D2COMMON, GetStatValue3, int __stdcall, (UnitAny * ptUnit, int StatId, int StatTblIndex), -0x2919); 			// #10521
D2FUNCPTR(D2COMMON, GetMaxHealth, int __stdcall, (UnitAny * ptUnit), -0x2944); 							// #10564
D2FUNCPTR(D2COMMON, GetMaxMana, int __stdcall, (UnitAny * ptUnit), -0x2945); 							// #10565
D2FUNCPTR(D2COMMON, GetMaxStamina, int __stdcall, (UnitAny * ptUnit), -0x2946); 						// #10566
D2FUNCPTR(D2COMMON, GetItemCode, char * __stdcall, (int itemid), -0x2968); 								// #10600

D2FUNCPTR(D2COMMON, GetItemTxt, ItemTxt * __stdcall, (DWORD itemno), -0x2968)
D2FUNCPTR(D2COMMON, GetObjectTxt, ObjectTxt * __stdcall, (DWORD objno), -0x2982)
D2FUNCPTR(D2COMMON, GetLevelTxt, LevelTxt * __stdcall, (DWORD levelno), -0x2987)
D2FUNCPTR(D2CLIENT, GetMonsterTxt, MonsterTxt * __fastcall, (DWORD monno), 0xE570)
D2FUNCPTR(D2COMMON, GetSuperuniqueTxt, SuperuniqueTxt * __stdcall, (DWORD monno), -0x29AC)

D2FUNCPTR(D2COMMON, GetExperienceForLevel, int __stdcall, (int PlayerClass, int TargetLevel), -0x2984); 			// #10628
D2FUNCPTR(D2COMMON, GetItemQuality, int __stdcall, (UnitAny * ptUnit), -0x29C7);						// #10695
D2FUNCPTR(D2COMMON, GetItemPage, int __stdcall, (UnitItem * ptItem), -0x29DF);							// #10719
D2FUNCPTR(D2COMMON, TwoHandedItemCheck, bool __stdcall, (UnitItem * ptItem), -0x2A0D);						// #10765
D2FUNCPTR(D2COMMON, GetNumberOfUnidItems, int __stdcall, (UnitPlayer * ptPlayer), -0x2A18);					// #10776
D2FUNCPTR(D2COMMON, GetMaxItemQuantity, int __stdcall, (UnitItem * ptItem), -0x2A2B);						// #10795
D2FUNCPTR(D2COMMON, GetMaxItemSockets, int __stdcall, (UnitItem * ptItem), -0x2A3F);						// #10815
D2FUNCPTR(D2COMMON, PrintOverhead, Msg * __stdcall, (DWORD Unk/*always 0*/ , char * Message,  DWORD dwMsgId), -0x2A8C);		// #10892
D2FUNCPTR(D2COMMON, GetUnitSkill, Skill * __fastcall, (UnitPlayer * ptPlayer, WORD SkillId, DWORD Flags), -0x2AC5);		// #10949
D2FUNCPTR(D2COMMON, GetSkillFlags, DWORD * __stdcall, (Skill * hSkill), -0x2ACD);						// #10957
D2FUNCPTR(D2COMMON, GetSkillType, short __stdcall, (Skill * ptSkill), -0x2AD3);							// #10963

#define D2COMMON_END D2COMMON_GetSkillType

#define D2OTHER_START D2GAME_IsSwitched
// functions
D2FUNCPTR(D2GAME, IsSwitched, BYTE __fastcall ,(PlrMsg *ptPlrMsg), 0x45B0);
D2FUNCPTR(D2NET, Recv, void __fastcall, (void), 0x15D5);
D2FUNCPTR(BNCLIENT, ChatSrvSay, void __fastcall, (char *pszMessage), 0x77C0); //6FF077C0

// exports:
// d2win:
D2FUNCPTR(D2WIN, DrawText, void __fastcall, (wchar_t * pszwText, int x, int y, int nColor, int unknown), -0x2785);
D2FUNCPTR(D2WIN, GetTextWidth, int __fastcall, (wchar_t * pszwText), -0x2789);
D2FUNCPTR(D2WIN, GetNTextWidth, int __fastcall, (wchar_t * pszwText, int nChar), -0x278A);
D2FUNCPTR(D2WIN, GetFontHeight, BYTE __fastcall, (void), -0x278D);
D2FUNCPTR(D2WIN, DrawTextFont, DWORD __fastcall, (int fontnum), -0x278F);


D2FUNCPTR(D2WIN, MouseHandler, void __stdcall, (D2MSG * ptMsg), 0xE070);

//D2VARPTR(D2WIN, FirstHandler, D2HANDLER, 0x1DE20);

D2FUNCPTR(D2NET, Send, void __stdcall, (DWORD dwFlags, LPBYTE lpBuf, DWORD len), -0x2715);
D2FUNCPTR(D2GFX, GetWindowHandle, HWND __stdcall, (void), -0x272B);
D2FUNCPTR(D2GFX, DrawRectangle, void __stdcall, (int X1, int Y1, int X2, int Y2, int color, int Transparency), -0x2747);
D2FUNCPTR(D2GFX, DrawRectangle2, void __stdcall, (int X1, int Y1, int X2, int Y2, int color, int Transparency), -0x2748);
D2FUNCPTR(FOG, MemAlloc, void * __fastcall, (int Mempool, int size, int unk, int line, const char * file), -0x273D);
D2FUNCPTR(FOG, MemDeAlloc, void __fastcall, (int Mempool, void * MemChunk, int unk, int line, const char * file), -0x273E);
D2FUNCPTR(FOG, Send, void __fastcall, (DWORD *bnClient, LPBYTE lpBuf, DWORD len), -0x2717); // #10007
D2FUNCPTR(D2GFX, PrintImage, void __stdcall, (void * ptStruct, int Xpos, int Ypos, DWORD layer, DWORD _2, DWORD _3), -0x2758);
D2FUNCPTR(STORM, FreeMemory, void __stdcall, (DWORD _1, char * ident, DWORD _2, DWORD _3), -0x193);


D2FUNCPTR(D2CLIENT, CallStartEndGame, void __stdcall, (void), 0x2050);

D2FUNCPTR(D2LAUNCH, UIEnterBnet, void __stdcall, (void), 0x9910);
D2FUNCPTR(D2LAUNCH, UIEnterMainMenu, void __stdcall, (void), 0x9320);
D2FUNCPTR(D2LAUNCH, UIExitD2, void __stdcall, (void), 0x12900);
D2FUNCPTR(D2LAUNCH, EnterBnet, int __stdcall, (void * arg), 0x11FD0);


D2ASMPTR(D2LAUNCH, CallEndGame, 0xD490);
D2ASMPTR(D2LAUNCH, CallEnterBnet1, 0x238A0);
D2ASMPTR(D2LAUNCH, CallEnterBnet2, 0x219F0);

D2VARPTR(BNCLIENT, fnLoadLibraryA, DWORD*, 0x160C4)

D2VARPTR(D2WIN, CurrentFont, DWORD, 0x1DB24);
D2VARPTR(STORM, dwUnk, DWORD, 0x3A1E4);
D2VARPTR(D2LAUNCH, bnData, BnetData*, 0x2CD44);
D2VARPTR(BNCLIENT, bnClient, DWORD*, 0x1D158);

D2FUNCPTR(D2LANG, GetLocaleText, wchar_t *__fastcall, (WORD nLocaleTxtNo), -10004)
D2VARPTR(D2CLIENT, InteractedNPCUniqueID, DWORD, 0x115CF5);
D2VARPTR(D2CLIENT, IsInteractedWithNPC, BOOL, 0x115CF9);
D2VARPTR(D2CLIENT, InteractedNPCClassID, DWORD, 0x115CFD);
D2VARPTR(D2CLIENT, InteractedWPUniqueID, DWORD, 0x11A9B5);
D2VARPTR(D2CLIENT, IsNotInteractedWithWP, BOOL, 0x11A9BD);

#define D2OTHER_END p_BNCLIENT_bnClient

#endif // __D2PTRS_H__