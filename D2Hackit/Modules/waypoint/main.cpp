#include <fstream>
#include <vector>
#include <set>
#include <algorithm>

#include "../../Includes/ClientCore.cpp"

std::vector<int> waypointIds;
PRESETUNIT targetWaypointPresetUnit;
GAMEUNIT targetWaypointGameUnit;
DWORD destinationMapId = 4;

bool isMovingToWp = false;
bool isOpeniningWaypoint = false;
bool isIgnoringCloseWaypointUIPacket = false;
bool isUsingChat = false;

void AcceptWP(DWORD waypointId, DWORD destination)
{
	unsigned char teleportWp[9];

	GAMEUNIT gu;
	gu.dwUnitID = waypointId;
	gu.dwUnitType = UNIT_TYPE_OBJECT;

	if(!server->VerifyUnit(&gu))
	{
		if(isUsingChat)
		{
			me->Say("ÿc5Waypointÿc0: AcceptWP: Failed to verify waypoint");
		}
		server->GameStringf("ÿc5Waypointÿc0: AcceptWP: Failed to verify waypoint");
		return;
	}

	teleportWp[0] = 0x49;
	*((unsigned int *)&teleportWp[1]) = waypointId;
	*((unsigned int *)&teleportWp[5]) = destination;
	 
	isIgnoringCloseWaypointUIPacket = true;
	server->GameSendPacketToServer(teleportWp, sizeof(teleportWp));
	me->CloseAllUIs();
}

void OpenWP(DWORD waypointId)
{
	unsigned char openWp[9];

	GAMEUNIT gu;
	gu.dwUnitID = waypointId;
	gu.dwUnitType = UNIT_TYPE_OBJECT;

	if(!server->VerifyUnit(&gu))
	{
		if(isUsingChat)
		{
			me->Say("ÿc5Waypointÿc0: OpenWP: Failed to verify waypoint");
		}
		server->GameStringf("ÿc5Waypointÿc0: OpenWP: Failed to verify waypoint");
		return;
	}

	openWp[0] = 0x13;
	*((unsigned int *)&openWp[1]) = UNIT_TYPE_OBJECT;
	*((unsigned int *)&openWp[5]) = waypointId;

	isOpeniningWaypoint = true;
	server->GameSendPacketToServer(openWp, sizeof(openWp));
}

BOOL CALLBACK enumPresetFunc(LPCPRESETUNIT lpPresetUnit, LPARAM lParam)
{
	if(lpPresetUnit->dwType != UNIT_TYPE_OBJECT)
	{
		return TRUE;
	}

	if(std::find(waypointIds.begin(), waypointIds.end(), lpPresetUnit->dwID) == waypointIds.end())
	{
		return TRUE;
	}

	memcpy((void *)lParam, lpPresetUnit, sizeof(PRESETUNIT));

	return FALSE;
}

BOOL PRIVATE TeleportTo(char** argv, int argc)
{
	if(argc < 3 || argc > 4)
	{
		if(isUsingChat)
		{
			me->Say("ÿc5Waypointÿc0: Usage: .wp tp <destination> <bUseChat>");
		}
		server->GameStringf("ÿc5Waypointÿc0: Usage: .wp tp <destination> <bUseChat>");
		return FALSE;
	}

	destinationMapId = atoi(argv[2]);
	if(argc == 4)
	{
		isUsingChat = atoi(argv[3]) == 1;
	}

	if(server->EnumPresetUnits(enumPresetFunc, (LPARAM)&targetWaypointPresetUnit))
	{
		if(isUsingChat)
		{
			me->Say("ÿc5Waypointÿc0: Failed to find waypoint");
		}

		server->GameStringf("ÿc5Waypointÿc0: Failed to find waypoint");
		return TRUE;
	}

	PATH pathToWp;
	server->CalculatePath(targetWaypointPresetUnit.x, targetWaypointPresetUnit.y, &pathToWp, 15);
	for (int i = 0; i < pathToWp.iNodeCount; i++)
	{
		isMovingToWp = true;
		me->TeleportTo(pathToWp.aPathNodes[i].x, pathToWp.aPathNodes[i].y, true);
	}
	
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
	isMovingToWp = false;
	isOpeniningWaypoint = false;
	isIgnoringCloseWaypointUIPacket = false;
}

BOOL EXPORT OnClientStart()
{
	waypointIds.push_back(119); // waypoint portal
	waypointIds.push_back(145); // waypointi inner hell
	waypointIds.push_back(156); // waypoint act 2
	waypointIds.push_back(157); // waypoint act 1 wilderness
	waypointIds.push_back(237); // act3waypoint town
	waypointIds.push_back(238); // waypointh
	waypointIds.push_back(288); // waypoint, celler
	waypointIds.push_back(323); // waypoint act2 sewer
	waypointIds.push_back(324); // waypoint act3 travincal
	waypointIds.push_back(398); // waypoint pandamonia fortress
	waypointIds.push_back(402); // waypoint valleywaypoint
	waypointIds.push_back(429); // expansion no snow
	waypointIds.push_back(494); // baals_waypoint
	waypointIds.push_back(496); // wilderness_waypoint
	waypointIds.push_back(511); // icecave 
	waypointIds.push_back(539); // temple

	return TRUE;
}



DWORD EXPORT OnGamePacketBeforeSent(BYTE* aPacket, DWORD aLen)
{
	// Packet 0x49 - waypoint interaction
	// Packet 0x49 with a destination of 0 is sent to the server when we close the Waypoint UI.
	//  Unfortunatly the UI isn't closed automaticly when we use the WP via sending the packet
	//  so this packet will be sent when the user eventualy closes the waypoint UI that is left
	//  open after using the waypoint. We must block this packet after a successful teleport.
	if(aPacket[0] == 0x49 && isIgnoringCloseWaypointUIPacket && *((int *)&aPacket[5]) == 0)
	{
		if(isUsingChat)
		{
			me->Say("ÿc5Waypointÿc0: Complete");
		}

		isIgnoringCloseWaypointUIPacket = false;
		return 0;
	}

	return aLen;
}


VOID EXPORT OnGamePacketAfterReceived(BYTE* aPacket, DWORD aLen)
{
	// Packet 0x63 - Waypoint status sent after we open the waypoint UI
	// If we get this far then we should be good to go and teleport to our destination.
	// TODO: Verify we actually have this wp before going there!
	if(isOpeniningWaypoint && aPacket[0] == 0x63)
	{
		isOpeniningWaypoint = false;
		AcceptWP(targetWaypointGameUnit.dwUnitID, destinationMapId);
	}
}

DWORD EXPORT OnGamePacketBeforeReceived(BYTE* aPacket, DWORD aLen)
{
	return aLen;
}

VOID EXPORT OnThisPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	if(nMessage == PM_MOVECOMPLETE)
	{
		if(!isMovingToWp)
		{
			return;
		}
		isMovingToWp = false;

		if(!server->FindUnitByClassID(targetWaypointPresetUnit.dwID, UNIT_TYPE_OBJECT, &targetWaypointGameUnit))
		{
			if(isUsingChat)
			{
				me->Say("ÿc5Waypointÿc0: Failed to find waypoint.");
			}

			server->GameStringf("ÿc5Waypointÿc0: Failed to find waypoint.");
			return;
		}

		OpenWP(targetWaypointGameUnit.dwUnitID);
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
	"wp",
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
		"tp",
		TeleportTo,
		"Teleports to the current map's waypoint and warps to the specified destination"
	},
	{NULL}
};
