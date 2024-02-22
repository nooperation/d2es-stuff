#pragma once 

#include <cstdint>
#include "../D2Composit.h"

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
