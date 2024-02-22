#pragma once

#include <cstdint>
#include "D2DrlgDrlg.h"
#include "D2DrlgDrlgGrid.h"

#pragma pack(1)

union D2C_PackedTileInformation
{
	uint32_t nPackedValue;
	struct {
		// If not wall nor floor, then it is a roof.
		uint32_t bIsWall           : 1; // BIT(0)     Is this a wall.
		uint32_t bIsFloor          : 1; // BIT(1)     Is this a floor.
		uint32_t bLOS              : 1; // BIT(2)     code-generated; added on edges
		uint32_t bEnclosed         : 1; // BIT(3)     seems to delimit an enclosure inside another area, or trees
		uint32_t bExit             : 1; // BIT(4)     arch or doorway or gateway in a wall
		uint32_t bUnk0x20          : 1; // BIT(5)     Unknown flag
		uint32_t bLayerBelow       : 1; // BIT(6)     only floor
		uint32_t bLayerAbove       : 1; // BIT(7)     wall & floor
		uint32_t nTileSequence     : 8; // BIT(8-15)  AKA tile subindex
		uint32_t bFillLOS          : 1; // BIT(16)    all tiles will get wall collision
		uint32_t bUnwalkable       : 1; // BIT(17) 
		uint32_t nWallLayer        : 2; // BIT(18-19) code-generated; set when at least Floor2 or Wall2  // (nLayer - 1) << 0x12
		//uint32_t bOverlappedLayer3 : 1; // BIT(19)    code-generated; set when has more walls, incl' of other orientations than usual; obj, shd, tree, roof, lower
		uint32_t nTileStyle        : 6; // BIT(20-25) AKA tile index
		uint32_t bRevealHidden     : 1; // BIT(26)    looks like an upper wall brought to a layer in front
		uint32_t bShadow           : 1; // BIT(27)    this layer is a shadow layer | Lectem's note: seems to be roof instead ? Or are shadow tiles interpreted as roof tiles ?
		uint32_t bLinkage          : 1; // BIT(28)    near wp, lvl links, paths // will never get hidden
		uint32_t bObjectWall       : 1; // BIT(29)    wall tiles with props; may be block reverb / other sounds (crates, barrels, tables etc.)
		uint32_t bUnk0x40000000    : 1; // BIT(30)    Unknown flag
		uint32_t bHidden           : 1; // BIT(31) 
	};
};

enum D2C_RoomTileConstants
{
	DRLGROOMTILE_TILES_SIZE = 8,
	DRLGROOMTILE_SUBTILES_SIZE = DRLGROOMTILE_TILES_SIZE * 5,
};

#pragma pack()
