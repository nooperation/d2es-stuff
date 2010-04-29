////////////////////////////////////////////////////////////
// BitFields.h 
//
// Defines the CBitFields class. Originally ThoHell's code
// with some modification.
////////////////////////////////////////////////////////////

#ifndef __BITFIELDS_H__
#define __BITFIELDS_H__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// from mousepad with some small mods

class BitFields 
{
public:
	BitFields(const BYTE *d, DWORD mx) 
	{
		data = d; 
		pos = 0; 
		max = (mx * 8);  // convert to max bits
	}

	void Skip(DWORD len)
	{
		pos += len;
	}

	DWORD GetField(DWORD len) 
	{
		unsigned int startAt = 0;

		// trying to read beyond the end of the packet
		if(pos + len > max) 
		{
			if(pos >= max) 
			{
				return 0;
			}

			len = max - pos;
		}

		// Was crashing when using __int64 (before), using standard 32 bit integer now, hope whatever
		//   was crashing before is gone
		startAt = *(unsigned int *)(data + pos/8);

		DWORD result = (DWORD)(startAt << (32 - len - (pos & 7)) >> (32 - len));

		pos += len;
		return result;
	}

	DWORD GetMax()
	{
		return max;
	}
	DWORD GetPos()
	{
		return pos;
	}

private:
	const BYTE *data;
	DWORD pos;
	DWORD max;
};

#endif // __BITFIELDS_H__