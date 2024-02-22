#pragma once

#include <cstdint>
#include "Path.h"

// Note: Very similar to D1's path finding, but using a cache for the open/closed set

#pragma pack(1)

struct D2PathFoWallNodeStrc					    //sizeof 0x38
{
	D2PathPointStrc tPoint;						//0x00
	int16_t nFScore;							//0x04
	int16_t nHeuristicDistanceToTarget;			//0x06
	int16_t nBestDistanceFromStart;				//0x08
	uint16_t wPad;								//0x0A
	D2PathFoWallNodeStrc* pBestParent;		    //0x0C
	D2PathFoWallNodeStrc* pChildren[8];			//0x10
	D2PathFoWallNodeStrc* pNextCachePoint;		//0x30
	D2PathFoWallNodeStrc* pNextSortedByFScore;	//0x34
};

struct D2PathFoWallContextStrc						  //sizeof 0x32EC
{
	static const size_t CACHE_SIZE = 128;
	static const size_t STORAGE_SIZE = 200;
	static const size_t STACK_SIZE = 200;

	D2PathFoWallNodeStrc* aPendingCache[CACHE_SIZE];  //0x0000 Open set
	D2PathFoWallNodeStrc* aVisitedCache[CACHE_SIZE];  //0x0200 Closed set
	D2PathFoWallNodeStrc* pSortedListByFScore;		  //0x0400 Sorted in ascending order (best first)
	D2PathFoWallNodeStrc aNodesStorage[STORAGE_SIZE]; //0x0404 Allocator
	int32_t nNodesCount;							  //0x2FC4 Number of allocated nodes
	D2PathFoWallNodeStrc* aPointsStack[STACK_SIZE];   //0x2FC8 Stack used for propagation of best distance
	int32_t nStackCount;							  //0x32E8
};


#pragma pack()
