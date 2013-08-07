#pragma once

#include "../../Includes/D2Client.h"

enum States
{
	STATE_IDLE = 0,
	STATE_AMMO_FROM_EQUIP,
	STATE_AMMO_TO_EQUIP,
	STATE_AMMO_TO_CUBE,
	STATE_AMMO_FROM_CUBE,
	STATE_TRANSMUTE,
};

class AmmoGuard
{
public:
	AmmoGuard();

	void ReloadAmmo();

	void Tick();
	void OnItemToCursor(DWORD itemId);
	void OnItemToCube(DWORD itemId);
	void OnItemToEquip(DWORD itemId);
	void Abort();
	
private:
	void OnComplete();

	DWORD ammoId;
	bool inventoryOpenAtStart;
	int ticksUntilNextCheck;
	int currentState;
};