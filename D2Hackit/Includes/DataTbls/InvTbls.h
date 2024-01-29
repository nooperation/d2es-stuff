#pragma once 

#include <cstdint>

#pragma pack(1)

struct D2InvRectStrc
{
	int32_t nLeft;		//0x00
	int32_t nRight;		//0x04
	int32_t nTop;		//0x08
	int32_t nBottom;	//0x0C
};

struct D2InvCompGridStrc
{
	D2InvRectStrc rect;						//0x00
	uint8_t nWidth;							//0x10
	uint8_t nHeight;						//0x11
	uint16_t pad0x12;						//0x12
};

struct D2InventoryGridInfoStrc
{
	uint8_t nGridX;							//0x00
	uint8_t nGridY;							//0x01
	uint16_t pad0x02;						//0x02
	int32_t nGridLeft;						//0x04
	int32_t nGridRight;						//0x08
	int32_t nGridTop;						//0x0C
	int32_t nGridBottom;					//0x10
	uint8_t nGridBoxWidth;					//0x14
	uint8_t nGridBoxHeight;					//0x15
	uint16_t pad0x16;						//0x16
};

enum D2InventoryComponent
{
	//INV_COMP_INV,
	//INV_COMP_GRID,
	INV_COMP_RARM,
	INV_COMP_TORSO,
	INV_COMP_LARM,
	INV_COMP_HEAD,
	INV_COMP_NECK,
	INV_COMP_RHAND,
	INV_COMP_LHAND,
	INV_COMP_BELT,
	INV_COMP_FEET,
	INV_COMP_GLOVES,
	NUM_INV_COMPS

};
struct D2InventoryTxt
{
	D2InvRectStrc pRect;					//0x00
	D2InventoryGridInfoStrc pGridInfo;		//0x10
	D2InvCompGridStrc pComponents[NUM_INV_COMPS];//0x28
};

#pragma pack()
