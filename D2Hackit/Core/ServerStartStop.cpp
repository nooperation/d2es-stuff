//////////////////////////////////////////////////////////////////////
// ServerStartStop.cpp
// -------------------------------------------------------------------
// Initialize/destroy server.
//
// <thohell@home.se>
// <Dan_Goon@diabloworld.com>
//////////////////////////////////////////////////////////////////////
#include "D2HackItServer.h"
#include "Server20.h"
#include "criticalSections.h"

DWORD g_dwBannerHook = 0;
BOOL g_bDrawBanner = TRUE;

// These are the dll's we want to force-load to get them in memory.
//char* NeededDlls[] = { "D2Common.dll", "D2Game.dll", "D2Multi.dll", "D2Client.dll", NULL };
LPCSTR	NeededDlls[] = { "D2Common.dll", "D2Game.dll", "D2Client.dll", "D2Win.dll", "D2Gfx.dll", NULL }; // 2.00
LPCSTR	szErrTTl = "D2Hackit Error!";

// Added v2.00 by Abin, screen drawing initialization/cleanup and otherd
void DefineOffsets();
BOOL D2InitializeScreenHook();
BOOL D2UndrawAll();


//////////////////////////////////////////////////////////////////////
// ServerStart()
// -------------------------------------------------------------------
// Responsible for setting up the server.
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE ServerStart(HANDLE hModule)
{
	DGTRACE((DBGHDR "ServerStart!\n"));

	if(!g_hD2) return FALSE;

	DefineOffsets();

	char tbf[MAX_PATH];

	psi =	new PRIVATESERVERINFO;
	if(!psi)
	{
		MessageBox(NULL, "Unable to allocate buffer", szErrTTl, MB_ICONERROR);
		return FALSE;
	}
	ZeroMemory(psi, sizeof(PRIVATESERVERINFO));

	//////////////////////////////////////////////////////////////////
	// Build initial data of the PRIVATESERVERINFO structure
	//////////////////////////////////////////////////////////////////

   // Get the process ID and the process handle
	psi->pid = GetCurrentProcessId();
	GetCurrentProcess();
	psi->hProcess = OpenProcess(STANDARD_RIGHTS_READ | STANDARD_RIGHTS_WRITE, FALSE, psi->pid);

	if (!psi->hProcess)
	{
		MessageBox(NULL, "Can't get Diablo II's process handle", szErrTTl, MB_ICONERROR);
	}

	//////////////////////////////////////////////////////////////////
	// Before anything else, create the global structures we use in 
	// the hack. Make sure we delete these in ServerStop.
	//////////////////////////////////////////////////////////////////
	si =		new SERVERINFO;					
	pfep =		new PRIVATEFUNCTIONENTRYPOINTS; 
	thisgame =	new THISGAMESTRUCT;				

	if(!si || !pfep || !thisgame)
	{
		MessageBox(NULL, "Unable to allocate buffer", szErrTTl, MB_ICONERROR);
	}

	ZeroMemory(si, sizeof(SERVERINFO));
	ZeroMemory(pfep, sizeof(PRIVATEFUNCTIONENTRYPOINTS));
	ZeroMemory(thisgame, sizeof(THISGAMESTRUCT));

	//////////////////////////////////////////////////////////////////
	// Build initial data of the SERVERINFO structure
	//////////////////////////////////////////////////////////////////

	si->hD2Wnd = psi->hwnd = g_hD2;

	// Get plugin path
	if (!GetModuleFileName((HINSTANCE)hModule, tbf, MAX_PATH))
	{ 
		MessageBox(NULL, "Unable to get PluginPath!", szErrTTl, MB_ICONERROR);
		return FALSE;
	}
	
	int p = strlen(tbf);
	while (p) 
	{
		if (tbf[p] == '\\') { tbf[p] = 0; p = 0; }
		else p--;
	}

	si->PluginDirectory = new char[strlen(tbf)+1];
	strcpy(si->PluginDirectory, tbf);

	strcat(tbf, "\\D2HackIt.ini");
	psi->IniFile = new char[strlen(tbf)+1];
	strcpy(psi->IniFile, tbf);
	
	if( _access(psi->IniFile, 0))
	{ 
		tbf[strlen(tbf)-4] = 0;
		strcat(tbf, "_New.ini");
		
		if( _access(tbf, 0))
		{
			_snprintf(tbf, sizeof(tbf), "Unable to open INI file:\n[%s]:%d", psi->IniFile, errno);
			MessageBox(NULL, tbf, szErrTTl, MB_ICONERROR);
			return FALSE;
		}
		else
		{
			CopyFile(tbf, psi->IniFile, TRUE);
		}
	}

	si->Version = __SERVERVERSION__;
	strcpy(psi->BuildDate, __DATE__);
	strcpy(psi->BuildTime, __TIME__);

	psi->DontShowErrors = FALSE;


	//////////////////////////////////////////////////////////////////
	// Build initial callbacks in PRIVATEFUNCTIONENTRYPOINTS 
	//////////////////////////////////////////////////////////////////
	pfep->GetBaseAddress = &GetBaseAddress;
	pfep->GetImageSize = &GetImageSize;

	//////////////////////////////////////////////////////////////////
	// Set default prompts
	//////////////////////////////////////////////////////////////////
	if(GetIniString("D2HackIt", "Misc", "InfoPrompt", tbf, sizeof(tbf)))
		lstrcpyn(psi->InfoPrompt, tbf, MAXPROMPTLENGTH-1);
	else
		lstrcpyn(psi->InfoPrompt, DEFAULTINFOPROMPT, MAXPROMPTLENGTH-1);

	if(GetIniString("D2HackIt", "Misc", "ErrorPrompt", tbf, sizeof(tbf)))
		lstrcpyn(psi->ErrorPrompt, tbf, MAXPROMPTLENGTH-1);
	else
		lstrcpyn(psi->ErrorPrompt, DEFAULTERRORPROMPT, MAXPROMPTLENGTH-1);

	if(GetIniString("D2HackIt", "Misc", "VerbosePrompt", tbf, sizeof(tbf)))
		lstrcpyn(psi->VerbosePrompt, tbf, MAXPROMPTLENGTH-1);
	else
		lstrcpyn(psi->VerbosePrompt, DEFAULTVERBOSEPROMPT, MAXPROMPTLENGTH-1);
	
	if(GetIniString("D2HackIt", "Misc", "Verbose", tbf, sizeof(tbf)) && !_stricmp(tbf, "on"))
		psi->Verbose = TRUE;
	else
		psi->Verbose = FALSE;

	// ++ B
	if(GetIniString("D2HackIt", "Misc", "CommandCharacter", tbf, sizeof(tbf)))
		psi->CommandCharacter = tbf[0];
	else
		psi->CommandCharacter = '.';

	psi->TickRate = GetPrivateProfileInt("Misc", "TickRate", 100, psi->IniFile);
	psi->VendorItemTimeout = GetPrivateProfileInt("Vendor", "ItemTimeout", 150, psi->IniFile);
	psi->VendorMenuTimeout = GetPrivateProfileInt("Vendor", "MenuTimeout", 300, psi->IniFile);
	psi->VendorInteractTimeout = GetPrivateProfileInt("Vendor", "InteractTimeout", 100, psi->IniFile);
	psi->VendorWalkToTimeout = GetPrivateProfileInt("Vendor", "WalkToTimeout", 100, psi->IniFile);


	// -- B

	//////////////////////////////////////////////////////////////////
	// Force-load needed dll's so we can patch thei'r memory space.
	// We should unload these is ServerStop.
	//////////////////////////////////////////////////////////////////
	//for (int i=0; NeededDlls[i] != NULL; i++)
	//{
	//	if(!LoadLibrary(NeededDlls[i]))
	//	{
	//		DGTRACE((DBGHDR "LoadLibrary Failed [%s]!\n", NeededDlls[i]));
	//	}
	//}

	//////////////////////////////////////////////////////////////////
	// Start by binding a way to print to screen. This is vital, so if
	// we are unable to do this, exit with an error message!
	//////////////////////////////////////////////////////////////////
	FINGERPRINTSTRUCT fps;
	if(!GetFingerprint("D2HackIt", "GamePrintStringLocation", fps))
	{
		MessageBox(NULL, "Fingerprint information for 'GamePrintStringLocation'\nmissing or corrupt!",
			szErrTTl, MB_ICONERROR);
		return FALSE;
	}
	psi->GamePrintStringLocation = fps.AddressFound;

	if(!psi->GamePrintStringLocation)
	{ 
		MessageBox(NULL, "Unable to find entrypoint for 'GamePrintStringLocation'!", szErrTTl, MB_ICONERROR);
		return FALSE;
	}

	// Get playerinfo struct
	if(!GetFingerprint("D2HackIt", "pPlayerInfoStruct", fps))
		{ GamePrintError("Fatal error! Exiting!"); return FALSE; }

	// Messy pointers :)
	thisgame->player = (PLAYERINFOSTRUCT*)*(DWORD*)(*(DWORD*)fps.AddressFound);


	// Get gameinfo struct
	if(!GetFingerprint("D2HackIt", "pGameInfoStruct", fps)) 
		{ GamePrintError("Fatal error! Exiting!"); return FALSE; }
	thisgame->CurrentGame = (GAMESTRUCT*)*(DWORD*)(*(DWORD*)fps.AddressFound);
	
	// Get GameSendMessageToChat
	psi->GameSendMessageToChatLocation = NULL;
	if(GetFingerprint("D2HackIt", "GameSendMessageToChat", fps))
		psi->GameSendMessageToChatLocation = fps.AddressFound;

	// GameSaveAndExit
	psi->GameSaveAndExitLocation = NULL;
	if(GetFingerprint("D2HackIt", "GameSaveAndExit", fps))
		psi->GameSaveAndExitLocation = fps.AddressFound;

	// GameKeyDownIntercept
	psi->GameKeyDownIntercept = NULL;
	if(GetFingerprint("D2HackIt", "GameKeyDownIntercept", fps))
		psi->GameKeyDownIntercept = fps.AddressFound;
	
	// Initialize critical section
	InitializeCriticalSection(&psi->csData);
	InitializeCriticalSection(&csLoadingItems);


	// Initialize insert receive buffers
	psi->nRecvBufferPos = 0;
	
	//////////////////////////////////////////////////////////////////
	// Print startup banner. 
	//////////////////////////////////////////////////////////////////
	sprintf(tbf, "Starting ÿc4D2HackIt! ÿc2v%d.%.2d ÿc0(%s@%s)",
		LOWORD(si->Version), HIWORD(si->Version), psi->BuildDate, psi->BuildTime);
	GamePrintInfo(tbf);

	//////////////////////////////////////////////////////////////////
	// Get loader data
	//////////////////////////////////////////////////////////////////
	BOOL UsingD2Loader = FALSE;

	psi->DontShowErrors = TRUE;
	if(!GetFingerprint("D2HackIt", "LoaderStruct", fps))
	{
		psi->DontShowErrors = FALSE;
		strcpy(fps.ModuleName, "Diablo II.exe");
		UsingD2Loader = TRUE;
		if((fps.AddressFound = GetMemoryAddressFromPattern(fps.ModuleName, fps.FingerPrint, fps.Offset)) < 0x100)
		{
			sprintf(fps.ModuleName, "D2Loader.exe");
			if((fps.AddressFound = GetMemoryAddressFromPattern(fps.ModuleName, fps.FingerPrint, fps.Offset)) < 0x100)
			{
				//GamePrintError("Unable to find loader data in 'Game.exe', 'Diablo II.exe' or 'D2Loader.exe'!");
				//GamePrintError("Fatal error! Exiting!"); return FALSE; 
				fps.AddressFound=0;
			}	
		}
	}
	
	psi->loader = (LOADERDATA*)fps.AddressFound;

	if(psi->loader) 
	{
		DGTRACE((DBGHDR "D2Loader Found!\n"));
	
		//sprintf(tbf, "Loader version ÿc4%d.%.2d. ÿc0Game is %sstarted with D2Loader.",
		//	LOWORD(psi->loader->LoaderVersion), HIWORD(psi->loader->LoaderVersion),
		//	(UsingD2Loader?"":"ÿc4not ÿc0"));
	}
	else
	{
		DGTRACE((DBGHDR "D2Loader NOT Found!\n"));

		//sprintf(tbf, "D2Hackit was loaded without loader");
	}
	//GamePrintInfo(tbf);

		
	//////////////////////////////////////////////////////////////////
	// Continue binding entrypoints and intercepts
	//////////////////////////////////////////////////////////////////
	DGTRACE((DBGHDR "Found 'GamePrintStringLocation' at %.8x\n", psi->GamePrintStringLocation));

	sprintf(tbf, "Found 'GamePrintStringLocation' at %.8x", psi->GamePrintStringLocation);
	GamePrintVerbose(tbf);

	psi->bSupportBnet = TRUE;

	/* 
	 * No need for this
	 *
	// Get socket location
	if (!GetFingerprint("D2HackIt", "pGameSocketLocation", fps)) 
		{ GamePrintError("Fatal error! Exiting!"); return FALSE; }
	psi->GameSocketLocation=*(DWORD*)fps.AddressFound;
	*/

	// Get GameSendPacketToGameLocation
	// Thanks to TechWarrior
	if(!GetFingerprint("D2HackIt", "GameSendPacketToGameLocation", fps)) 
	{ 
		DGTRACE((DBGHDR "GameSendPacketToGameLocation NOT Found!\n"));
		GamePrintError("Fatal error! Exiting!"); 
		return FALSE; 
	}
	psi->GameSendPacketToGameLocation = fps.AddressFound;

	psi->DontShowErrors = TRUE;
	if(!GetFingerprint("D2HackIt", "BnetConnStruct", fps) ||
		!(psi->BnetConnectionStructLocation=*(DWORD**)fps.AddressFound))
		psi->bSupportBnet = FALSE;
	psi->DontShowErrors = FALSE;

	if (psi->bSupportBnet &&
		(!GetFingerprint("D2HackIt", "RealmConnStruct", fps) || !(psi->RealmConnectionStructLocation=*(DWORD**)fps.AddressFound)))
		psi->bSupportBnet = FALSE;

	if (psi->bSupportBnet &&
		!GetFingerprint("D2HackIt", "BnetRecvSave", psi->fps.BnetPacketReceivedSave))
		psi->bSupportBnet = FALSE;

	if (psi->bSupportBnet &&
		!GetFingerprint("D2HackIt", "BnetRecv", psi->fps.BnetPacketReceivedIntercept))
		psi->bSupportBnet = FALSE;

	if (psi->bSupportBnet &&
		!GetFingerprint("D2HackIt", "BnetSent", psi->fps.BnetPacketSentIntercept))
		psi->bSupportBnet = FALSE;

	//
	// From now on, will check for all crucial FingerPrint locations first.
	// And then will do actual memory intercepts if all passed ok.
	//

	// Get GamePacketReceivedIntercept
	if (!GetFingerprint("D2HackIt", "GamePacketReceivedIntercept", psi->fps.GamePacketReceivedIntercept)) 
	{
//		szErr = "GamePacketReceivedIntercept";
		DGTRACE((DBGHDR "GamePacketReceivedIntercept NOT Found!\n"));
err_quit:
		GamePrintError("Fatal error! Exiting!");
		MessageBox(NULL, "Unable to find some crucial fingerprint definition in D2HackIt.ini!", szErrTTl, MB_ICONERROR);
		return FALSE;
	}

	// Get GamePacketReceivedIntercept2
	if (!GetFingerprint("D2HackIt", "GamePacketReceivedIntercept2", psi->fps.GamePacketReceivedIntercept2)) 
	{ 
		DGTRACE((DBGHDR "GamePacketReceivedIntercept2 NOT Found!\n"));
		goto err_quit;
	}

	// Get GamePacketSentIntercept
	if (!GetFingerprint("D2HackIt", "GamePacketSentIntercept", psi->fps.GamePacketSentIntercept)) 
	{ 
		DGTRACE((DBGHDR "GamePacketSentIntercept NOT Found!\n"));
		goto err_quit;
	}

	// Get GamePlayerInfoIntercept
	if (!GetFingerprint("D2HackIt", "GamePlayerInfoIntercept", psi->fps.GamePlayerInfoIntercept)) 
	{ 
		DGTRACE((DBGHDR "GamePlayerInfoIntercept NOT Found!\n"));
		goto err_quit;
	}

	// Get AntiDetectionIntercepts
	if (!GetFingerprint("D2HackIt", "AntiDetectPacket64Intercept", psi->fps.AntiDetectPacket64Intercept)) 
	{ 
		DGTRACE((DBGHDR "AntiDetectPacket64Intercept NOT Found!\n"));
		goto err_quit;
	}
	if (!GetFingerprint("D2HackIt", "AntiDetectPacket65Intercept", psi->fps.AntiDetectPacket65Intercept)) 
	{ 
		DGTRACE((DBGHDR "AntiDetectPacket65Intercept NOT Found!\n"));
		goto err_quit;
	}
	if (!GetFingerprint("D2HackIt", "AntiDetectVersionCheckingDllIntercept", psi->fps.AntiDetectVersionCheckingDllIntercept)) 
	{ 
		DGTRACE((DBGHDR "AntiDetectVersionCheckingDllIntercept NOT Found!\n"));
		goto err_quit;
	}
	if (!GetFingerprint("D2HackIt", "AntiDetectExtraworkDllIntercept", psi->fps.AntiDetectExtraworkDllIntercept)) 
	{ 
		DGTRACE((DBGHDR "AntiDetectExtraworkDllIntercept NOT Found!\n"));
		goto err_quit;
	}

	if (!GetFingerprint("D2HackIt", "GameKeyDownIntercept", psi->fps.GameKeyDownIntercept)) 
	{ 
		DGTRACE((DBGHDR "GameKeyDownIntercept NOT Found!\n"));
		goto err_quit;
	}

	// Get GameSendPacketToGameLocation
	// Thanks to TechWarrior
	if (!GetFingerprint("D2HackIt", "GameSendPacketToGameLocation", fps)) 
	{ 
		DGTRACE((DBGHDR "GameSendPacketToGameLocation NOT Found!\n"));
		goto err_quit;
	}
	psi->GameSendPacketToGameLocation = fps.AddressFound;

	Intercept(INST_CALL,
		psi->fps.GamePacketReceivedIntercept.AddressFound,
		(DWORD)GetFuncAddress(&GamePacketReceivedInterceptSTUB),
		psi->fps.GamePacketReceivedIntercept.PatchSize);
	Intercept(INST_JMP,
		psi->fps.GamePacketReceivedIntercept2.AddressFound,
		(DWORD)GetFuncAddress(&GamePacketReceivedIntercept2STUB),
		psi->fps.GamePacketReceivedIntercept2.PatchSize);
	Intercept(INST_CALL,
		psi->fps.GamePacketSentIntercept.AddressFound,
		(DWORD)GetFuncAddress(&GamePacketSentInterceptSTUB),
		psi->fps.GamePacketSentIntercept.PatchSize);
	Intercept(INST_CALL,
		psi->fps.GamePlayerInfoIntercept.AddressFound,
		(DWORD)GetFuncAddress(&GamePlayerInfoInterceptSTUB),
		psi->fps.GamePlayerInfoIntercept.PatchSize);

	Intercept(INST_CALL,
		psi->fps.AntiDetectPacket64Intercept.AddressFound,
		(DWORD)GetFuncAddress(&AntiDetectionPacket64STUB),
		psi->fps.AntiDetectPacket64Intercept.PatchSize);
	Intercept(INST_CALL,
		psi->fps.AntiDetectPacket65Intercept.AddressFound,
		(DWORD)GetFuncAddress(&AntiDetectionPacket65STUB),
		psi->fps.AntiDetectPacket65Intercept.PatchSize);
	Intercept(INST_CALL,
		psi->fps.AntiDetectVersionCheckingDllIntercept.AddressFound,
		(DWORD)GetFuncAddress(&AntiDetectionVersionCheckingDllSTUB),
		psi->fps.AntiDetectVersionCheckingDllIntercept.PatchSize);
	Intercept(INST_CALL,
		psi->fps.AntiDetectExtraworkDllIntercept.AddressFound,
		(DWORD)GetFuncAddress(&AntiDetectionExtraWorkDllSTUB),
		psi->fps.AntiDetectExtraworkDllIntercept.PatchSize);

	Intercept(INST_CALL,
		psi->fps.GameKeyDownIntercept.AddressFound,
		(DWORD)GetFuncAddress(&GameKeyDownInterceptSTUB),
		psi->fps.GameKeyDownIntercept.PatchSize);
	
	if(psi->bSupportBnet)
	{
		Intercept(INST_JMP,
			psi->fps.BnetPacketReceivedSave.AddressFound,
			(DWORD)GetFuncAddress(&BnetPacketReceivedSaveSTUB),
			psi->fps.BnetPacketReceivedSave.PatchSize);
		Intercept(INST_CALL,
			psi->fps.BnetPacketReceivedIntercept.AddressFound,
			(DWORD)GetFuncAddress(&BnetPacketReceivedInterceptSTUB),
			psi->fps.BnetPacketReceivedIntercept.PatchSize);
		Intercept(INST_JMP,
			psi->fps.BnetPacketSentIntercept.AddressFound,
			(DWORD)GetFuncAddress(&BnetPacketSentInterceptSTUB),
			psi->fps.BnetPacketSentIntercept.PatchSize);
	}

	// Start TickThread, We dont care about closing it later.
	// It will be destroyed when unloading the dll.
	DWORD dummy=0;
	psi->TickShutDown = 0;
	psi->TickThreadHandle = CreateThread(NULL, 0, TickThread, (void*)&ClientModules, 0, &dummy);
	psi->TickThreadActive = psi->TickThreadHandle!=NULL;

	// Subclass Diablo II Window before loading modules
	HookHackitWndProc(g_hD2, TRUE);

	// Now draw some banner like what d2jsp does, v2.00 by Abin
	if (!D2InitializeScreenHook())
		GamePrintError("D2InitializeScreenHook Failed!");

	tbf[0] = 0;
	GetIniString("D2HackIt", "Misc", "Banner", tbf, sizeof(tbf));
	g_bDrawBanner = !tbf[0] || _stricmp(tbf, "0");
	
	if (thisgame->player)
		Proc_OnGameJoin(thisgame);

	// Load any clients listed in Autorun
	if(GetIniString("D2HackIt", "Misc", "Autoload", tbf, sizeof(tbf)))
	{
		LoadGroupList(tbf, NULL);
	}
	else
	{
		DGTRACE((DBGHDR "No AutoLoads!\n"));
	}

	GamePrintInfo("D2HackIt! Loaded! Type ÿc4.helpÿc0 for help on commands.");

	DGTRACE((DBGHDR "Returning from ServerStart Properly!\n"));
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// ServerStop()
// -------------------------------------------------------------------
// Responsible for stopping the server.
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE ServerStop(void)
{
	DGTRACE((DBGHDR "ServerStop()\n"));

	if(!psi)
	{
		DGTRACE((DBGHDR "Server never started...\n"));
		return FALSE;
	}

	D2UndrawAll();
	Proc_OnGameLeave();

	//int i;
	if(psi->TickThreadActive)
	{
		psi->TickShutDown = 25;
		psi->TickThreadActive = FALSE;
		// Give TickThread 2.5 seconds max to clean up before killing it
		while(--psi->TickShutDown > 0) Sleep(psi->TickRate);

		if(psi->TickShutDown == 0)
		{
			// Force Terminate
			TerminateThread(psi->TickThreadHandle, 1234);
		}

		CloseHandle(psi->TickThreadHandle);
	}


	// Unload any loaded clients
	LinkedItem *li = ClientModules.GetLastItem();
	while(li)
	{
		LinkedItem *pi = ClientModules.GetPrevItem(li);
		UnloadClientModule(li);
		li = pi;
	}

	DGTRACE((DBGHDR "Client Modules have been unloaded\n"));

	// Delete critical section
	DeleteCriticalSection(&psi->csData);

	if(psi->ModuleGroup) delete psi->ModuleGroup;
	psi->ModuleGroup = NULL;

	if(psi->ModuleGroupName) delete [] psi->ModuleGroupName;
	psi->ModuleGroupName = NULL;

	// Unhook Diablo II Window Proc
	HookHackitWndProc(g_hD2, FALSE);

	// Un-patch intercept locations
	Intercept(INST_CALL,
		(DWORD)GetFuncAddress(&GamePlayerInfoInterceptSTUB),
		psi->fps.GamePlayerInfoIntercept.AddressFound,
		psi->fps.GamePlayerInfoIntercept.PatchSize);
	Intercept(INST_CALL,
		(DWORD)GetFuncAddress(&GamePacketSentInterceptSTUB),
		psi->fps.GamePacketSentIntercept.AddressFound,
		psi->fps.GamePacketSentIntercept.PatchSize);
	Intercept(INST_JMP,
		(DWORD)GetFuncAddress(&GamePacketReceivedIntercept2STUB),
		psi->fps.GamePacketReceivedIntercept2.AddressFound,
		psi->fps.GamePacketReceivedIntercept2.PatchSize);
	Intercept(INST_CALL,
		(DWORD)GetFuncAddress(&GamePacketReceivedInterceptSTUB),
		psi->fps.GamePacketReceivedIntercept.AddressFound,
		psi->fps.GamePacketReceivedIntercept.PatchSize);

	Intercept(INST_CALL,
		(DWORD)GetFuncAddress(&AntiDetectionPacket64STUB),
		psi->fps.AntiDetectPacket64Intercept.AddressFound,
		psi->fps.AntiDetectPacket64Intercept.PatchSize);
	Intercept(INST_CALL,
		(DWORD)GetFuncAddress(&AntiDetectionPacket65STUB),
		psi->fps.AntiDetectPacket65Intercept.AddressFound,
		psi->fps.AntiDetectPacket65Intercept.PatchSize);
	Intercept(INST_CALL,
		(DWORD)GetFuncAddress(&AntiDetectionVersionCheckingDllSTUB),
		psi->fps.AntiDetectVersionCheckingDllIntercept.AddressFound,
		psi->fps.AntiDetectVersionCheckingDllIntercept.PatchSize);
	Intercept(INST_CALL,
		(DWORD)GetFuncAddress(&AntiDetectionExtraWorkDllSTUB),
		psi->fps.AntiDetectExtraworkDllIntercept.AddressFound,
		psi->fps.AntiDetectExtraworkDllIntercept.PatchSize);

	Intercept(INST_CALL,		
		(DWORD)GetFuncAddress(&GameKeyDownInterceptSTUB),
		psi->fps.GameKeyDownIntercept.AddressFound,
		psi->fps.GameKeyDownIntercept.PatchSize);
	
	if(psi->bSupportBnet)
	{
		Intercept(INST_CALL,
			(DWORD)GetFuncAddress(&BnetPacketReceivedInterceptSTUB),
			psi->fps.BnetPacketReceivedIntercept.AddressFound,
			psi->fps.BnetPacketReceivedIntercept.PatchSize);
		Intercept(INST_JMP,
			(DWORD)GetFuncAddress(&BnetPacketReceivedSaveSTUB),
			psi->fps.BnetPacketReceivedSave.AddressFound,
			psi->fps.BnetPacketReceivedSave.PatchSize);
		Intercept(INST_JMP,
			(DWORD)GetFuncAddress(&BnetPacketSentInterceptSTUB),
			psi->fps.BnetPacketSentIntercept.AddressFound,
			psi->fps.BnetPacketSentIntercept.PatchSize);
	}


	// Release dll's that we loaded upon entry.
	//for(i=0; NeededDlls[i] != NULL; i++) FreeLibrary (GetModuleHandle(NeededDlls[i]));

	GamePrintInfo("D2HackIt! Unloaded.");

	if(thisgame) delete thisgame;
	thisgame = NULL;

	if(pfep) delete pfep;
	pfep = NULL;

	if(si)
	{
		if(si->PluginDirectory) delete [] si->PluginDirectory;
		delete si;
		si = NULL;
	}

	if(psi)
	{
		if(psi->IniFile) delete [] psi->IniFile;
		delete psi;
		psi = NULL;
	}

	DGTRACE((DBGHDR "ServerStop() return successfull\n"));
	return TRUE;
}

BOOL EXPORT IsClientLoaded(LPCSTR lpszClientName)
{
	return FindClientStructByName(lpszClientName) != NULL;
}