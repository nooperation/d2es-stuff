#include "../../Includes/D2Client.h"
#include "AutoJewel.h"
#include <string>
#include <sstream>
#include <fstream>

AutoJewel::AutoJewel()
{
	kRingItemCodes.insert("rin");
	kRingItemCodes.insert("zrn");
	kRingItemCodes.insert("srn");
	kRingItemCodes.insert("nrn");
	kRingItemCodes.insert("prn");
	kRingItemCodes.insert("brg");
	kRingItemCodes.insert("drn");
	kRingItemCodes.insert("arn");

	kRingItemCodes.insert("zam");
	kRingItemCodes.insert("sam");
	kRingItemCodes.insert("nam");
	kRingItemCodes.insert("pam");
	kRingItemCodes.insert("bam");
	kRingItemCodes.insert("dam");
	kRingItemCodes.insert("aam");

	kUniqueIdsWeCanDestroy.insert(410);
	kUniqueIdsWeCanDestroy.insert(411);
	kUniqueIdsWeCanDestroy.insert(412);
	kUniqueIdsWeCanDestroy.insert(413);
	kUniqueIdsWeCanDestroy.insert(414);
	kUniqueIdsWeCanDestroy.insert(1070);

	kUniqueIdsWeCanDestroy.insert(438); // Lovers' Pendant
	kUniqueIdsWeCanDestroy.insert(439);	// Eye of the Witch
	kUniqueIdsWeCanDestroy.insert(440);	// Collar of the Minotaur
	kUniqueIdsWeCanDestroy.insert(441);	// Beads of the Guardian
	kUniqueIdsWeCanDestroy.insert(442);	// Eye of the Beholder
	//kUniqueIdsWeCanDestroy.insert(443);	// Spellcasters Aide
	kUniqueIdsWeCanDestroy.insert(444);	// Talisman of Benzaiten
	kUniqueIdsWeCanDestroy.insert(445);	// Talisman of Fukurokuju
	kUniqueIdsWeCanDestroy.insert(446);	// Talisman of Ebisu
	kUniqueIdsWeCanDestroy.insert(447);	// Talisman of Daikokuten
	kUniqueIdsWeCanDestroy.insert(448);	// Talisman of Bishamonten
	kUniqueIdsWeCanDestroy.insert(449);	// Talisman of Hotei
	kUniqueIdsWeCanDestroy.insert(450);	// Talisman of Kisshoten
	kUniqueIdsWeCanDestroy.insert(451);	// Valhalla's Pendant
	kUniqueIdsWeCanDestroy.insert(452);	// Jade's Locket
	kUniqueIdsWeCanDestroy.insert(453);	// Daeveran's Stone
	kUniqueIdsWeCanDestroy.insert(454);	// Holy Deviant
	kUniqueIdsWeCanDestroy.insert(455);	// Khisanth
	kUniqueIdsWeCanDestroy.insert(456);	// Grim Creaper
	kUniqueIdsWeCanDestroy.insert(457);	// Sub Sanity
	// kUniqueIdsWeCanDestroy.insert(458); //Plague's Last Arrowhead
	// kUniqueIdsWeCanDestroy.insert(459); //The Mistress' Leather Collar
	// kUniqueIdsWeCanDestroy.insert(460); //The Cursed Donut
	// kUniqueIdsWeCanDestroy.insert(461); //BlackKnight's Holy Signet
	// kUniqueIdsWeCanDestroy.insert(462); //Smaqdoun's Painful Pride
	// kUniqueIdsWeCanDestroy.insert(463); //Yagul's Bloody Rage
	// kUniqueIdsWeCanDestroy.insert(464); //Dweia's Revenge
}

void AutoJewel::SetState(AutoJewelState newState)
{
	if (false)
	{
		const char *const stateNames[] = {
			"Uninitialized",
			"PickupRingFromInventory",
			"DropRingToCube",
			"TransmuteRingAndStocker",
			"PickupJewelFromCube",
			"DropJewelToInventory",
		};
		server->GameStringf("ÿc:AutoJewelÿc0: State %s -> %s", stateNames[(int)this->currentState], stateNames[(int)newState]);
	}

	this->currentState = newState;
}

bool AutoJewel::Start()
{
	SetState(AutoJewelState::Uninitialized);
	this->itemWaitingOn = 0;
	this->targetRingsToTransmute.clear();

	server->GameStringf("ÿc:AutoJewelÿc0: Starting");

	if (!OpenCube())
	{
		Abort();
		return false;
	}

	FindRecipe();

	return true;
}

void AutoJewel::FindRecipe()
{
	static const int kNumRingsToTransmute = 3;

	this->targetRingsToTransmute.clear();
	this->numRingsInCube = 0;
	this->ancientScrollId = 0;
	this->jewelIdToMoveFromCubeToInventory = 0;
	this->numTransmuteResultsWaitingFor = 0;

	me->EnumStorageItems(STORAGE_INVENTORY, enumItemProc, (LPARAM)this);
	if (!me->EnumStorageItems(STORAGE_CUBE, enumItemProcCube, (LPARAM)this)) {
		return;
	}

	if (this->ancientScrollId == 0)
	{
		server->GameStringf("Please put an ancient scroll 29 in your cube");
		Abort();
		return;
	}

	if (numRingsInCube == kNumRingsToTransmute)
	{
		// We have 3 valid rings and ancient scroll 29 in our cube, transmute them
		this->TransmuteRingAndStocker();
	}
	else if (numRingsInCube + this->targetRingsToTransmute.size() < kNumRingsToTransmute)
	{
		// We ran out of rings
		server->GameStringf("No more rings to transmute");
		Abort();
		return;
	}
	else 
	{
		PickupRingFromInventory();
	}
}

void AutoJewel::PickupRingFromInventory()
{
	if (this->targetRingsToTransmute.empty())
	{
		server->GameStringf("ÿc:AutoJewelÿc0: No more rings to move");
		Abort();
		return;
	}

	this->itemWaitingOn = targetRingsToTransmute.back();
	this->targetRingsToTransmute.pop_back();

	SetState(AutoJewelState::PickupRingFromInventory);
	if (!me->PickStorageItemToCursor(itemWaitingOn))
	{
		server->GameStringf("ÿc:AutoJewelÿc0: Failed to pick up ring");

		Abort();
		return;
	}
}

/// <summary>
/// Called whenever an item is pickedup to the cursor from the player's inventory
/// </summary>
/// <param name="item">Item picked up from the player's inventory.</param>
void AutoJewel::OnItemFromInventory(const ITEM& item)
{
	if (this->currentState == AutoJewelState::Uninitialized)
	{
		return;
	}

	const auto itemCode = std::string_view(item.szItemCode);

	if (this->currentState == AutoJewelState::PickupRingFromInventory)
	{
		if (!IsRing(itemCode))
		{
			server->GameStringf("ÿc:AutoJewelÿc0: I was expecting to pickup a ring, but got a %s instead", itemCode.data());
			return;
		}

		this->DropRingToCube();
		return;
	}
}

void AutoJewel::OnItemFromCube(const ITEM& item)
{
	if (this->currentState != AutoJewelState::PickupJewelFromCube)
	{
		return;
	}

	const auto itemCode = std::string_view(item.szItemCode);
	if (itemCode != "jew")
	{
		server->GameStringf("ÿc:AutoJewelÿc0: I was expecting to pickup a jewel, but got a %s instead", itemCode.data());
		return;
	}

	SetState(AutoJewelState::DropJewelToInventory);
	me->DropCursorItemToStorage(STORAGE_INVENTORY);
}

void AutoJewel::OnItemToInventory(const ITEM& item)
{
	if (this->currentState != AutoJewelState::DropJewelToInventory)
	{
		return;
	}

	const auto itemCode = std::string_view(item.szItemCode);

	if (itemCode != "jew")
	{
		server->GameStringf("ÿc:AutoJewelÿc0: I was expecting to drop a jewel, but got a %s instead", itemCode.data());
		return;
	}

	this->FindRecipe();
}

void AutoJewel::DropRingToCube()
{
	if (me->GetCursorItem() != this->itemWaitingOn)
	{
		server->GameStringf("ÿc:AutoJewelÿc0: DropRingToCube - Unknown cursor item. Expected our ring");
		return;
	}

	SetState(AutoJewelState::DropRingToCube);
	if (!me->DropCursorItemToStorage(STORAGE_CUBE))
	{
		server->GameStringf("ÿc:AutoJewelÿc0: Failed to drop the ring");

		Abort();
		return;
	}
}


/// <summary>
/// Called whenever an item is moved to the cube
/// </summary>
/// <param name="item">The item that was moved to the cube.</param>
void AutoJewel::OnItemToCube(const ITEM &item)
{
	if (this->currentState == AutoJewelState::Uninitialized)
	{
		return;
	}

	const auto itemCode = std::string_view(item.szItemCode);

	if (this->currentState == AutoJewelState::DropRingToCube)
	{
		if (!IsRing(itemCode))
		{
			server->GameStringf("ÿc:AutoJewelÿc0: Cube was expecting a ring, but got a %s instead", itemCode.data());
			return;
		}

		this->FindRecipe();
		return;
	}
	else if (currentState == AutoJewelState::TransmuteRingAndStocker)
	{
		--numTransmuteResultsWaitingFor;

		if (itemCode == "jew") {
			this->jewelIdToMoveFromCubeToInventory = item.dwItemID;
		}

		if (numTransmuteResultsWaitingFor == 0)
		{
			if (jewelIdToMoveFromCubeToInventory == 0) {
				server->GameStringf("We didn't get a jewel result from this transmute, something went wrong");
				Abort();
				return;
			}

			SetState(AutoJewelState::PickupJewelFromCube);
			me->PickStorageItemToCursor(this->jewelIdToMoveFromCubeToInventory);
		}
	}
}

void AutoJewel::TransmuteRingAndStocker()
{
	this->numTransmuteResultsWaitingFor = 2;

	SetState(AutoJewelState::TransmuteRingAndStocker);
	if (!me->Transmute())
	{
		server->GameStringf("ÿc:AutoJewelÿc0: Failed to transmute");
	
		Abort();
		return;
	}
}

bool AutoJewel::OpenCube()
{
	if (!me->OpenCube())
	{
		server->GameStringf("ÿc:AutoJewelÿc0: You must open your cube before using");

		Abort();
		return false;
	}

	return true;
}

bool AutoJewel::CheckCubeUI()
{
	if (!me->IsUIOpened(UI_CUBE))
	{
		server->GameStringf("ÿc:AutoJewelÿc0: Cube UI closed, aborting");
		Abort();
	}

	return true;
}

void AutoJewel::Abort()
{
	if (currentState != AutoJewelState::Uninitialized)
	{
		server->GameStringf("ÿc:AutoJewelÿc0: Done");
		SetState(AutoJewelState::Uninitialized);
	}
}

bool AutoJewel::IsRing(const std::string_view &itemCode) const
{
	return this->kRingItemCodes.find(itemCode.data()) != this->kRingItemCodes.end();
}

bool AutoJewel::IsOkayToDestroy(const ITEM &item) const {
	if (!this->IsRing(item.szItemCode)) 
	{
		return false;
	}

	if (item.iQuality != ITEM_QUALITY_UNIQUE)
	{
		return false;
	}

	if (item.iLocation != ITEM_LOCAIION_STORED) 
	{
		return false;
	}

	if (item.iSocketed)
	{
		return false;
	}

	if (item.iIsEquipped)
	{
		return false;
	}

	GAMEUNIT unit;
	unit.dwUnitID = item.dwItemID;
	unit.dwUnitType = UNIT_TYPE_ITEM;

	// Ignore any melded item
	const auto item_meldflag = server->GetUnitStat(&unit, 251);
	if (item_meldflag > 0)
	{
		return false;
	}

	// Ignore any *** Forged (Jewel) *** item
	const auto item_craftflag2 = server->GetUnitStat(&unit, 463);
	if (item_craftflag2 > 0)
	{
		return false;
	}

	// Ignore any *** Forged (Equipment) *** item
	const auto item_craftflag = server->GetUnitStat(&unit, 202);
	if (item_craftflag > 0)
	{
		return false;
	}

	// Ignore any item that has +level requirements
	const auto item_levelreq = server->GetUnitStat(&unit, 92);
	if (item_levelreq > 0)
	{
		return false;
	}

	// Keep those fedoras
	if (item.iPersonalized)
	{
		return false;
	}
	
	if (kUniqueIdsWeCanDestroy.find(item.wSetUniqueID) == kUniqueIdsWeCanDestroy.end()) 
	{
		return false;
	}

	return true;
}

void AutoJewel::ProcessInventoryItem(const ITEM *item)
{
	if (IsOkayToDestroy(*item)) 
	{
		this->targetRingsToTransmute.push_back(item->dwItemID);
	}
}

bool AutoJewel::ProcessCubeItem(const ITEM* item)
{
	if (strcmp(item->szItemCode, "29a") == 0)
	{
		this->ancientScrollId = item->dwItemID;
		return true;
	}
	else if (!IsOkayToDestroy(*item)) 
	{
		GAMEUNIT unit;
		unit.dwUnitID = item->dwItemID;
		unit.dwUnitType = UNIT_TYPE_ITEM;

		char itemNameBuffer[1024] = {};
		server->GetUnitName(&unit, itemNameBuffer, sizeof(itemNameBuffer) - 1);
		server->GameStringf("Please empty your cube, you current have a %s (%s) in it, which is not okay to destroy", item->szItemCode, itemNameBuffer);
		this->Abort();
		return false;
	}
	else {
		this->numRingsInCube++;
		return true;
	}
}

BOOL CALLBACK enumItemProc(LPCITEM item, LPARAM lParam)
{
	auto autoJewel = (AutoJewel *)lParam;
	autoJewel->ProcessInventoryItem(item);

	return TRUE;
}

BOOL CALLBACK enumItemProcCube(LPCITEM item, LPARAM lParam)
{
	auto autoJewel = (AutoJewel*)lParam;
	return autoJewel->ProcessCubeItem(item);
}
