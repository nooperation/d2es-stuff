//////////////////////////////////////////////////////////////////////
// ScrollDraw.h
//
// Opens the scroll UI and draw custom text on it. Can be used for
// displaying module info, credits, command syntax, etc. Looks cool.
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#ifndef __SCROLLDRAW_H__
#define __SCROLLDRAW_H__

#include <windows.h>
#include "..\\includes\\utilities\\ArrayEx.h"

#define SCROLL_TEXT_MAX	32

struct SCRLINE
{
	char szLine[SCROLL_TEXT_MAX + 1];
};

class CScrollDraw  
{
public:

	CScrollDraw();
	virtual ~CScrollDraw();
	
	/////////////////////////////////////////////////////////////
	// Called By Developers
	/////////////////////////////////////////////////////////////
	void ClearAll();
	BOOL AddLine(LPCSTR lpszLine);
	void SetTitle(LPCSTR lpszTitle);
	BOOL Draw();	
	
	/////////////////////////////////////////////////////////////
	// Called By D2Hackit Only
	/////////////////////////////////////////////////////////////
	void OnTimerTick();
	void Undraw();

private:
	
	POINT GetButtonPos() const;
	POINT GetScrollTopLeft() const;
	DWORD m_dwHookID;
	BOOL m_bDrawn;	
	char m_szTitle[SCROLL_TEXT_MAX + 1];
	CArrayEx<SCRLINE, const SCRLINE&> m_aLines;
};

#endif // __SCROLLDRAW_H__
