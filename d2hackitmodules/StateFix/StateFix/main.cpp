#include "../../d2hackit/includes/ClientCore.cpp"
#include <stdio.h>

#include "BitField.h"
#include "ItemStatCost.h"

CLIENTINFO
(
	1,0,
	"State Fix v2.0",
	"",
	"State Fix v2.0",
	""
)
// 0xAA crash 25 states
// unsigned char packet[] = {0xAA,0x00,0x02,0x00,0x00,0x00,0xFB,0x03,0x49,0xC0,0x40,0x01,0xC1,0x11,0x2A,0x29,0x21,0x78,0x25,0xF3,0x95,0xE5,0x3F,0x44,0x18,0xAE,0x06,0x31,0x70,0x0D,0xEE,0x38,0xC6,0x7F,0x8D,0xAB,0xAC,0x03,0xFE,0x7B,0xF4,0xAA,0xE3,0xAB,0xCD,0x7F,0x90,0x4C,0x2A,0x01,0x9A,0x54,0x02,0xFC,0x2F,0xB9,0x0A,0x23,0xC0,0x2B,0x9A,0xAF,0x0D,0xFF,0x61,0x6E,0x10,0x0E,0x48,0x03,0xC1,0x2B,0x9E,0xAF,0x07,0xFF,0x67,0xFE,0x09,0xF8,0x7F,0x34,0x29,0x42,0x78,0xED,0xF1,0x95,0xE6,0x3F,0x52,0xA5,0xF8,0xE0,0xF5,0xC7,0x57,0x98,0xFF,0x4B,0x93,0x16,0x80,0xD2,0x02,0x58,0x5A,0x00,0x4C,0x0B,0xC0,0xEB,0xE5,0xAF,0x23,0xE2,0x07,0xE0,0x7F,0x53,0xAF,0xBE,0xBF,0x8E,0xFC,0xC7,0xBA,0x6A,0x2D,0xA0,0x69,0x00,0xAF,0x32,0xBE,0xCA,0xFC,0x4F,0xCB,0xCA,0xFF,0x95,0x99,0x28,0x04,0x78,0x55,0xF9,0x55,0xE5,0x3F,0x66,0xAB,0x2E,0xCE,0xB9,0xEA,0x62,0x0F,0xAC,0x2E,0x1C,0xBC,0xF2,0xFB,0x5A,0xF2,0xFF,0x35,0x0C,0xAE,0xAA,0x18,0x6D,0x55,0x5E,0x8F,0x7F,0xFD,0xF8,0x9F,0x3B,0x03,0x05,0x30,0xA1,0x00,0xF0,0x4A,0xFA,0x6B,0xC9,0xFF,0xE7,0x55,0x24,0xFC,0x11,0xF0,0x9A,0xF7,0x2B,0xC3,0x7F,0xEB,0x24,0x9C,0xAC,0x12,0x08,0xF0,0x8A,0xF8,0xEB,0xC8,0xFF,0xEF,0xC8,0x08,0xE7,0xB5,0xF2,0xD7,0x91,0xFF,0xF2,0x2D,0x6C,0x00,0x60,0xB8,0x01,0x00,0xB5,0xF1,0x6A,0xF9,0xEB,0x88,0x74,0x32,0x4C,0x27,0xC3,0xFF,0xFD,0x5E,0x7B,0x7F,0x1D,0x01};
// unsigned char packet[] = {0xAA,0x00,0x01,0x00,0x00,0x00,0xFB,0x03,0x49,0xC0,0x40,0x01,0xC1,0x11,0x2A,0x29,0x21,0x78,0x25,0xF3,0x95,0xE5,0x3F,0x44,0x18,0xAE,0x06,0x31,0x70,0x0D,0xEE,0x38,0xC6,0x7F,0x8D,0xAB,0xAC,0x03,0xFE,0x7B,0xF4,0xAA,0xE3,0xEB,0xCB,0x7F,0x90,0x4C,0xCE,0x00,0x9A,0x9C,0x01,0xFC,0x2F,0xB9,0xAA,0x2D,0xC0,0x2B,0x9A,0xAF,0x08,0xFF,0x61,0x6E,0x10,0x0E,0x48,0x03,0xC1,0x2B,0x9E,0xAF,0x07,0xFF,0x67,0xFE,0x09,0xF8,0x7F,0x34,0x29,0x42,0x78,0xED,0xF1,0x95,0xE6,0x3F,0x52,0xA5,0xF8,0xE0,0xF5,0xC7,0x57,0x98,0xFF,0x4B,0x93,0x16,0x80,0xD2,0x02,0x58,0x5A,0x00,0x4C,0x0B,0xC0,0xEB,0xE5,0xAF,0x23,0xE2,0x07,0xE0,0x7F,0x53,0xAF,0xBE,0xBF,0x8E,0xFC,0xC7,0xBA,0x6A,0x2D,0xA0,0x69,0x00,0xAF,0x32,0xBE,0xCA,0xFC,0x4F,0xCB,0xCA,0xFF,0x95,0x99,0x28,0x04,0x78,0x55,0xF9,0x55,0xE5,0x3F,0x66,0xAB,0x2E,0xCE,0xB9,0xEA,0x62,0x0F,0xAC,0x2E,0x1C,0xBC,0xF2,0xFB,0x5A,0xF2,0xFF,0x35,0x8C,0x15,0xAA,0x18,0x3C,0x54,0x5E,0x8F,0x7F,0xFD,0xF8,0x9F,0x3B,0x03,0x05,0x30,0xA1,0x00,0xF0,0x4A,0xFA,0x6B,0xC8,0xFF,0xE7,0x55,0x24,0xFC,0x11,0xF0,0x9A,0xF7,0x2B,0xC3,0x7F,0xEB,0x24,0x9C,0xAC,0x12,0x08,0xF0,0x8A,0xF8,0xEB,0xC8,0xFF,0xEF,0xC8,0x08,0xE7,0xB5,0xF2,0xD7,0x91,0xFF,0xF2,0x2D,0x6C,0x00,0x60,0xB8,0x01,0x00,0xB5,0xF1,0x6A,0xF9,0xEB,0x88,0x74,0x32,0x4C,0x27,0xC3,0xFF,0xFD,0x5E,0x7B,0x7F,0x1D,0x01};

// maphack version ?
// unsigned char packet[] = {0xAA,0x00,0x02,0x00,0x00,0x00,0xF8,0x03,0x49,0xC0,0x40,0x01,0xC1,0x11,0x2A,0x29,0x21,0x78,0x25,0xF3,0x95,0xE5,0x3F,0x44,0x18,0xAE,0x06,0x31,0x70,0x0D,0xEE,0x38,0xC6,0x7F,0x8D,0xAB,0xAC,0x03,0xFE,0x7B,0xF4,0xAA,0xE3,0xAB,0xCD,0x7F,0x90,0x4C,0x2A,0x01,0x9A,0x54,0x02,0xFC,0x87,0xB9,0x41,0x38,0x20,0x0D,0x04,0xAF,0x78,0xBE,0x1E,0xFC,0x9F,0xF9,0x27,0xE0,0xFF,0xD1,0xA4,0x08,0xE1,0xB5,0xC7,0x57,0x9A,0xFF,0x48,0x95,0xE2,0x83,0xD7,0x1F,0x5F,0x61,0xFE,0x2F,0x4D,0x5A,0x00,0x4A,0x0B,0x60,0x69,0x01,0x30,0x2D,0x00,0xAF,0x97,0xBF,0x8E,0x88,0x1F,0x80,0xFF,0x4D,0xBD,0xFA,0xFE,0x3A,0xF2,0x1F,0xEB,0xAA,0xB5,0x80,0xA6,0x01,0xBC,0xCA,0xF8,0x2A,0xF3,0x3F,0x2D,0x2B,0xFF,0x57,0x66,0xA2,0x10,0xE0,0x55,0xE5,0x57,0x95,0xFF,0x98,0xAD,0xBA,0x38,0xE7,0xAA,0x8B,0x3D,0xB0,0xBA,0x70,0xF0,0xCA,0xEF,0x6B,0xC9,0xFF,0xD7,0x30,0xB8,0xAA,0x62,0xB4,0x55,0x79,0x3D,0xFE,0xF5,0xE3,0x7F,0xEE,0x0C,0x14,0xC0,0x84,0x02,0xC0,0x2B,0xE9,0xAF,0x25,0xFF,0x9F,0x57,0x91,0xF0,0x47,0xC0,0x6B,0xDE,0xAF,0x0C,0xFF,0xAD,0x93,0x70,0xB2,0x4A,0x20,0xC0,0x2B,0xE2,0xAF,0x23,0xFF,0xBF,0x23,0x23,0x9C,0xD7,0xCA,0x5F,0x47,0xFE,0xCB,0xB7,0xB0,0x01,0x80,0xE1,0x06,0x00,0xD4,0xC6,0xAB,0xE5,0xAF,0x23,0xD2,0xC9,0x30,0x9D,0x0C,0xFF,0xF7,0x7B,0xED,0xFD,0x75,0x64,0xFC,0x00,0x98,0x57,0x80,0xFF,0xFF};
// unsigned char packet[] = {0xAA,0x00,0x01,0x00,0x00,0x00,0xF7,0x03,0x49,0xC0,0x40,0x01,0xC1,0x11,0x2A,0x29,0x21,0x78,0x25,0xF3,0x95,0xE5,0x3F,0x44,0x18,0xAE,0x06,0x31,0x70,0x0D,0xEE,0x38,0xC6,0x7F,0x8D,0xAB,0xAC,0x03,0xFE,0x7B,0xF4,0xAA,0xE3,0xEB,0xCB,0x7F,0x90,0x4C,0xCE,0x00,0x9A,0x9C,0x01,0xFC,0x2F,0xB9,0xAA,0x2D,0xC0,0x2B,0x9A,0xAF,0x08,0xFF,0x61,0x6E,0x10,0x0E,0x48,0x03,0xC1,0x2B,0x9E,0xAF,0x07,0xFF,0x67,0xFE,0x09,0xF8,0x7F,0x34,0x29,0x40,0x78,0xED,0xF1,0x55,0xE6,0x3F,0x52,0xA5,0xF0,0xE0,0xF5,0xC7,0x57,0x97,0xFF,0x4B,0x93,0x16,0x80,0xD2,0x02,0x58,0x5A,0x00,0x4C,0x0B,0xC0,0xEB,0xE5,0xAF,0x21,0xE2,0x07,0xE0,0x7F,0x53,0xAF,0xBE,0xBF,0x86,0xFC,0xC7,0xBA,0x6A,0x2D,0xA0,0x69,0x00,0xAF,0x32,0xBE,0xCA,0xFC,0x4F,0xCB,0xCA,0xFF,0x95,0x99,0x28,0x04,0x78,0x55,0xF9,0x55,0xE5,0x3F,0x66,0xAB,0x2E,0xB6,0xB9,0xEA,0xA2,0x0E,0xAC,0x2E,0x1B,0xBC,0xF2,0xFB,0x3A,0xF2,0xFF,0x35,0x8C,0x15,0xAA,0x18,0x3C,0x54,0x5E,0x8F,0x7F,0xFD,0xF8,0x9F,0x3B,0x03,0x05,0x30,0xA1,0x00,0xF0,0x4A,0xFA,0x6B,0xC8,0x7F,0xEB,0x24,0x9C,0xAC,0x12,0x08,0xF0,0x8A,0xF8,0xEB,0xC8,0xFF,0xEF,0xC8,0x08,0xE7,0xB5,0xF2,0xD7,0x90,0xFF,0xF2,0x2D,0x6C,0x00,0x60,0xB8,0x01,0x00,0xB5,0xF1,0x6A,0xF9,0x6B,0x88,0x74,0x32,0x4C,0x27,0xC3,0xFF,0xFD,0x5E,0x7B,0x7F,0x0D,0x19,0x3F,0x00,0xE6,0x15,0xE0,0xFF,0x3F};


bool CheckAndFixStateData(BYTE *stateDataBegin, DWORD stateDataLength)
{
	BitField field(stateDataBegin, stateDataLength);
	int bitIndex = 0;
	int totalBits = stateDataLength*8;
	int entryIndexBit = bitIndex;
	bool invalidStateData = false;

	// If our bit index exceeds totalBits then our current state is fubar
	#define CHECKSTATEDATA(n) {invalidStateData = ((n) > totalBits); if(invalidStateData) { break; } }

	while(!invalidStateData)
	{
		// Remember where this state entry begins so if it fails we can just force it to be 0xFF
		//   to force the state parsing to end
		entryIndexBit = bitIndex;

		// First 8 bits contain the state index or 0xFF to indicate end of state list
		unsigned char state = field.ReadBits(bitIndex, 8);
		CHECKSTATEDATA(bitIndex += 8);

		// State of 0xFF indicates end of state list
		if(state == 0xFF)
		{
			break;
		}

		// I suppose this state has no stat data if this flag is set
		bool flagContinue = field.ReadBit(bitIndex);
		CHECKSTATEDATA(++bitIndex);
		if(!flagContinue)
		{
			continue;
		}

		// Read the first stat index for this state. Stat index of 0x1ff indicates end of stat data
		int statIndex = field.ReadBits(bitIndex, 9);
		CHECKSTATEDATA(bitIndex += 9);

		while(!invalidStateData && statIndex != 0x1ff)
		{
			// Skip actual stat data for this state (send bits + send bits param)
			CHECKSTATEDATA(bitIndex += itemStatCosts[statIndex].SendBitsTotal);

			// read the next stat index
			statIndex = field.ReadBits(bitIndex, 9);
			CHECKSTATEDATA(bitIndex += 9);
		}
	}

	if(invalidStateData)
	{
		// Force the last checked state in the state list to become the end of this list by
		//  setting it's index to 0xFF to indicate the end.
		field.WriteBits(entryIndexBit, 8, 0xff);

		return true;
	}

	return false;
}

DWORD EXPORT OnGamePacketBeforeReceived(BYTE* aPacket, DWORD aLen)
{   
	/*
	if(aPacket[0] == 0xAA)
	{
		FILE *debugFile = fopen("d:\\packetAA.txt", "a");
	
		fprintf(debugFile, "{");
		for(int i = 0; i < aLen; ++i)
		{
			fprintf(debugFile, "0x%02X%s", aPacket[i], i < (aLen-1) ? "," : "");
		}
		fprintf(debugFile, "},\n");
		fflush(debugFile);
		fclose(debugFile);
	}*/

	if(aPacket[0] == 0xAA)
	{
		BYTE *stateDataBegin = aPacket + 7;
		int remainingLength = aPacket[6] - 6;

		if(CheckAndFixStateData(stateDataBegin, remainingLength))
		{
			server->GameStringf("Fixing 0xAA packet...");
		}
	}

	return aLen;
}


MODULECOMMANDSTRUCT ModuleCommands[]=
{
	{
		"help",
			OnGameCommandHelp,
			"help�c0 List commands available in this module.\n"
			"<command> help�c0 Shows detailed help for <command> in this module."
	},
	{NULL}
};