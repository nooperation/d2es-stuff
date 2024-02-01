#pragma once

#include <cstdint>
#include "CommonDefinitions.h"
#include "D2Inventory.h"

#pragma pack(1)

enum D2Composits
{
	COMPOSIT_HEAD,
	COMPOSIT_TORSO,
	COMPOSIT_LEGS,
	COMPOSIT_RIGHTARM,
	COMPOSIT_LEFTARM,
	COMPOSIT_RIGHTHAND,
	COMPOSIT_LEFTHAND,
	COMPOSIT_SHIELD,
	COMPOSIT_SPECIAL1,
	COMPOSIT_SPECIAL2,
	COMPOSIT_SPECIAL3,
	COMPOSIT_SPECIAL4,
	COMPOSIT_SPECIAL5,
	COMPOSIT_SPECIAL6,
	COMPOSIT_SPECIAL7,
	COMPOSIT_SPECIAL8,
	NUM_COMPONENTS,
};

enum D2WeaponClasses
{
	WEAPONCLASS_HTH, // Hand To Hand (Bare fists)
	WEAPONCLASS_BOW, // Bow (All Bow class weapons)
	WEAPONCLASS_1HS, // One-Handed: Swing (axes, swords, flails, maces, hammers, clubs and wands)
	WEAPONCLASS_1HT, // One-Handed: Thrust (Daggers, javelins, potions)
	WEAPONCLASS_STF, // Staff (Staves, polearms, axes, mauls)
	WEAPONCLASS_2HS, // Two-Handed: Swing (swords)
	WEAPONCLASS_2HT, // Two-Handed: Thrust (All Spear class weapons)
	WEAPONCLASS_XBW, // Crossbow (All Crossbow class weapons)
	WEAPONCLASS_1JS, // 2 x One-handed: left Jab  , right Swing
	WEAPONCLASS_1JT, // 2 x One-handed: left Jab  , right Thrust
	WEAPONCLASS_1SS, // 2 x One-handed: left Swing, right Swing
	WEAPONCLASS_1ST, // 2 x One-handed: left Swing, right Thrust
	WEAPONCLASS_HT1, // One Hand To hand (Katar, Claw, Scissors type assassin class weapons)
	WEAPONCLASS_HT2, // Two Hand To hand (Katar, claw, scissors type assassin class weapons)
	NUM_WEAPON_CLASSES,

};

struct D2CompositStrc
{
	int32_t nWeaponClassCode;					//0x00
	int32_t nWeaponClassId;						//0x04
};

#pragma pack()

