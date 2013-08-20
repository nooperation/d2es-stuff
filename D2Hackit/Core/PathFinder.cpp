///////////////////////////////////////////////////////////
// PathFinder.cpp
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////

#include "PathFinder.h"
#include "D2HackIt.h"
#include "d2functions.h"
#include "Server20.h"
#include "Constants.h"

#define TP_RANGE		30		// Miximum teleport range
#define RANGE_INVALID	10000  // invalid range flag
#define DIM				1000 // maximum graph dimension(worst case only)

/////////////////////////////////////////////////////////////////////
// Path Finding Result
/////////////////////////////////////////////////////////////////////
enum {   PATH_FAIL = 0,     // Failed, error occurred or no available path
		 PATH_CONTINUE,	    // Path OK, destination not reached yet
		 PATH_REACHED };    // Path OK, destination reached(Path finding completed successfully)


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPathFinder::CPathFinder()
{
	// Allocates memory
	m_graph = new short*[DIM];

	if (!m_graph)
		return;
	
	for (int i = 0; i < DIM; i++)
		m_graph[i] = new short[DIM];

	Reset();
}

CPathFinder::~CPathFinder()
{
	// Free heap
	if (m_graph)
	{
		for (int i = 0; i < DIM; i++)
		{
			if (m_graph[i])
				delete [] m_graph[i];
		}
		delete [] m_graph;
	}
}

void CPathFinder::Reset()
{
	if (VerifyMemory())
	{
		for (int i = 0; i < DIM; i++)
		{
			for (int j = 0; j < DIM; j++)
				m_graph[i][j] = 0;
		}
	}

	memset(&m_ptLevelLeftTop, 0, sizeof(POINT));
	memset(&m_ptAbsDest, 0, sizeof(POINT));
	memset(&m_ptRelDest, 0, sizeof(POINT));	
	memset(&m_rightBottom, 0, sizeof(POINT));
	m_lefttTop.x = DIM;
	m_lefttTop.y = DIM;
	m_aSearched.RemoveAll();
}

void CPathFinder::Search(RoomOther *ro, POINT ptPlayer)
{
	if (!ro)
		return;

	if(ro->ptDrlgLevel->LevelNo != GetCurrentMapID())
		return;

	if (m_aSearched.Find(ro) != -1)
		return;

	UnitPlayer* pPlayer = D2CLIENT_GetPlayerUnit();
	if (pPlayer == NULL)
		return;

	BOOL add_room=FALSE;
	if(!ro->pRoom)
	{
		add_room=TRUE;
		D2COMMON_AddRoomData(pPlayer->ptAct, GetCurrentMapID(), ro->xPos, ro->yPos, pPlayer);
	}

	m_aSearched.Add(ro);
	m_aSearched.Sort();	

	if (ro->pRoom)
		AddCollisionData((LPVOID)ro->pRoom->pColl, ptPlayer);
	//DumpCollisionMap((LPVOID)ro->pRoom->pColl);		
	
	
	RoomOther **n = ro->ptList;
	for(int i=0;i<ro->nRoomList;i++)
	{
		Search(n[i], ptPlayer);
	}
	
	if(add_room)
	{
		D2COMMON_RemoveRoomData(pPlayer->ptAct, GetCurrentMapID(), ro->xPos, ro->yPos, pPlayer);
	}
}

BOOL CPathFinder::Search()
{	
	UnitAny *pPlayer = D2CLIENT_GetPlayerUnit();
	if(!pPlayer)
		return FALSE;
	
	Room *pRoom1 = D2COMMON_GetRoomFromUnit(pPlayer);
	if(!pRoom1)
		return FALSE;	

	RoomOther *pRoom2 = pRoom1->ptRoomOther;
	if(!pRoom2) 
		return FALSE;

	DrlgLevel *pLevel = pRoom2->ptDrlgLevel;
	if(!pLevel)
		return FALSE;

	Reset();

	m_ptLevelLeftTop.x = pLevel->LevelX;
	m_ptLevelLeftTop.y = pLevel->LevelY;
	
	POINT ptPlayer = MapPos2Point(GetPosition());
	MapToGraph(ptPlayer);

	Search(pRoom2, ptPlayer);	
	return TRUE;
}

void CPathFinder::DumpMap1(LPCPATH lpPath) const
{	
	FILE *fp = fopen( "c:\\map1.txt", "w+" );
	if(fp == NULL )
		return;

	fprintf(fp, "LeftTop %d, %d      RightBottom %d, %d\n-----------------------------------\n",
		m_lefttTop.x, m_lefttTop.y, m_rightBottom.x, m_rightBottom.y);


	MAPPOS mp = GetPosition();
	mp.x -= static_cast<WORD>(m_ptLevelLeftTop.x) * 5;
	mp.y -= static_cast<WORD>(m_ptLevelLeftTop.y) * 5;

	POINT pt1 = m_lefttTop;
	POINT pt2 = m_rightBottom;

	// make the picture border
	if (pt1.x)
		pt1.x--;
	if (pt1.y)
		pt1.y--;

	if (pt2.x < DIM - 1)
		pt2.x++;
	if (pt2.y < DIM - 1)
		pt2.y++;
	
	for (int y = pt1.y; y < pt2.y; y++)
	{			
		for (int x = pt1.x; x < pt2.x; x++)
		{
			BOOL bOccupied = FALSE;
			if (m_ptRelDest.x == x && m_ptRelDest.y == y)
			{
				fprintf(fp, "%C", 'E'); // E - destination
				bOccupied = TRUE;
			}
			else if (mp.x == x && mp.y == y)
			{
				fprintf(fp, "%C", 'P'); // P - player
				bOccupied = TRUE;
			}
			else if (lpPath && lpPath->iNodeCount)
			{
				for (BYTE i = 0; i < lpPath->iNodeCount; i++)
				{
					POINT pos;
					pos.x = lpPath->aPathNodes[i].x;
					pos.y = lpPath->aPathNodes[i].y;
					MapToGraph(pos);
					if (pos.x == x && pos.y == y)
					{
						fprintf(fp, "%C", '*'); // * - path nodes
						bOccupied = TRUE;
						break;
					}
				}
			}

			if (!bOccupied)
			{
				fprintf(fp, "%C", m_graph[x][y] ? ' ' : 'X'); // X - unreachable
			}
		}

		fprintf(fp, "%c", '\n');
	}	
	
	fclose(fp);
}

void CPathFinder::MakeMap2()
{
	// convert the graph into a distance table
	for (int j = m_lefttTop.y; j < m_rightBottom.y; j++)	
	{
		for (int i = m_lefttTop.x; i < m_rightBottom.x; i++)
		{
			if (m_graph[i][j])
				m_graph[i][j] = static_cast<short>(GetDistance(i, j, m_ptRelDest.x, m_ptRelDest.y));
			else
				m_graph[i][j] = RANGE_INVALID;
		}
	}

	//AdjustWalls();
	
	if (IsValidPos(m_ptRelDest))
	{
		//m_graph[m_ptRelDest.x][m_ptRelDest.y] = 1;
		m_graph[m_ptRelDest.x][m_ptRelDest.y] = 1;
	}	
/*
	// original Niren7's code, not working properly
	BOOL over=FALSE;
	int cur=1;
	while(!over)
	{
		over=TRUE;
		int i,j;
		for(i=0;i<DIM;i++)
		for(j=0;j<DIM;j++)
		{
			if(map[i][j] && m_disTable[i][j]==cur)
			{
				int x,y;
				for(x=i-1;x<=i+1;x++)
				for(y=j-1;y<=j+1;y++)
				{
					if(x<0||y<0||x>=DIM||y>=DIM) continue;
					if(!map[x][y]) continue;
					if(m_disTable[x][y]>0) continue;
					m_disTable[x][y]=cur+1;
				}
				over=FALSE;
			}
		}
		cur++;
	}
	
	for(i=0;i<DIM;i++)
	for(j=0;j<DIM;j++)
	{
		if(m_graph[i][j]==1)
		{
			int x,y;
			int d=5;
			for(x=i-5;x<=i+5;x++)
			for(y=j-5;y<=j+5;y++)
			{
				if(x<0||y<0||x>=DIM||y>=DIM) continue;
				if(m_graph[x][y]==0)
				{
					int d1=abs(x-i)+abs(y-j);
					if(d1<d)d=d1;
				}
			}
			m_disTable[i][j]+=5-d;
		}
	}
	//*/
}


/////////////////////////////////////////////////////////////////////
// The "Get Best Move" Algorithm
//
// Originally developed by Niren7, Modified by Abin
/////////////////////////////////////////////////////////////////////
BOOL CPathFinder::GetBestMove(POINT& pos, int nAdjust)
{	
	if(Distance(m_ptAbsDest,pos) <= TP_RANGE)
	{
		pos = m_ptAbsDest;
		return PATH_REACHED; // we reached the destination
	}

	MapToGraph(pos);
	if (!IsValidPos(pos))
		return PATH_FAIL; // fail

	Block(pos, nAdjust);

	POINT p, best;
	int value = RANGE_INVALID;

	for (p.x = pos.x - TP_RANGE; p.x <= pos.x + TP_RANGE; p.x++)
	{
		for (p.y = pos.y - TP_RANGE; p.y <= pos.y + TP_RANGE; p.y++)
		{			
			if (!IsValidPos(p))
				continue;
			
			if (m_graph[p.x][p.y] < value && Distance(p, pos) <= TP_RANGE)
			{
				value = m_graph[p.x][p.y];
				best = p;					
			}			
		}
	}

	if (value >= RANGE_INVALID)
		return PATH_FAIL; // no path at all	
	
	pos = best;
	Block(pos, nAdjust);	
	GraphToMap(pos);
	return PATH_CONTINUE; // ok but not reached yet
}

int CPathFinder::Distance(const POINT &p1, const POINT &p2)
{
	return GetDistance(p1.x, p1.y, p2.x, p2.y);
}

BYTE CPathFinder::CalculatePathTo(WORD x, WORD y, LPPATH lpBuffer, int nAdjust)
{
	if (lpBuffer == NULL || x == 0 || y == 0 || !VerifyMemory())
		return 0;

	memset(lpBuffer, 0, sizeof(PATH));
	lpBuffer->posStart = GetPosition();
	if (lpBuffer->posStart.x == 0 || lpBuffer->posStart.y == 0)
		return 0;
	
	if (!Search())
		return 0;

	//GameInfof("Mapsize %d, %d", m_rightBottom.x - m_lefttTop.x, m_rightBottom.y - m_lefttTop.y);

	m_ptAbsDest.x = x;
	m_ptAbsDest.y = y;

	m_ptRelDest = m_ptAbsDest;
	MapToGraph(m_ptRelDest);

	//GameInfof("des %d, %d", m_ptRelDest.x, m_ptRelDest.y);

	// verify destination, see if it's in our map
	if (!IsValidPos(m_ptRelDest))
		return 0;

	MakeMap2();	
	//DumpMap2();

	POINT pos;
	pos.x = (short)lpBuffer->posStart.x;
	pos.y = (short)lpBuffer->posStart.y;

	lpBuffer->iNodeCount = 0;

	BOOL bOK = FALSE;
	int nRes = GetBestMove(pos, nAdjust);
	while (nRes != PATH_FAIL && lpBuffer->iNodeCount < 255)
	{
		// Reached?
		if (nRes == PATH_REACHED)
		{
			bOK = TRUE;
			lpBuffer->aPathNodes[lpBuffer->iNodeCount].x = x;
			lpBuffer->aPathNodes[lpBuffer->iNodeCount].y = y;
			lpBuffer->iNodeCount++;
			break; // Finished
		}

		// Perform a redundancy check
		int nRedundancy = GetRedundancy(lpBuffer, pos);
		if (nRedundancy == -1)
		{
			// no redundancy
			lpBuffer->aPathNodes[lpBuffer->iNodeCount].x = (WORD)pos.x;
			lpBuffer->aPathNodes[lpBuffer->iNodeCount].y = (WORD)pos.y;
			lpBuffer->iNodeCount++;
		}
		else
		{
			// redundancy found, discard all redundant steps
			lpBuffer->iNodeCount = nRedundancy + 1;
			lpBuffer->aPathNodes[lpBuffer->iNodeCount].x = (WORD)pos.x;
			lpBuffer->aPathNodes[lpBuffer->iNodeCount].y = (WORD)pos.y;
		}	

		nRes = GetBestMove(pos, nAdjust);
	}	

	if (!bOK)
	{
		lpBuffer->iNodeCount = 0;
	}
	else
	{
		lpBuffer->posEnd.x = x;
		lpBuffer->posEnd.y = y;
	}
	
	//DumpMap1(lpBuffer);	// debug pictures
	return lpBuffer->iNodeCount;	
}

void CPathFinder::DumpCollisionMap(LPVOID lpCol) const
{
	DrlgColl* pCol = (DrlgColl*)lpCol;
	if (pCol == NULL)
		return;

	char szFile[MAX_PATH + 1] = "";
	sprintf(szFile, "c:\\col\\%03d%03d.txt", pCol->nPosGameX - m_ptLevelLeftTop.x * 5, pCol->nPosGameY - m_ptLevelLeftTop.y * 5);
	if (::GetFileAttributes(szFile) != -1)
		return;

	FILE* fp = fopen(szFile, "w");
	if (!fp)
	{
		GameErrorf("Fail %s", szFile);
		return;
	}

	int cx = pCol->nSizeGameX;
	int cy = pCol->nSizeGameY;
	fprintf(fp, "XY %d, %d, Size %d, %d\n\n", pCol->nPosGameX - m_ptLevelLeftTop.x * 5,
		pCol->nPosGameY - m_ptLevelLeftTop.y * 5, cx, cy);

	WORD* currentloc = pCol->pMapStart;

	int i,j;
		
	for(i=0; i < cx; i++)		
	{
		char sz[256] = "";
		for(j=0; j < cy; j++)
		{
			sprintf(sz, "%s%C", sz, *currentloc ? 'X' : ' ');
			currentloc++;
		}

		fprintf(fp, "%s\n", sz);
	}
	fclose(fp);
}


void CPathFinder::AddCollisionData(LPVOID lpCol, POINT ptPlayer)
{
	DrlgColl* pCol = (DrlgColl*)lpCol;
	if (pCol == NULL)
		return;

	int x = pCol->nPosGameX - m_ptLevelLeftTop.x * 5;
	int y = pCol->nPosGameY - m_ptLevelLeftTop.y * 5;
	int cx = pCol->nSizeGameX;
	int cy = pCol->nSizeGameY;

	if (x < 0 || x + cx > DIM || y < 0 || y + cy > DIM)
	{
		//GamePrintError("invalid col");
		return;
	}	
	
	WORD* currentloc = pCol->pMapStart;
	int nLimitX = x + cx;
	int nLimitY = y + cy;
	
	for (int j = y; j < nLimitY; j++)		
	{
		for (int i = x; i < nLimitX; i++)
		{
			if ((*currentloc % 2) == 0)
			{
				m_graph[i][j] = 1;

				// record reach-able area edges
				if (m_lefttTop.x > i)
					m_lefttTop.x = i;
				if (m_lefttTop.y > j)
					m_lefttTop.y = j;

				if (m_rightBottom.x < i + 1)
					m_rightBottom.x = i + 1;
				if (m_rightBottom.y < j + 1)
					m_rightBottom.y = j + 1;
			}
			
			currentloc++;
		}
	}
}

BOOL CPathFinder::IsValidPos(POINT pos) const
{
	return pos.x >= m_lefttTop.x && pos.x >= 0
		&& pos.y >= m_lefttTop.y && pos.y >= 0
		&& pos.x < m_rightBottom.x && pos.x < DIM
		&& pos.y < m_rightBottom.y && pos.y < DIM;
}

void CPathFinder::MapToGraph(POINT &pos) const
{
	pos.x -= m_ptLevelLeftTop.x * 5;
	pos.y -= m_ptLevelLeftTop.y * 5;
}

void CPathFinder::GraphToMap(POINT &pos) const
{
	pos.x += m_ptLevelLeftTop.x * 5;
	pos.y += m_ptLevelLeftTop.y * 5;
}

BOOL CPathFinder::VerifyMemory() const
{
	if (m_graph == NULL)
		return FALSE;

	for (int i = 0; i < DIM; i++)
	{
		if (m_graph[i] == NULL)
			return FALSE;
	}
	return TRUE;
}

POINT CPathFinder::MapPos2Point(MAPPOS pos)
{
	POINT pt;
	pt.x = (long)pos.x;
	pt.y = (long)pos.y;
	return pt;
}

void CPathFinder::DumpMap2() const
{
	FILE *fp = fopen( "c:\\map2.txt", "w+" );
	if(fp == NULL )
		return;

	for (int y = m_lefttTop.y; y < m_rightBottom.y; y++)
	{			
		for (int x = m_lefttTop.x; x < m_rightBottom.x; x++)
		{
			if (!m_graph[x][y])
			{
				fprintf(fp, "%s ", "  ");
			}
			else
			{
				if (m_graph[x][y] < 256)
					fprintf(fp, "%02x ", m_graph[x][y]);
				else
					fprintf(fp, "%s ", "??");
			}
		}

		fprintf(fp, "%c", '\n');
	}

	fclose(fp);
}

void CPathFinder::Block(POINT pos, int nRange)
{
	for (int i = pos.x - nRange; i < pos.x + nRange; i++)
	{
		for (int j = pos.y - nRange; j < pos.y + nRange; j++)
		{
			if (i >= 0 && i < DIM && j >= 0 && j < DIM)
				m_graph[i][j] = RANGE_INVALID;
		}
	}
}

int CPathFinder::GetRedundancy(LPPATH lpPath, const POINT &pos)
{
	// step redundancy check
	if (lpPath == NULL || lpPath->iNodeCount == 0)
		return -1;

	for (BYTE i = 0; i < lpPath->iNodeCount; i++)
	{
		if (GetDistance(lpPath->aPathNodes[i].x, lpPath->aPathNodes[i].y, pos.x, pos.y) <= TP_RANGE / 2)
			return i;
	}

	return -1;
}
