#pragma once 

#include <cstdint>

#pragma pack()

enum D2MonSeqEvent : uint8_t
{
	MONSEQ_EVENT_NONE,
	MONSEQ_EVENT_MELEE_ATTACK,
	MONSEQ_EVENT_MISSILE_ATTACK,
	MONSEQ_EVENT_PLAY_SOUND,
	MONSEQ_EVENT_TRIGGER_SKILL,

};

struct D2MonSeqTxt
{
	uint16_t wSequence;					//0x00
	uint8_t nMode;						//0x02
	uint8_t nFrame;						//0x03
	uint8_t nDir;						//0x04
	D2MonSeqEvent nEvent;				//0x05
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

struct D2SeqRecordStrc
{
	D2MonSeqTxt* pMonSeqTxtRecord;		//0x00
	int32_t nSeqFramesCount;			//0x04
	int32_t nFramesCount;				//0x08
};

//TODO: Find a better name
struct D2PlayerWeaponSequencesStrc
{
	D2SeqRecordStrc weaponRecords[NUM_WEAPON_CLASSES];		//0x00
};

#pragma pack()
