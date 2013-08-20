#include <fstream>
#include <vector>
#include <set>
#include <algorithm>

#include "../../Includes/ClientCore.cpp"

enum States
{
	STATE_Idle,
	STATE_TeleportingToStairsRoom,
	STATE_TeleportingToStairsObject,
	STATE_UsingStairs
};

States currentState = STATE_Idle;

std::set<DWORD> stairRooms;
std::set<DWORD> levelWarpsDown;
std::set<DWORD> levelWarpsUp;

GAMEUNIT stairsGameUnit;
PATH path;
int pathIndex = 0;

BOOL PRIVATE Pos(char **argv, int argc)
{
		MAPPOS myPos = me->GetPosition();
	server->GameStringf("Player position %d, %d  room %d", myPos.x, myPos.y, server->D2GetCurrentRoomNum());
	return TRUE;
}
BOOL PRIVATE Start(char** argv, int argc)
{
	pathIndex = 0;
	memset(&path, 0, sizeof(PATH));
	memset(&stairsGameUnit, 0, sizeof(GAMEUNIT));
	currentState = STATE_Idle;

	MAPPOS stairRoomCoord;

	MAPPOS myPos = me->GetPosition();
	//server->GameStringf("Player position %d, %d  room %d", myPos.x, myPos.y, server->D2GetCurrentRoomNum());

	ROOMPOS allRoomCoords[64];
	DWORD numRooms = server->D2GetAllRoomCoords(allRoomCoords, ARRAYSIZE(allRoomCoords));

	if(numRooms == 0)
	{
		server->GameStringf("Failed to find all rooms");
		return TRUE;
	}

	bool foundStairsRoom = false;
	for(unsigned int i = 0; i < numRooms; ++i)
	{
		if(stairRooms.find(allRoomCoords[i].roomnum) != stairRooms.end())
		{
			foundStairsRoom = true;
			stairRoomCoord = allRoomCoords[i].pos;
			break;
		}
	}

	if(!foundStairsRoom)
	{
		server->GameStringf("No stairs found");
		return TRUE;
	}
	
	stairRoomCoord.x *= 5;
	stairRoomCoord.y *= 5;
	
	//server->GameStringf("target position %d, %d", stairRoomCoord.x, stairRoomCoord.y);
	
	BYTE steps;
	for (int i = 0; i < 3; i++)
	{
		steps = server->CalculatePath(stairRoomCoord.x + 5 + 3*i, stairRoomCoord.y + 5 + 3*i, &path, 5);
		if(steps != 0)
		{
			server->GameStringf("Found path after %d iterations", i);
			break;
		}
	}
	if(steps == 0)
	{
		server->GameStringf("Failed to find spot :(");
		return TRUE;
	}
	
	currentState = STATE_TeleportingToStairsRoom;

	//server->GameStringf("pathIndex = %d  nodeCount = %d  (%d, %d)", pathIndex, path.iNodeCount, path.aPathNodes[pathIndex].x, path.aPathNodes[pathIndex].y);
	

	me->CastOnMap(D2S_TELEPORT, path.aPathNodes[pathIndex].x, path.aPathNodes[pathIndex].y, false);
	++pathIndex;

	return TRUE;
}


////////////////////////////////////////////
//
//               EXPORTS
//
/////////////////////////////////////////////

DWORD EXPORT OnGameTimerTick()
{
	return 0;
}

VOID EXPORT OnGameJoin(THISGAMESTRUCT* thisgame)
{
	currentState = STATE_Idle;
}

VOID EXPORT OnGameLeave(THISGAMESTRUCT* thisgame)
{
}

BOOL EXPORT OnClientStart()
{
	stairRooms.insert(51); //Act 1 - Cave Entrance	
	stairRooms.insert(52); //Act 1 - DOE Entrance	

	stairRooms.insert(87); // Act 1 - Cave Next W
	stairRooms.insert(88); // Act 1 - Cave Next E
	stairRooms.insert(89); // Act 1 - Cave Next S
	stairRooms.insert(90); // Act 1 - Cave Next N
	stairRooms.insert(91); // Act 1 - Cave Down W
	stairRooms.insert(92); // Act 1 - Cave Down E
	stairRooms.insert(93); // Act 1 - Cave Down S
	stairRooms.insert(94); // Act 1 - Cave Down N
	stairRooms.insert(143); // Act 1 - Crypt Next W
	stairRooms.insert(144); // Act 1 - Crypt Next E
	stairRooms.insert(145); // Act 1 - Crypt Next S
	stairRooms.insert(146); // Act 1 - Crypt Next N
	stairRooms.insert(198); // Act 1 - Barracks Next W
	stairRooms.insert(199); // Act 1 - Barracks Next E
	stairRooms.insert(200); // Act 1 - Barracks Next S
	stairRooms.insert(201); // Act 1 - Barracks Next N
	stairRooms.insert(240); // Act 1 - Jail Next W
	stairRooms.insert(241); // Act 1 - Jail Next E
	stairRooms.insert(242); // Act 1 - Jail Next S
	stairRooms.insert(243); // Act 1 - Jail Next N
	stairRooms.insert(291); // Act 1 - Catacombs Next W
	stairRooms.insert(292); // Act 1 - Catacombs Next E
	stairRooms.insert(293); // Act 1 - Catacombs Next S
	stairRooms.insert(294); // Act 1 - Catacombs Next N
	stairRooms.insert(337); // Act 2 - Sewer Next W
	stairRooms.insert(338); // Act 2 - Sewer Next E
	stairRooms.insert(339); // Act 2 - Sewer Next S
	stairRooms.insert(340); // Act 2 - Sewer Next N
	stairRooms.insert(448); // Act 2 - Tomb Next W
	stairRooms.insert(449); // Act 2 - Tomb Next E
	stairRooms.insert(450); // Act 2 - Tomb Next S
	stairRooms.insert(451); // Act 2 - Tomb Next N
	stairRooms.insert(501); // Act 2 - Lair Next W
	stairRooms.insert(502); // Act 2 - Lair Next E
	stairRooms.insert(503); // Act 2 - Lair Next S
	stairRooms.insert(504); // Act 2 - Lair Next N
	stairRooms.insert(699); // Act 3 - Dungeon Next W
	stairRooms.insert(700); // Act 3 - Dungeon Next E
	stairRooms.insert(701); // Act 3 - Dungeon Next S
	stairRooms.insert(702); // Act 3 - Dungeon Next N
	stairRooms.insert(788); // Act 3 - Mephisto Next W
	stairRooms.insert(789); // Act 3 - Mephisto Next E
	stairRooms.insert(790); // Act 3 - Mephisto Next S
	stairRooms.insert(791); // Act 3 - Mephisto Next N
	stairRooms.insert(1022); // Act 5 - Ice Next W
	stairRooms.insert(1023); // Act 5 - Ice Next E
	stairRooms.insert(1024); // Act 5 - Ice Next S
	stairRooms.insert(1025); // Act 5 - Ice Next N
	stairRooms.insert(1078); // Act 5 - Baal Next N
	stairRooms.insert(1079); // Act 5 - Baal Next S
	stairRooms.insert(1080); // Act 5 - Baal Next E
	stairRooms.insert(1081); // Act 5 - Baal Next W


	levelWarpsUp.insert(	4	); //	Act 1 Cave Up
	levelWarpsUp.insert(	8	); //	Act 1 Crypt Up
	levelWarpsUp.insert(	13	); //	Act 1 Jail Up
	levelWarpsUp.insert(	16	); //	Act 1 Catacombs to Cathedral
	levelWarpsUp.insert(	17	); //	Act 1 Catacombs Up
	levelWarpsUp.insert(	22	); //	Act 2 Sewer Up
	levelWarpsUp.insert(	26	); //	Act 2 Harem Up 1
	levelWarpsUp.insert(	27	); //	Act 2 Harem Up 2
	levelWarpsUp.insert(	30	); //	Act 2 Basement Up 1
	levelWarpsUp.insert(	31	); //	Act 2 Basement Up 2
	levelWarpsUp.insert(	45	); //	Act 2 Tomb Up
	levelWarpsUp.insert(	48	); //	Act 2 Lair Up
	levelWarpsUp.insert(	55	); //	Act 3 Dungeon Up
	levelWarpsUp.insert(	58	); //	Act 3 Sewer Up L
	levelWarpsUp.insert(	59	); //	Act 3 Sewer Up R
	levelWarpsUp.insert(	62	); //	Act 3 Temple Up L
	levelWarpsUp.insert(	63	); //	Act 3 Temple Up R
	levelWarpsUp.insert(	65	); //	Act 3 Mephisto Up L
	levelWarpsUp.insert(	66	); //	Act 3 Mephisto Up R
	levelWarpsUp.insert(	73	); //	Act 5 Ice Caves Up
	levelWarpsUp.insert(	73	); //	Act 5 Ice Caves Up
	levelWarpsUp.insert(	78	); //	Act 5 Temple Up
	levelWarpsUp.insert(	81	); //	Act 5 Baal Temple Up
	levelWarpsUp.insert(	81	); //	Act 5 Baal Temple Up


	levelWarpsDown.insert(	0	); //	Act 1 Wilderness to Cave Cliff L
	levelWarpsDown.insert(	1	); //	Act 1 Wilderness to Cave Cliff R
	levelWarpsDown.insert(	2	); //	Act 1 Wilderness to Cave Floor L
	levelWarpsDown.insert(	3	); //	Act 1 Wilderness to Cave Floor R
	levelWarpsDown.insert(	5	); //	Act 1 Cave Down
	levelWarpsDown.insert(	6	); //	Act 1 Graveyard to Crypt 1
	levelWarpsDown.insert(	7	); //	Act 1 Graveyard to Crypt 2
	levelWarpsDown.insert(	9	); //	Act 1 Crypt Down
	levelWarpsDown.insert(	10	); //	Act 1 Wilderness to Tower
	levelWarpsDown.insert(	11	); //	Act 1 Tower to Wilderness
	levelWarpsDown.insert(	12	); //	Act 1 Tower to Crypt
	levelWarpsDown.insert(	14	); //	Act 1 Jail Down
	levelWarpsDown.insert(	15	); //	Act 1 Cathedral To Catacombs
	levelWarpsDown.insert(	18	); //	Act 1 Catacombs Down
	levelWarpsDown.insert(	19	); //	Act 2 Town to Sewer Trap
	levelWarpsDown.insert(	20	); //	Act 2 Town to Sewer Dock
	levelWarpsDown.insert(	21	); //	Act 2 Sewer Dock to Town
	levelWarpsDown.insert(	23	); //	Act 2 Sewer Down
	levelWarpsDown.insert(	24	); //	Act 2 Town to Harem
	levelWarpsDown.insert(	25	); //	Act 2 Harem to Town
	levelWarpsDown.insert(	28	); //	Act 2 Harem Down 1
	levelWarpsDown.insert(	29	); //	Act 2 Harem Down 2
	levelWarpsDown.insert(	32	); //	Act 2 Basement Down
	levelWarpsDown.insert(	33	); //	Act 2 Desert to Tomb L 1
	levelWarpsDown.insert(	34	); //	Act 2 Desert to Tomb L 2
	levelWarpsDown.insert(	35	); //	Act 2 Desert to Tomb R 1
	levelWarpsDown.insert(	36	); //	Act 2 Desert to Tomb R 2
	levelWarpsDown.insert(	37	); //	Act 2 Desert to Tomb Viper
	levelWarpsDown.insert(	38	); //	Act 2 Desert to Tomb Tal 1
	levelWarpsDown.insert(	39	); //	Act 2 Desert to Tomb Tal 2
	levelWarpsDown.insert(	40	); //	Act 2 Desert to Tomb Tal 3
	levelWarpsDown.insert(	41	); //	Act 2 Desert to Tomb Tal 4
	levelWarpsDown.insert(	42	); //	Act 2 Desert to Tomb Tal 5
	levelWarpsDown.insert(	43	); //	Act 2 Desert to Tomb Tal 6
	levelWarpsDown.insert(	44	); //	Act 2 Desert to Tomb Tal 7
	levelWarpsDown.insert(	46	); //	Act 2 Tomb Down
	levelWarpsDown.insert(	47	); //	Act 2 Desert to Lair
	levelWarpsDown.insert(	49	); //	Act 2 Lair Down
	levelWarpsDown.insert(	50	); //	Act 2 Desert to Sewer Trap
	levelWarpsDown.insert(	51	); //	Act 3 Jungle to Spider
	levelWarpsDown.insert(	52	); //	Act 3 Spider to Jungle
	levelWarpsDown.insert(	53	); //	Act 3 Jungle to Dungeon Fort
	levelWarpsDown.insert(	54	); //	Act 3 Jungle to Dungeon Hole
	levelWarpsDown.insert(	56	); //	Act 3 Dungeon Down
	levelWarpsDown.insert(	57	); //	Act 3 Kurast to Sewer
	levelWarpsDown.insert(	60	); //	Act 3 Sewer Down
	levelWarpsDown.insert(	61	); //	Act 3 Kurast to Temple
	levelWarpsDown.insert(	64	); //	Act 3 Travincal to Mephisto
	levelWarpsDown.insert(	67	); //	Act 3 Mephisto Down L
	levelWarpsDown.insert(	68	); //	Act 3 Mephisto Down R
	levelWarpsDown.insert(	69	); //	Act 4 Mesa to Lava
	levelWarpsDown.insert(	70	); //	Act 4 Lava to Mesa
	levelWarpsDown.insert(	71	); //	Act 5 Barricade Down Wall
	levelWarpsDown.insert(	71	); //	Act 5 Barricade Down Wall
	levelWarpsDown.insert(	72	); //	Act 5 Barricade Down Floor
	levelWarpsDown.insert(	74	); //	Act 5 Ice Caves Down
	levelWarpsDown.insert(	74	); //	Act 5 Ice Caves Down
	levelWarpsDown.insert(	75	); //	Act 5 Ice Caves Down Floor
	levelWarpsDown.insert(	76	); //	Act 5 Temple Entrance
	levelWarpsDown.insert(	77	); //	Act 5 Temple Down
	levelWarpsDown.insert(	79	); //	Act 5 Mountain Top To Ice
	levelWarpsDown.insert(	80	); //	Act 5 Mountain Top To Baal
	levelWarpsDown.insert(	82	); //	Act 5 Baal Temple Down
	levelWarpsDown.insert(	82	); //	Act 5 Baal Temple Down

	return TRUE;
}

DWORD EXPORT OnGamePacketBeforeSent(BYTE* aPacket, DWORD aLen)
{
	if(aPacket[0] == 0x15 && aPacket[1] == 0x01)
	{
		char *chatMessage = (char *)(aPacket+3);

		if(strncmp(chatMessage, "ÿc5Waypointÿc0:", 15) == 0)
		{
			if(strcmp(chatMessage, "ÿc5Waypointÿc0: Complete") == 0)
			{
			}
			else
			{
			}

			return 0;
		}
	}

	return aLen;
}


VOID EXPORT OnGamePacketAfterReceived(BYTE* aPacket, DWORD aLen)
{

}

BOOL CALLBACK enumUnitProc(LPCGAMEUNIT lpUnit, LPARAM lParam)
{
	if(lpUnit->dwUnitType != UNIT_TYPE_ROOMTILE)
		return TRUE;

	DWORD unitClassId = server->GetUnitClassID(lpUnit);
	if(unitClassId == 0)
		return TRUE;

	//server->GameStringf("unitClassId = %d", unitClassId);

	if(levelWarpsDown.find(unitClassId) == levelWarpsDown.end())
		return TRUE;

	*((GAMEUNIT*)lParam) = *lpUnit;

	return FALSE;
}

DWORD EXPORT OnGamePacketBeforeReceived(BYTE* aPacket, DWORD aLen)
{
	if(currentState == STATE_TeleportingToStairsRoom && aPacket[0] == 0x09 && aPacket[1] == 0x05)
	{
		// A cave/stairs/entrance has come into range, check to see if it's our staircase
		
		if(server->EnumUnits(UNIT_TYPE_ROOMTILE, enumUnitProc, (LPARAM)&stairsGameUnit))
		{
			return aLen;
		}
		
		MAPPOS stairsPos = server->GetUnitPosition(&stairsGameUnit);
		
		//server->GameStringf("!! Stairs object Found at (%d, %d)", stairsPos.x, stairsPos.y);
		
		currentState = STATE_TeleportingToStairsObject;
		
		me->CleanJobs();
		pathIndex = 0;
		server->CalculatePath(stairsPos.x +5, stairsPos.y+ 5, &path, 15);
		
		me->CastOnMap(D2S_TELEPORT, path.aPathNodes[pathIndex].x, path.aPathNodes[pathIndex].y, false);
		++pathIndex;
	}

	else if(aPacket[0] == 0x15 && *((DWORD *)&aPacket[2]) == me->GetID())
	{
		if(currentState != STATE_TeleportingToStairsRoom && currentState != STATE_TeleportingToStairsObject)
		{
			return aLen;
		}

		//server->GamePrintPacket(true, aPacket, aLen);
		// We have been re-assigned (happens after teleporting and other actions (?))
		if(pathIndex >= path.iNodeCount)
		{
		
			//server->GameStringf("We have arrived at our destination");
		
			if(currentState == STATE_TeleportingToStairsObject)
			{
				server->GameStringf("Using stairs...");
				currentState = STATE_UsingStairs;
				me->Interact(&stairsGameUnit);
			}
		
			return aLen;
		}
		
		//server->GameStringf("pathIndex = %d  nodeCount = %d  (%d, %d)", pathIndex, path.iNodeCount, path.aPathNodes[pathIndex].x, path.aPathNodes[pathIndex].y);
		
		me->CastOnMap(D2S_TELEPORT, path.aPathNodes[pathIndex].x, path.aPathNodes[pathIndex].y, false);
		++pathIndex;
	}
	return aLen;
}

VOID EXPORT OnThisPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	if(nMessage == PM_MOVECOMPLETE)
	{
		//if(currentState == STATE_TeleportingToStairsObject)
		//{
		//	currentState = STATE_UsingStairs;
		//	me->Interact(&stairsGameUnit);
		//}
	}
	else if(nMessage == PM_MAPCHANGE)
	{
		if(currentState == STATE_UsingStairs)
		{
			currentState = STATE_Idle;
			Start(nullptr, 0);
		}
	}
}

BYTE EXPORT OnGameKeyDown(BYTE iKeyCode)
{
	return iKeyCode;
}


CLIENTINFO
(
	0,1,
	"",
	"",
	"bot",
	""
)

MODULECOMMANDSTRUCT ModuleCommands[]=
{
	{
		"help",
		OnGameCommandHelp,
		"helpÿc0 List commands available in this module.\n"
		"<command> helpÿc0 Shows detailed help for <command> in this module."
	},
	{
		"Pos",
		Pos,
		"Pos Pos"
	},
		{
		"Start",
		Start,
		"Start bot"
	},
	{NULL}
};
