#include "../../Includes/D2Client.h"
#include "Follow.h"

BOOL CALLBACK enumObjectsForPortal(LPCGAMEUNIT lpUnit, LPARAM lParam);
BOOL CALLBACK FindMaster(DWORD dwPlayerID, LPCSTR lpszPlayerName, DWORD dwPlayerClass, DWORD dwPvpFlags, BYTE iMapID, LPARAM lParam);


Follow::Follow()
{
    this->currentState = State::Uninitialized;
    this->fleeLoaded = false;
    this->master = "";
    this->portalOwnershipMap.clear();
}

void Follow::SetState(State newState)
{
    if (false)
    {
        const char* const stateNames[] = {
            "Uninitialized",
            "PickupNextItemToDrop",
            "DropNextItemToDrop",
            "PickupNextOre",
            "DropNextOreToCube",
            "WaitingToRunAutoExtractor",
            "RunAutoExtractor",
            "RunEmptyCube",
            "RunAutoStocker",
        };
        server->GameStringf("ÿc5Followÿc0: State %s -> %s", stateNames[(int)this->currentState], stateNames[(int)newState]);
    }

    this->currentState = newState;
}

void Follow::Abort()
{
    server->GameStringf("ÿc5Followÿc0: Aborted");
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

        server->EnumPlayers(FindMaster, (LPARAM)&payload);

        if (!payload.isSuccess)
        {
            server->GameStringf("ÿc5Followÿc0: Failed to find master '%s'", from.data());
            me->Say("I cannot find you. Please rejoin or something");
            return;
        }

        this->master = payload.masterName;
        this->masterId = payload.masterId;

        server->GameStringf("ÿc5Followÿc0: Now following %s (id = %d)", this->master.c_str(), this->masterId);
        char buff[128] = {};
        sprintf_s(buff, "Ok, now following %s", this->master.c_str());
        me->Say(&buff[0]);
    }


    if (from != this->master)
    {
        return;
    }

    if (message == "flee" || message == "return to town")
    {
        if (!me->IsInTown())
        {
            if (!this->FindAndUsePortal())
            {
                server->GameCommandf("flee tp");
            }
        }
    }
    else if (message == "flee!")
    {
        server->GameCommandf("flee tp");
    }
    else if (message == "tp" || message == "take my portal")
    {
        if (me->IsInTown)
        {
            this->FindAndUsePortal();
        }
    }
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


    server->EnumPlayers(FindMaster, 0);

    for (auto const& portalId : foundPortals)
    {
        auto const& portalInfo = this->portalOwnershipMap.find(portalId);
        if (portalInfo == this->portalOwnershipMap.end())
        {
            server->GameStringf("ÿc5Followÿc0: Unknown portal. Skipping");
            continue;
        }

        if (portalInfo->second == this->masterId)
        {
            server->GameStringf("ÿc5Followÿc0: Found a portal owned by our master, entering it...");

            Packet_CS_InteractWithEntity interact;
            interact.EntityKind = 0x02;
            interact.ID = portalInfo->first;

            BYTE interactPacket[9];
            interactPacket[0] = 0x13;
            memcpy(&interactPacket[1], &interact, sizeof(Packet_CS_InteractWithEntity));
            if (me->GetHPPercent() > 0)
            {
                server->GameSendPacketToServer(&interactPacket[0], sizeof(interactPacket));
            }

            return true;
        }
    }

    return false;
}

void Follow::OnPortalOwnershipUpdate(uint32_t ownerId, std::string_view ownerName, uint32_t unitId)
{
    this->portalOwnershipMap[unitId] = ownerId;
}

void Follow::StartFollow()
{
}

void Follow::OnTick()
{

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