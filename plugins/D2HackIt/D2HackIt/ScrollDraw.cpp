//////////////////////////////////////////////////////////////////////
// ScrollDraw.cpp
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#include "ScrollDraw.h"
#include "D2HackIt.h"
#include "d2functions.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScrollDraw::CScrollDraw()
{
	m_dwHookID = 0;
	m_bDrawn = FALSE;
	::memset(m_szTitle, 0, sizeof(m_szTitle));
}

CScrollDraw::~CScrollDraw()
{
	if (m_dwHookID)
		ReleaseScreenHook(m_dwHookID);
}

void CScrollDraw::OnTimerTick()
{
	if (m_bDrawn && m_dwHookID)
	{
		if (!IsUIOpened(UI_SCROLL))
		{
			Undraw();
		}
	}
}

BOOL CScrollDraw::Draw()
{
	if (m_dwHookID)
		return FALSE;	

	// create hook
	m_dwHookID = CreateScreenHook();
	if (!m_dwHookID)
		return FALSE;

	if (!IsUIOpened(UI_SCROLL))
	{
		// popup the scroll
		ChatMsg cm = {0};
		cm.Identifier = 0x26;
		cm.MessageType = 0x07;
		cm.Style = 0x01;
		D2CLIENT_PrintMessage(&cm);
	}

	// draw the texts
	int nTotalLines = m_aLines.GetSize();
	if (m_szTitle[0])
		nTotalLines++;

	LPD2DRAWDATA pLines = new D2DRAWDATA[nTotalLines + 1];
	::memset(pLines, 0, sizeof(D2DRAWDATA) * (nTotalLines + 1));
	POINT pt = GetScrollTopLeft();

	int nStart = 0;
	if (m_szTitle[0])
	{
		// we have a title
		pLines[0].nFont = D2FONT_CAPS;
		pLines[0].x = pt.x + 20;
		pLines[0].y = pt.y;
		pt.y += 20;
		::strcpy(pLines[0].szText, m_szTitle);
		nStart = 1;
	}

	// the rest of text lines
	for (int i = 0; i <nTotalLines; i++)
	{
		pLines[i + nStart].x = pt.x;
		pLines[i + nStart].y = pt.y + i * 15;
		::strcpy(pLines[i + nStart].szText, m_aLines[i].szLine);
	}

	// draw button "x"
	pLines[nTotalLines].nFont = D2FONT_BIGBIGROUGH;
	strcpy(pLines[nTotalLines].szText, "X");
	pt = GetButtonPos();
	pLines[nTotalLines].x = pt.x;
	pLines[nTotalLines].y = pt.y;

	// draw
	DrawMultipleTexts(m_dwHookID, pLines, nTotalLines + 1);
	delete [] pLines;

	// ok
	m_bDrawn = TRUE;
	return TRUE;
}

void CScrollDraw::Undraw()
{
	m_bDrawn = FALSE;

	if (m_dwHookID)
	{
		ReleaseScreenHook(m_dwHookID);
		m_dwHookID = 0;
	}	
}

POINT CScrollDraw::GetScrollTopLeft() const
{
	POINT pt = { 130, 118 };
	if (GetScreenSize().cx == 640)
	{
		pt.x = 50;
		pt.y = 58;
	}
	return pt;
}

void CScrollDraw::SetTitle(LPCSTR lpszTitle)
{
	::memset(m_szTitle, 0, sizeof(m_szTitle));
	if (lpszTitle)
		::strncpy(m_szTitle, lpszTitle, SCROLL_TEXT_MAX);
}

BOOL CScrollDraw::AddLine(LPCSTR lpszLine)
{
	SCRLINE sl = {0};
	if (lpszLine)
		::strncpy(sl.szLine, lpszLine, SCROLL_TEXT_MAX);
	m_aLines.Add(sl);
	return m_aLines.GetSize();
}

void CScrollDraw::ClearAll()
{	
	Undraw();
	::memset(m_szTitle, 0, sizeof(m_szTitle));
	m_aLines.RemoveAll();
}

POINT CScrollDraw::GetButtonPos() const
{
	POINT pt = { 359, 484 };
	if (GetScreenSize().cx == 640)
	{
		pt.x = 279;
		pt.y = 424;
	}
	return pt;
}
