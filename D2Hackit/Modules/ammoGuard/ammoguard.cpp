#include "ammoguard.h"

AmmoGuard::AmmoGuard()
{
	ticksUntilNextCheck = 0;
	ammoId = 0;
	currentState = STATE_IDLE;
	inventoryOpenAtStart = false;
}

void AmmoGuard::ReloadAmmo()
{
	LPCITEM leftHandItem = me->GetEquipItem(EQUIP_LEFT_PRIMARY);

	if(leftHandItem == NULL)
	{
		server->GameStringf("ÿc5AmmoGuardÿc0: Unable to find item in ammo slot");
		return;
	}

	if(me->GetStat(STAT_AMMOQUANTITY) <= 0)
	{
		server->GameStringf("ÿc5AmmoGuardÿc0: Item has no quantity");
		return;
	}

	if(leftHandItem->iEthereal)
	{
		server->GameStringf("ÿc5AmmoGuardÿc0: Cannot reload ethereal items");
		return;
	}

	if(!me->OpenCube())
	{
		server->GameStringf("ÿc5AmmoGuardÿc0: Unable to open cube");
		return;
	}

	inventoryOpenAtStart = me->IsUIOpened(UI_INVENTORY) == TRUE;

	ammoId = leftHandItem->dwItemID;
	currentState = STATE_AMMO_FROM_EQUIP;

	// 1C 05 00 -> pickup EQUIP_LEFT_PRIMARY
	BYTE aPacket[3] = {0x1C, 0x05, 0x00};
	server->GameSendPacketToServer(aPacket, 3);
}

void AmmoGuard::OnItemToCursor(DWORD itemId)
{
	if(itemId != ammoId)
		return;

	if(currentState == STATE_AMMO_FROM_EQUIP)
	{
		currentState = STATE_AMMO_TO_CUBE;
		me->DropItemToStorage(STORAGE_CUBE, itemId);
	}
	else if(currentState == STATE_AMMO_FROM_CUBE)
	{
		currentState = STATE_AMMO_TO_EQUIP;

		// Equip item to EQUIP_LEFT_PRIMARY
		BYTE aPacket[9] = {0x1A, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00};
		memcpy(aPacket+1, &ammoId, 4);
		server->GameSendPacketToServer(aPacket, 9);
	}
}

void AmmoGuard::OnItemToCube(DWORD itemId)
{
	if(currentState == STATE_AMMO_TO_CUBE)
	{
		if(itemId != ammoId)
		{
			server->GameStringf("ÿc5AmmoGuardÿc0: Unknown item placed in cube, aborting");
			currentState = STATE_IDLE;
			OnComplete();
			return;
		}

		currentState = STATE_TRANSMUTE;
		me->Transmute();
	}
	else if(currentState == STATE_TRANSMUTE)
	{
		ammoId = itemId;

		currentState = STATE_AMMO_FROM_CUBE;
		me->PickStorageItemToCursor(itemId);
	}
}

void AmmoGuard::OnItemToEquip(DWORD itemId)
{
	if(currentState == STATE_AMMO_TO_EQUIP)
	{
		currentState = STATE_IDLE;
		OnComplete();
	}
}

void AmmoGuard::Abort()
{
	if(currentState != STATE_IDLE)
	{
		currentState = STATE_IDLE;
		OnComplete();
	}
}

void AmmoGuard::OnComplete()
{
	ammoId = 0;

	if(!inventoryOpenAtStart)
	{
		// This is too annoying
		//me->CloseAllUIs();
	}

	server->GameStringf("ÿc5AmmoGuardÿc0: Complete");
}

void AmmoGuard::Tick()
{
	if(!me->IsInTown() && ++ticksUntilNextCheck >= 2 * (1000 / server->GetTickRate()))
	{
		if(me->GetStat(STAT_AMMOQUANTITY) < 80 && me->GetStat(STAT_AMMOQUANTITY) > 0)
		{
			if(me->GetStat(STAT_AMMOQUANTITY) < 40)
			{
				me->OverHead("ÿc1Ammo count < 40, returning to town", TRUE);
				me->CleanJobs();
				me->BackToTown();
			}
			else if(me->GetStat(STAT_AMMOQUANTITY) < 60)
			{
				server->GamePrintInfo("ÿc1Ammo *REALLY* low!");
			}
			else
			{
				server->GamePrintInfo("ÿc1Ammo running low!");
			}
		}

		ticksUntilNextCheck = 0;
	}
}