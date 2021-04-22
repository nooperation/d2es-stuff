#include <fstream>
#include <vector>
#include <set>
#include <algorithm>

#include "../../Includes/ClientCore.cpp"

enum States
{
	STATE_Idle,
	STATE_TeleportingToWaypoint,
	STATE_OpeningWaypoint,
	STATE_UsingWaypoint,
};

#include <map>

States currentState = STATE_Idle;

std::map<DWORD, int> waypointBitIndices;
std::vector<int> waypointObjectIds;
GAMEUNIT targetWaypointGameUnit;
PRESETUNIT targetWaypointPresetUnit;
DWORD destinationMapId;

bool leftHandCasting = true;

bool isUsingChat = false;

unsigned int currentPathToWaypointIndex;
PATH pathToWaypoint;

/// <summary>
/// Outputs the specified string to the game. Also outputs the string using in game chat if 'isUsingChat' flag
///   has been set.
/// </summary>
/// <param name="message">String to print.</param>
void Output(const char *message)
{
	char buff[256];
	sprintf_s(buff, "ÿc5Waypointÿc0: %s", message);

	if(isUsingChat)
	{
		me->Say(buff);
	}

	server->GamePrintString(buff);
}

/// <summary>
/// Uses the waypoint to teleport to a new level.
/// WARNING: The client will keep the waypoint UI open after the teleport. If the user closes this UI then the
///   client will send the close UI message to the server. This is an invalid message at this time so the server
///   will kick us if we don't block it.
/// </summary>
/// <param name="waypointId">ID of the waypoint we're using.</param>
/// <param name="destination">Destination we're teleporting to.</param>
void SendUseWaypointPacket(DWORD waypointId, DWORD destination)
{
	unsigned char packet[9];

	GAMEUNIT gu;
	gu.dwUnitID = waypointId;
	gu.dwUnitType = UNIT_TYPE_OBJECT;

	if(!server->VerifyUnit(&gu))
	{
		Output("UseWaypoint: Failed to verify waypoint");
		return;
	}

	packet[0] = 0x49;
	*((unsigned int *)&packet[1]) = waypointId;
	*((unsigned int *)&packet[5]) = destination;
	 
	currentState = STATE_UsingWaypoint;
	server->GameSendPacketToServer(packet, sizeof(packet));
	me->CloseAllUIs();
}

/// <summary>
/// Opens the specified waypoint.
/// </summary>
/// <param name="waypointId">ID of the waypoint to open.</param>
void OpenWaypoint(DWORD waypointId)
{
	unsigned char packet[9];

	GAMEUNIT gu;
	gu.dwUnitID = waypointId;
	gu.dwUnitType = UNIT_TYPE_OBJECT;

	if(!server->VerifyUnit(&gu))
	{
		Output("OpenWaypoint: Failed to verify waypoint");
		return;
	}

	packet[0] = 0x13;
	*((unsigned int *)&packet[1]) = UNIT_TYPE_OBJECT;
	*((unsigned int *)&packet[5]) = waypointId;

	currentState = STATE_OpeningWaypoint;
	server->GameSendPacketToServer(packet, sizeof(packet));
}

/// <summary>
/// Takes the next step towards the waypoint by using the pregenerated path to the waypoint.
/// </summary>
void TakeNextStep()
{
	if(currentPathToWaypointIndex >= pathToWaypoint.iNodeCount)
	{
		if(!server->FindUnitByClassID(targetWaypointPresetUnit.dwID, UNIT_TYPE_OBJECT, &targetWaypointGameUnit))
		{
			Output("Failed to find waypoint.");
			return;
		}

		auto myPosition = me->GetPosition();
		auto waypointPosition = server->GetUnitPosition(&targetWaypointGameUnit);
		if (server->GetDistance(myPosition.x, myPosition.y, waypointPosition.x, waypointPosition.y) < 4)
		{
			OpenWaypoint(targetWaypointGameUnit.dwUnitID);
			return;
		}
		else
		{
			Output("Still too far away from waypoint. Retrying step");
			--currentPathToWaypointIndex;
		}
	}

	if (me->GetSelectedSpell(leftHandCasting) != D2S_TELEPORT)
	{
		Output("Teleport not selected.");
		currentState = STATE_Idle;
		return;
	}

	if (me->CastOnMap(D2S_TELEPORT, pathToWaypoint.aPathNodes[currentPathToWaypointIndex].x, pathToWaypoint.aPathNodes[currentPathToWaypointIndex].y, leftHandCasting))
	{
		++currentPathToWaypointIndex;
	}
}

/// <summary>
/// Checks to see if there is a waypoint object nearby.
/// </summary>
/// <param name="lpPresetUnit">Preset game unit, see /data/global/excel/Objects.txt for list of all objects.</param>
/// <param name="lParam">Pointer to a PRESETUNIT to be filled out if waypoint object is found.</param>
/// <returns>TRUE if waypoint was not found. FALSE if waypoint was found.</returns>
BOOL CALLBACK enumPresetFunc(LPCPRESETUNIT lpPresetUnit, LPARAM lParam)
{
	if(lpPresetUnit->dwType != UNIT_TYPE_OBJECT)
	{
		return TRUE;
	}

	if(std::find(waypointObjectIds.begin(), waypointObjectIds.end(), lpPresetUnit->dwID) == waypointObjectIds.end())
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
		Output("Usage: .wp start [destination = 0..111] [chat = 0..1]");
		return FALSE;
	}

	if (me->GetSelectedSpell(true) == D2S_TELEPORT)
	{
		leftHandCasting = true;
	}
	else if (me->GetSelectedSpell(false) == D2S_TELEPORT)
	{
		leftHandCasting = false;
	}
	else
	{
		Output("Teleport not selected.");
		currentState = STATE_Idle;
		return TRUE;
	}

	destinationMapId = atoi(argv[2]);
	if(destinationMapId == 0 || waypointBitIndices.find(destinationMapId) == waypointBitIndices.end())
	{
		Output("Invalid waypoint");
		return TRUE;
	}

	if(argc == 4)
	{
		isUsingChat = atoi(argv[3]) == 1;
	}

	if(server->EnumPresetUnits(enumPresetFunc, (LPARAM)&targetWaypointPresetUnit))
	{
		Output("Failed to find waypoint");
		return TRUE;
	}

	if (server->CalculatePath(targetWaypointPresetUnit.x, targetWaypointPresetUnit.y, &pathToWaypoint, 15) == 0)
	{
		Output("Calculate path returned 0 nodes");
		return TRUE;
	}

	currentPathToWaypointIndex = 0;

	currentState = STATE_TeleportingToWaypoint;

	TakeNextStep();

	return TRUE;
}

////////////////////////////////////////////
//
//               EXPORTS
//
/////////////////////////////////////////////

VOID EXPORT OnGameJoin(THISGAMESTRUCT* thisgame)
{
	currentState = STATE_Idle;
	isUsingChat = false;
}

BOOL EXPORT OnClientStart()
{
	waypointObjectIds.push_back(119); // waypoint portal
	waypointObjectIds.push_back(145); // waypointi inner hell
	waypointObjectIds.push_back(156); // waypoint act 2
	waypointObjectIds.push_back(157); // waypoint act 1 wilderness
	waypointObjectIds.push_back(237); // act3waypoint town
	waypointObjectIds.push_back(238); // waypointh
	waypointObjectIds.push_back(288); // waypoint, celler
	waypointObjectIds.push_back(323); // waypoint act2 sewer
	waypointObjectIds.push_back(324); // waypoint act3 travincal
	waypointObjectIds.push_back(398); // waypoint pandamonia fortress
	waypointObjectIds.push_back(402); // waypoint valleywaypoint
	waypointObjectIds.push_back(429); // expansion no snow
	waypointObjectIds.push_back(494); // baals_waypoint
	waypointObjectIds.push_back(496); // wilderness_waypoint
	waypointObjectIds.push_back(511); // icecave 
	waypointObjectIds.push_back(539); // temple

	int i = 0;
	waypointBitIndices[WAYPOINTDEST_RogueEncampment        ] = i++;
	waypointBitIndices[WAYPOINTDEST_ColdPlains             ] = i++;
	waypointBitIndices[WAYPOINTDEST_StonyFields            ] = i++;
	waypointBitIndices[WAYPOINTDEST_DarkWood               ] = i++;
	waypointBitIndices[WAYPOINTDEST_BlackMarsh             ] = i++;
	waypointBitIndices[WAYPOINTDEST_OuterCloister          ] = i++;
	waypointBitIndices[WAYPOINTDEST_JailLevel1             ] = i++;
	waypointBitIndices[WAYPOINTDEST_InnerCloister          ] = i++;
	waypointBitIndices[WAYPOINTDEST_CataCombsLevel2        ] = i++;
	waypointBitIndices[WAYPOINTDEST_LutGholein             ] = i++;
	waypointBitIndices[WAYPOINTDEST_SewersLevel2           ] = i++;
	waypointBitIndices[WAYPOINTDEST_DryHills               ] = i++;
	waypointBitIndices[WAYPOINTDEST_HallsOfTheDeadLevel2   ] = i++;
	waypointBitIndices[WAYPOINTDEST_FarOasis               ] = i++;
	waypointBitIndices[WAYPOINTDEST_LostCity               ] = i++;
	waypointBitIndices[WAYPOINTDEST_PalaceCellarLevel1     ] = i++;
	waypointBitIndices[WAYPOINTDEST_ArcainSanctuary        ] = i++;
	waypointBitIndices[WAYPOINTDEST_CanyonOfTheMagi        ] = i++;
	waypointBitIndices[WAYPOINTDEST_KurastDocks            ] = i++;
	waypointBitIndices[WAYPOINTDEST_SpiderForest           ] = i++;
	waypointBitIndices[WAYPOINTDEST_GreatMarsh             ] = i++;
	waypointBitIndices[WAYPOINTDEST_FlayerJungle           ] = i++;
	waypointBitIndices[WAYPOINTDEST_LowerKurast            ] = i++;
	waypointBitIndices[WAYPOINTDEST_KurastBazaar           ] = i++;
	waypointBitIndices[WAYPOINTDEST_UpperKurast            ] = i++;
	waypointBitIndices[WAYPOINTDEST_Travincal              ] = i++;
	waypointBitIndices[WAYPOINTDEST_DuranceOfHateLevel2    ] = i++;
	waypointBitIndices[WAYPOINTDEST_ThePandeminoumFortress ] = i++;
	waypointBitIndices[WAYPOINTDEST_CityOfTheDamned        ] = i++;
	waypointBitIndices[WAYPOINTDEST_RiverOfFlame           ] = i++;
	waypointBitIndices[WAYPOINTDEST_Harrogath              ] = i++;
	waypointBitIndices[WAYPOINTDEST_FrigidHighlands        ] = i++;
	waypointBitIndices[WAYPOINTDEST_ArreatPlateau          ] = i++;
	waypointBitIndices[WAYPOINTDEST_CrystallinePassage     ] = i++;
	waypointBitIndices[WAYPOINTDEST_GlacialTrail           ] = i++;
	waypointBitIndices[WAYPOINTDEST_HallsOfPain            ] = i++;
	waypointBitIndices[WAYPOINTDEST_FrozenTundra           ] = i++;
	waypointBitIndices[WAYPOINTDEST_TheAncientsWay         ] = i++;
	waypointBitIndices[WAYPOINTDEST_WorldstoneKeepLevel2   ] = i++;

	return TRUE;
}

DWORD EXPORT OnGamePacketBeforeSent(BYTE* aPacket, DWORD aLen)
{
	if (currentState == STATE_Idle)
	{
		return aLen;
	}

	// Packet 0x49 - waypoint interaction
	// Packet 0x49 with a destination of 0 is sent to the server when we close the Waypoint UI.
	//  Unfortunatly the UI isn't closed automaticly when we use the WP via sending the packet
	//  so this packet will be sent when the user eventualy closes the waypoint UI that is left
	//  open after using the waypoint. We must block this packet after a successful teleport.
	if(currentState == STATE_UsingWaypoint && aPacket[0] == 0x49 && *((int *)&aPacket[5]) == 0)
	{
		Output("Complete");

		currentState = STATE_Idle;
		return 0;
	}

	return aLen;
}

VOID EXPORT OnGamePacketAfterReceived(BYTE* aPacket, DWORD aLen)
{
	if (currentState == STATE_Idle)
	{
		return;
	}

	// Packet 0x63 - Waypoint status sent after we open the waypoint UI
	// If we get this far then we should be good to go and teleport to our destination.
	if(currentState == STATE_OpeningWaypoint && aPacket[0] == 0x63)
	{
		DWORD waypointId = *((DWORD *)&aPacket[1]);
		BYTE *availableWaypoints = &aPacket[7];

		// There are 8*14 bits of waypoint flags starting at byte 7 to determine
		//   if we have access to the specified waypoint. Use 'waypointBitIndices'
		//   to get the index of the bit we need to check to see if we have access
		//   to the waypoint.
		if (waypointBitIndices.find(destinationMapId) == waypointBitIndices.end())
		{
			Output("Invalid waypoint");
			currentState = STATE_Idle;
			return;
		}

		int waypointBitIndex = waypointBitIndices[destinationMapId];

		int bitIndex = waypointBitIndex % 8;
		int byteIndex = waypointBitIndex / 8;

		if (((*(availableWaypoints + byteIndex)) & (1 << bitIndex)) == 0)
		{
			Output("Waypoint not available");
			currentState = STATE_Idle;
			return;
		}

		SendUseWaypointPacket(targetWaypointGameUnit.dwUnitID, destinationMapId);
	}
	else if (aPacket[0] == 0x15 && *((DWORD *)&aPacket[2]) == me->GetID())
	{
		// Packet 0x15 is the reassignment packet, meaning a character has been assigned to a new 
		//  position. Happens mostly during teleporting and players coming into view.

		if (currentState != STATE_TeleportingToWaypoint)
		{
			return;
		}
		
		TakeNextStep();
	}
}


DWORD EXPORT OnGamePacketBeforeReceived(BYTE* aPacket, DWORD aLen)
{
	if (currentState == STATE_Idle)
	{
		return aLen;
	}

	return aLen;
}

BYTE EXPORT OnGameKeyDown(BYTE iKeyCode)
{
	if (currentState == STATE_Idle)
	{
		return iKeyCode;
	}

	if(iKeyCode == VK_SPACE)
	{
		currentState = STATE_Idle;
	}

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
		"start",
		TeleportTo,
		"Teleports to the current map's waypoint and warps to the specified destination"
	},
	{NULL}
};
