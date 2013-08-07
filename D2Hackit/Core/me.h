///////////////////////////////////////////////////////////
// Me.h
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////
#ifndef __ME_H__
#define __ME_H__

#include <windows.h>
#include "D2HackIt.h"
#include "AffectionDef.h"
#include "MapDef.h"
#include "item.h"

// Types of movement
enum { MJ_UNKNOWN = 0, MJ_POSITION, MJ_UNIT, MJ_TELEPORT };
typedef struct tagMoveJob
{
	int nJobType; // Type of movement
	GAMEUNIT unit;	// target unit
	WORD x;	// Destination position x
	WORD y; // Destination position y
	int nMaxRetry; // max retries allowed
	int nRetried; // retried
	DWORD dwMaxWait; // max wait time in milliseconds
	DWORD dwElapsed; // milliseconds elapsed
} MOVEJOB, *LPMOVEJOB;
typedef const tagMoveJob* LPCMOVEJOB;

DWORD Player_TimerTick();
void Player_OnGamePacketAfterReceived(BYTE* aPacket, DWORD aLen);
DWORD Player_OnGamePacketBeforeSent(BYTE* aPacket, DWORD aLen);
VOID Player_OnGameJoin(THISGAMESTRUCT* thisgame);
VOID Player_OnGameLeave();

void AnalyzeVendorItem(DWORD dwItemID, LPCSTR lpszItemCode);
BOOL IsThisPlayerID(const BYTE* ptr);
int CompareHealingPotion(LPCSTR lpszCode1, LPCSTR lpszCode2);
int CompareManaPotion(LPCSTR lpszCode1, LPCSTR lpszCode2);
BOOL BuyMiscItem(DWORD dwItemID, LPCGAMEUNIT lpNpcUnit, int nQuantity);

void NotifyUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam = 0, LPARAM lParam = 0);
void NotifyThisPlayerMessage(UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0);

BYTE CalcPercent(DWORD dwVal, DWORD dwMaxVal, BYTE iMin = 0);
void MakePlayerData(LPGAMEUNIT lpBuffer);
void CheckPlayerMap();
void CheckPlayerHPMana(WORD wNewHP, WORD wNewMana);
DWORD GetSafePlayerID();
LPCSTR GetSafePlayerName();
BYTE GetCurrentAct();
BOOL IsGameJoined();
void AnalyzeItem(const ITEM &item);

BOOL PrepareInteractWithNPC(LPCGAMEUNIT lpNpcUnit);
BOOL OpenNpcStore(LPCGAMEUNIT lpNpcUnit, BOOL bGamble); // Open an npc's store
BOOL GoToEntity(LPCGAMEUNIT lpUnit, BOOL bShowClientMove = TRUE);
BOOL GoToPosition(WORD x, WORD y, BOOL bShowClientMove = TRUE);
LPCITEM FindFirstBeltItem(LPCSTR lpszItemCode);
BOOL IsBeltColFull(int nCol);
void CompleteJobs(BOOL bSuccessful);
BOOL DoFirstJob(BOOL bRedo);
void DoJobProc();
void DoTPProc();
void DoNpcProc();
void StepDone();
void OnMyPortalAppeared(LPCGAMEUNIT lpUnit);
void NotifyNpcSession(BOOL bSuccessful);
BOOL IsStanding();
void StepDone();

DWORD FindFirstBeltPotion(int nType, DWORD& dwClassID);
DWORD FindFirstInventoryPotion(int nType, DWORD& dwClassID);

BOOL VerifyItem(DWORD dwItemID);
void DoWPProc();
void FinishWPProc(BOOL bSucceeded);
BOOL IsNpcActive(int nInteractType = 0);
BOOL IsNpcBullShitting();

//BOOL GetWPPageCoords(BYTE iMapID, POINT& rCorrds);
//int GetWpButtonCoords(BYTE iMapID, POINT& rCorrds);
//BOOL ClickWPTab(BYTE iDestMap);
//BOOL ClickWpButton(BYTE iDestMap);

#endif // __ME_H__