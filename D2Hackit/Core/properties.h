#ifndef _D2_PROPERTIES_H_
#define _D2_PROPERTIES_H_

///////////////////////////////////////////////
// Originally ackmed's Code(Shopbot)
///////////////////////////////////////////////
struct D2_PROPERTIE
{
	int id;
	char numFields;
	union
	{
		char bits[4];
		struct
		{
			char bits1;
			char bits2;
			char bits3;
			char bits4;
		};
	};
	int bias;
//	char* format;
};

const D2_PROPERTIE* GetPropertiesRow(int id);

#endif