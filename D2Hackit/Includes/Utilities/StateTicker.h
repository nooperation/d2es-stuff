/////////////////////////////////////////////////////////////////////
// StateTicker.h: interface for the CStateTicker class.
//
// Used as a ticker-counter in lengthy tasks.
//
// Written by Abin
//////////////////////////////////////////////////////////////////////

#ifndef __STATETICKER_H__
#define __STATETICKER_H__

#include <windows.h>

class CStateTicker  
{
public:		

	//////////////////////////////////////////////////////////////
	// Constructor & Destructor
	//////////////////////////////////////////////////////////////
	CStateTicker();
	virtual ~CStateTicker();

	//////////////////////////////////////////////////////////////
	// Attributes
	//////////////////////////////////////////////////////////////
	DWORD GetState() const; // current state
	DWORD GetElapsed() const; // how many milliseconds elapsed?
	DWORD GetRemainer() const; // how many milliseconds remaining?
	BOOL IsTickEnded() const; // Is tick ended?
	BOOL IsTickerInPeriod(DWORD dwPeriod, BOOL bAllowZero = FALSE) const; // Is the remainer in a period?

	//////////////////////////////////////////////////////////////
	// Operations
	//////////////////////////////////////////////////////////////
	void Clear(); // clear previous state & wait time
	void Update(DWORD dwNewState, DWORD dwWait = 0); // change state & wait time
	void StartWait(DWORD dwStateOnOK, DWORD dwStateOnFail, DWORD dwNewCounterAfterWait = 0); // enter a wait state
	void EndWait(BOOL bSucceeded); // leave a wait state
	void TickIt(DWORD dwCount = 100); // shall be called in "OnGameTimerTick"

private:
	
	//////////////////////////////////////////////////////////////
	// Member Data
	//////////////////////////////////////////////////////////////
	DWORD m_dwStartWith; // Start at ? count
	DWORD m_dwCount; // Remaining tick counter
	DWORD m_dwState; // Current state
	DWORD m_dwOnWaitOK; // new state if waiting ended successfully
	DWORD m_dwOnWaitFail; // New state if waiting failed
	DWORD m_dwNextCount; // New tick counter after waiting ended 
};

#endif // __STATETICKER_H__
