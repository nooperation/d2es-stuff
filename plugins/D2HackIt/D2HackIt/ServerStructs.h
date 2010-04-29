
class CClientGroup;

//////////////////////////////////////////////////////////////////////
// LOADERDATA *loader in psi;
// -------------------------------------------------------------------
// Contains information passed from the loader.
//////////////////////////////////////////////////////////////////////
typedef struct loaderdata_t
{
	DWORD	LoaderMagic;
	DWORD	LoaderVersion;
	DWORD	CodeLocation;
	HWND	hwnd;
	DWORD	pid;
	HANDLE	hProcess;
	DWORD	pLoadLibraryA;
	DWORD	pFreeLibrary;
	DWORD	pGetModuleHandleA;
	char	ModuleName[0x10];
	DWORD	WinProcLocation;
	DWORD	WinProcPatchsize;

} LOADERDATA;

//////////////////////////////////////////////////////////////////////
// PRIVATESERVERINFO *psi;
// -------------------------------------------------------------------
// Contains private information about the server.
//////////////////////////////////////////////////////////////////////
#define MAXINSERTRECVBUFFERS	20
#define MAXRECVBUFFERLEN		0x80

typedef struct privateserverinfo_t
{
	DWORD			pid;
	HWND			hwnd;
	HANDLE			hProcess;
	CRITICAL_SECTION csData;
	LOADERDATA*		loader;
	char			BuildDate[16];
	char			BuildTime[16];
	LPSTR			IniFile;
	char			ErrorPrompt[MAXPROMPTLENGTH];
	char			InfoPrompt[MAXPROMPTLENGTH];
	char			VerbosePrompt[MAXPROMPTLENGTH];
	BOOL			Verbose;

	// ++ B
	char			CommandCharacter;
	// -- B

	// Tick thread
	volatile int	TickShutDown;
	BOOL			TickThreadActive;
	HANDLE			TickThreadHandle;
	// Quick hack
	BOOL			DontShowErrors;
	// Insert receive buffers
	BYTE	InsertRecvBuffer[MAXINSERTRECVBUFFERS][MAXRECVBUFFERLEN];
	volatile DWORD	InsertRecvLen[MAXINSERTRECVBUFFERS];
	volatile DWORD	nRecvBufferPos;
	char*			ModuleGroupName;
	CClientGroup*	ModuleGroup;
	BOOL			GroupFuncDisabled;
	// Fingerprinted information
	DWORD			GameSocketLocation;
	DWORD			GamePrintStringLocation;
	DWORD			GameKeyDownIntercept;
	DWORD			pPlayerInfoStruct;
	DWORD			GameSendPacketToGameLocation;
	DWORD			GameSendMessageToChatLocation;
	DWORD			GameSaveAndExitLocation;
	BOOL			bSupportBnet;
	DWORD*			BnetConnectionStructLocation;
	DWORD*			RealmConnectionStructLocation;
	struct
	{
		FINGERPRINTSTRUCT	GamePacketReceivedIntercept;
		FINGERPRINTSTRUCT	GamePacketReceivedIntercept2;
		FINGERPRINTSTRUCT	GamePacketSentIntercept;
		FINGERPRINTSTRUCT	BnetPacketReceivedSave;
		FINGERPRINTSTRUCT	BnetPacketReceivedIntercept;
		FINGERPRINTSTRUCT	BnetPacketSentIntercept;
		FINGERPRINTSTRUCT	GamePlayerInfoIntercept;
		FINGERPRINTSTRUCT	AntiDetectPacket64Intercept;
		FINGERPRINTSTRUCT	AntiDetectPacket65Intercept;
		FINGERPRINTSTRUCT	AntiDetectVersionCheckingDllIntercept;
		FINGERPRINTSTRUCT	AntiDetectExtraworkDllIntercept;
		FINGERPRINTSTRUCT	GameKeyDownIntercept;
	} fps;

} PRIVATESERVERINFO;

//////////////////////////////////////////////////////////////////////
// PRIVATEFUNCTIONENTRYPOINTS *pfep;
// -------------------------------------------------------------------
// Contains information about entrypoints only used by the server.
//////////////////////////////////////////////////////////////////////
// Typedefs for toolhelp32
typedef BOOL	(WINAPI *fnModule32First)(HANDLE hSnapshot, LPMODULEENTRY32 lpme);
typedef BOOL	(WINAPI *fnModule32Next)(HANDLE hSnapshot, LPMODULEENTRY32 lpme);
typedef HANDLE	(WINAPI *fnCreateToolhelp32Snapshot)(DWORD dwFlags, DWORD th32ProcessID);  
// Typedefs for psapi
typedef struct _MODULEINFO {LPVOID lpBaseOfDll;DWORD SizeOfImage;LPVOID EntryPoint;} MODULEINFO, *LPMODULEINFO;
typedef BOOL	(*fnEnumProcessModules)(HANDLE hProcess, HMODULE * lphModule, DWORD cb, LPDWORD lpcbNeeded);
typedef DWORD	(*fnGetModuleBaseName)(HANDLE hProcess, HMODULE hModule, LPTSTR lpBaseName, DWORD nSize);
typedef BOOL	(*fnGetModuleInformation)(HANDLE hProcess,HMODULE hModule,LPMODULEINFO lpmodinfo,DWORD cb);

// Common typedefs
typedef DWORD	(PRIVATE *fnGetBaseAddress)(LPSTR ModuleName);
typedef DWORD	(PRIVATE *fnGetImageSize)(LPSTR ModuleName);

typedef struct privatefunctionentrypoints_t
{
	fnGetBaseAddress				GetBaseAddress;
	fnGetImageSize					GetImageSize;

	struct
	{
		fnCreateToolhelp32Snapshot		CreateToolhelp32Snapshot;
		fnModule32First					Module32First;
		fnModule32Next					Module32Next;
	
	} toolhelp;
	
	struct
	{
		fnEnumProcessModules			EnumProcessModules;
		fnGetModuleBaseName				GetModuleBaseName;
		fnGetModuleInformation			GetModuleInformation;
	
	} psapi;

} PRIVATEFUNCTIONENTRYPOINTS;


//////////////////////////////////////////////////////////////////////
// CLIENTINFOSTRUCT
// -------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////
typedef DWORD	(__cdecl *fnOnBnetPacketBeforeSent)(BYTE* aPacket, DWORD aLen);
typedef DWORD	(__cdecl *fnOnBnetPacketBeforeReceived)(BYTE* aPacket, DWORD aLen);
typedef DWORD	(__cdecl *fnOnRealmPacketBeforeSent)(BYTE* aPacket, DWORD aLen);
typedef DWORD	(__cdecl *fnOnRealmPacketBeforeReceived)(BYTE* aPacket, DWORD aLen);
typedef DWORD	(__cdecl *fnOnGamePacketBeforeSent)(BYTE* aPacket, DWORD aLen);
typedef DWORD	(__cdecl *fnOnGamePacketBeforeReceived)(BYTE* aPacket, DWORD aLen);
typedef	BOOL	(__cdecl *fnOnGameCommandLine)(char** argv, int argc);
typedef	VOID	(__cdecl *fnOnGameJoin)(THISGAMESTRUCT* thisgame);
typedef	VOID	(__cdecl *fnOnGameLeave)(THISGAMESTRUCT* thisgame);
typedef LPCSTR	(__cdecl *fnGetModuleAuthor)();
typedef	LPCSTR	(__cdecl *fnGetModuleWebsite)();
typedef LPCSTR	(__cdecl *fnGetModuleEmail)();
typedef LPCSTR	(__cdecl *fnGetModuleDescription)();
typedef	DWORD	(__cdecl *fnGetModuleVersion)();
typedef	DWORD	(__cdecl *fnOnGameTimerTick)();
typedef BOOL	(__cdecl *fnOnClientStart)();
typedef BOOL	(__cdecl *fnOnClientStop)();
typedef VOID	(__cdecl *fnOnUnitMessage)(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam, LPARAM lParam);
typedef VOID	(__cdecl *fnOnThisPlayerMessage)(UINT nMessage, WPARAM wParam, LPARAM lParam);
typedef DWORD	(__cdecl *fnOnQueryModuleData)(DWORD dwDataID, LPVOID lpBuffer, DWORD dwMaxLength, LPARAM lParam);
typedef BYTE (__cdecl *fnOnGameKeyDown)(BYTE iKeyCode);

typedef struct clientinfostruct_t
{
	HANDLE	hModule;			// Handle to DLL
	char*	Name;
	DWORD	dwVersion;			// Client Version !

	// Event handlers
	fnOnGamePacketBeforeSent		OnGamePacketBeforeSent;
	fnOnGamePacketBeforeReceived	OnGamePacketBeforeReceived;
	fnOnGameCommandLine				OnGameCommandLine;
	fnOnGameLeave					OnGameLeave;
	fnOnGameJoin					OnGameJoin;
	// Property get
	fnGetModuleAuthor				GetModuleAuthor;
	fnGetModuleWebsite				GetModuleWebsite;
	fnGetModuleEmail				GetModuleEmail;
	fnGetModuleDescription			GetModuleDescription;
	fnGetModuleVersion				GetModuleVersion;

	// Added v0.42
	fnOnGameTimerTick				OnGameTimerTick;

	// Added v0.44
	fnOnClientStart					OnClientStart;
	fnOnClientStop					OnClientStop;

	// Added v0.58
	char*	pDir;	// For GetHackProfileString

	// Added v0.62
	fnOnBnetPacketBeforeSent		OnBnetPacketBeforeSent;
	fnOnBnetPacketBeforeReceived	OnBnetPacketBeforeReceived;
	fnOnRealmPacketBeforeSent		OnRealmPacketBeforeSent;
	fnOnRealmPacketBeforeReceived	OnRealmPacketBeforeReceived;

	// Added v0.80
	PF_HookProc						ClientWndProc;

	// Added v2.00 by Abin
	fnOnUnitMessage					OnUnitMessage;
	fnOnThisPlayerMessage			OnThisPlayerMessage;
	fnOnQueryModuleData				OnQueryModuleData;
	fnOnGameKeyDown					OnGameKeyDown;

} CLIENTINFOSTRUCT, *PCIS;

#define BIND(x)		pCIS->##x##=(fn##x##)GetProcAddress((HMODULE)hModule,#x ##);

//#############################################################################
// LinkedList Derivatives
//#############################################################################
class CClients : public LinkedList
{
public:
	CClients() {};
	~CClients();

	void RemoveAll();
	PLinkedItem RemoveItem(PLinkedItem pItem, BOOL bUnlock = FALSE);
	PCIS GetCIS(PLinkedItem pItem, BOOL bLock = FALSE);
};

class CLoadedGroup : public LinkedList
{
public:
	CLoadedGroup() {};
	~CLoadedGroup();

	void RemoveAll();
	PLinkedItem RemoveItem(PLinkedItem pItem);

	void UnLoadAll();
};

class CClientGroup : public LinkedList
{
public:
	CClientGroup() {};
	~CClientGroup();

	CLoadedGroup	LoadedModules;

	PLinkedItem AddItem(LPSTR pszName);
	void LoadAll();
};

