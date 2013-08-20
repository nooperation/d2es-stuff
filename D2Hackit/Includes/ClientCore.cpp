//////////////////////////////////////////////////////////////////////
// ClientCore.cpp
// -------------------------------------------------------------------
// Default Client Dll entrypoint.
//
// <thohell@home.se>
// <Dan_Goon@diabloworld.com>
//
// 0.7	: Updated by Dan_Goon
//////////////////////////////////////////////////////////////////////
#include "D2Client.h"


//////////////////////////////////////////////////////////////////////
// Global server struct holding pointers to exported functions
//////////////////////////////////////////////////////////////////////
FUNCTIONENTRYPOINTS		g_SF = { 0 };	
FUNCTIONENTRYPOINTS		*server = &g_SF;	
HMODULE					g_hModule = NULL;			// New Global
PSERVERINFO				g_pSI = NULL;				// 0.7
PLAYERENTRYPOINTS		g_player = { 0 };			// 2.00 (Abin)
PLAYERENTRYPOINTS*		me = &g_player;				// 2.00 (Abin)
SCREENENTRYPOINTS		g_screen = { 0 };			// 2.00 (Abin)
SCREENENTRYPOINTS*		screen = &g_screen;			// 2.00 (Abin)

BOOL PRIVATE SetupClient(HMODULE hModule, LPCSTR szModule);

LPCSTR					szLodErr = "D2Client Load Error";
LPSTR					m_szLargeBuffer = NULL;
DWORD					m_nLargeBufferSize = 0;
DWORD					m_nSectNameIndex = 0;

#ifdef	D2HACKIT_DEBUG
char	g_szDebug[512];
#endif

#ifdef _AFXDLL
static AFX_EXTENSION_MODULE D2H_CLIENT = { NULL, NULL };
#endif

//********************************************************************************************
// Important Notice on UNLOADING PROCESS!!
//
//	Client Module's DllMain(DLL_PROCESS_DETACH) can be called in two different sequences as follows! 
//
//	1. When FreeLibray() is called by D2HackIt during normal unloading process
//		(by unload command or during ServerStop() process).
//	2. Diablo II is exiting by user op (this is called prior to D2HackIt's ServerStop()!).
//		In this case, all client's thread memory is detached before OnClientStop() call!!!!
//		So, if client references any memory allocated in client's thread while OnClientStop() process,
//		it may casue memory exception, crash, hanging or remained-in-memory.
//		To prevent this situation, the unloading process is changed as follows:
//
//	New Unloading Process
//
//	1. When client's DllMain(DLL_PROCESS_DETACH) is called, calls server->ClenaMeUp()
//	2. server->CleanMeUp() handles actuall unloading process and calls client's OnGameLeave() and OnClientStop().
//	3. While unloading client by normal command or in ServerStop() process, just do as before.
//
//	So, the actual unload sequence for two different cases will be :
//
//	1. Normal Sequence (Unloading D2HackIt by second "D2Loader.exe" execution or by ".unload" command)
//
//		1) D2HackIt's DllMain(DLL_PROCESS_DETACH) : ServerStop() - this is D2HackIt's unloading process.
//		2) calls optional OnGameLeave() and OnClientStop() of client.
//		3) frees up client info structure and delete linked list.
//		4) calls FreeLibrary(hClient).
//		5) After D2HackIt is destructed, client's DllMain(DLL_PROCESS_DETACH) will be called.
//		6) server->CleanMeUp() is called : BUT THIS IS TREATED AS DUMMY CALL - DO NOTHING.
//
//	2. D2-fisrt Exiting Sequence (Exit from D2 by user op or accidental exiting)
//
//		1) Client's DllMain(DLL_PROCESS_DETACH) is called first.
//		2) server->CleanMeUp() is called.
//		3) then calls optional OnGameLeave() and OnClientStop() of client.
//		4) frees up client info structure and delete linked list.
//		5) then D2HackIt's DllMain(DLL_PROCESS_DETACH) : ServerStop() is called.
//		6) clients are already unloaded - DO NOTHING.
//
//********************************************************************************************


//////////////////////////////////////////////////////////////////////
// Dll entry/exit
//////////////////////////////////////////////////////////////////////
extern "C" BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	char szTemp[MAX_PATH], szFile[MAX_PATH];
	BOOL bResult = TRUE;

    switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:

#ifdef _AFXDLL
		if (!AfxInitExtensionModule(D2H_CLIENT, (HINSTANCE)hModule)) return FALSE;
#endif

		DGTRACE(( CHDR "DLL_PROCESS_ATTACH\n" ));

		g_hModule = (HMODULE)hModule;
		GetModuleFileName( g_hModule, szTemp, sizeof(szTemp) );
		_splitpath_s(szTemp, NULL, 0, NULL, 0, szFile, sizeof(szFile)/sizeof(szFile[0]), NULL, 0);		// Extract File Title

		bResult = FALSE;
		// Bind exported functions from server
		HMODULE hD2Hackit;
		hD2Hackit = (HMODULE)GetModuleHandle("D2HackIt");
		if(!hD2Hackit)
		{
			sprintf_s(szTemp, "[%s] : Failed to get D2HackIt!", szFile);
			MessageBox(NULL, szTemp, szLodErr, MB_OK);
			break;
		}

		// Fill in the server & g_pSI (server info)
		return SetupClient(hD2Hackit, szFile);

	case DLL_THREAD_DETACH:

		// Fall here first in case of accidental exit of Diablo II
		DGTRACE(( CHDR "DLL_THREAD_DETACH\n" ));
		break;

	case DLL_PROCESS_DETACH:

		// This is normal call from FreeLibrary
		DGTRACE(( CHDR "DLL_PROCESS_DETACH\n" ));

		// This call is available from V0.8
		if(!IsBadCodePtr((FARPROC)server->CleanMeUp)) server->CleanMeUp(g_hModule);

		if(m_szLargeBuffer) delete [] m_szLargeBuffer;
		m_szLargeBuffer = NULL;

#ifdef _AFXDLL
		AfxTermExtensionModule(D2H_CLIENT);
#endif
		break;
    } 

    return bResult;
}

// From V0.7 by Dan_Goon

BOOL PRIVATE SetupClient(HMODULE hModule, LPCSTR szModule)
{
	char tbf[256];

	BIND_TO_SERVER(GetServerInfo);			// This is available from V0.59

	// We need at least 0.59
	if(!server->GetServerInfo)
	{
		sprintf_s(tbf, "[%s] : Failed to bind server functions!\n\nPlease use D2HackIt! V0.59 or later.", szModule);
		MessageBox(NULL, tbf, szLodErr, MB_OK);
		return FALSE;
	}

	g_pSI = server->GetServerInfo(__SERVERVERSION__, szModule);

	BIND_TO_SERVER(GamePrintString);
	BIND_AND_CHECK(GamePrintInfo);
	BIND_AND_CHECK(GamePrintError);
	BIND_AND_CHECK(GamePrintVerbose);
	BIND_AND_CHECK(GamePrintString);
	BIND_AND_CHECK(GameCommandLine);
	BIND_AND_CHECK(GameSendPacketToServer);
	BIND_AND_CHECK(GameSendPacketToGame);
	BIND_AND_CHECK(GetFingerprint);
	BIND_AND_CHECK(Intercept);
	BIND_AND_CHECK(GetThisgameStruct);
//	BIND_AND_CHECK(GetHackProfileString);		// This will be obsoleted. Don't use this after 0.7!
//	BIND_AND_CHECK(GetHackProfileStringEx);		// This will be obsoleted. Don't use this after 0.7!
//	BIND_AND_CHECK(GetHackProfileSectionNames);	// This will be obsoleted. Don't use this after 0.7!
	BIND_AND_CHECK(SetHackProfileString);

	BIND_AND_CHECK(GameSendPacketToGame);
	BIND_AND_CHECK(GameInsertPacketToGame);		// 0.59
	BIND_AND_CHECK(GameSendMessageToChat);		// 0.59
	BIND_AND_CHECK(D2FindFile);
//	BIND_AND_CHECK(FreeHackProfileString);		// 0.62 This will be obsoleted. Don't use this after 0.7!
	BIND_AND_CHECK(GameSaveAndExit);			// 0.62

	BIND_AND_CHECK(GetIniFileName);				// 0.71
	BIND_AND_CHECK(GetIniString);				// 0.71
	BIND_AND_CHECK(GetIniSectionNames);			// 0.71

	BIND_AND_CHECK(FindMyD2WndHandle);			// 0.73
	BIND_AND_CHECK(HookD2WndProc);				// 0.80
	BIND_AND_CHECK(CleanMeUp);					// 0.80

	BIND_AND_CHECK(DebugTest);
	
	BIND_AND_CHECK(GetModuleDirectory);			// 2.00 (Abin)
	BIND_AND_CHECK(IsClientLoaded);				// 2.00 (Abin)
	BIND_AND_CHECK(GameInfof);					// 2.00 (Abin)
	BIND_AND_CHECK(GameErrorf);					// 2.00 (Abin)
	BIND_AND_CHECK(GameStringf);				// 2.00 (Abin)
	BIND_AND_CHECK(GameCommandf);				// 2.00 (Abin)
	BIND_AND_CHECK(GamePrintPacket);			// 2.00 (Abin)
	BIND_AND_CHECK(IsGameReady);				// 2.00 (Abin)
	BIND_AND_CHECK(WasD2HackitLoadedBeforeJoinGame); // 2.00 (Abin)
	BIND_AND_CHECK(EnumUnits);					// 2.00 (Abin)
	BIND_AND_CHECK(VerifyUnit);					// 2.00 (Abin)
	BIND_AND_CHECK(FindUnitByClassID);			// 2.00 (Abin)
	BIND_AND_CHECK(FindUnitByName);				// 2.00 (Abin)
	BIND_AND_CHECK(GetUnitPosition);			// 2.00 (Abin)
	BIND_AND_CHECK(GetUnitName);				// 2.00 (Abin)
	BIND_AND_CHECK(GetUnitOwnerID);				// 2.00 (Abin)
	BIND_AND_CHECK(GetUnitClassID);				// 2.00 (Abin)
	BIND_AND_CHECK(GetUnitHPPercent);			// 2.00 (Abin)
	BIND_AND_CHECK(GetUnitStat);				// 2.00 (Abin)
	BIND_AND_CHECK(GetUnitAffection);			// 2.00 (Abin)
	BIND_AND_CHECK(GetMonsterType);				// 2.00 (Abin)
	BIND_AND_CHECK(GetBossEnchants);			// 2.00 (Abin)
	BIND_AND_CHECK(GetDistance);				// 2.00 (Abin)
	BIND_AND_CHECK(IsMercUnit);					// 2.00 (Abin)
	BIND_AND_CHECK(GetPing);					// 2.00 (Abin)
	BIND_AND_CHECK(GetAffectionName);			// 2.00 (Abin)
	BIND_AND_CHECK(AffectionIsAura);			// 2.00 (Abin)
	BIND_AND_CHECK(GetMapName);					// 2.00 (Abin)
	BIND_AND_CHECK(MapIsTown);					// 2.00 (Abin)
	BIND_AND_CHECK(GetTownMap);					// 2.00 (Abin)
	BIND_AND_CHECK(GetActFromMap);				// 2.00 (Abin)
	BIND_AND_CHECK(ParseItem);					// 2.00 (Abin)
	BIND_AND_CHECK(GetItemSize);				// 2.00 (Abin)
	BIND_AND_CHECK(SendKey);					// 2.00 (Abin)
	BIND_AND_CHECK(GetSpellName);				// 2.00 (Abin)
	BIND_AND_CHECK(GetGameDifficulty);			// 2.00 (Abin)	
	BIND_AND_CHECK(GetD2WindowTitle);			// 2.00 (Abin)
	BIND_AND_CHECK(GetInGameTime);				// 2.00 (Abin)
	BIND_AND_CHECK(GetRealmName);				// 2.00 (Abin)
	BIND_AND_CHECK(GetGameName);				// 2.00 (Abin)
	BIND_AND_CHECK(GetGamePassword);			// 2.00 (Abin)
	BIND_AND_CHECK(GetGameIP);					// 2.00 (Abin)
	BIND_AND_CHECK(EnumPresetUnits);			// 2.00 (Abin)
	BIND_AND_CHECK(CalculatePath);				// 2.00 (Abin)
	BIND_AND_CHECK(GetItemCode);				// 2.00 (Abin)
	BIND_AND_CHECK(GetItemCodeEx);				// 2.00 (Abin)
	BIND_AND_CHECK(D2GetCurrentRoomNum);		// 2.00 (Abin)
	BIND_AND_CHECK(D2GetAllRoomCoords);		// 2.00 (Abin)
	BIND_AND_CHECK(D2GetRoomCoords);			// 2.00 (Abin)
	BIND_AND_CHECK(LeftClick);					// 2.00 (Abin)
	BIND_AND_CHECK(RightClick);					// 2.00 (Abin)
	BIND_AND_CHECK(QueryModuleData);			// 2.00 (Abin)
	BIND_AND_CHECK(GetKeyName);					// 2.00 (Abin)

	// ++ B
	BIND_AND_CHECK(GetItemQuality);
	BIND_AND_CHECK(LeftClickHere);
	BIND_AND_CHECK(RightClickHere);	
	BIND_AND_CHECK(GetCommandCharacter);
	BIND_AND_CHECK(GetTickRate);

	BIND_AND_CHECK(IsStackable);
	BIND_AND_CHECK(IsWeapon);
	BIND_AND_CHECK(IsArmor);
	BIND_AND_CHECK(IsPotion);
	BIND_AND_CHECK(GetItemName);
	BIND_AND_CHECK(GetItemSetName);
	BIND_AND_CHECK(GetItemBeltRows);

	BIND_TO_PLAYER(RemoveFromStorage);
	BIND_TO_PLAYER(DumpStorage);
	BIND_TO_PLAYER(GetCubeID);
	BIND_TO_PLAYER(OpenCube);
	// -- B

	// Bind player functions
	BIND_TO_PLAYER(HaveSpell);					// 2.00 (Abin)
	BIND_TO_PLAYER(IsInTown);					// 2.00 (Abin)
	BIND_TO_PLAYER(GetMyMercUnit);				// 2.00 (Abin)
	BIND_TO_PLAYER(GetID);						// 2.00 (Abin)
	BIND_TO_PLAYER(GetName);					// 2.00 (Abin)
	BIND_TO_PLAYER(GetMode);					// 2.00 (Abin)
	BIND_TO_PLAYER(GetSelectedSpell);			// 2.00 (Abin)
	BIND_TO_PLAYER(GetHPPercent);				// 2.00 (Abin)
	BIND_TO_PLAYER(GetManaPercent);				// 2.00 (Abin)
	BIND_TO_PLAYER(GetItemDurabilityPercent);	// 2.00 (Abin)
	BIND_TO_PLAYER(GetStat);					// 2.00 (Abin)
	BIND_TO_PLAYER(GetAffection);				// 2.00 (Abin)
	BIND_TO_PLAYER(SelectSpell);				// 2.00 (Abin)
	BIND_TO_PLAYER(GetPosition);				// 2.00 (Abin)
	BIND_TO_PLAYER(GetMyCorpseCount);			// 2.00 (Abin)
	BIND_TO_PLAYER(PickMyCorpse);				// 2.00 (Abin)
	BIND_TO_PLAYER(GetHostilerCount);			// 2.00 (Abin)
	BIND_TO_PLAYER(GetCurrentMapID);			// 2.00 (Abin)
	BIND_TO_PLAYER(GetDistanceFrom);			// 2.00 (Abin)
	BIND_TO_PLAYER(HealMe);						// 2.00 (Abin)
	BIND_TO_PLAYER(HealMerc);					// 2.00 (Abin)
	BIND_TO_PLAYER(SupplyMana);					// 2.00 (Abin)
	BIND_TO_PLAYER(DrinkBeltPotion);			// 2.00 (Abin)
	BIND_TO_PLAYER(DrinkInventoryPotion);		// 2.00 (Abin)
	BIND_TO_PLAYER(IsBeltFull);					// 2.00 (Abin)
	BIND_TO_PLAYER(GetBeltRows);				// 2.00 (Abin)
	BIND_TO_PLAYER(GetBeltItem);				// 2.00 (Abin)
	BIND_TO_PLAYER(CastNoTarget);				// 2.00 (Abin)
	BIND_TO_PLAYER(CastOnMap);					// 2.00 (Abin)
	BIND_TO_PLAYER(CastOnUnit);					// 2.00 (Abin)
	BIND_TO_PLAYER(LeaveGame);					// 2.00 (Abin)
	BIND_TO_PLAYER(PauseJobs);					// 2.00 (Abin)
	BIND_TO_PLAYER(CleanJobs);					// 2.00 (Abin)
	BIND_TO_PLAYER(BackToTown);					// 2.00 (Abin)
	BIND_TO_PLAYER(MoveTo);						// 2.00 (Abin)
	BIND_TO_PLAYER(MoveToUnit);					// 2.00 (Abin)
	BIND_TO_PLAYER(TeleportTo);					// 2.00 (Abin)
	BIND_TO_PLAYER(Say);						// 2.00 (Abin)
	BIND_TO_PLAYER(OverHead);					// 2.00 (Abin)
	BIND_TO_PLAYER(SendSound);					// 2.00 (Abin)
	BIND_TO_PLAYER(AntiIdle);					// 2.00 (Abin)
	BIND_TO_PLAYER(WeaponSwitch);				// 2.00 (Abin)
	BIND_TO_PLAYER(EnterWaypoint);				// 2.00 (Abin)
	BIND_TO_PLAYER(Interact);					// 2.00 (Abin)
	BIND_TO_PLAYER(PickGroundItem);				// 2.00 (Abin)
	BIND_TO_PLAYER(ResurrectMerc);				// 2.00 (Abin)
	BIND_TO_PLAYER(BuyItem);					// 2.00 (Abin)
	BIND_TO_PLAYER(RepairItem);					// 2.00 (Abin)
	BIND_TO_PLAYER(IdentifyItems);				// 2.00 (Abin)
	BIND_TO_PLAYER(DepositGoldToStash);			// 2.00 (Abin)
	BIND_TO_PLAYER(RedrawClient);				// 2.00 (Abin)
	BIND_TO_PLAYER(GetStashGoldLimit);			// 2.00 (Abin)
	BIND_TO_PLAYER(GetInventoryGoldLimit);		// 2.00 (Abin)
	BIND_TO_PLAYER(GetMaxDifficulty);			// 2.00 (Abin)
	BIND_TO_PLAYER(IsHardcore);					// 2.00 (Abin)
	BIND_TO_PLAYER(GetAlwaysRun);				// 2.00 (Abin)
	BIND_TO_PLAYER(GetCursorItem);				// 2.00 (Abin)
	BIND_TO_PLAYER(GetAccountName);				// 2.00 (Abin)
	BIND_TO_PLAYER(DropCursorItemToGround);		// 2.00 (Abin)
	BIND_TO_PLAYER(Transmute);					// 2.00 (Abin)
	BIND_TO_PLAYER(IsUIOpened);					// 2.00 (Abin)
	BIND_TO_PLAYER(EnumStorageItems);			// 2.00 (Abin)
	BIND_TO_PLAYER(PickStorageItemToCursor);	// 2.00 (Abin)
	BIND_TO_PLAYER(FindItemPosition);
	BIND_TO_PLAYER(FindFirstStorageSpace);		// 2.00 (Abin)
	BIND_TO_PLAYER(FindFirstStorageSpaceEx);
	BIND_TO_PLAYER(DropCursorItemToStorage);	// 2.00 (Abin)
	BIND_TO_PLAYER(DropCursorItemToStorageEx);	
	BIND_TO_PLAYER(DropItemToStorage);
	BIND_TO_PLAYER(DropItemToStorageEx);
	BIND_TO_PLAYER(CloseAllUIs);				// 2.00 (Abin)
	BIND_TO_PLAYER(GetOpenedUI);				// 2.00 (Abin)
	BIND_TO_PLAYER(BuyHealingPotion);			// 2.00 (Abin)
	BIND_TO_PLAYER(BuyManaPotion);				// 2.00 (Abin)
	BIND_TO_PLAYER(BuyTownScroll);				// 2.00 (Abin)
	BIND_TO_PLAYER(BuyIdentifyScroll);			// 2.00 (Abin)
	BIND_TO_PLAYER(BuyArrows);					// 2.00 (Abin)
	BIND_TO_PLAYER(BuyBolts);					// 2.00 (Abin)
	BIND_TO_PLAYER(SellItem);					// 2.00 (Abin)
	BIND_TO_PLAYER(StartNpcSession);			// 2.00 (Abin)
	BIND_TO_PLAYER(EndNpcSession);				// 2.00 (Abin)
	BIND_TO_PLAYER(DrinkAllInventoryPotions);	// 2.00 (Abin)
	BIND_TO_PLAYER(IsMercAlive);				// 2.00 (Abin)
	BIND_TO_PLAYER(DropGold);					// 2.00 (Abin)
	BIND_TO_PLAYER(Gamble);						// 2.00 (Abin)
	BIND_TO_PLAYER(GetSpellCharges);			// 2.00 (Abin)
	BIND_TO_PLAYER(GetUnidItemCount);			// 2.00 (Abin)
	BIND_TO_PLAYER(GetLowestEquipDurabilityPercent); // 2.00 (Abin)
	BIND_TO_PLAYER(GetEquipItem);				// 2.00 (Abin)

	// Bind screen functions
	BIND_TO_SCREEN(CreateScreenHook);			// 2.00 (Abin)
	BIND_TO_SCREEN(ReleaseScreenHook);			// 2.00 (Abin)
	BIND_TO_SCREEN(SendToFront);				// 2.00 (Abin)
	BIND_TO_SCREEN(SendToBack);					// 2.00 (Abin)
	BIND_TO_SCREEN(DrawSingleText);				// 2.00 (Abin)
	BIND_TO_SCREEN(DrawMultipleTexts);			// 2.00 (Abin)
	BIND_TO_SCREEN(GetTextSize);				// 2.00 (Abin)
	BIND_TO_SCREEN(GetScreenSize);				// 2.00 (Abin)
	BIND_TO_SCREEN(ScreenToMapCoords);			// 2.00 (Abin)
	BIND_TO_SCREEN(MapToScreenCoords);			// 2.00 (Abin)
	BIND_TO_SCREEN(GetMousePosition);			// 2.00 (Abin)
	BIND_TO_SCREEN(SetScrollTitle);				// 2.00 (Abin)
	BIND_TO_SCREEN(AddScrollLine);				// 2.00 (Abin)
	BIND_TO_SCREEN(ShowScroll);					// 2.00 (Abin)
	BIND_TO_SCREEN(ClearScroll);				// 2.00 (Abin)
	
	if(!g_pSI)
	{
		server->GamePrintError("D2HackIt Server Info not available!");
	}
	else
	{
		_snprintf_s(tbf, sizeof(tbf), sizeof(tbf)/sizeof(tbf[0]), "D2HackIt directory = %s", g_pSI->PluginDirectory);
		server->GamePrintVerbose(tbf);
	}

	return TRUE;
}


//======================================================================
//				EnumIniSectionNames (Helper Funtion)
//======================================================================
//
// This helper function illustrates how to utilize GetIniSectionNames call.
// This maybe useful for modules which handles long lines of ini file text.
// You may remove this function from your code, if you don't need this feature.
//
// Usage:
//
//	LPSTR pString = EnumIniSectionNames( "YourD2hName", ENUM_START );
//	while( pString && *pString )
//	{
//		// <<<== Do your job here with returned pString
//
//		pString = EnumIniSectionNames( "YourD2hName", ENUM_NEXT );
//	}
//	EnumIniSectionNames( "YourD2hName", ENUM_FINISH );
//
//======================================================================
LPSTR PRIVATE EnumIniSectionNames( LPCSTR lpHackName, int nEnumAction )
{
	if(!server->GetIniSectionNames) return NULL;

	switch(nEnumAction)
	{
	case ENUM_START:	// = Start Enum

		if(m_szLargeBuffer) delete [] m_szLargeBuffer;
		m_szLargeBuffer = NULL;

		// Get required buffer size
		m_nLargeBufferSize = server->GetIniSectionNames(lpHackName, NULL, 0);
		if(!m_nLargeBufferSize) break;

		m_szLargeBuffer = new char[m_nLargeBufferSize];
		if(!m_szLargeBuffer)
		{
			server->GamePrintError("Failed to allocate buffer");
			break;
		}

		server->GetIniSectionNames(lpHackName, m_szLargeBuffer, m_nLargeBufferSize);
		m_nSectNameIndex = 0;
		return m_szLargeBuffer;

	case ENUM_NEXT:		// = Enum Next

		if(!m_szLargeBuffer) break;

		m_nSectNameIndex += strlen(&m_szLargeBuffer[m_nSectNameIndex]) + 1;	// point to next z-string start

		if(m_nSectNameIndex >= m_nLargeBufferSize ||
			!m_szLargeBuffer[m_nSectNameIndex]) break;	// end of buffer reached
		
		return &m_szLargeBuffer[m_nSectNameIndex];

	case ENUM_FINISH:	// = Finish Enum

		if(m_szLargeBuffer) delete [] m_szLargeBuffer;
		m_szLargeBuffer = NULL;
		m_nSectNameIndex = 0;
		break;
	}

	return NULL;
}

//======================================================================
//				GetHackProfileString
//======================================================================
//
// This private function is for previous modules which used to call
//  server->GetHackProfileString, to keep its function-level consistency.
// Just remove your current 'server->' part.
//
// Notice : Never delete returned string pointer and do not save this pointer
//   for future use. The string context pointed by this pointer will be
//	 changed to new value whenever this function is called again.
//
//======================================================================
LPSTR PRIVATE GetHackProfileString( LPCSTR lpHackName, LPCSTR lpSectionName, LPCSTR lpKeyName)
{
	static char szLineBuffer[1024];		// this maybe enough for most of ini file lines

	if(!server->GetIniString ||
		!server->GetIniString(lpHackName, lpSectionName, lpKeyName, szLineBuffer, 1024))

		szLineBuffer[0] = 0;
	
	return szLineBuffer;
}


//======================================================================
//				D2WndProcHooker
//======================================================================
// Code below is for illustration of Diablo II Window Procedure Hooker.
//
//======================================================================
/*
BOOL CALLBACK MyHooker(LRESULT* pResult, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(uMsg >= WM_USER)
	{
		if(uMsg == WM_MY_SPECIFIC_COMMAND)
		{
			.
			.
			.

			*pResult = 0x1234;	// This is return value for message-sender
			return TRUE;		// "I processed this message"
		}
	}

	return FALSE;				// "I did not process this message"
}

BOOL EXPORT OnClientStart()
{
	server->HookD2WndProc(g_hModule, MyHooker);		// Setup Hook Link of 'MyHooker'
	.
	.
	.

	return TRUE;
}

BOOL EXPORT OnClientStop()
{
	.
	.
	.

	server->HookD2WndProc(g_hModule, NULL);			// Unhook me
	return TRUE;
}
*/

//////////////////////////////////////////////////////////////////////
// Stubfunctions for 'property get' functions.
//////////////////////////////////////////////////////////////////////
LPCSTR	EXPORT GetModuleAuthor()		{return ModuleAuthor;}
LPCSTR	EXPORT GetModuleWebsite()		{return ModuleWebsite;}
DWORD	EXPORT GetModuleVersion()		{return ModuleVersion;}
LPCSTR	EXPORT GetModuleEmail()			{return ModuleEmail;}
LPCSTR	EXPORT GetModuleDescription()	{return ModuleDescription;}

//////////////////////////////////////////////////////////////////////
// OnClientCommandLine
// -------------------------------------------------------------------
// The modules own extension of the command line interface. Any custom
// commands you add are parsed here.
//
// Return value should be TRUE, but it is not used at this 
// time.
//
// Arguments when we get here:
// argv[0]			Name of module
// argv[1]			Name of command (If supplied)
// argv[2 ... n]	The rest
//
// Syntax in the game: .<module> <arguments>
//////////////////////////////////////////////////////////////////////
BOOL EXPORT OnGameCommandLine(char* argv[], int argc)
{
	DGTRACE((DBGHDR "OnGameCommandLine : (%d) [%s]..\n", argc, argc > 0 ? argv[0] : "Mola"));

	// Check if user supplied anything at all, if not assume help...
	if (argc==1) argv[argc++] = "help";


	MODULECOMMANDSTRUCT* mcs = ModuleCommands;

	while (mcs->szName)
	{
		if (!_stricmp(mcs->szName, argv[1])) break;
		mcs++;
	}

	char *p, fMsg[256];
	char *pNext; 

	//
	// Is this a built-in function ?
	if (mcs->szName)
	{
		// If functions returns false, show usage help
		if (!mcs->pFunc(argv, argc))
		{
			server->GamePrintInfo("Usage:");

			p = strtok_s(mcs->szUsage, "\n", &pNext);
			while(p)
			{
				sprintf_s(fMsg, "ÿc4.%s %s", argv[0], p);
				server->GamePrintInfo(fMsg);

				p = strtok_s(NULL, "\n", &pNext);
			}
		}
	}
	else
	{
		// Unknown command, show catch-all help phraze.
		sprintf_s(fMsg, "Unknown command ÿc4'%s %s'ÿc0 - try ÿc4'.%s help'ÿc0 to get help.",
			argv[0], argv[1], argv[0]);
		server->GamePrintError(fMsg);
	}

	return TRUE;
}


//////////////////////////////////////////////////////////////////////
// OnClientCommandHelp
// -------------------------------------------------------------------
// Our default help function.
//
// Syntax in the game: .<module> <arguments>
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE OnGameCommandHelp(char** argv, int argc)
{
	// If command line is longer than 2, show usage for 'help'
	if (argc>2) return FALSE;

	char t[1024];
	sprintf_s(t, "Available commands for %s:", argv[0]);
	server->GamePrintInfo(t);

	// Loop through ModuleCommands[] and print their names
	for (int i=0; ModuleCommands[i].szName != NULL; i++)
	{
		sprintf_s(t, "ÿc4.%s %s", argv[0], ModuleCommands[i].szName);
		server->GamePrintInfo(t);
	}

	sprintf_s(t, "For help on a specific command, type ÿc4.%s <command> help", argv[0]);
	server->GamePrintInfo(t);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// assert()
// -------------------------------------------------------------------
// Thanks to ClckWcher for contributing assert code
//
// Use assert(expression) or assert(expression, "Message") in your 
// module.
//
//////////////////////////////////////////////////////////////////////
#define assert(exp) (void)( (exp) || (_assert(#exp, __FILE__, __LINE__), 0) )
#define msg_assert(exp,msg) (void)( (exp) || (_assert(msg, __FILE__, __LINE__), 0) )
void _assert(char * str,char * file,int line)
{
	char t[1024];
	sprintf_s(t, "Assertion failed: ÿc4%sÿc0, file ÿc4%sÿc0, line ÿc4%d", str, file, line);
	server->GamePrintError(t);
}
