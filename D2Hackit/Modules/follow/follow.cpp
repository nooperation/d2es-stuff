#include "../../Includes/D2Client.h"
#include "Follow.h"

BOOL CALLBACK enumObjectsForPortal(LPCGAMEUNIT lpUnit, LPARAM lParam);


Follow::Follow()
{
    this->currentState = State::Uninitialized;
    this->fleeLoaded = false;
    this->master = "";
    this->playerIdToNameMap.clear();
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

void Follow::OnChatMessage(const std::string_view& from, const std::string_view& message)
{
    if (message == "follow")
    {
        this->master = from;
        this->masterId = 0;

        for (const auto& item : this->playerIdToNameMap)
        {
            if (item.second == this->master)
            {
                this->masterId = item.first;
                break;
            }
        }

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

void Follow::UpdatePlayerInfo(DWORD playerId, const std::string_view& playerName)
{
    if (playerName == this->master && playerId != this->masterId)
    {
        this->masterId = playerId;
    }

    this->playerIdToNameMap[playerId] = playerName;
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