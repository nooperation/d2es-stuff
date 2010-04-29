//////////////////////////////////////////////////////////////////////
// PathFinder.h
//
// Diablo II game path finding algorithms. Also used for Preset-Unit
// enumeration.
//
// I first used Niren7's algorithm in his ABot, then found out it was
// not working properly and was not accurate. Then I got enlightened by
// Ninjai's D2BS code which is better than the former.
//
// Of course a lot of modifications had to be done in order to make this
// class work for this project, but nevertheless I must give enough
// credits to both Niren7 and Ninjai, their code helped me to start
// this class to say the least.
//
// Ustc_tweeg also helped me a lot on completing this algorithm.
//
// Written by Abin(abinn32@yahoo.com)
// Sep 1oth, 2004
////////////////////////////////////////////////////////////////////////

#ifndef __PATHFINDER_H__
#define __PATHFINDER_H__

#include "CommonStructs.h"
#include "definitions.h"
#include "..\\includes\\utilities\\ArrayEx.h"

class CPathFinder  
{
public:	
	
	CPathFinder();
	virtual ~CPathFinder();	

	BYTE CalculatePathTo(WORD x, WORD y, LPPATH lpBuffer, int nAdjust = 5); // Calculate path

private:
	static int GetRedundancy(LPPATH lpPath, const POINT& pos);
	void Block(POINT pos, int nRange);
	void Reset();
	static POINT MapPos2Point(MAPPOS pos);
	BOOL VerifyMemory() const;
	int Distance(const POINT &p1, const POINT &p2);
	BOOL IsValidPos(POINT pos) const;
	void DumpCollisionMap(LPVOID lpCol) const;
	BOOL GetBestMove(POINT& rResult, int nAdjust);
	void DumpMap1(LPCPATH lpPath) const;	
	void DumpMap2() const;
	BOOL Search();	
	void GraphToMap(POINT& pos) const;
	void MapToGraph(POINT& pos) const;
	void AddCollisionData(LPVOID lpCol, POINT ptPlayer);
	void Search(RoomOther *ro, POINT ptPlayer);
	void MakeMap2();

	short** m_graph;	// graph
	POINT m_ptLevelLeftTop; // level top-left
	POINT m_ptAbsDest; // absolute destination
	POINT m_ptRelDest; // relative destination
	POINT m_lefttTop;	// effective left-top
	POINT m_rightBottom; // effective right-bottom
	CArrayEx<LPCVOID, LPCVOID> m_aSearched;	// search records
};

#endif // __PATHFINDER_H__
