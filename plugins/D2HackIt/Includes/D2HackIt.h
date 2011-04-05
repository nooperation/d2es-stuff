//////////////////////////////////////////////////////////////////////
// D2HackIt.h
// -------------------------------------------------------------------
// Main incude file for D2HackIt Exports.
//
// <thohell@home.se>
//
//
// ## This file will define only Server/Client common used exports ##
// <Dan_Goon@diabloworld.com>
//////////////////////////////////////////////////////////////////////
#ifndef __D2HACKIT_H__
#define __D2HACKIT_H__

#include <windows.h>
#include <iostream>
#include <io.h>
#include <stdio.h>
#include <tlhelp32.h>
#include "LinkedList\LinkedList.h"

#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif  /* __cplusplus */

#ifdef EXPORT
#undef EXPORT
#endif
#define	EXPORT			__declspec( dllexport ) __cdecl
#define PRIVATE			__cdecl

#ifdef D2HACKIT_DEBUG
extern	char g_szDebug[512];

#define	DBGHDR			g_szDebug, "D2HACKIT : "
#define DGTRACE( _x_ )	sprintf _x_; OutputDebugString(g_szDebug);
#else
#define DGTRACE( _x_ )
#endif

#define D2_VERSION 110
#include "CommonStructs.h"


#define TICK_RATE 10

//////////////////////////////////////////////////////////////////////
// Version history.
// -------------------------------------------------------------------
// (See D2Hackit.txt for Version History)
//////////////////////////////////////////////////////////////////////
#define __SERVERVERSION__		MAKELONG(2, 0)
#define __SERVERBUILD__			MAKELONG(9, 20)

// Sane defaults for prompts
#define DEFAULTINFOPROMPT		"ÿc3::ÿc0"
#define DEFAULTERRORPROMPT		"ÿc1::ÿc0"
#define DEFAULTVERBOSEPROMPT	"ÿc2::ÿc0"

#define MAXPROMPTLENGTH			0x20

// ClientHandler.cpp
BOOL	EXPORT HookD2WndProc(HMODULE hModule, PF_HookProc pHookProc);
BOOL	EXPORT CleanMeUp(HMODULE hModule);

// GameSendPacketToServer.cpp
DWORD	EXPORT GameSendPacketToServer(LPBYTE buf, DWORD len);
DWORD	EXPORT BnetSendPacketToServer(LPBYTE buf, DWORD len);
DWORD	EXPORT RealmSendPacketToServer(LPBYTE buf, DWORD len);
BOOL	EXPORT GameSendMessageToChat(LPSTR msg);

// GameSendPacketToGame.cpp
DWORD	EXPORT GameSendPacketToGame(LPBYTE buf, DWORD len);
BOOL	EXPORT GameInsertPacketToGame(LPBYTE buf, DWORD len);

// IniFileHandlers.cpp
LPSTR	EXPORT GetHackProfileString(LPCSTR lpHackName, LPCSTR lpSectionName, LPCSTR lpKeyName);
LPSTR	EXPORT GetHackProfileStringEx(LPCSTR lpHackName, LPCSTR lpSectionName, LPCSTR lpKeyName, DWORD& cSize);
LPSTR	EXPORT GetHackProfileSectionNames( LPCSTR lpHackName, DWORD& cSize );
BOOL	EXPORT SetHackProfileString(LPCSTR lpHackName, LPCSTR lpSectionName, LPCSTR lpKeyName, LPCSTR lpValue);
BOOL	EXPORT FreeHackProfileString(LPCSTR lpString);
BOOL	EXPORT GetFingerprint(LPCSTR szModule, LPCSTR szFingerprintName, FINGERPRINTSTRUCT &fps);
//*****************************************************************************************************************
// New HackProfile related functions (Dan_Goon Versions)
//
// 1. These functions can be used to get the buffer size required, when NULL value is passed in lpOutBuffer.
//		In this case, the Function will only return the required buffer size without allocating the buffer.
// 2. And then caller should allocate the buffer as amount of return value in caller's routine.
// 3. Finally call those functions with caller-allocated buffer in lpOutBuffer and buffer size in nBufferSize.
// 4. Caller should free-up those buffers which are allocated by this method, when they are no more needed.
// 5. Or you can call these functions with enough sized in-stack buffers at once.
//		In this case, the returned data in buffer may not guarantee full information you required.
// 6. The output string is terminated with zero and the return value always reflects required buffer size
//		including this zero. 
// 7. When supplied buffer size is not enough to hold all results, the function returns larger value than nBufferSize.
//	   And the output string will be truncated. Caller should check the return value for this reson.
//*****************************************************************************************************************
DWORD	EXPORT GetIniFileName( LPCSTR lpHackName, LPSTR lpOutFileName, DWORD nBufferSize );
DWORD	EXPORT GetIniString( LPCSTR lpHackName, LPCSTR lpSectionName, LPCSTR lpKeyName, LPSTR lpOutBuffer, DWORD nBufferSize);
DWORD	EXPORT GetIniSectionNames( LPCSTR lpHackName, LPSTR lpOutBuffer, DWORD nBufferSize );

// GamePrintFunctions.cpp
BOOL	EXPORT GamePrintInfo(LPCSTR buf);
BOOL	EXPORT GamePrintError(LPCSTR buf);
BOOL	EXPORT GamePrintVerbose(LPCSTR buf);
BOOL	EXPORT GamePrintString(LPCSTR buf);

// GameSendPacketToServer.cpp
DWORD	EXPORT GameSendPacketToServer(LPBYTE buf, DWORD len);

// OtherExportedFunctions.cpp
PTHISGAMESTRUCT EXPORT GetThisgameStruct(void);
PSERVERINFO EXPORT GetServerInfo(DWORD dwVersion, LPCSTR szModule);
BOOL	EXPORT GameSaveAndExit();
HWND	EXPORT FindMyD2WndHandle();

// HelperFunctions.cpp
#define INST_NOP 0x90
#define INST_CALL 0xe8
#define INST_JMP 0xe9
BOOL	EXPORT Intercept(int instruction, DWORD lpSource, DWORD lpDest, int len);
BOOL	EXPORT D2FindFile(LPSTR szDest, LPCSTR szFileName, int nDestSize = _MAX_PATH);

// GameCommandLine.cpp
BOOL	EXPORT GameCommandLine(char* buf);
CHAR	EXPORT GetCommandCharacter();
int		EXPORT GetTickRate();

void EXPORT DebugTest(); // For debug only

// Text drawing, added v2.00 by Abin
DWORD EXPORT CreateScreenHook();
BOOL EXPORT ReleaseScreenHook(DWORD dwHookID);
BOOL EXPORT SendToFront(DWORD dwHookID);
BOOL EXPORT SendToBack(DWORD dwHookID);
BOOL EXPORT DrawSingleText(DWORD dwHookID, LPCSTR lpszText, long x, long y, int nFont = 0, BOOL bRightAlign = FALSE); // Draw one text
BOOL EXPORT DrawMultipleTexts(DWORD dwHookID, LPCD2DRAWDATA aData, int nDataCount); // Draw multiple texts
SIZE EXPORT GetTextSize(LPCSTR lpszText, int nFont); // Retrieves text size
SIZE EXPORT GetScreenSize(); // Get the game screen size	

// Module directory for convinience, added v2.00 by Abin
LPCSTR EXPORT GetModuleDirectory();

// Check if a particular client(*.d2h, *.dll) was loaded, added v2.00 by Abin
BOOL EXPORT IsClientLoaded(LPCSTR lpszClientName);

// Convenient string routes, added v2.00 by Abin
BOOL EXPORT GameInfof(LPCSTR lpszFormat, ...);
BOOL EXPORT GameErrorf(LPCSTR lpszFormat, ...);
BOOL EXPORT GameStringf(LPCSTR lpszFormat, ...);
BOOL EXPORT GameCommandf(LPCSTR lpszFormat, ...);

// Packet printing, added v2.00 by Abin
BOOL EXPORT GamePrintPacket(BOOL bReceived, const BYTE *aPacket, DWORD aLen);

// Game ready check, added v2.00 by Abin
BOOL EXPORT IsGameReady();
BOOL EXPORT WasD2HackitLoadedBeforeJoinGame();

// Unit enumeration
BOOL EXPORT EnumUnits(DWORD dwUnitType, fnEnumUnitProc lpfnEnumUnitProc, LPARAM lParam = 0);

BOOL EXPORT VerifyUnit(LPCGAMEUNIT lpUnit);
BOOL EXPORT FindUnitByClassID(DWORD dwUnitClassID, DWORD dwUnitType, LPGAMEUNIT lpBuffer);
BOOL EXPORT FindUnitByName(LPCSTR lpszUnitName, DWORD dwUnitType, LPGAMEUNIT lpBuffer);
MAPPOS EXPORT GetUnitPosition(LPCGAMEUNIT lpUnit);
DWORD EXPORT GetUnitName(LPCGAMEUNIT lpUnit, LPSTR lpszBuffer, DWORD dwMaxChars);
DWORD EXPORT GetUnitOwnerID(LPCGAMEUNIT lpUnit); // extremely useful for pets/summonings
DWORD EXPORT GetUnitClassID(LPCGAMEUNIT lpUnit);
BYTE EXPORT GetUnitHPPercent(LPCGAMEUNIT lpUnit); // hp percent 0-100
DWORD EXPORT GetUnitStat(LPCGAMEUNIT lpUnit, int nStatID);
BOOL EXPORT GetUnitAffection(LPCGAMEUNIT lpUnit, int nAffectionID);
DWORD EXPORT GetAffectionName(BYTE iAffectionID, LPTSTR lpszBuffer, DWORD dwMaxChars);
BOOL EXPORT AffectionIsAura(BYTE iAffectionID);
DWORD EXPORT IsMercUnit(LPCGAMEUNIT lpUnit); // Check if an unit is a merc
int EXPORT GetMonsterType(LPCGAMEUNIT lpUnit); // normal/champion/boss/minion
BOOL EXPORT GetBossEnchants(LPCGAMEUNIT lpUnit, LPBOSSENCHANTS lpBuffer); // boss enchants
DWORD EXPORT GetDistance(int x1, int y1, int x2, int y2); // distance between two positions, in yards
DWORD EXPORT GetPing(); // Get ping value (lag indicator)
BOOL EXPORT SendKey(BYTE iVKCode); // Send a key-down message to d2 window
DWORD EXPORT GetSpellName(WORD wSpell, LPSTR lpszBuffer, DWORD dwMaxChars);

// map related
DWORD EXPORT GetMapName(BYTE iMapID, LPTSTR lpszBuffer, DWORD dwMaxChars); // Name of the map
BOOL EXPORT MapIsTown(BYTE iMapID); // Is town?
BYTE EXPORT GetTownMap(BYTE iAct);
BYTE EXPORT GetActFromMap(BYTE iMapID);

// item related
BOOL EXPORT ParseItem(const BYTE* aPacket, DWORD aLen, ITEM& item); // Parses item info from a packet
SIZE EXPORT GetItemSize(LPCSTR lpszItemCode);
BOOL EXPORT IsStackable(LPCSTR lpszItemCode);
BOOL EXPORT IsWeapon(LPCSTR lpszItemCode);
BOOL EXPORT IsArmor(LPCSTR lpszItemCode);
LPCSTR EXPORT GetItemName(LPCSTR lpszItemCode);
LPCSTR EXPORT GetItemSetName(LPCSTR lpszItemCode);
int EXPORT IsPotion(LPCSTR lpszItemCode);
BYTE EXPORT GetItemBeltRows(LPCSTR lpszItemCode); // if it's a belt, get the row count(1-4)


// game attributes
DWORD EXPORT GetD2WindowTitle(LPSTR lpszBuffer, DWORD dwMaxChars);
DWORD EXPORT GetInGameTime(); // time elapsed since joining
int EXPORT GetGameDifficulty(); // current game difficulty, normal/nm/hell
MAPPOS EXPORT ScreenToMapCoords(POINT ptScreenCoords);
POINT EXPORT MapToScreenCoords(MAPPOS mapPosition);
POINT EXPORT GetMousePosition();

LPCSTR EXPORT GetRealmName();
LPCSTR EXPORT GetGameName();
LPCSTR EXPORT GetGamePassword();
LPCSTR EXPORT GetGameIP();

BOOL EXPORT EnumPresetUnits(fnEnumPresetUnitProc lpfnEnumPresetUnitProc, LPARAM lParam = 0);
BYTE EXPORT CalculatePath(WORD x, WORD y, LPPATH lpBuffer, int nAdjust = 5);

///////////////////////////////////////////////////////////////
// Player Operations & Attributes
///////////////////////////////////////////////////////////////
LPCSTR EXPORT GetAccountName();
BOOL EXPORT HaveSpell(WORD wSpellID);
BOOL EXPORT IsInTown();
BOOL EXPORT GetMyMercUnit(LPGAMEUNIT lpBuffer); // Get player's mercenary
DWORD EXPORT GetID(); // player ID
LPCSTR EXPORT GetName(); // Player name
int EXPORT GetMode();	// Get player's current mode(walking/running/attacking etc)
WORD EXPORT GetSelectedSpell(BOOL bLeftHand); // Get the selected spell
BYTE EXPORT GetHPPercent();	// Player HP percent 0-100
BYTE EXPORT GetManaPercent();	// Player mana percent 0-100
BYTE EXPORT GetItemDurabilityPercent();	// Player item durability percent 0-100
DWORD EXPORT GetStat(int nStatID);	// General stat access, hp/maxhp/mana/maxmana/mf/ias etc
BOOL EXPORT GetAffection(int nAffectionID);
BOOL EXPORT SelectSpell(WORD wSpell, BOOL bLeftHand = FALSE);
MAPPOS EXPORT GetPosition(); // player position
int EXPORT GetMyCorpseCount();
BOOL EXPORT PickMyCorpse();
int EXPORT GetHostilerCount(); // number of players that are hostiled to me
BYTE EXPORT GetCurrentMapID();
DWORD EXPORT GetDistanceFrom(WORD x, WORD y); // Player's distance from a position
DWORD EXPORT HealMe(); // Drink a purple/red potion
DWORD EXPORT HealMerc(); // Feed the merc a purple/red potion
DWORD EXPORT SupplyMana(BOOL bDrinkPurpleIfNoBlue = TRUE); // Drink a blue/purple potion for mana
BOOL EXPORT DrinkBeltPotion(DWORD dwItemID, BOOL bFeedMerc);
BOOL EXPORT DrinkInventoryPotion(DWORD dwItemID);
BOOL EXPORT IsBeltFull();
BYTE EXPORT GetBeltRows(); // Row number of the player's belt(1-4)
LPCITEM EXPORT GetBeltItem(BYTE iRow, BYTE iCol); // Belt item access
BOOL EXPORT CastNoTarget(WORD wSpell, BOOL bLeftHand = FALSE);
BOOL EXPORT CastOnMap(WORD wSpell, WORD x, WORD y, BOOL bLeftHand = FALSE);
BOOL EXPORT CastOnUnit(WORD wSpell, LPCGAMEUNIT lpUnit, BOOL bLeftHand = FALSE);
BOOL EXPORT LeaveGame(); // save & exit
void EXPORT PauseJobs(BOOL bPause);
void EXPORT CleanJobs();
BOOL EXPORT BackToTown(); // Cast a town portal and go back to town immediately
BOOL EXPORT MoveTo(WORD x, WORD y, BOOL bQueueJob = TRUE); // Run to a position
BOOL EXPORT MoveToUnit(LPCGAMEUNIT lpUnit, BOOL bQueueJob = TRUE); // Run to a static object
BOOL EXPORT TeleportTo(WORD x, WORD y, BOOL bQueueJob = TRUE); // teleport to a position
BOOL EXPORT Say(LPCSTR lpszMessage); // Say a message
BOOL EXPORT OverHead(LPCSTR lpszMessage, BOOL bClientOnly = TRUE); // Display an over-head message
BOOL EXPORT SendSound(BYTE iSound); // Send a sound
BOOL EXPORT AntiIdle(); // Anti-idle
BOOL EXPORT WeaponSwitch(); // Do weapon switch
BOOL EXPORT EnterWaypoint(LPCGAMEUNIT lpWPUnit, BYTE iDestinationMapID); // take WP to another area
BOOL EXPORT Interact(LPCGAMEUNIT lpUnit);
BOOL EXPORT DepositGoldToStash(DWORD dwAmount = 0); // Deposit inventory gold to stash
BOOL EXPORT RedrawClient(BOOL bFade = TRUE); // Redraw client view
DWORD EXPORT GetStashGoldLimit(); // stash gold limit
DWORD EXPORT GetInventoryGoldLimit(); // inventory gold limit
int EXPORT GetMaxDifficulty();	// the max difficulty to which this player has access
BOOL EXPORT IsHardcore(); // is hardcore player?
BOOL EXPORT GetAlwaysRun(); // is "always run" on?
DWORD EXPORT GetCursorItem(); // returns ID of the item on cursor, if any
BOOL EXPORT DropCursorItemToGround();

BOOL EXPORT PickGroundItem(DWORD dwItemID, BOOL bShowClientMove = TRUE); // Pick up a ground item
BOOL EXPORT Transmute();
BOOL EXPORT IsUIOpened(int nUIType);

BOOL EXPORT EnumStorageItems(int nSTorageType, fnEnumItemProc lpfnEnumItemProc, LPARAM lParam = 0);
BOOL EXPORT PickStorageItemToCursor(DWORD dwItemID);

BOOL EXPORT FindFirstStorageSpace(int nStorageType, SIZE minSize, LPPOINT lpBuffer);
BOOL EXPORT DropCursorItemToStorage(int nStorageType);
BOOL EXPORT GetItemCode(DWORD dwItemID, LPSTR lpszBuffer, DWORD dwMaxChars);
BOOL EXPORT GetItemCodeEx(DWORD dwItemID, LPSTR lpszBuffer, DWORD dwMaxChars, int maxRetryCount, int delayBeteenRetries);

// ++ B
int EXPORT GetItemQuality(DWORD dwItemID);
BOOL EXPORT LeftClickHere();
BOOL EXPORT RightClickHere();
BOOL EXPORT RemoveFromStorage(int storageType, DWORD dwItemID);
BOOL EXPORT DumpStorage(LPCSTR path);
BOOL EXPORT DropCursorItemToStorageEx(int nStorageType, POINT preferedPosition);
BOOL EXPORT DropItemToStorageEx(int nStorageType, POINT preferedPosition, DWORD dwItemID);
BOOL EXPORT DropItemToStorage(int nStorageType, DWORD dwItemID);
BOOL EXPORT FindFirstStorageSpaceEx(int nStorageType, SIZE minSize, LPPOINT lpBuffer, POINT preferedPosition);
BOOL EXPORT FindItemPosition(int nStorageType, DWORD dwItemID, LPPOINT position);
BOOL EXPORT OpenCube();
DWORD EXPORT GetCubeID();
// -- B

BOOL EXPORT LeftClick(long x, long y);
BOOL EXPORT RightClick(long x, long y);
BYTE EXPORT GetOpenedUI();
void EXPORT CloseAllUIs();

BOOL EXPORT ResurrectMerc(); // Resurrect merc
BOOL EXPORT IdentifyItems();
BOOL EXPORT BuyItem(DWORD dwItemID); // Buy an item from npc
BOOL EXPORT RepairItem(DWORD dwItemID = 0); // Repair items

BOOL EXPORT BuyHealingPotion(int nQuantity = 0);
BOOL EXPORT BuyManaPotion(int nQuantity = 0);
BOOL EXPORT BuyTownScroll(int nQuantity = 0);
BOOL EXPORT BuyIdentifyScroll(int nQuantity = 0);
BOOL EXPORT BuyArrows(int nQuantity);
BOOL EXPORT BuyBolts(int nQuantity);
BOOL EXPORT Gamble(DWORD dwItemID); // gamble an item
BOOL EXPORT SellItem(DWORD dwItemID);
BOOL EXPORT StartNpcSession(LPCGAMEUNIT lpNpcUnit, int nInteractType = NPC_NORMAL);
void EXPORT EndNpcSession();
int EXPORT DrinkAllInventoryPotions(BOOL bKeepFullRejuv = FALSE);
BOOL EXPORT IsMercAlive();
BOOL EXPORT DropGold(DWORD dwGoldAmount);
DWORD EXPORT GetSpellCharges(WORD wSpellID);

void EXPORT SetScrollTitle(LPCSTR lpszTitle);
BOOL EXPORT AddScrollLine(LPCSTR lpszText);
BOOL EXPORT ShowScroll();
void EXPORT ClearScroll();

int EXPORT GetUnidItemCount();
BYTE EXPORT GetLowestEquipDurabilityPercent(BOOL bIncludeSecondarySlots = FALSE);
LPCITEM EXPORT GetEquipItem(int nEquipLocation);

DWORD EXPORT QueryModuleData(LPCSTR lpszModuleName, DWORD dwDataID, LPVOID lpBuffer, DWORD dwMaxLength, LPARAM lParam = 0);
DWORD EXPORT GetKeyName(BYTE iKCode, LPSTR lpszBuffer, DWORD dwMaxChars);

#ifdef __cplusplus
}		             /* Assume C declarations for C++ */
#endif  /* __cplusplus */

#endif // __D2HACKIT_H__

