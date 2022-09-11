#pragma once

#include <windows.h>
#include <vector>

#include "../../Includes/D2Client.h"

enum class State
{
    Uninitialized,
    PickupNextItemToDrop,
    DropNextItemToDrop,
    PickupNextOre,
    DropNextOreToCube,
    WaitingToRunAutoExtractor,
    RunAutoExtractor,
    RunEmptyCube,
    RunAutoStocker,
};

class Follow
{
public:
    Follow();

    void OnChatMessage(const std::string_view& from, const std::string_view& message);
    void UpdatePlayerInfo(DWORD playerId, const std::string_view &playerName);
    void OnPortalOwnershipUpdate(uint32_t ownerId, std::string_view ownerName, uint32_t unitId);

    void OnTick();
    void Abort();

    void OnItemDropped(DWORD itemId);

private:
    void SetState(State newState);

    bool FindAndUsePortal();
    void StartFollow();

    bool fleeLoaded;

    State currentState;
    std::string master;
    DWORD masterId;

    std::map<DWORD, std::string> playerIdToNameMap;
    std::map<uint32_t, uint32_t> portalOwnershipMap;
};

BOOL CALLBACK enumFindCubeItems(LPCITEM item, LPARAM lParam);
BOOL CALLBACK enumFindOre(LPCITEM item, LPARAM lParam);
