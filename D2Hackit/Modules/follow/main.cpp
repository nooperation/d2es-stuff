#include "../../Includes/ClientCore.cpp"
#include "follow.h"

Follow follow;

CLIENTINFO
(
    1, 0,
    "Follow v1.0",
    "",
    "Follow v1.0",
    ""
)

BOOL PRIVATE Start(char** argv, int argc)
{

    if (argc != 3)
    {
        server->GameStringf("Usage: Follow <masterName>");
        return TRUE;
    }

   // auto master = std::string(argv[2]);
  //  follow.Start(master);

    return TRUE;
}


DWORD EXPORT OnGamePacketBeforeSent(BYTE* aPacket, DWORD aLen)
{
    return aLen;
}

struct Packet_SC_PortalOwnership
{
	DWORD OwnerID;
	char OwnerName[16];
	DWORD LocalID;
	DWORD RemoteID;
};




DWORD EXPORT OnGamePacketBeforeReceived(BYTE* aPacket, DWORD aLen)
{
    if (aPacket[0] == 0x26)
    {
        const auto source = std::string_view((char*)(&aPacket[10]));
        const auto message = std::string_view((char*)(&aPacket[10] + source.length() + 1));

        follow.OnChatMessage(source, message);
    }

    else if(aPacket[0] == 0xa7)
	{
		// No more tp delay :)
		if(aPacket[6] == 102)
		{
			return 0;
		}
	}
    
    // D2GS_PORTALOWNERSHIP
    if (aPacket[0] == 0x82)
    {
        const auto ownershipPacket = (Packet_SC_PortalOwnership *)&aPacket[1];

        //const auto ownerId= *((uint32_t *)&aPacket[1]);
        //const auto ownerName= std::string_view((const char *)&aPacket[5]);
        //const auto localId = *((uint32_t *)&aPacket[aLen - 8]);
        //const auto remoteId = *((uint32_t *)&aPacket[aLen - 4]);

        //server->GameStringf("Portal ownership: ownerId=%d ownerName=%s localId=%d", ownerId, ownerName.data(), localId);
        follow.OnPortalOwnershipUpdate(ownershipPacket->OwnerID, ownershipPacket->OwnerName, ownershipPacket->LocalID);
    }

    return aLen;
}

VOID EXPORT OnThisPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam)
{
    if (nMessage == PM_UICLOSED)
    {
        follow.Abort();
    }
    else if (nMessage == PM_LEAVETOWN)
    {
        follow.OnLeaveTown();
    }
    else if (nMessage == PM_ENTERTOWN)
    {
        follow.OnEnterTown();
    }
    else if (nMessage == PM_DEATH)
    {
        follow.OnMyDeath();
    }
}
VOID EXPORT OnUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam, LPARAM lParam)
{
	if(nMessage == UM_DEATH)
	{
        if (lpUnit->dwUnitType != UNIT_TYPE_PLAYER)
        {
            return;
        }

        follow.OnPlayerDeath(lpUnit->dwUnitID);
	}
    else if(nMessage == UM_DESTROY)
	{
        if (lpUnit->dwUnitType != UNIT_TYPE_PLAYER)
        {
            return;
        }

        follow.OnPlayerDisappear(lpUnit->dwUnitID);
	}
}


DWORD EXPORT OnGameTimerTick()
{
    // OnGameTimerTick happens after all the modules have processed OnGamePacketAfterReceived (which is when the OnUnitMessage above is called)
    follow.OnTick();
    return 0;
}

/*
void EXPORT OnGamePacketAfterReceived(BYTE *aPacket, DWORD aLen)
{
    if (aPacket == nullptr)
    {
        return;
    }

    if (aPacket[0] == 0x9c && aPacket[1] == ITEM_ACTION_DROP)
    {
        const auto itemId = *((DWORD *)&aPacket[4]);
        Follow.OnItemDropped(itemId);
    }
}*/

BYTE EXPORT OnGameKeyDown(BYTE iKeyCode)
{
    switch (iKeyCode)
    {
    case VK_SPACE:
        follow.Abort();
        break;
    }

    return iKeyCode;
}

MODULECOMMANDSTRUCT ModuleCommands[] =
{
    {
        "help",
        OnGameCommandHelp,
        "helpÿc0 List commands available in this module.\n"
        "<command> helpÿc0 Shows detailed help for <command> in this module."
    },
    {
        "Start",
        Start,
        "Usage: Start",
    },
    {NULL}
};


