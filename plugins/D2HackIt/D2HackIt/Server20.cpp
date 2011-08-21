///////////////////////////////////////////////////////////////////////
// Server20.cpp
//
// Implememtation of the "server" API set functions that were added in
// D2Hackit 2.0
//
// Abin(abinn32@yahoo.com)
///////////////////////////////////////////////////////////////////////
#include "server20.h"
#include "D2HackItServer.h"
#include "Template\\LinkedList.h"
#include "..\\includes\\utilities\\ArrayEx.h"
#include "d2functions.h"
#include "definitions.h"
#include "D2HackIt.h"
#include "MapDef.h"
#include <math.h>
#include "MsgSender.h"
#include "PathFinder.h"
#include "ScrollDraw.h"
#include "SpellDef.h"
#include "KeyDef.h"

BOOL RevealPresetUnits(CArrayEx<PRESETUNIT, const PRESETUNIT&>& aUnits);
void FindPresetUnitsFromRoom2(RoomOther *ro, CArrayEx<DWORD, DWORD>& aChecked, CArrayEx<PRESETUNIT, const PRESETUNIT&>& aUnits);
BOOL operator==(const PRESETUNIT& lhs, const PRESETUNIT& rhs)
{
	return ::memcmp(&lhs, &rhs, sizeof(PRESETUNIT)) == 0;
}

BOOL operator>(const PRESETUNIT& lhs, const PRESETUNIT& rhs)
{
	return ::memcmp(&lhs, &rhs, sizeof(PRESETUNIT)) > 0;
}

CLinkedList<PKT, const PKT&, FreePktData> g_aPackets; // Packet list
BOOL g_bLoadedBeforeJoin = FALSE; // d2Hackit loaded before joining game?
DWORD g_dwInGameTime = 0; // total in-game time(ms)
CScrollDraw g_scroll;

VOID Proc_OnGameJoin(THISGAMESTRUCT* thisgame)
{
	DrawBanner();
	g_dwInGameTime = 0;

	if (!g_bLoadedBeforeJoin)
	{
		// displays a warning
		GamePrintString("ÿc8--------------------------------------------------------------");
		GamePrintString("ÿc8          D2Hackit Warning");
		GamePrintString(" ");
		GamePrintString("ÿc2D2Hackit! ÿc9was ÿc1NOT ÿc9loaded before joining the game!");
		GamePrintString("ÿc9As a result, some features may not work properly");
		GamePrintString("ÿc9until you enter a new game.");
		GamePrintString("ÿc8--------------------------------------------------------------");
	}
	
	Player_OnGameJoin(thisgame);	
}

VOID Proc_OnGameLeave()
{
	g_scroll.Undraw();
	g_dwInGameTime = 0;
	g_bLoadedBeforeJoin = FALSE;
	Player_OnGameLeave();
	g_aPackets.RemoveAll();	
}

DWORD Proc_OnGameTimerTick()
{
	g_dwInGameTime += psi->TickRate; // update in-game time
	g_scroll.OnTimerTick();

	Player_TimerTick();

	POSITION pos = g_aPackets.GetHeadPosition();
	while (pos)
	{
		// We deposit all packets received in "OnGamepacketBeforeReceived" to 
		// a packet queue, then let "OnGamepacketBeforeReceived" return immediately.
		// Now in this function we process those packets.
		//
		// The reason I'm doing this is because we need such a function(I'd call
		// it "OnGamePacketAfterReceived"). 
		//
		// For example, if we doing this in "OnGamePacketBeforeReceived",
		// when an unit spawn packet 0xac is received, UN_SPAWN will be sent to
		// user modules before "OnGamePacketBeforeReceived" returns, the user may want to
		// access that unit immediately, but the problem is that, by that moment,
		// the game has NOT received that packet yet(hence "BeforeReceived") so that
		// unit does not even exist in the game, and errors will arise.
		//
		// Now I process the packet AFTER the game receives it("OnGamePacketBeforeReceived"
		// already returned), the unit will be valid even if the user tries to access
		// as soon as he receives UM_SPAWN message.
		//
		const PKT& pkt = g_aPackets.GetNext(pos);
		OnGamePacketAfterReceived(pkt.aPacket, pkt.aLen);

		// Pass packet to all clients who wants to snoop
		LinkedItem *li = ClientModules.GetFirstItem();
		while(li)
		{	
			PCIS pCIS = ClientModules.GetCIS(li);
			if(pCIS && pCIS->OnGamePacketAfterReceived)
			{
				pCIS->OnGamePacketAfterReceived(pkt.aPacket, pkt.aLen);
			}
			li = ClientModules.GetNextItem(li);
		}

		g_aPackets.RemoveFront();
	}

	return 0;
}

DWORD Proc_OnGamePacketBeforeSent(BYTE* aPacket, DWORD aLen)
{
	return Player_OnGamePacketBeforeSent(aPacket, aLen);
}

void OnGamePacketAfterReceived(BYTE* aPacket, DWORD aLen) // after received
{
	Player_OnGamePacketAfterReceived(aPacket, aLen);
	
	GAMEUNIT unit;
	WORD x, y;

	//////////////////////////////////////////////////////////////////////
	// Unit Affection
	//////////////////////////////////////////////////////////////////////
	if ((aPacket[0] == 0xa8 || aPacket[0] == 0xaa) && aLen >= 8)
	{
		::memcpy(&unit.dwUnitID, aPacket + 2, 4);
		unit.dwUnitType = UNIT_TYPE_MONSTER;

		if (unit.dwUnitID == GetSafePlayerID())
			NotifyThisPlayerMessage(PM_AFFECT, aPacket[1], aPacket[7]);
		else
			NotifyUnitMessage(UM_AFFECT, &unit, aPacket[1], aPacket[7]);
	}
	
	//////////////////////////////////////////////////////////////////////
	// Unit Disaffection
	//////////////////////////////////////////////////////////////////////
	if (aLen >= 7 && aPacket[0] == 0xa9)
	{
		::memcpy(&unit.dwUnitID, aPacket + 2, 4);
		unit.dwUnitType = UNIT_TYPE_MONSTER;

		if (unit.dwUnitID == GetSafePlayerID())
			NotifyThisPlayerMessage(PM_DISAFFECT, aPacket[1], aPacket[6]);
		else
			NotifyUnitMessage(UM_DISAFFECT, &unit, aPacket[1], aPacket[6]);
	}

	//////////////////////////////////////////////////////////////////////
	// Static Object Spawned
	//////////////////////////////////////////////////////////////////////
	if (aLen > 12 && aPacket[0] == 0x51)
	{
		::memcpy(&unit.dwUnitID, aPacket + 2, 4);
		unit.dwUnitType = aPacket[1];
		WORD x, y;		
		
		::memcpy(&x, aPacket + 8, 2);
		::memcpy(&y, aPacket + 10, 2);
		NotifyUnitMessage(UM_SPAWN, &unit, 0, MAKELPARAM(x, y));
	}

	//////////////////////////////////////////////////////////////////////
	// Unit Spawned
	//////////////////////////////////////////////////////////////////////
	if (aPacket[0] == 0xac && aLen >= 13)
	{
		::memcpy(&unit.dwUnitID, aPacket + 1, 4);
		unit.dwUnitType = UNIT_TYPE_MONSTER;
		::memcpy(&x, aPacket + 7, 2);
		::memcpy(&y, aPacket + 9, 2);
		NotifyUnitMessage(UM_SPAWN, &unit, 0, MAKELPARAM(x, y));
	}
	
	//////////////////////////////////////////////////////////////////////
	// Other Player/Corpse Spawned, My Corpse Spawned
	//////////////////////////////////////////////////////////////////////
	if (aLen == 26 && aPacket[0] == 0x59 && IsGameJoined() && !IsThisPlayerID(aPacket + 1))
	{
		::memcpy(&unit.dwUnitID, aPacket + 1, 4);
		unit.dwUnitType = UNIT_TYPE_PLAYER;

		char szPlayerName[17] = "";
		::memcpy(&szPlayerName, aPacket + 6, 17);		
		::memcpy(&x, aPacket + 22, 2);
		::memcpy(&y, aPacket + 24, 2);

		if (_stricmp(szPlayerName, GetSafePlayerName()))
		{			
			// it's other player, or other player's corpse	
			NotifyUnitMessage(UM_SPAWN, &unit, 0, MAKELPARAM(x, y));
		}
		else
		{
			// it's my corpse
			NotifyThisPlayerMessage(PM_CORPSE, unit.dwUnitID, MAKELPARAM(x, y));
		}
	}

	//////////////////////////////////////////////////////////////////////
	// Object/Unit is Destroyed by the Game (be Gone from Player's Vision) 
	//////////////////////////////////////////////////////////////////////
	if (aLen == 6 && aPacket[0] == 0x0a)
	{
		::memcpy(&unit.dwUnitID, aPacket + 2, 4);
		unit.dwUnitType = aPacket[1];		
		 NotifyUnitMessage(UM_DESTROY, &unit);
	}
	
	//////////////////////////////////////////////////////////////////////
	// Monster HP Changed
	//////////////////////////////////////////////////////////////////////
	if (aLen >= 12 && aPacket[0] == 0x69 && aPacket[5] == 0x06)
	{
		::memcpy(&unit.dwUnitID, aPacket + 1, 4);
		unit.dwUnitType = UNIT_TYPE_MONSTER;
		NotifyUnitMessage(UM_HPPERCENT, &unit, CalcPercent(GetUnitStat(&unit, STAT_HP), GetUnitStat(&unit, STAT_MAXHP)));
	}

	//////////////////////////////////////////////////////////////////////
	// Merc Life Percentage Changed
	//////////////////////////////////////////////////////////////////////
	if (aLen == 10 && aPacket[0] == 0x7f && aPacket[1] == 0x00)
	{
		GAMEUNIT unit;
		::memcpy(&unit.dwUnitID, aPacket + 4, 4);
		unit.dwUnitType = UNIT_TYPE_MONSTER;
		NotifyUnitMessage(UM_HPPERCENT, &unit, CalcPercent(GetUnitStat(&unit, STAT_HP), GetUnitStat(&unit, STAT_MAXHP)));

	}

	//////////////////////////////////////////////////////////////////////
	// Monster Death
	//////////////////////////////////////////////////////////////////////
	if ( aLen >= 12 && aPacket[0] == 0x69 && aPacket[5] == 0x08)
	{
		::memcpy(&unit.dwUnitID, aPacket + 1, 4);
		unit.dwUnitType = UNIT_TYPE_MONSTER;
		NotifyUnitMessage(UM_DEATH, &unit);
	}

	//////////////////////////////////////////////////////////////////////
	// Monster Corpse Appeared
	//////////////////////////////////////////////////////////////////////
	if ( aLen >= 12 && aPacket[0] == 0x69 && aPacket[5] == 0x09)
	{
		::memcpy(&unit.dwUnitID, aPacket + 1, 4);
		unit.dwUnitType = UNIT_TYPE_MONSTER;
		::memcpy(&x, aPacket + 6, 2);
		::memcpy(&y, aPacket + 8, 2);
		NotifyUnitMessage(UM_CORPSE, &unit, 0, MAKELPARAM(x, y));
	}

	//////////////////////////////////////////////////////////////////////
	// Monster Attacking
	//////////////////////////////////////////////////////////////////////
	if (aLen == 21 && aPacket[0] == 0x68)
	{
		GAMEUNIT unitAttacker, unitVictim;
		::memcpy(&unitAttacker.dwUnitID, aPacket + 1, 4);
		::memcpy(&unitVictim.dwUnitID, aPacket + 11, 4);		

		unitAttacker.dwUnitType = UNIT_TYPE_MONSTER;
		unitVictim.dwUnitType = aPacket[10];

		::memcpy(&x, aPacket + 6, 2);
		::memcpy(&y, aPacket + 8, 2);

		NotifyUnitMessage(UM_ATTACK, &unitAttacker, (WPARAM)&unitVictim, MAKELPARAM(x, y));
		NotifyUnitMessage(UM_UNDERATTACK, &unitVictim, (WPARAM)&unitAttacker, MAKELPARAM(x, y));
	}

	//////////////////////////////////////////////////////////////////////
	// Monster Revived by It's Resurrecter (Shaman, Greater Mummy, etc) 
	//////////////////////////////////////////////////////////////////////
	if (aLen == 16 && aPacket[0] == 0x6b)
	{
		::memcpy(&unit.dwUnitID, aPacket + 1, 4);
		unit.dwUnitType = UNIT_TYPE_MONSTER;
		::memcpy(&x, aPacket + 12, 2);
		::memcpy(&y, aPacket + 14, 2);
		NotifyUnitMessage(UM_REVIVED, &unit, (WPARAM)1, MAKELPARAM(x, y));
	}

	//////////////////////////////////////////////////////////////////////
	// Monster Revived by Itself (Act5 Zombies) 
	//////////////////////////////////////////////////////////////////////
	if (aLen == 17 && aPacket[0] == 0x4d && aPacket[1] == 0x01)
	{
		::memcpy(&unit.dwUnitID, aPacket + 2, 4);
		unit.dwUnitType = UNIT_TYPE_MONSTER;
		MAPPOS mp = GetUnitPosition(&unit);
		NotifyUnitMessage(UM_REVIVED, &unit, (WPARAM)0, MAKELPARAM(mp.x, mp.y));
	}
	
	//////////////////////////////////////////////////////////////////////
	// Unit Starts Moving
	//////////////////////////////////////////////////////////////////////
	if (aPacket[0] == 0x67 && aLen == 16)
	{	
		::memcpy(&unit.dwUnitID, aPacket + 1, 4);
		unit.dwUnitType = UNIT_TYPE_MONSTER;
		::memcpy(&x, aPacket + 6, 2);
		::memcpy(&y, aPacket + 8, 2);
		NotifyUnitMessage(UM_STARTMOVE, &unit, 0, MAKELPARAM(x, y));
	}

	//////////////////////////////////////////////////////////////////////
	// Unit Ends Moving
	//////////////////////////////////////////////////////////////////////
	if (aPacket[0] == 0x6d && aLen == 10)
	{
		::memcpy(&unit.dwUnitID, aPacket + 1, 4);
		unit.dwUnitType = UNIT_TYPE_MONSTER;
		::memcpy(&x, aPacket + 5, 2);
		::memcpy(&y, aPacket + 7, 2);
		NotifyUnitMessage(UM_ENDMOVE, &unit, 0, MAKELPARAM(x, y));
	}
	
	//////////////////////////////////////////////////////////////////////
	// Cave Notification
	//////////////////////////////////////////////////////////////////////
	if (aLen == 11 && aPacket[0] == 0x09)
	{
		::memcpy(&unit.dwUnitID, aPacket + 2, 4);
		unit.dwUnitType = aPacket[1];
		::memcpy(&x, aPacket + 7, 2);
		::memcpy(&y, aPacket + 9, 2);
		NotifyUnitMessage(UM_CAVENOTIFY, &unit, aPacket[6], MAKELPARAM(x, y));
	}
}

DWORD Proc_OnGamePacketBeforeReceived(BYTE* aPacket, DWORD aLen)
{
	if (!IsGameJoined() && aLen == 26 && aPacket[0] == 0x59)
	{
		g_bLoadedBeforeJoin = TRUE;
	}

	// We shall only process packets AFTER the game has processed them, not BEFORE!
	PKT pkt;
	pkt.aLen = aLen;
	pkt.aPacket = new BYTE[aLen];
	memcpy(pkt.aPacket, aPacket, aLen);
	g_aPackets.InsertRear(pkt); // Insert this packet to list, process later
	return aLen;	
}

BOOL EXPORT VerifyUnit(LPCGAMEUNIT lpUnit)
{
	if (lpUnit == NULL)
		return FALSE;

	return (BOOL)D2CLIENT_GetUnit(lpUnit->dwUnitID, lpUnit->dwUnitType);
}

BOOL EXPORT FindUnitByClassID(DWORD dwUnitClassID, DWORD dwUnitType, LPGAMEUNIT lpBuffer)
{
	if (lpBuffer == NULL)
		return FALSE;

	::memset(lpBuffer, 0, sizeof(GAMEUNIT));

	Room* pRoom = D2COMMON_GetRoomFromUnit(D2CLIENT_GetPlayerUnit());
	if (!pRoom)
		return FALSE; // not in a game?

	for (pRoom = pRoom->ptAct->ptFirstRoom; pRoom; pRoom = pRoom->ptNext)
	{
		for (UnitAny* pUnit = pRoom->ptFirstUnit; pUnit; pUnit = pUnit->ptListNext)
		{
			if (dwUnitType == pUnit->dwType && dwUnitClassID == pUnit->dwClassId)
			{
				lpBuffer->dwUnitID = pUnit->dwId;
				lpBuffer->dwUnitType = pUnit->dwType;
				return TRUE;
			}			
		}
	}

	return FALSE;
}

BOOL EXPORT FindUnitByName(LPCSTR lpszUnitName, DWORD dwUnitType, LPGAMEUNIT lpBuffer)
{
	if (lpszUnitName == NULL || !lpszUnitName[0] || lpBuffer == NULL)
		return FALSE;

	::memset(lpBuffer, 0, sizeof(GAMEUNIT));

	// convert to unicode first
	wchar_t wszName[256] = {0};
	ConvertToUnicode(lpszUnitName, wszName, 255);

	Room* pRoom = D2COMMON_GetRoomFromUnit(D2CLIENT_GetPlayerUnit());
	if (!pRoom)
		return FALSE; // not in a game?

	for (pRoom = pRoom->ptAct->ptFirstRoom; pRoom; pRoom = pRoom->ptNext)
	{
		for (UnitAny* pUnit = pRoom->ptFirstUnit; pUnit; pUnit = pUnit->ptListNext)
		{
			if (dwUnitType == pUnit->dwType)
			{
				wchar_t* pwsz = D2CLIENT_GetNameFromUnit(pUnit);
				if (pwsz && !_wcsicmp(wszName, pwsz))
				{
					lpBuffer->dwUnitID = pUnit->dwId;
					lpBuffer->dwUnitType = pUnit->dwType;
					return TRUE;
				}				
			}			
		}
	}

	return FALSE;
}

LPWSTR ConvertToUnicode(LPCTSTR lpszAnsiString, LPWSTR lpUnicodeString, int maxlen)
{
	::MultiByteToWideChar(CP_ACP, 0, lpszAnsiString, -1, lpUnicodeString, maxlen);
	return lpUnicodeString;
}

LPSTR ConvertFromUnicode(LPCWSTR lpUnicodeString, LPSTR lpszAnsiString, int maxlen)
{
	::WideCharToMultiByte(CP_ACP, 0, lpUnicodeString, -1, lpszAnsiString, maxlen, NULL, NULL);
	return lpszAnsiString;
}

DWORD EXPORT GetUnitName(LPCGAMEUNIT lpUnit, LPSTR lpszBuffer, DWORD dwMaxChars)
{
	if (lpUnit == NULL || lpszBuffer == NULL || dwMaxChars == 0)
		return 0;

	::memset(lpszBuffer, 0, dwMaxChars);
	UnitAny* pUnit = D2CLIENT_GetUnit(lpUnit->dwUnitID, lpUnit->dwUnitType);
	if (pUnit == NULL)
		return 0;

	wchar_t* wszName = D2CLIENT_GetNameFromUnit(pUnit);
	if (wszName == NULL)
		return 0;

	ConvertFromUnicode(wszName, lpszBuffer, dwMaxChars);
	return strlen(lpszBuffer);
}

MAPPOS EXPORT GetUnitPosition(LPCGAMEUNIT lpUnit)
{
	MAPPOS mp = { 0 };
	UnitAny* p = (UnitAny*)VerifyUnit(lpUnit);
	if (p == NULL)
		return mp;

	if (lpUnit->dwUnitType == UNIT_TYPE_PLAYER
		|| lpUnit->dwUnitType == UNIT_TYPE_MONSTER
		|| lpUnit->dwUnitType == UNIT_TYPE_MISSILE)
	{
		mp.x = (WORD)D2COMMON_GetUnitX(p->hPath);
		mp.y = (WORD)D2COMMON_GetUnitY(p->hPath);		
	}
	else
	{
		mp.x = (WORD)p->hOPath->Xpos;
		mp.y = (WORD)p->hOPath->Ypos;
	}

	return mp;
}

DWORD EXPORT GetUnitOwnerID(LPCGAMEUNIT lpUnit)
{
	if (lpUnit == NULL)
		return -1;

	UnitAny* p = (UnitAny*)VerifyUnit(lpUnit);
	if (p == NULL)
		return -1;

	if (lpUnit->dwUnitType == UNIT_TYPE_MONSTER)
		return D2CLIENT_GetMonsterOwner(lpUnit->dwUnitID);

	if (lpUnit->dwUnitType == UNIT_TYPE_OBJECT)
	{
		if (!::_stricmp(GetSafePlayerName(), p->ptObjectData->parent))
			return GetSafePlayerID();
	}

	if (lpUnit->dwUnitType == UNIT_TYPE_ITEM)
	{
		if (p->ptItemData->ptOwnerInv && p->ptItemData->ptOwnerInv->ptOwner)
			return p->ptItemData->ptOwnerInv->ptOwner->dwId;
	}

	return -1;
}

DWORD EXPORT GetUnitClassID(LPCGAMEUNIT lpUnit)
{
	UnitAny* p = D2CLIENT_GetUnit(lpUnit->dwUnitID, lpUnit->dwUnitType);
	if (p == NULL)
		return 0;

	return p->dwClassId;
}

DWORD EXPORT GetUnitStat(LPCGAMEUNIT lpUnit, int nStatID)
{
	UnitAny* p = D2CLIENT_GetUnit(lpUnit->dwUnitID, lpUnit->dwUnitType);
	if (p == NULL)
		return 0;

	return D2COMMON_GetBaseStat(p, nStatID, 0);
}

BYTE EXPORT GetUnitHPPercent(LPCGAMEUNIT lpUnit)
{
	UnitAny* p = D2CLIENT_GetUnit(lpUnit->dwUnitID, lpUnit->dwUnitType);
	if (p == NULL)
		return 0;

	return CalcPercent(D2COMMON_GetBaseStat(p, STAT_HP, 0), D2COMMON_GetBaseStat(p, STAT_MAXHP, 0), 1);
}

BOOL EXPORT GetUnitAffection(LPCGAMEUNIT lpUnit, int nAffectionID)
{
	UnitAny* p = D2CLIENT_GetUnit(lpUnit->dwUnitID, lpUnit->dwUnitType);
	if (p == NULL)
		return 0;

	return D2COMMON_GetUnitState(p, nAffectionID);
}

int EXPORT GetMonsterType(LPCGAMEUNIT lpUnit) // normal/champion/boss/minion
{
	UnitAny* p = D2CLIENT_GetUnit(lpUnit->dwUnitID, lpUnit->dwUnitType);
	if (p == NULL || p->ptMonsterData == NULL)
		return MONSTER_UNKNOWN;	

	if (p->ptMonsterData->fNormal & 1)
		return MONSTER_NORMAL;

	if (p->ptMonsterData->fChamp & 1)
		return MONSTER_CHAMPION;

	if (p->ptMonsterData->fBoss & 1)
		return MONSTER_BOSS;

	if (p->ptMonsterData->fMinion & 1)
		return MONSTER_MINION;

	return MONSTER_UNKNOWN;
}

BOOL EXPORT GetBossEnchants(LPCGAMEUNIT lpUnit, LPBOSSENCHANTS lpBuffer)
{
	if (lpUnit == NULL || lpBuffer == NULL)
		return FALSE;
	
	::memset(lpBuffer, 0, sizeof(BOSSENCHANTS));
	UnitAny* p = D2CLIENT_GetUnit(lpUnit->dwUnitID, lpUnit->dwUnitType);
	if (p == NULL || p->ptMonsterData == NULL)
		return FALSE;

	for (int i = 0; i < 9 && p->ptMonsterData->anEnchants[i]; i++)
	{
		switch (p->ptMonsterData->anEnchants[i])
		{
		case 0x05: // Extra Strong
			lpBuffer->bExtraStrong = TRUE;
			break;

		case 0x06: // Extra Fast
			lpBuffer->bExtraFast = TRUE;
			break;

		case 0x1a: // Teleport
			lpBuffer->bTeleport = TRUE;
			break;

		case 0x1c: // Stone Skin
			lpBuffer->bStoneSkin = TRUE;
			break;

		case 0x1b: // Spectral Hit
			lpBuffer->bSpectralHit = TRUE;
			break;

		case 0x09: // Fire Enchanted
			lpBuffer->bFireEnchanted = TRUE;
			break;

		case 0x11: // Lightning Enchanted
			lpBuffer->bLightningEnchanted = TRUE;
			break;

		case 0x12: // Cold Enchanted
			lpBuffer->bColdEnchanted = TRUE;
			break;

		case 0x1d: // Multishot
			lpBuffer->bMultiShot = TRUE;
			break;

		case 0x1e: // Aura Enchanted
			lpBuffer->bAuraEnchanted = TRUE;
			break;

		case 0x08: // Magic resistant
			lpBuffer->bMagicResistant = TRUE;
			break;

		case 0x19: // Mana Burn
			lpBuffer->bManaBurn = TRUE;
			break;

		case 0x07: // Cursed
			lpBuffer->bCursed = TRUE;
			break;

		default:
			break;
		}
	}

	return TRUE;
}

void TrimString(LPSTR lpszString)
{
	if (lpszString == NULL)
		return;

	int nLen = strlen(lpszString);

	// '\n' and '\r' are actually not possible in this case, but anyway...
	
	// Trim right side
	while (nLen > 0
		&& (lpszString[nLen - 1] == ' '
			|| lpszString[nLen - 1] == '\t'
			|| lpszString[nLen - 1] == '\r'
			|| lpszString[nLen - 1] == '\n'))
	{
		lpszString[--nLen] = 0;
	}

	// Trim left side
	LPCTSTR p = lpszString; 
	while (*p ==' '
			|| *p == '\t'
			|| *p == '\r'
			|| *p == '\n')
	{
		p = &p[1];
	}

	if (p != lpszString)
	{
		LPTSTR psz = _strdup(p);
		strcpy(lpszString, psz);
		delete [] psz;
	}
}

DWORD EXPORT GetDistance(int x1, int y1, int x2, int y2) // distance between two positions, in yards
{
	return (DWORD)::sqrt((double)((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)));
}

void CALLBACK FreePktData(PKT& pkt)
{
	if (pkt.aPacket)
		delete [] pkt.aPacket;

	memset(&pkt, 0, sizeof(PKT));
}

BOOL EXPORT GetMyMercUnit(LPGAMEUNIT lpBuffer) // Get player's mercenary
{
	if (lpBuffer == NULL)
		return FALSE;

	::memset(lpBuffer, 0, sizeof(GAMEUNIT));

	const DWORD PID = GetSafePlayerID();
	if (PID == 0)
		return FALSE;

	Room* pRoom = D2COMMON_GetRoomFromUnit(D2CLIENT_GetPlayerUnit());
	if (!pRoom)
		return FALSE; // not in a game?

	for (pRoom = pRoom->ptAct->ptFirstRoom; pRoom; pRoom = pRoom->ptNext)
	{
		for (UnitAny* pUnit = pRoom->ptFirstUnit; pUnit; pUnit = pUnit->ptListNext)
		{
			if (pUnit->dwType == UNIT_TYPE_MONSTER // type check
				&& IsMercClassID(pUnit->dwClassId) // class id check
				&& D2CLIENT_GetMonsterOwner(pUnit->dwId) == PID) // owner check
			{
				lpBuffer->dwUnitID = pUnit->dwId;
				lpBuffer->dwUnitType = pUnit->dwType;
				return TRUE;
			}			
		}
	}

	return FALSE;
}

DWORD EXPORT IsMercUnit(LPCGAMEUNIT lpUnit) // Check if an unit is a merc
{
	if (lpUnit == NULL || lpUnit->dwUnitType != UNIT_TYPE_MONSTER || !IsMercClassID(GetUnitClassID(lpUnit)))
		return 0;

	DWORD dwOwnerID = GetUnitOwnerID(lpUnit);
	return dwOwnerID == -1 ? 0 : dwOwnerID;
}

BOOL IsMercClassID(DWORD dwClassID)
{
	return dwClassID == MERC_A1 || dwClassID == MERC_A2 || dwClassID == MERC_A3 || dwClassID == MERC_A5;
}

DWORD EXPORT GetPing()
{
	WORD wPing = 0;
	if (!::ReadProcessMemory(::GetCurrentProcess(), (LPCVOID)0x6FBA7774, &wPing, 2, NULL))
		return 0;
	return wPing;	
}

BOOL EXPORT EnumUnits(DWORD dwUnitType, fnEnumUnitProc lpfnEnumUnitProc, LPARAM lParam)
{
	if (lpfnEnumUnitProc == NULL)
		return FALSE;

	Room* pRoom = D2COMMON_GetRoomFromUnit(D2CLIENT_GetPlayerUnit());
	if (!pRoom)
		return FALSE; // not in a game?

	GAMEUNIT gu;
	for (pRoom = pRoom->ptAct->ptFirstRoom; pRoom; pRoom = pRoom->ptNext)
	{
		for (UnitAny* pUnit = pRoom->ptFirstUnit; pUnit; pUnit = pUnit->ptListNext)
		{
			if (dwUnitType == pUnit->dwType)
			{
				gu.dwUnitID = pUnit->dwId;
				gu.dwUnitType = pUnit->dwType;

				if (!lpfnEnumUnitProc(&gu, lParam))
					return FALSE;
			}			
		}
	}

	return TRUE;
}

SIZE EXPORT GetScreenSize()
{
	SIZE cz;
	cz.cx = *p_D2CLIENT_ClientScreenWidth;
	cz.cy = cz.cx == 800 ? 600 : 480;
	return cz;
}

BOOL EXPORT WasD2HackitLoadedBeforeJoinGame()
{
	return g_bLoadedBeforeJoin;
}

DWORD EXPORT GetAffectionName(BYTE iAffectionID, LPTSTR lpszBuffer, DWORD dwMaxChars)
{
	return D2GetAffectionName(iAffectionID, lpszBuffer, dwMaxChars);
}

BOOL EXPORT AffectionIsAura(BYTE iAffectionID)
{
	return D2IsAura(iAffectionID);
}

DWORD EXPORT GetMapName(BYTE iMapID, LPTSTR lpszBuffer, DWORD dwMaxChars) // Name of the map
{
	return D2GetMapName(iMapID, lpszBuffer, dwMaxChars);
}

BOOL EXPORT MapIsTown(BYTE iMapID) // Is town?
{
	return D2MapIsTown(iMapID);
}

BYTE EXPORT GetTownMap(BYTE iAct)
{
	return D2GetTownMap(iAct);
}

BYTE EXPORT GetActFromMap(BYTE iMapID)
{
	return D2GetActFromMap(iMapID);
}

CHAR EXPORT GetCommandCharacter()
{
	return psi->CommandCharacter;
}

int EXPORT GetTickRate()
{
	return psi->TickRate;
}

// item related
BOOL EXPORT ParseItem(const BYTE* aPacket, DWORD aLen, ITEM& item)
{
	return D2ParseItem(aPacket, aLen, item);
}

SIZE EXPORT GetItemSize(LPCSTR lpszItemCode)
{
	return D2GetItemSize(lpszItemCode);
}

BOOL EXPORT IsStackable(LPCSTR lpszItemCode)
{
	return D2IsStackable(lpszItemCode);
}

LPCSTR EXPORT GetItemName(LPCSTR lpszItemCode)
{
	return D2GetItemName(lpszItemCode);
}

LPCSTR EXPORT GetItemSetName(LPCSTR lpszItemCode)
{
	return D2GetItemSetName(lpszItemCode);
}

BOOL EXPORT IsWeapon(LPCSTR lpszItemCode)
{
	return D2IsWeapon(lpszItemCode);
}

BOOL EXPORT IsArmor(LPCSTR lpszItemCode)
{
	return D2IsArmor(lpszItemCode);
}

int EXPORT IsPotion(LPCSTR lpszItemCode)
{
	return D2IsPotion(lpszItemCode);
}

BOOL EXPORT GetItemQuality(DWORD dwItemID)
{
	UnitAny* p = D2CLIENT_GetUnit(dwItemID, UNIT_TYPE_ITEM);
	if(p == NULL)
		return FALSE;

	return D2COMMON_GetItemQuality(p);
}

BYTE EXPORT GetItemBeltRows(LPCSTR lpszItemCode)
{
	return D2GetBeltRows(lpszItemCode);
} 

int EXPORT GetMyCorpseCount()
{
	int nCount = 0;

	const DWORD PID = GetSafePlayerID();
	if (PID == 0)
		return 0;

	// convert to unicode first
	wchar_t wszName[256] = {0};
	ConvertToUnicode(GetSafePlayerName(), wszName, 255);

	Room* pRoom = D2COMMON_GetRoomFromUnit(D2CLIENT_GetPlayerUnit());
	if (!pRoom)
		return 0; // not in a game?

	for (pRoom = pRoom->ptAct->ptFirstRoom; pRoom; pRoom = pRoom->ptNext)
	{
		for (UnitAny* pUnit = pRoom->ptFirstUnit; pUnit; pUnit = pUnit->ptListNext)
		{
			if (pUnit->dwType == UNIT_TYPE_PLAYER && pUnit->dwId != PID)
			{
				wchar_t* pwsz = D2CLIENT_GetNameFromUnit(pUnit);
				if (pwsz && !_wcsicmp(wszName, pwsz))
				{
					nCount++;
				}				
			}			
		}
	}

	return nCount;
}

BOOL EXPORT PickMyCorpse()
{
	// just pick the first corpse
	const DWORD PID = GetSafePlayerID();
	if (PID == 0)
		return 0;

	// convert to unicode first
	wchar_t wszName[256] = {0};
	ConvertToUnicode(GetSafePlayerName(), wszName, 255);

	Room* pRoom = D2COMMON_GetRoomFromUnit(D2CLIENT_GetPlayerUnit());
	if (!pRoom)
		return 0; // not in a game?

	for (pRoom = pRoom->ptAct->ptFirstRoom; pRoom; pRoom = pRoom->ptNext)
	{
		for (UnitAny* pUnit = pRoom->ptFirstUnit; pUnit; pUnit = pUnit->ptListNext)
		{
			if (pUnit->dwType == UNIT_TYPE_PLAYER && pUnit->dwId != PID)
			{
				wchar_t* pwsz = D2CLIENT_GetNameFromUnit(pUnit);
				if (pwsz && !_wcsicmp(wszName, pwsz))
				{
					// its my corpse
					GAMEUNIT gu;
					gu.dwUnitID = pUnit->dwId;
					gu.dwUnitType = UNIT_TYPE_PLAYER;
					MAPPOS mp = GetUnitPosition(&gu);
					if (GetDistanceFrom(mp.x, mp.y) > 5)
						GoToEntity(&gu);

					return Interact(&gu);
				}				
			}			
		}
	}

	return FALSE;
}

BOOL EXPORT SendKey(BYTE iVKCode) // Send a key-down message to d2 window
{
	CMsgSender sender;
	sender.SetTargetHWnd(FindMyD2WndHandle());
	return sender.SendSysKey(iVKCode);
}


BOOL EXPORT LeftClick(long x, long y)
{
	POINT pt = GetMousePosition();
	CMsgSender sender;
	sender.SetTargetHWnd(FindMyD2WndHandle());
	sender.LeftClick(x, y);
	sender.MouseMove(pt.x, pt.y);
	return TRUE;
}

BOOL EXPORT RightClick(long x, long y)
{
	POINT pt = GetMousePosition();
	CMsgSender sender;
	sender.SetTargetHWnd(FindMyD2WndHandle());
	sender.RightClick(x, y);
	sender.MouseMove(pt.x, pt.y);
	return TRUE;
}

// ++ B

BOOL EXPORT LeftClickHere()
{
	CMsgSender sender;
	sender.SetTargetHWnd(FindMyD2WndHandle());
	POINT pt = GetMousePosition();
	sender.LeftClick(pt.x, pt.y);
	return TRUE;
}

BOOL EXPORT RightClickHere()
{
	CMsgSender sender;
	sender.SetTargetHWnd(FindMyD2WndHandle());
	POINT pt = GetMousePosition();
	sender.RightClick(pt.x, pt.y);
	return TRUE;
}
// -- B

DWORD EXPORT GetSpellName(WORD wSpell, LPSTR lpszBuffer, DWORD dwMaxChars)
{
	if (lpszBuffer == NULL || dwMaxChars == 0)
		return FALSE;

	::memset(lpszBuffer, 0, dwMaxChars);

	D2SPELLINFO dsi = {0};
	if (!D2GetSpellInfo(wSpell, &dsi))
		return 0;

	strncpy(lpszBuffer, dsi.szSpellName, dwMaxChars);
	return strlen(lpszBuffer);
}

int EXPORT GetGameDifficulty() // current game difficulty, normal/nm/hell
{
	return *p_D2CLIENT_nDiff + 1;
}

MAPPOS EXPORT ScreenToMapCoords(POINT ptScreenCoords)
{
	// I thought these are Mousepad's code, but actually Ninjai's
	ptScreenCoords.x += D2CLIENT_GetMouseXOffset();
	ptScreenCoords.y += D2CLIENT_GetMouseYOffset();
	D2COMMON_AbsScreenToMapCoords(&ptScreenCoords.x, &ptScreenCoords.y);
	MAPPOS pos;
	pos.x = (WORD)ptScreenCoords.x;
	pos.y = (WORD)ptScreenCoords.y;
	return pos;
}

POINT EXPORT MapToScreenCoords(MAPPOS mapPosition)
{
	POINT pt;
	pt.x = (long)mapPosition.x;
	pt.y = (long)mapPosition.y;
	D2COMMON_MapToAbsScreenCoords(&pt.x, &pt.y);
	pt.x -= D2CLIENT_GetMouseXOffset();
	pt.y -= D2CLIENT_GetMouseYOffset();
	return pt;
}

DWORD EXPORT GetD2WindowTitle(LPSTR lpszBuffer, DWORD dwMaxChars)
{
	if (lpszBuffer == NULL || dwMaxChars == 0)
		return 0;

	::memset(lpszBuffer, 0, dwMaxChars);
	return ::GetWindowText(FindMyD2WndHandle(), lpszBuffer, dwMaxChars);
}

DWORD EXPORT GetInGameTime() // time elapsed since joining
{
	return g_dwInGameTime;
}

POINT EXPORT GetMousePosition()
{
	POINT pt = { 0 };
	pt.x = D2CLIENT_GetMouseX();
	pt.y = D2CLIENT_GetMouseY();
	return pt;
}

LPCSTR EXPORT GetRealmName()
{
	return (*p_D2LAUNCH_bnData) ? (**p_D2LAUNCH_bnData).RealmName : NULL;
}

LPCSTR EXPORT GetGameName()
{
	return (*p_D2LAUNCH_bnData) ? (**p_D2LAUNCH_bnData).GameName : NULL;
}

LPCSTR EXPORT GetGamePassword()
{
	return (*p_D2LAUNCH_bnData) ? (**p_D2LAUNCH_bnData).GamePass : NULL;
}

LPCSTR EXPORT GetGameIP()
{
	if (!thisgame || !thisgame->CurrentGame)
		return NULL;

	return thisgame->CurrentGame->ServerIP;
}

BOOL EXPORT EnumPresetUnits(fnEnumPresetUnitProc lpfnEnumPresetUnitProc, LPARAM lParam)
{
	if (lpfnEnumPresetUnitProc == NULL)
		return FALSE;

	CArrayEx<PRESETUNIT, const PRESETUNIT&> aUnits;
	RevealPresetUnits(aUnits);
	
	for (int i = 0; i < aUnits.GetSize(); i++)
	{
		if (!lpfnEnumPresetUnitProc(&aUnits[i], lParam))
			return FALSE;
	}

	return TRUE;
}

BYTE EXPORT CalculatePath(WORD x, WORD y, LPPATH lpBuffer, int nAdjust)
{
	CPathFinder pf;
	return pf.CalculatePathTo(x, y, lpBuffer, nAdjust);
}

BOOL EXPORT GetItemCodeEx(DWORD dwItemID, LPSTR lpszBuffer, DWORD dwMaxChars, int maxRetryCount, int delayBeteenRetries)
{
	int retryCount = 0;

	while(!GetItemCode(dwItemID, lpszBuffer, dwMaxChars))
	{
		if(retryCount > maxRetryCount)
		{
			return FALSE;
		}

		Sleep(delayBeteenRetries);
		retryCount++;
	}

	return TRUE;
}

BOOL EXPORT GetItemCode(DWORD dwItemID, LPSTR lpszBuffer, DWORD dwMaxChars)
{
	if (lpszBuffer == NULL ||dwMaxChars == 0)
		return FALSE;

	::memset(lpszBuffer, 0, dwMaxChars);

	GAMEUNIT gu;
	gu.dwUnitID = dwItemID;
	gu.dwUnitType = UNIT_TYPE_ITEM;
	UnitAny* pUnit = (UnitAny*)VerifyUnit(&gu);
	if (pUnit == NULL)
		return FALSE;

	ItemTxt* ptxt = D2COMMON_GetItemTxt(pUnit->dwClassId);
	if (ptxt == NULL)
		return FALSE;

	::strncpy(lpszBuffer, ptxt->szCode, min(dwMaxChars, 3));
	return strlen(lpszBuffer);
}

BOOL RevealPresetUnits(CArrayEx<PRESETUNIT, const PRESETUNIT&>& aUnits)
{
	CArrayEx<DWORD, DWORD> aChecked;
	aUnits.RemoveAll();

	UnitAny *player=D2CLIENT_GetPlayerUnit();
	if(!player)
	{
		GamePrintError("Invalid player");
		return FALSE;
	}

	Room *curRoom=D2COMMON_GetRoomFromUnit(player);
	if(!curRoom)
	{
		GamePrintError("Invalid Room");
		return FALSE;
	}
	
	RoomOther *curRoomOther = curRoom->ptRoomOther;
	if(!curRoomOther)
	{
		GamePrintError("Invalid Room2");
		return FALSE;
	}

	FindPresetUnitsFromRoom2(curRoomOther, aChecked, aUnits);
	return aUnits.GetSize();
}

void FindPresetUnitsFromRoom2(RoomOther *ro, CArrayEx<DWORD, DWORD>& aChecked, CArrayEx<PRESETUNIT, const PRESETUNIT&>& aUnits)
{
	if(ro->ptDrlgLevel->LevelNo!=GetCurrentMapID())
		return;

	if (aChecked.Find((DWORD)ro) != -1)
		return;

	BOOL add_room=FALSE;
	if(!ro->pRoom)
	{
		add_room=TRUE;
		D2COMMON_AddRoomData(D2CLIENT_GetPlayerUnit()->ptAct, GetCurrentMapID(), ro->xPos, ro->yPos, D2CLIENT_GetPlayerUnit());
	}

	aChecked.Add((DWORD)ro);
	aChecked.Sort();
	
	PresetUnit *unit = ro->pPresetUnits;
	while (unit) 
	{	
		PRESETUNIT pu;
		pu.dwID = unit->nTxtFileNo;
		pu.dwType = unit->nUnitType;
		pu.x = static_cast<WORD>(unit->xPos + ro->xPos * 5);
		pu.y = static_cast<WORD>(unit->yPos + ro->yPos * 5);

		if (aUnits.Find(pu) == -1)
			aUnits.Add(pu);

		unit = unit->pNext;
	}

	RoomOther **n = ro->ptList;
	for(int i=0;i<ro->nRoomList;i++)
	{
		FindPresetUnitsFromRoom2(n[i], aChecked, aUnits);
	}
	if(add_room)
	{
		D2COMMON_RemoveRoomData(D2CLIENT_GetPlayerUnit()->ptAct, GetCurrentMapID(), ro->xPos, ro->yPos, D2CLIENT_GetPlayerUnit());
	}
}

/////////////////////////////////////////////////////
// DrawBanner
//
// Draw the text "D2Hackit! vx.xx" on the top-middle screen
// Available since 2.00, by Abin
/////////////////////////////////////////////////////
BOOL DrawBanner()
{
	if (!g_bDrawBanner)
	{
		ReleaseScreenHook(g_dwBannerHook);
		g_dwBannerHook = 0;
		return TRUE;
	}

	if (g_dwBannerHook == 0)
		g_dwBannerHook = CreateScreenHook();
	
	if (g_dwBannerHook == 0)
		return FALSE;// failed to create screen hook for our banner text
	
	char szBanner[128] = "";
	sprintf(szBanner,
			"ÿc4D2Hackit! ÿc2%d.%02d ÿc4Build ÿc2%02d%02d", 
			LOWORD(__SERVERVERSION__),
			HIWORD(__SERVERVERSION__), 
			LOWORD(__SERVERBUILD__),
			HIWORD(__SERVERBUILD__));

	SIZE cScreen = GetScreenSize();
	SIZE cText = GetTextSize(szBanner, D2FONT_SMOOTH);
	long x = cScreen.cx / 2 - cText.cx / 2;
	DrawSingleText(g_dwBannerHook, szBanner, x, 15, D2FONT_SMOOTH, FALSE);
	return TRUE;
}

void EXPORT SetScrollTitle(LPCSTR lpszTitle)
{
	g_scroll.SetTitle(lpszTitle);
}

BOOL EXPORT AddScrollLine(LPCSTR lpszText)
{
	return g_scroll.AddLine(lpszText);
}

BOOL EXPORT ShowScroll()
{
	return g_scroll.Draw();
}

void EXPORT ClearScroll()
{
	g_scroll.ClearAll();
}

DWORD EXPORT QueryModuleData(LPCSTR lpszModuleName, DWORD dwDataID, LPVOID lpBuffer, DWORD dwMaxLength, LPARAM lParam)
{
	if (lpszModuleName == NULL || !lpszModuleName[0])
		return 0;

	PCIS pCIS = NULL;
	LinkedItem *li = ClientModules.GetFirstItem();
	while(li)
	{	
		pCIS = ClientModules.GetCIS(li);
		if(pCIS && !_stricmp(pCIS->Name, lpszModuleName))
		{
			if (pCIS->OnQueryModuleData)
			{
				return pCIS->OnQueryModuleData(dwDataID, lpBuffer, dwMaxLength, lParam);
			}
			else
			{
				GameErrorf("\"OnQueryModuleData\" not exported by module \"%s\"!", lpszModuleName);
				return 0;
			}
		}

		li = ClientModules.GetNextItem(li);
	}

	GameErrorf("QueryModuleData Error: module \"%s\" not exists!", lpszModuleName);
	return 0; // module not found
}

DWORD EXPORT GetKeyName(BYTE iKCode, LPSTR lpszBuffer, DWORD dwMaxChars)
{
	return D2GetKeyName(iKCode, lpszBuffer, dwMaxChars);
}

void EXPORT DebugTest()
{
	UnitAny* pUnit = D2CLIENT_GetPlayerUnit();
	if (!pUnit)
		return;

	Inventory* pInv = pUnit->ptInventory;

	if (!pInv)
		return;

	for (UnitItem* p = pInv->pFirstItem; p && p->ptItemData; p = p->ptItemData->ptNextInvenItem)
	{
		GameInfof("%08X, %d", p->dwId, p->ptItemData->ItemLocation/*D2COMMON_GetItemNodePage(p)*/);
	}
}

