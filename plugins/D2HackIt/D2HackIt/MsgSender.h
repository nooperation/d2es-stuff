//////////////////////////////////////////////////////////////////////
// MsgSender.h: interface for the CMsgSender class.
//
// Sending messages to a particular window.
//
// Written by Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#ifndef __MSGSENDER_H__
#define __MSGSENDER_H__

#include <windows.h>

class CMsgSender  
{
public:
	void SetKeyDelay(DWORD dwKeyDelay);
	BOOL SendSysKey(int nKeyCode) const;
	BOOL LDblClick(int x, int y) const;
	BOOL SendChar(TCHAR ch) const;
	BOOL SendString(LPCTSTR lpString) const;
	BOOL RightClick(int x, int y) const;
	static DWORD TransPosition(int x, int y);
	BOOL LeftClick(int x, int y) const;
	BOOL MouseMove(int x, int y) const;
	HWND GetTargetHWnd() const;
	HWND SetTargetHWnd(HWND hWnd);
	CMsgSender();
	virtual ~CMsgSender();


protected:
	HWND m_hTarget;
	DWORD m_dwKeyDelay;
};

#endif // __MSGSENDER_H__
