#ifndef _EXPWATCHER_H_
#define _EXPWATCHER_H_

#include <windows.h>

class ExpWatcher
{
	public:
		ExpWatcher();
		~ExpWatcher();

		void CheckForExp();
		void StartZoneExp();
		void StartGameExp();

		void ResetAverageExp();
		DWORD GetAverageExp();
		DWORD GetLastExp();
		DWORD GetTotalZoneExp();
		DWORD GetTotalGameExp();

		void DisableWatcher();
		void EnableWatcher(bool verbose = false);
		void PrintExpInfo();
		void TellExpInfo();

private:
		bool showExpGains;
		bool showExpGainsVerbose;
		DWORD lastExp;
		DWORD gameExpStart;
		DWORD zoneExpStart;
		int expGains[75];
		int expGainsIndex;
		DWORD screenHook;

		void UpdateExpAverage();
};
#endif