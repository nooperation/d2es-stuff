#include "../../Includes/D2Client.h"
#include "../../Core/definitions.h"

#include "Follow.h"

#include <chrono>

BOOL CALLBACK enumObjectsForPortal(LPCGAMEUNIT lpUnit, LPARAM lParam);
BOOL CALLBACK FindMaster(DWORD dwPlayerID, LPCSTR lpszPlayerName, DWORD dwPlayerClass, DWORD dwPvpFlags, BYTE iMapID, LPARAM lParam);


Follow::Follow()
{
    Reset();
}

void Follow::Reset()
{
    this->currentState = State::Uninitialized;
    this->fleeLoaded = false;
    this->portalOwnershipMap.clear();
    this->master = "";
    this->masterUnit.dwUnitID = 0;
    this->masterUnit.dwUnitType = UNIT_TYPE_PLAYER;
    this->portalWeAreWalkingTo.dwUnitID = 0;
    this->portalWeAreWalkingTo.dwUnitType = UNIT_TYPE_OBJECT;
    this->isBuffNeeded = false;
    this->statePriorToBuffing = State::Uninitialized;
}

void Follow::SetState(State newState)
{
    this->currentState = newState;
}

void Follow::Abort()
{
    server->GameStringf("ÿc5Followÿc0: Aborted");
}

void Follow::OnEnterTown()
{
    this->SetState(State::Waiting);
}

void Follow::OnLeaveTown()
{
    this->SetState(State::FollowingMaster);
}

void Follow::OnPlayerDeath(uint32_t playerId)
{
    OnPlayerDisappear(playerId);
}

void Follow::OnMyDeath()
{
    Reset();
    me->Say("I have died and no longer have a master set");
}

void Follow::OnPlayerDisappear(uint32_t playerId)
{
    if (playerId != this->masterUnit.dwUnitID)
    {
        return;
    }

    if (me->IsInTown())
    {
        return;
    }

    bool isMasterPortalAvailable = false;
    for (auto const& item : this->portalOwnershipMap)
    {
        if (item.second.ownerId != this->masterUnit.dwUnitID)
        {
            continue;
        }

        GAMEUNIT mastersPortal;
        mastersPortal.dwUnitID = item.second.portalId;
        mastersPortal.dwUnitType = UNIT_TYPE_OBJECT;

        if (!server->VerifyUnit(&mastersPortal))
        {
            continue;
        }

        isMasterPortalAvailable = true;
        break;
    }

    if (isMasterPortalAvailable)
    {
        server->GameStringf("Master disappeared, but left us a portal. We're following it");
        this->FindAndUsePortal();
    }
    else
    {
        // TODO: Find and use nearby stairs if master is not dead
        SetState(State::Uninitialized);
        me->Say("Master disappeared and I couldn't find a portal so I'm going home");
        server->GameCommandf("flee tp");
    }
}

struct FindMasterPayload
{
    std::string masterName;
    uint32_t masterId;
    bool isSuccess;
};
void Follow::OnChatMessage(const std::string_view& from, const std::string_view& message)
{
    if (message == "follow")
    {
        FindMasterPayload payload;
        payload.isSuccess = false;
        payload.masterId = 0;
        payload.masterName = from;

        this->masterUnit.dwUnitID = 0;
        this->masterUnit.dwUnitType = UNIT_TYPE_PLAYER;

        server->EnumPlayers(FindMaster, (LPARAM)&payload);

        if (!payload.isSuccess)
        {
            server->GameStringf("ÿc5Followÿc0: Failed to find master '%s'", from.data());
            me->Say("I cannot find you. Please rejoin or something");
            return;
        }

        this->master = payload.masterName;
        this->masterUnit.dwUnitID = payload.masterId;

        server->GameStringf("ÿc5Followÿc0: Now following %s (id = %d)", this->master.c_str(), this->masterUnit.dwUnitID);
        char buff[128] = {};
        sprintf_s(buff, "Ok, now following %s", this->master.c_str());
        me->Say(&buff[0]);

        SetState(State::FollowingMaster);
    }


    if (from != this->master)
    {
        return;
    }

    if (message == "flee")
    {
        if (!me->IsInTown())
        {
            if (!this->FindAndUsePortal())
            {
                server->GameCommandf("flee tp");
            }
        }
    }
    else if (message == "stop")
    {
        this->SetState(State::Waiting);
    }
    else if (message == "flee!")
    {
        server->GameCommandf("flee tp");
    }
    else if (message == "tp")
    {
        if (me->IsInTown)
        {
            this->FindAndUsePortal();
        }
    }
    else if (message == "buff")
    {
        BuffMe();
    }
}

void Follow::BuffMe()
{
    statePriorToBuffing = currentState;
    SetState(State::WaitingForBuffToFinish);
    server->GameCommandf("buff buff 1");
}

bool Follow::OnBuffMeMessage(const std::string_view& message)
{
    if (message == "Rebuff needed")
    {
        isBuffNeeded = true;
        return false;
    }

    if (message == "Done")
    {
        isBuffNeeded = false;
        this->SetState(statePriorToBuffing);
        return false;
    }

    if (currentState != State::WaitingForBuffToFinish)
	{
		return false;
	}

    return true;
}

struct FindPortalResult
{
    DWORD unitId;
    DWORD masterId;
    bool isSuccessful;
};

struct Packet_CS_InteractWithEntity
{
    DWORD EntityKind;
    DWORD ID;
};

bool Follow::FindAndUsePortal()
{
    std::vector<uint32_t> foundPortals;

    server->EnumUnits(UNIT_TYPE_OBJECT, enumObjectsForPortal, (LPARAM)&foundPortals);
    if (foundPortals.empty())
    {
        server->GameStringf("ÿc5Followÿc0: We could not find any portals");
        return false;
    }

    for (auto const& portalId : foundPortals)
    {
        auto const& portalInfo = this->portalOwnershipMap.find(portalId);
        if (portalInfo == this->portalOwnershipMap.end())
        {
            server->GameStringf("ÿc5Followÿc0: Unknown portal. Skipping");
            continue;
        }

        if (portalInfo->second.ownerId == this->masterUnit.dwUnitID)
        {
            server->GameStringf("ÿc5Followÿc0: Found a portal owned by our master, entering it...");

            this->portalWeAreWalkingTo.dwUnitID = portalId;
            this->portalWeAreWalkingTo.dwUnitType = UNIT_TYPE_OBJECT;

            me->MoveToUnit(&this->portalWeAreWalkingTo, false);
            if (me->IsInTown)
            {
                SetState(State::WalkingToPortal_LeavingTown);
            }
            else
            {
                SetState(State::WalkingToPortal_EnteringTown);
            }

            return true;
        }
    }

    return false;
}

void Follow::OnPortalOwnershipUpdate(uint32_t ownerId, std::string_view ownerName, uint32_t unitId)
{
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

    auto &portalInfo = this->portalOwnershipMap[unitId];
    portalInfo.lastUpdate = now;
    portalInfo.ownerId = ownerId;
    portalInfo.portalId = unitId;
}

void Follow::OnTick()
{
    static const uint32_t kFollowAttemptFrequencyMs = 200;
    static const uint32_t kMaxInteractionDistance = 8;

    if (currentState == State::WaitingForBuffToFinish || currentState == State::Waiting)
    {
        return;
    }

    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    if ((now - this->lastFollowAttemptTimeMs).count() < kFollowAttemptFrequencyMs)
    {
        return;
    }
    this->lastFollowAttemptTimeMs = now;

    if (currentState == State::WalkingToPortal_LeavingTown || currentState == State::WalkingToPortal_EnteringTown)
    {
        const auto portalLocation = server->GetUnitPosition(&this->portalWeAreWalkingTo);
        
        if (me->GetDistanceFrom(portalLocation.x, portalLocation.y) < kMaxInteractionDistance)
        {
            me->Interact(&this->portalWeAreWalkingTo);
        }
    }

    if (currentState == State::FollowingMaster)
    {
        if (this->masterUnit.dwUnitID == 0)
        {
            return;
        }
    
        me->MoveToUnit(&this->masterUnit, false);
    }

    if (isBuffNeeded)
    {
        BuffMe();
    }
}

BOOL CALLBACK FindMaster(DWORD dwPlayerID, LPCSTR lpszPlayerName, DWORD dwPlayerClass, DWORD dwPvpFlags, BYTE iMapID, LPARAM lParam)
{
    auto payload = (FindMasterPayload *)lParam;
    if (lpszPlayerName == payload->masterName)
    {
        payload->masterId = dwPlayerID;
        payload->isSuccess = true;
        return FALSE;
    }

    return TRUE;
}

BOOL CALLBACK enumObjectsForPortal(LPCGAMEUNIT lpUnit, LPARAM lParam)
{
    auto foundPortals = (std::vector<uint32_t>*)lParam;

    const auto unitClassId = server->GetUnitClassID(lpUnit);
    if (unitClassId != 59)
    {
        return TRUE;
    }

    foundPortals->push_back(lpUnit->dwUnitID);
    return TRUE;
}