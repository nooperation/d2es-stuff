// MsgSender.cpp: implementation of the CMsgSender class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "MsgSender.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgSender::CMsgSender()
{
	m_hTarget = NULL;
	m_dwKeyDelay = 0;
}

CMsgSender::~CMsgSender()
{

}

HWND CMsgSender::SetTargetHWnd(HWND hWnd)
{
	HWND h = m_hTarget;
	m_hTarget = hWnd;
	return h;
}

HWND CMsgSender::GetTargetHWnd() const
{
	return m_hTarget;
}

BOOL CMsgSender::MouseMove(int x, int y) const
{
	if (m_hTarget == NULL)
		return FALSE;

	return ::PostMessage(m_hTarget, WM_MOUSEMOVE, NULL, TransPosition(x, y));
}

BOOL CMsgSender::LeftClick(int x, int y) const
{
	return (MouseMove(x, y)
		&&::PostMessage(m_hTarget, WM_LBUTTONDOWN, MK_LBUTTON, TransPosition(x, y))
		&& ::PostMessage(m_hTarget, WM_LBUTTONUP, MK_LBUTTON, TransPosition(x, y)));
}

DWORD CMsgSender::TransPosition(int x, int y)
{
	return (y << 16) + x;
}

BOOL CMsgSender::RightClick(int x, int y) const
{
	return (MouseMove(x, y)
		&&::PostMessage(m_hTarget, WM_RBUTTONDOWN, MK_RBUTTON, TransPosition(x, y))
		&& ::PostMessage(m_hTarget, WM_RBUTTONUP, MK_RBUTTON, TransPosition(x, y)));
}

BOOL CMsgSender::SendString(LPCTSTR lpString) const
{
	if (m_hTarget == NULL || lpString == NULL)
		return FALSE;

	DWORD len = ::strlen(lpString);
	for (DWORD i = 0; i < len; i++)
	{
		SendChar(lpString[i]);
		if (m_dwKeyDelay > 0)
			Sleep(m_dwKeyDelay);
	}
	return TRUE;
}

BOOL CMsgSender::SendChar(TCHAR ch) const
{
	if (m_hTarget == NULL)
		return FALSE;

	return	::PostMessage(m_hTarget, WM_CHAR, WPARAM(ch), LPARAM(0));
}

BOOL CMsgSender::LDblClick(int x, int y) const
{
	if (m_hTarget == NULL)
		return FALSE;

	LeftClick(x, y);
	LeftClick(x, y);

	return TRUE;
}

BOOL CMsgSender::SendSysKey(int nKeyCode) const
{
	if (m_hTarget == NULL)
		return FALSE;

	char keybuf[5] = "";
	char oemchar[3] = "  ";	 // 2 character buffer
	::CharToOem(keybuf, &oemchar[0]);
	DWORD scan = ::OemKeyScan(oemchar[0]) & 0xFF;
	DWORD scancode = (::MapVirtualKey(nKeyCode, 0) << 16);

	return ::PostMessage(m_hTarget, WM_KEYDOWN, (WPARAM)nKeyCode, (LPARAM)(scan | 0x00000001))
		&& ::PostMessage(m_hTarget, WM_KEYUP, (WPARAM)nKeyCode, (LPARAM)(scan | 0xC0000001));
}

void CMsgSender::SetKeyDelay(DWORD dwKeyDelay)
{
	m_dwKeyDelay = dwKeyDelay;
}
