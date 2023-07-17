#pragma once

#include <windows.h>
#include <vector>
#include <chrono>

#include "../../Includes/D2Client.h"

enum class State
{
    Uninitialized,
    Waiting,
    FollowingMaster,
    WalkingToPortal_LeavingTown,
    WalkingToPortal_EnteringTown,
    WalkingToStairs,
    WaitingForBuffToFinish,
};

struct PortalOwnershipData
{
    std::chrono::milliseconds lastUpdate;
    uint32_t ownerId;
    uint32_t portalId;
};

class Follow
{
public:
    Follow();

    void OnChatMessage(const std::string_view& from, const std::string_view& message);
    bool OnBuffMeMessage(const std::string_view& message);
    void OnPortalOwnershipUpdate(uint32_t ownerId, std::string_view ownerName, uint32_t unitId);
    void OnEnterTown();
    void OnLeaveTown();
    void OnMyDeath();
    void OnPlayerDeath(uint32_t playerId);
    void OnPlayerDisappear(uint32_t playerId);

    void OnTick();
    void Abort();

private:
    void SetState(State newState);
    void Reset();
    bool FindAndUsePortal();
    void BuffMe();

    bool fleeLoaded;
    bool followEnabled;

    State currentState;
    std::string master;
    GAMEUNIT masterUnit;
    GAMEUNIT portalWeAreWalkingTo;

    bool isBuffNeeded;
    State statePriorToBuffing;


    std::chrono::milliseconds lastFollowAttemptTimeMs;
    std::map<uint32_t, PortalOwnershipData> portalOwnershipMap;
};

BOOL CALLBACK enumFindCubeItems(LPCITEM item, LPARAM lParam);
BOOL CALLBACK enumFindOre(LPCITEM item, LPARAM lParam);
