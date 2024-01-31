#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
#include <queue>
#include <chrono>

#include "../../Includes/ClientCore.cpp"

void GoBackToTown();

enum States
{
	STATE_Idle,
	STATE_LoadingMap,
	STATE_TeleportingToStairsRoom,
	STATE_TeleportingToStairsRoom_WaitBlink,
	STATE_TeleportingToStairsObject,
	STATE_TeleportingToStairsObject_WaitBlink,
	STATE_UsingStairs,
	STATE_UsingWaypoint,
	STATE_BackToTown,
	STATE_SearchingForStairs,
	STATE_RetryNextRoomTransition,
	STATE_WaitingForBuffs,
};

struct StairsData
{
	StairsData()
	{
		Id = 0;
		XOffset = 0;
		YOffset = 0;
	}
	StairsData(DWORD id, WORD xOffset, WORD yOffset)
	{
		Id = id;
		XOffset = xOffset;
		YOffset = yOffset;
	}

	DWORD Id;
	short XOffset;
	short YOffset;
};

struct BotTravelData
{
	void InitSteps(int numSteps)
	{
		RoomTransitions.resize(numSteps);
		NumSteps = numSteps;
		CurrentStep = -1;
	}

	void Reset()
	{
		CurrentStep = -1;
	}

	std::map<DWORD, StairsData>& Current()
	{
		return RoomTransitions[CurrentStep];
	}

	std::string Name;
	int WaypointDestination;
	std::vector<std::map<DWORD, StairsData>> RoomTransitions;
	unsigned int CurrentStep;
	unsigned int NumSteps;
};

bool CheckForOurStairs();
void NextStep();
BOOL CALLBACK enumUnitProc(LPCGAMEUNIT lpUnit, LPARAM lParam);
bool CalculatePathToStairsRoom();
void NextRoomTransition();

States currentState = STATE_Idle;
unsigned long currentStateTicks = 0;

int stairsRoomNum = 0;

GAMEUNIT stairsGameUnit;
PATH path;
int pathIndex = 0;
int attemptingPathIndex = 0;

BotTravelData toBloodRaven;
BotTravelData toBloodRavenLair;
BotTravelData toCountress;
BotTravelData toAndarial;

BotTravelData toMephisto;
BotTravelData toBaal;
BotTravelData toDeadEnd;

std::vector<BotTravelData*> allTravelData;
BotTravelData* currentTravelData = nullptr;
bool isLeftHandCasting = false;

std::chrono::system_clock::time_point lastMapChange;
bool isLoadingMap = false;

static const int kDefaultRetriesRemainingForNextRoomTransition = 50;
int numRetriesRemainingForNextRoomTransition = kDefaultRetriesRemainingForNextRoomTransition;

#include "../../Core/definitions.h"

void Abort()
{
	if (currentState == STATE_Idle)
	{
		return;
	}

	server->GameStringf("�c5Goto�c0: Complete");
	currentState = STATE_Idle;

	auto fleeModule = GetModuleHandle("Flee.d2h");
	if (fleeModule != NULL)
	{
		server->GameCommandf("flee SuppressAutoTp 0");
	}
}

void SetState(States newState)
{
	/*
	switch (newState)
	{
	case STATE_Idle:
		server->GameStringf("Moved to state STATE_Idle");
		break;
	case STATE_TeleportingToStairsRoom:
		server->GameStringf("Moved to state STATE_TeleportingToStairsRoom");
		break;
	case STATE_TeleportingToStairsRoom_WaitBlink:
		server->GameStringf("Moved to state STATE_TeleportingToStairsRoom_WaitBlink");
		break;
	case STATE_TeleportingToStairsObject:
		server->GameStringf("Moved to state STATE_TeleportingToStairsObject");
		break;
	case STATE_TeleportingToStairsObject_WaitBlink:
		server->GameStringf("Moved to state STATE_TeleportingToStairsObject_WaitBlink");
		break;
	case STATE_UsingStairs:
		server->GameStringf("Moved to state STATE_UsingStairs");
		break;
	case STATE_UsingWaypoint:
		server->GameStringf("Moved to state STATE_UsingWaypoint");
		break;
	case STATE_BackToTown:
		server->GameStringf("Moved to state STATE_BackToTown");
		break;
	case STATE_SearchingForStairs:
		server->GameStringf("Moved to state STATE_SearchingForStairs");
		break;
	case STATE_RetryNextRoomTransition:
		server->GameStringf("Moved to state STATE_RetryNextRoomTransition");
		break;
	default:
		break;
	}*/

	currentState = newState;
	currentStateTicks = 0;
}

void InteractWithStairs()
{
	//server->GameStringf("InteractWithStairs()");

	SetState(STATE_UsingStairs);
	me->Interact(&stairsGameUnit);
}

BOOL PRIVATE Pos(char** argv, int argc)
{
	MAPPOS coords;
	RoomOther* room = (RoomOther*)server->D2GetRoomCoords(server->D2GetCurrentRoomNum(), &coords, nullptr);

	if (room == nullptr)
	{
		server->GameStringf("Failed to find room 1026");
		return TRUE;
	}

	if (room->pRoom == nullptr)
	{
		server->GameStringf("room->pRoom == nullptr");
		return TRUE;
	}

	MAPPOS myPos = me->GetPosition();


	server->GameStringf("Room %d: %d, %d", room->pPresetType2info->roomNum, room->pRoom->pColl->nPosRoomX, room->pRoom->pColl->nPosRoomY);
	server->GameStringf("Player position %d, %d  room %d offset x: %d y: %d", (int)myPos.x, (int)myPos.y, server->D2GetCurrentRoomNum(), (int)myPos.x - (int)room->pRoom->pColl->nPosGameX, (int)myPos.y - (int)room->pRoom->pColl->nPosGameY);



	//static ROOMPOS allRoomCoords[2048];
	//DWORD numRooms = server->D2GetAllRoomCoords(allRoomCoords, ARRAYSIZE(allRoomCoords));
	//if(numRooms == 0)
	//{
	//	server->GameStringf("Failed to find all rooms");
	//	return TRUE;
	//}

	//server->GameStringf("Found %d rooms", numRooms);

	//MAPPOS stairRoomCoord;
	//for(unsigned int i = 0; i < numRooms; ++i)
	//{
	//	server->GameStringf("%d: %d", i, allRoomCoords[i].roomnum);
	//}

	return TRUE;
}


BOOL CALLBACK enumUnitProc(LPCGAMEUNIT lpUnit, LPARAM lParam)
{
	char buff[1024] = {};
	server->GetUnitName(lpUnit, buff, 1024);
	//server->GameStringf("Unit %d: %s", lpUnit->dwUnitType, buff);

	DWORD unitClassId = server->GetUnitClassID(lpUnit);
	if (unitClassId == 0)
	{
		//server->GameStringf("unitClassId == 0");
		return TRUE;
	}


	if (currentTravelData->Current()[stairsRoomNum].Id != unitClassId)
	{
		// HACK: Meph stairs down can be 67 or 68. Just winging it to make this work for now
		if (currentTravelData->Current()[stairsRoomNum].Id != 67 && unitClassId != 68) {
			//server->GameStringf("currentTravelData->Current()[stairsRoomNum].Id != unitClassId || %d != %d", currentTravelData->Current()[stairsRoomNum].Id, unitClassId);
			return TRUE;
		}

	}

	//UnitAny *unit = (UnitAny *)server->VerifyUnit(lpUnit);
	//if(unit == nullptr)
	//{
	//	server->GameStringf("unit == nullptr");
	//	return TRUE;
	//}
	//if(unit->hPath == nullptr)
	//{
	//	server->GameStringf("unit->hPath == nullptr");
	//	return TRUE;
	//}
	//if(unit->hOPath->ptRoom == nullptr)
	//{
	//	server->GameStringf("unit->hOPath->ptRoom == nullptr");
	//	return TRUE;
	//}
	//if(unit->hOPath->ptRoom->ptRoomOther == nullptr)
	//{
	//	server->GameStringf("unit->hOPath->ptRoom->ptRoomOther == nullptr");
	//	return TRUE;
	//}
	//if(unit->hOPath->ptRoom->ptRoomOther->pPresetType2info == nullptr)
	//{
	//	server->GameStringf("unit->hOPath->ptRoom->ptRoomOther->pPresetType2info == nullptr");
	//	return TRUE;
	//}
	//
	//int roomNum = unit->hOPath->ptRoom->ptRoomOther->pPresetType2info->roomNum;

	GAMEUNIT* previouslyFoundStairs = (GAMEUNIT*)lParam;
	if (previouslyFoundStairs->dwUnitID != 0 && previouslyFoundStairs->dwUnitType != 0)
	{
		MAPPOS previousStairsPos = server->GetUnitPosition(previouslyFoundStairs);
		MAPPOS thisStairsPos = server->GetUnitPosition(lpUnit);

		double distanceToPreviousStairs = me->GetDistanceFrom(previousStairsPos.x, previousStairsPos.y);
		double distanceToThisStairs = me->GetDistanceFrom(thisStairsPos.x, thisStairsPos.y);

		if (distanceToThisStairs < distanceToPreviousStairs)
		{
			*((GAMEUNIT*)lParam) = *lpUnit;
		}
	}
	else
	{
		*((GAMEUNIT*)lParam) = *lpUnit;
	}

	return TRUE;
}

struct GameUnitDistance
{
	GAMEUNIT unit;
	float distance;
};


bool CheckForOurStairs()
{
	//server->GameStringf("%s", __FUNCTION__);
	// A cave/stairs/entrance has come into range, check to see if it's our staircase

	stairsGameUnit.dwUnitID = 0;
	stairsGameUnit.dwUnitType = 0;

	server->EnumUnits(UNIT_TYPE_ROOMTILE, enumUnitProc, (LPARAM)&stairsGameUnit);
	if (stairsGameUnit.dwUnitID == 0 && stairsGameUnit.dwUnitType == 0)
	{
		//server->GameStringf("stairsGameUnit.dwUnitID == 0 && stairsGameUnit.dwUnitType == 0");
		return false;
	}

	auto stairsPosition = server->GetUnitPosition(&stairsGameUnit);
	//server->GameStringf("Our stairs = %d", server->GetUnitClassID(&stairsGameUnit));

	SetState(STATE_TeleportingToStairsObject);

	pathIndex = 0;
	if (!server->CalculatePath(stairsPosition.x, stairsPosition.y, &path, 5))
	{
		server->GameStringf("Failed to calculate path. Trying a direct teleport path...");
		path.iNodeCount = 1;
		path.aPathNodes[0] = stairsPosition;
		path.posStart = me->GetPosition();
		path.posEnd = stairsPosition;
	}

	return true;
}

DWORD EXPORT OnGameTimerTick()
{
	if (currentState == STATE_Idle)
	{
		return 0;
	}

	if (isLoadingMap && ((std::chrono::system_clock::now() - lastMapChange) > std::chrono::milliseconds(250)))
	{
		server->GameStringf("MapChange logic resuming");

		isLoadingMap = false;
		numRetriesRemainingForNextRoomTransition = kDefaultRetriesRemainingForNextRoomTransition;
		//server->GameStringf("PM_MAPCHANGE");
		if (currentState == STATE_UsingStairs || currentState == STATE_UsingWaypoint)
		{
			if (me->IsInTown())
			{
				server->GameStringf("Unexpected map change. Aborting.");
				Abort();
				return 0;
			}

			NextRoomTransition();
		}
		else if (currentState == STATE_BackToTown)
		{
			SetState(STATE_UsingWaypoint);
			server->GameCommandf("load wp");
			server->GameCommandf("wp start %d", currentTravelData->WaypointDestination);
			return 0;
		}
		else
		{
			server->GameStringf("Unexpected map change. Aborting.");
			if (!me->IsInTown())
			{
				GoBackToTown();
			}
			Abort();
		}

		return 0;
	}

	auto tickRate = server->GetTickRate();
	auto oneSecond = 1 * (1000.0 / tickRate);
	if (currentStateTicks > 5 * oneSecond)
	{
		//server->GameStringf("SLOWSTATE: TickCount = %d | state = %d", currentStateTicks, currentState);
	}

	currentStateTicks++;

	if (currentState == STATE_SearchingForStairs)
	{
		currentStateTicks = 0;
		NextStep();
	}
	else if (currentState == STATE_RetryNextRoomTransition)
	{
		currentStateTicks = 0;
		NextRoomTransition();
	}
	else if (currentState == STATE_UsingStairs && currentStateTicks > 0.250 * oneSecond)
	{
		//server->GameStringf("TickCount = %d for STATE_UsingStairs, trying again", currentStateTicks);
		currentStateTicks = 0;

		InteractWithStairs();
	}
	else if (currentState == STATE_TeleportingToStairsRoom && currentStateTicks > 1 * oneSecond)
	{
		//server->GameStringf("TickCount = %d for STATE_TeleportingToStairsRoom, trying again", currentStateTicks);
		currentStateTicks = 0;

		if (pathIndex > 0)
		{
			pathIndex = attemptingPathIndex;
			NextStep();
		}
	}
	else if (currentState == STATE_TeleportingToStairsObject && currentStateTicks > 1 * oneSecond)
	{
		//server->GameStringf("TickCount = %d for STATE_TeleportingToStairsObject, trying again", currentStateTicks);
		currentStateTicks = 0;

		if (pathIndex > 0)
		{
			pathIndex = attemptingPathIndex;
			NextStep();
		}
	}
	else if (currentState == STATE_TeleportingToStairsRoom_WaitBlink && currentStateTicks > 1 * oneSecond)
	{
		//server->GameStringf("TickCount = %d for STATE_TeleportingToStairsRoom_WaitBlink, trying again", currentStateTicks);
		currentStateTicks = 0;

		if (pathIndex > 0)
		{
			pathIndex = attemptingPathIndex;
			NextStep();
		}
	}
	else if (currentState == STATE_TeleportingToStairsObject_WaitBlink && currentStateTicks > 1 * oneSecond)
	{
		//server->GameStringf("TickCount = %d for STATE_TeleportingToStairsObject_WaitBlink, trying again", currentStateTicks);
		currentStateTicks = 0;

		if (pathIndex > 0)
		{
			pathIndex = attemptingPathIndex;
			NextStep();
		}
	}
	return 0;
}


int waitingForBlinkSince = 0;
void NextStep()
{
	if (path.iNodeCount == 0) {
		server->GameStringf("Bad state, path.iNodeCount = 0");
		Abort();
		return;
	}
	if (pathIndex >= path.iNodeCount)
	{
		if (currentState == STATE_TeleportingToStairsObject)
		{
			//server->GameStringf("We have arrived at our stairs [class %X]", server->GetUnitClassID(&stairsGameUnit));
			//MAPPOS stairsPosition = server->GetUnitPosition(&stairsGameUnit);
			InteractWithStairs();
			return;
		}

		//server->GameStringf("We have arrived at our room, checking for stairs...");
		if (!CheckForOurStairs())
		{
			SetState(STATE_SearchingForStairs);
			return;
		}

		// STATE_TeleportingToStairsObject on successful CheckForOurStairs check
		// CheckForOurStairs will do another pathfinding check to walk to the stairs
	}

	// path is possibly modified in CheckForOurStairs...
	if (pathIndex < path.iNodeCount)
	{
		//	server->GameStringf("Teleporting to = %d [%d, %d]", pathIndex, path.aPathNodes[pathIndex].x, path.aPathNodes[pathIndex].y);
		attemptingPathIndex = pathIndex;
		if (me->CastOnMap(D2S_TELEPORT, path.aPathNodes[pathIndex].x, path.aPathNodes[pathIndex].y, isLeftHandCasting))
		{
			if (currentState == STATE_TeleportingToStairsObject)
			{
				SetState(STATE_TeleportingToStairsObject_WaitBlink);
			}
			else {
				SetState(STATE_TeleportingToStairsRoom_WaitBlink);
			}
		}
	}
}

int failureCount = 0;
void OnMapBlink() {
	if (currentState == STATE_TeleportingToStairsRoom_WaitBlink || currentState == STATE_TeleportingToStairsObject_WaitBlink) {
		MAPPOS myPos = me->GetPosition();
		auto distance = server->GetDistance(myPos.x, myPos.y, path.aPathNodes[pathIndex].x, path.aPathNodes[pathIndex].y);

		//	server->GameStringf("Our position (%d, %d) vs target position (%d, %d) = %d m", myPos.x, myPos.y, path.aPathNodes[pathIndex].x, path.aPathNodes[pathIndex].y, distance);

		if ((STATE_TeleportingToStairsRoom && distance < 20) || (STATE_TeleportingToStairsObject && distance <= 8)) {
			++pathIndex;
			failureCount = 0;
			//server->GameStringf("pathIndex++ = %d", pathIndex);
		}
		else {
			failureCount++;
			if (failureCount > 2)
			{
				server->GameStringf("Recalculating path to stairs room...");
				if (!CalculatePathToStairsRoom()) {
					return;
				}
			}
			//	server->GameStringf("Trying previous path again. we're %d away from where we should be", distance);
		}

		if (currentState == STATE_TeleportingToStairsObject_WaitBlink)
		{
			SetState(STATE_TeleportingToStairsObject);
		}
		else
		{
			SetState(STATE_TeleportingToStairsRoom);
		}
	}
}


void NextRoomTransition()
{
	//server->GameStringf("NextRoomTransition()");

	if (currentTravelData->CurrentStep + 1 < currentTravelData->NumSteps)
	{
		currentTravelData->CurrentStep++;
	}
	else {
		Abort();
		return;
	}

	if (!CalculatePathToStairsRoom()) {
		return;
	}

	//server->GameStringf("Step: %d/%d", currentTravelData->CurrentStep, currentTravelData->NumSteps);

	server->GameStringf("Teleporing to room %d", stairsRoomNum);
	SetState(STATE_TeleportingToStairsRoom);
	NextStep();
}

bool CalculatePathToStairsRoom()
{
	static ROOMPOS allRoomCoords[2048];

	pathIndex = 0;
	memset(&path, 0, sizeof(PATH));
	memset(&stairsGameUnit, 0, sizeof(GAMEUNIT));

	DWORD numRooms = server->D2GetAllRoomCoords(allRoomCoords, ARRAYSIZE(allRoomCoords));
	if (numRooms == 0)
	{
		server->GameStringf("Failed to find all rooms");
		return false;
	}

	//server->GameStringf("Found %d rooms", numRooms);

	MAPPOS stairRoomCoord;
	std::map<DWORD, StairsData>::iterator foundStairs;
	std::map<DWORD, StairsData>& currentStairsData = currentTravelData->Current();

	for (unsigned int i = 0; i < numRooms; ++i)
	{
		//server->GameStringf("%d: %d", i, allRoomCoords[i].roomnum);

		foundStairs = currentStairsData.find(allRoomCoords[i].roomnum);
		if (foundStairs != currentStairsData.end())
		{
			stairsRoomNum = allRoomCoords[i].roomnum;
			stairRoomCoord = allRoomCoords[i].pos;
			break;
		}
	}

	if (foundStairs == currentStairsData.end())
	{
		if (numRetriesRemainingForNextRoomTransition-- > 0)
		{
			SetState(STATE_RetryNextRoomTransition);
			currentTravelData->CurrentStep--;
		}

		server->GameStringf("No stairs found");
		Abort();
		return false;
	}
	numRetriesRemainingForNextRoomTransition = kDefaultRetriesRemainingForNextRoomTransition;

	server->GameStringf("stairRoomCoord %d, %d", stairRoomCoord.x, stairRoomCoord.y);


	stairRoomCoord.x = stairRoomCoord.x * 5 + foundStairs->second.XOffset;
	stairRoomCoord.y = stairRoomCoord.y * 5 + foundStairs->second.YOffset;

	//server->GameStringf("stairRoomCoord MOD %d, %d",stairRoomCoord.x, stairRoomCoord.y);

	for (size_t adjust = 5; adjust < 20; adjust += 5)
	{
		if (server->CalculatePath(stairRoomCoord.x, stairRoomCoord.y, &path, adjust) != 0)
		{
			return true;
		}
	}

	Abort();

	server->GameStringf("Failed to find path to stairs");
	//server->GameCommandf("flee tp");

	return false;
}

void ShowUsage()
{
	server->GameStringf("Usage: .bot start #");
	for (unsigned int i = 0; i < allTravelData.size(); ++i)
	{
		server->GameStringf("  %d: %s", i + 1, allTravelData[i]->Name.c_str());
	}
}

void GoBackToTown()
{
	SetState(STATE_BackToTown);
	server->GameCommandf("load flee");
	server->GameCommandf("flee tp");
}

BOOL PRIVATE Start(char** argv, int argc)
{
	if (argc != 3)
	{
		ShowUsage();
		return TRUE;
	}

	unsigned int travelIndex = (unsigned int)atoi(argv[2]);
	if (travelIndex == 0 || travelIndex > allTravelData.size())
	{
		ShowUsage();
		return TRUE;
	}


	if (me->GetSelectedSpell(true) == D2S_TELEPORT)
	{
		isLeftHandCasting = true;
	}
	else if (me->GetSelectedSpell(false) == D2S_TELEPORT)
	{
		isLeftHandCasting = false;
	}
	else
	{
		server->GamePrintInfo("You must select the teleport skill before starting.");
		return TRUE;
	}

	currentTravelData = allTravelData[travelIndex - 1];
	currentTravelData->Reset();

	if (!me->IsInTown())
	{
		GoBackToTown();
		return TRUE;
	}

	auto fleeModule = GetModuleHandle("Flee.d2h");
	if (fleeModule != NULL)
	{
		server->GameCommandf("flee SuppressAutoTp 1");
	}

	currentState = STATE_WaitingForBuffs;
	server->GameCommandf("buff buff chat");

	return TRUE;
}


////////////////////////////////////////////
//
//               EXPORTS
//
/////////////////////////////////////////////

VOID EXPORT OnGameJoin(THISGAMESTRUCT* thisgame)
{
	Abort();
}

VOID EXPORT OnClientStop(THISGAMESTRUCT* thisgame)
{
	server->GameStringf("OnClientStop");
	Abort();
}
VOID EXPORT OnGameLeave(THISGAMESTRUCT* thisgame)
{
	Abort();
}

BOOL EXPORT OnClientStart()
{
	currentTravelData = &toCountress;

	toDeadEnd.Name = "Dead End";
	toDeadEnd.InitSteps(4);
	toDeadEnd.WaypointDestination = WAYPOINTDEST_GlacialTrail;
	for (int i = 0; i < 3; ++i)
	{
		toDeadEnd.RoomTransitions[i][1018] = StairsData(73, 14, 7); //	Act 5 - Ice Prev W
		toDeadEnd.RoomTransitions[i][1019] = StairsData(73, 12, 19); //	Act 5 - Ice Prev E
		toDeadEnd.RoomTransitions[i][1020] = StairsData(73, 0, 0); //	Act 5 - Ice Prev S
		toDeadEnd.RoomTransitions[i][1021] = StairsData(73, 12, 25); //	Act 5 - Ice Prev N
	}

	allTravelData.push_back(&toDeadEnd);

	// BlackMarsh -> Countress
	toCountress.Name = "Countress";
	toCountress.InitSteps(6);
	toCountress.WaypointDestination = WAYPOINTDEST_BlackMarsh;
	toCountress.RoomTransitions[0][163] = StairsData(10, 14, 14); //	Act 1 Wilderness to Tower
	toCountress.RoomTransitions[1][164] = StairsData(12, 2, 12); //	Act 1 Tower to Crypt
	for (size_t i = 0; i < 4; i++)
	{
		toCountress.RoomTransitions[2 + i][143] = StairsData(9, 22, 14); //	Crypt Next W DOWN
		toCountress.RoomTransitions[2 + i][144] = StairsData(9, 2, 21); //	Crypt Next E DOWN
		toCountress.RoomTransitions[2 + i][145] = StairsData(9, 7, 19); //	Crypt Next S DOWN
		toCountress.RoomTransitions[2 + i][146] = StairsData(9, 7, 20); //	Crypt Next N DOWN
	}

	allTravelData.push_back(&toCountress);

	toAndarial.Name = "Andarial";
	toAndarial.InitSteps(2);
	toAndarial.WaypointDestination = WAYPOINTDEST_CataCombsLevel2;
	for (size_t i = 0; i < 2; i++)
	{
		toAndarial.RoomTransitions[i][291] = StairsData(18, 0, 0); // Act 1 - Catacombs Next W	
		toAndarial.RoomTransitions[i][292] = StairsData(18, 24, 29); // Act 1 - Catacombs Next E	
		toAndarial.RoomTransitions[i][293] = StairsData(18, 29, 19); // Act 1 - Catacombs Next S	
		toAndarial.RoomTransitions[i][294] = StairsData(18, 29, 38); // Act 1 - Catacombs Next N	
	}
	allTravelData.push_back(&toAndarial);


	toMephisto.Name = "Mephisto";
	toMephisto.InitSteps(1);
	toMephisto.WaypointDestination = WAYPOINTDEST_DuranceOfHateLevel2;
	toMephisto.RoomTransitions[0][788] = StairsData(67, 17, 12); // Act 3 - Mephisto Next W
	toMephisto.RoomTransitions[0][789] = StairsData(67, 12, 29); // Act 3 - Mephisto Next E
	toMephisto.RoomTransitions[0][790] = StairsData(67, 29, 12); // Act 3 - Mephisto Next S
	toMephisto.RoomTransitions[0][791] = StairsData(67, 7, 28); // Act 3 - Mephisto Next N
	allTravelData.push_back(&toMephisto);

	toBaal.Name = "Throne of destruction";
	toBaal.InitSteps(2);
	toBaal.WaypointDestination = WAYPOINTDEST_WorldstoneKeepLevel2;

	for (size_t i = 0; i < 2; i++)
	{
		toBaal.RoomTransitions[i][1078] = StairsData(82, 17, 18); // Act 3 - Mephisto Next W
		toBaal.RoomTransitions[i][1079] = StairsData(82, 17, 19); // Act 3 - Mephisto Next E
		toBaal.RoomTransitions[i][1080] = StairsData(82, 18, 17); // Act 3 - Mephisto Next S
		toBaal.RoomTransitions[i][1081] = StairsData(82, 19, 17); // Act 3 - Mephisto Next N
	}



	//toBloodRaven.Name = "Blood raven 1";
	//toBloodRaven.InitSteps(1);
	//toBloodRaven.WaypointDestination = WAYPOINTDEST_OuterCloister;
	//toBloodRaven.RoomTransitions[0][51] = StairsData(3, 15, 20); // Act 1 - Cave Entrance
	//allTravelData.push_back(&toBloodRaven);

	//toBloodRavenLair.Name = "Blood raven 2";
	//toBloodRavenLair.InitSteps(1);
	//toBloodRavenLair.WaypointDestination = WAYPOINTDEST_OuterCloister;
	//toBloodRavenLair.RoomTransitions[0][51] = StairsData(3, 15, 20); // Act 1 - Cave Entrance
	//toBloodRavenLair.RoomTransitions[0][91] = StairsData(5, 0, 0); // Act 1 - Cave Down W	
	//toBloodRavenLair.RoomTransitions[0][92] = StairsData(5, 0, 0); // Act 1 - Cave Down E	
	//toBloodRavenLair.RoomTransitions[0][93] = StairsData(5, 5, 34); // Act 1 - Cave Down S	
	//toBloodRavenLair.RoomTransitions[0][94] = StairsData(5, 16, 14); // Act 1 - Cave Down N	
	//allTravelData.push_back(&toBloodRavenLair);

	allTravelData.push_back(&toBaal);

	return TRUE;
}

DWORD EXPORT OnGamePacketBeforeSent(BYTE* aPacket, DWORD aLen)
{
	if (currentState == STATE_Idle)
	{
		return aLen;
	}

	//if(aPacket[0] == 0x13)
	//{
	//	DWORD unitType = *((DWORD *)&aPacket[1]);
	//	DWORD unitId = *((DWORD *)&aPacket[5]);
	//
	//	GAMEUNIT gu;
	//	gu.dwUnitID = unitId;
	//	gu.dwUnitType = unitType;
	//
	//	DWORD classId = server->GetUnitClassID(&gu);
	//	server->GameStringf("Interact with: transitions[%d] = %d;", server->D2GetCurrentRoomNum(), classId);
	//}

	if (aPacket[0] == 0x15 && aPacket[1] == 0x01)
	{
		char* chatMessage = (char*)(aPacket + 3);

		if (currentState == STATE_UsingWaypoint && strncmp(chatMessage, "�c5Waypoint�c0:", 15) == 0)
		{
			if (strcmp(chatMessage, "�c5Waypoint�c0: Complete") != 0)
			{
				Abort();
				return 0;
			}

			return 0;
		}
		else if (currentState == STATE_WaitingForBuffs && strncmp(chatMessage, "�c5BuffMe�c0:", 13) == 0)
		{
			const auto message = std::string_view(chatMessage + 14);
			if (message == "Done")
			{
				currentState = STATE_UsingWaypoint;
				server->GameCommandf("load wp");
				server->GameCommandf("wp start %d", currentTravelData->WaypointDestination);

				return 0;
			}

			return 0;
		}
	}

	return aLen;
}


VOID EXPORT OnGamePacketAfterReceived(BYTE* aPacket, DWORD aLen)
{
	if (currentState == STATE_Idle)
	{
		return;
	}

	//if(currentState == STATE_TeleportingToStairsRoom && aPacket[0] == 0x09 && aPacket[1] == 0x05)
	//{
	//	CheckForOurStairs();
	//}

	if (aPacket[0] == 0x15 && *((DWORD*)&aPacket[2]) == me->GetID())
	{
		if (currentState != STATE_TeleportingToStairsRoom && currentState != STATE_TeleportingToStairsObject)
		{
			return;
		}

		NextStep();
	}
	return;
}

VOID EXPORT OnThisPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	if (currentState == STATE_Idle)
	{
		return;
	}

	if (nMessage == PM_MOVECOMPLETE)
	{

		//server->GameStringf("PM_MOVECOMPLETE");
		//if(currentState == STATE_TeleportingToStairsObject)
		//{
		//	currentState = STATE_UsingStairs;
		//	me->Interact(&stairsGameUnit);
		//}
	}
	else if (nMessage == PM_MAPBLINK) {
		//server->GameStringf("PM_MAPBLINK");
		OnMapBlink();
	}
	else if (nMessage == PM_MAPCHANGE)
	{
		lastMapChange = std::chrono::system_clock::now();
		isLoadingMap = true;
		server->GameStringf("MapChange, delaying...");
	}
}


BYTE EXPORT OnGameKeyDown(BYTE iKeyCode)
{
	if (iKeyCode == VK_SPACE)
	{
		Abort();
	}
	return iKeyCode;
}


CLIENTINFO
(
	0, 1,
	"",
	"",
	"bot",
	""
)

MODULECOMMANDSTRUCT ModuleCommands[] =
{
	{
		"help",
		OnGameCommandHelp,
		"help�c0 List commands available in this module.\n"
		"<command> help�c0 Shows detailed help for <command> in this module."
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
