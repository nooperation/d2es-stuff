#pragma once

#include <cstdint>
#include "CommonDefinitions.h"
#include "Drlg/D2DrlgDrlg.h"
// TODO: Reimport from .cpp

#pragma pack(1)

struct D2BoundingBoxStrc
{
	int32_t nLeft;								//0x00
	int32_t nBottom;							//0x04
	int32_t nRight;								//0x08
	int32_t nTop;								//0x0C
};

// Size of the unit in subtiles
enum D2C_CollisionUnitSize
{
	COLLISION_UNIT_SIZE_NONE  = 0,
	COLLISION_UNIT_SIZE_POINT = 1, // Occupies 1 subtile in width
	COLLISION_UNIT_SIZE_SMALL = 2, // Occupies 2 subtiles in width
	COLLISION_UNIT_SIZE_BIG   = 3, // Occupies 3 subtiles in width
	COLLISION_UNIT_SIZE_COUNT
};

enum D2C_CollisionPattern
{
	COLLISION_PATTERN_NONE = 0,
	COLLISION_PATTERN_SMALL_UNIT_PRESENCE = 1,
	COLLISION_PATTERN_BIG_UNIT_PRESENCE = 2,
	// Actually linked to whether a monster may be attacked?
	COLLISION_PATTERN_SMALL_PET_PRESENCE  = 3,
	COLLISION_PATTERN_BIG_PET_PRESENCE    = 4,
	COLLISION_PATTERN_SMALL_NO_PRESENCE   = 5,
};

enum D2C_CollisionMaskFlags : uint16_t
{
	COLLIDE_NONE = 0x0000,
	COLLIDE_BLOCK_PLAYER = 0x0001,			// 'black space' in arcane sanctuary, cliff walls etc. Seems to be named "wall" only in original game?
	COLLIDE_BLOCK_MISSILE = 0x0002,			// tile based obstacles you can't shoot over
	COLLIDE_BARRIER = 0x0004,				// again used inconsistantly -.- Can guard against Missile / Flying ?
	COLLIDE_BLOCK_LEAP = 0x0008,
	COLLIDE_ALTERNATE_FLOOR = 0x0010,		// some floors have this set, others don't
	COLLIDE_BLANK = 0x0020,					// returned if the subtile is invalid
	COLLIDE_MISSILE = 0x0040,
	COLLIDE_PLAYER = 0x0080,
	COLLIDE_WATER = 0x00C0,
	COLLIDE_MONSTER = 0x0100,
	COLLIDE_ITEM = 0x0200,
	COLLIDE_OBJECT = 0x0400,
	COLLIDE_DOOR = 0x0800,
	COLLIDE_UNIT_RELATED = 0x1000,			// set for units sometimes, but not always
	COLLIDE_PET = 0x2000,					// linked to whether a monster that may be attacked is present
	COLLIDE_4000 = 0x4000,
	COLLIDE_CORPSE = 0x8000,				// also used by portals, but dead monsters are mask 0x8000
	COLLIDE_ALL_MASK = 0xFFFF,
	COLLIDE_MASK_INVALID = (COLLIDE_BLANK | COLLIDE_BARRIER | COLLIDE_BLOCK_MISSILE | COLLIDE_BLOCK_PLAYER),
	COLLIDE_MASK_WALKING_UNIT = COLLIDE_BLOCK_PLAYER | COLLIDE_BLOCK_LEAP | COLLIDE_OBJECT | COLLIDE_DOOR | COLLIDE_UNIT_RELATED,
	COLLIDE_MASK_FLYING_UNIT = COLLIDE_BARRIER | COLLIDE_DOOR | COLLIDE_UNIT_RELATED,
	COLLIDE_MASK_MONSTER_THAT_CAN_OPEN_DOORS = COLLIDE_BLOCK_PLAYER | COLLIDE_OBJECT | COLLIDE_UNIT_RELATED | COLLIDE_PET,
	COLLIDE_MASK_MONSTER_DEFAULT = COLLIDE_MASK_MONSTER_THAT_CAN_OPEN_DOORS | COLLIDE_DOOR,
	COLLIDE_MASK_PLAYER_WW = COLLIDE_BLOCK_PLAYER | COLLIDE_OBJECT | COLLIDE_DOOR,
	COLLIDE_MASK_DOOR_BLOCK_VIS = COLLIDE_DOOR | COLLIDE_BARRIER | COLLIDE_BLOCK_MISSILE,
};

struct D2RoomCollisionGridStrc
{
	D2DrlgCoordsStrc pRoomCoords;			//0x00
	uint16_t* pCollisionMask;					//0x20
};
#pragma pack()
