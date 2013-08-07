///////////////////////////////////////////////////////////
// StateTicker.cpp
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////

#include "StateTicker.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStateTicker::CStateTicker()
{
	Clear();
}

CStateTicker::~CStateTicker()
{}

void CStateTicker::Update(DWORD dwNewState, DWORD dwWait)
{
	m_dwState = dwNewState;
	m_dwStartWith = dwWait;
	m_dwCount = dwWait;
}

void CStateTicker::TickIt(DWORD dwCount)
{
	if (m_dwCount > dwCount)
		m_dwCount -= dwCount;
	else
		m_dwCount = 0;
}

BOOL CStateTicker::IsTickEnded() const
{
	return m_dwCount == 0;
}

DWORD CStateTicker::GetRemainer() const
{
	return m_dwCount;
}

DWORD CStateTicker::GetElapsed() const
{
	return m_dwCount > m_dwStartWith ? 0 : m_dwStartWith - m_dwCount;
}

DWORD CStateTicker::GetState() const
{
	return m_dwState;
}

void CStateTicker::Clear()
{
	m_dwCount = 0;
	m_dwStartWith = 0;
	m_dwState = 0;
	m_dwOnWaitOK = 0;
	m_dwOnWaitFail = 0;
	m_dwNextCount = 0;
}

void CStateTicker::StartWait(DWORD dwStateOnOK, DWORD dwStateOnFail, DWORD dwNewCounterAfterWait)
{
	m_dwOnWaitOK = dwStateOnOK;
	m_dwOnWaitFail = dwStateOnFail;
	m_dwNextCount = dwNewCounterAfterWait;
}

void CStateTicker::EndWait(BOOL bSucceeded)
{
	m_dwState = bSucceeded ? m_dwOnWaitOK : m_dwOnWaitFail;
	m_dwCount = m_dwNextCount;
}

BOOL CStateTicker::IsTickerInPeriod(DWORD dwPeriod, BOOL bAllowZero) const
{
	if (dwPeriod == 0)
		return FALSE;

	if (m_dwCount == 0 && !bAllowZero)
		return FALSE;

	return (m_dwCount % dwPeriod) == 0;
}
