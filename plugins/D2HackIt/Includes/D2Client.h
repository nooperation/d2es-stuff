#ifndef __D2CLIENT_H__
#define __D2CLIENT_H__

//#pragma warning( disable : 4530)	// Disable warnings for exceptions
#include "D2HackIt.h"

#ifdef D2HACKIT_DEBUG
#define	CHDR	g_szDebug, "D2CLIENT : "
#endif

// External Global Variables
extern FUNCTIONENTRYPOINTS*		server;
extern HMODULE					g_hModule;		// New Global
extern PSERVERINFO				g_pSI;			// 0.7
extern PLAYERENTRYPOINTS*		me;				// 2.00 (Abin)
extern SCREENENTRYPOINTS*		screen;			// 2.00 (Abin)

extern char ModuleWebsite[];
extern char ModuleAuthor[];
extern DWORD ModuleVersion;
extern char	ModuleDescription[];
extern char	ModuleEmail[];

// Exported functions
extern "C"
{
BOOL EXPORT OnClientStart();
BOOL EXPORT OnClientStop();
DWORD EXPORT OnGameTimerTick();
BOOL EXPORT OnGameCommandLine(char** argv, int argc);
DWORD EXPORT OnBnetPacketBeforeSent(BYTE* aPacket, DWORD aLen);
DWORD EXPORT OnBnetPacketBeforeReceived(BYTE* aPacket, DWORD aLen);
DWORD EXPORT OnRealmPacketBeforeSent(BYTE* aPacket, DWORD aLen);
DWORD EXPORT OnRealmPacketBeforeReceived(BYTE* aPacket, DWORD aLen);
DWORD EXPORT OnGamePacketBeforeSent(BYTE* aPacket, DWORD aLen);
DWORD EXPORT OnGamePacketBeforeReceived(BYTE* aPacket, DWORD aLen);
VOID EXPORT OnGamePacketAfterReceived(BYTE* aPacket, DWORD aLen);
LPCSTR EXPORT GetModuleAuthor();
LPCSTR EXPORT GetModuleWebsite();
LPCSTR EXPORT GetModuleEmail();
LPCSTR EXPORT GetModuleDescription();
DWORD EXPORT GetModuleVersion();
VOID EXPORT OnGameJoin(THISGAMESTRUCT* thisgame);
VOID EXPORT OnGameLeave(THISGAMESTRUCT* thisgame);

// Game message handlers
// Added since 2.00 by Abin
VOID EXPORT OnUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam, LPARAM lParam);
VOID EXPORT OnThisPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam);
DWORD EXPORT OnQueryModuleData(DWORD dwDataID, LPVOID lpBuffer, DWORD dwMaxLength, LPARAM lParam);
BYTE EXPORT OnGameKeyDown(BYTE iKeyCode);
}

// Private Global Functions
LPSTR PRIVATE EnumIniSectionNames( LPCSTR lpHackName, int nEnumAction );
enum
{
	ENUM_START = 0,
	ENUM_NEXT,
	ENUM_FINISH
};

// Commandline functions
typedef GAMECOMMANDSTRUCT	MODULECOMMANDSTRUCT;
extern MODULECOMMANDSTRUCT		ModuleCommands[];
BOOL PRIVATE OnGameCommandHelp(char** argv, int argc);

//
//
// Nice little enum for character classes
enum
{
	CharClassAmazon = 0,
	CharClassSorceress,
	CharClassNecromancer,
	CharClassPaladin,
	CharClassBarbarian,
	CharClassDruid,
	CharClassAssassin
};


// Helper stuff to make the code less messy :)
#define BIND_TO_SERVER(x)		server->##x## = (fn##x##)GetProcAddress(hModule,#x ##)
#define BIND_AND_CHECK(x)		if(!(BIND_TO_SERVER(x))) \
								{ \
									sprintf_s(tbf, "ÿc1[%s] Failed to bind : " #x ##, szModule); \
									server->GamePrintString(tbf); \
								}

#define BIND_TO_PLAYER(x)		if (!(me->##x## = (fn##x##)GetProcAddress(hModule,#x ##))) \
								{ \
									sprintf_s(tbf, "ÿc1[%s] Failed to bind : " #x ##, szModule); \
									server->GamePrintString(tbf); \
								}

#define BIND_TO_SCREEN(x)		if (!(screen->##x## = (fn##x##)GetProcAddress(hModule,#x ##))) \
								{ \
									sprintf_s(tbf, "ÿc1[%s] Failed to bind : " #x ##, szModule); \
									server->GamePrintString(tbf); \
								}


#define CLIENTINFO(_vMaj, _vMin, _Author, _url, _desc, _email)	\
	DWORD	ModuleVersion=		MAKELONG(_vMaj,_vMin);			\
	char	ModuleAuthor[]=		#_Author;						\
	char	ModuleWebsite[]=	#_url;							\
	char	ModuleDescription[]=#_desc;							\
	char	ModuleEmail[]=		#_email;

//#pragma warning( default : 4530)	// Enable warnings for exceptions
#endif // __D2CLIENT_H__
