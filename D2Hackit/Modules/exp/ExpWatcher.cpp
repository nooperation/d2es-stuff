#include "ExpWatcher.h"
#include "../../Includes/D2Client.h"

ExpWatcher::ExpWatcher()
{
	showExpGains = true;
	showExpGainsVerbose = false;
	lastExp = 0;
	gameExpStart = 0;
	expGainsIndex = 0;
	screenHook = 0;
	memset(expGains, 0, sizeof(expGains));
}

ExpWatcher::~ExpWatcher()
{
	screen->ReleaseScreenHook(screenHook);
}

void ExpWatcher::CheckForExp()
{

	if(!showExpGains && !showExpGainsVerbose)
	{
		return;
	}

	DWORD currentExp = me->GetStat(STAT_LEVEL + 1);
	if(lastExp == 0)
	{
		lastExp = currentExp;
	}

	if(currentExp - lastExp > 0)
	{
		expGainsIndex = (expGainsIndex + 1) % (sizeof(expGains) / sizeof(expGains[0]));
		expGains[expGainsIndex] = currentExp - lastExp;

		if(showExpGainsVerbose)
		{
			server->GameInfof("Exp gained: %u", currentExp - lastExp);
		}

		lastExp = currentExp;
		UpdateExpAverage();

	}
	else if(currentExp - lastExp < 0)
	{
		lastExp = currentExp;
	}
}

DWORD ExpWatcher::GetAverageExp()
{
	DWORD avgExp = 0;
	int zeroGainCount = 0;
	int i = 0;

	for(i = 0; i < sizeof(expGains)/sizeof(expGains[0]); ++i)
	{
		if(expGains[i] == 0)
		{
			zeroGainCount++;
		}
		else
		{
			avgExp += expGains[i];
		}
	}

	if(i - zeroGainCount <= 0)
	{
		return 0;
	}

	return avgExp/(i - zeroGainCount);
}

void ExpWatcher::StartZoneExp()
{
	zoneExpStart = me->GetStat(STAT_LEVEL + 1);
}

void ExpWatcher::StartGameExp()
{
	gameExpStart = me->GetStat(STAT_LEVEL + 1);
}

DWORD ExpWatcher::GetTotalZoneExp()
{
	if(zoneExpStart == 0 || me->GetStat(STAT_LEVEL + 1) < zoneExpStart)
	{
		zoneExpStart = me->GetStat(STAT_LEVEL + 1);
	}

	return me->GetStat(STAT_LEVEL + 1) - zoneExpStart;
}

DWORD ExpWatcher::GetTotalGameExp()
{
	if(gameExpStart == 0 || me->GetStat(STAT_LEVEL + 1) < gameExpStart)
	{
		gameExpStart = me->GetStat(STAT_LEVEL + 1);
	}

	return me->GetStat(STAT_LEVEL + 1) - gameExpStart;
}

void ExpWatcher::ResetAverageExp()
{
	if(screenHook > 0)
	{
		screen->ReleaseScreenHook(screenHook);
	}
	screenHook = screen->CreateScreenHook();

	memset(expGains, 0, sizeof(expGains));
	lastExp = me->GetStat(STAT_LEVEL + 1);
	expGainsIndex = 0;

	UpdateExpAverage();
}

DWORD ExpWatcher::GetLastExp()
{
	return lastExp;
}

void ExpWatcher::UpdateExpAverage()
{
	char expMessage[128];

	sprintf_s(expMessage, "Average Exp: %u", GetAverageExp());
	screen->DrawSingleText(screenHook, expMessage, 350, 10, D2FONT_SMALL, FALSE);
}

void ExpWatcher::DisableWatcher()
{
	showExpGains = false;
	showExpGainsVerbose = false;

	screen->ReleaseScreenHook(screenHook);
	screenHook = screen->CreateScreenHook();
}

void ExpWatcher::EnableWatcher(bool verbose)
{
	showExpGainsVerbose = verbose;
	showExpGains = true;
}

void ExpWatcher::PrintExpInfo()
{
	server->GameStringf("Expierence gained for zone: %u", GetTotalZoneExp());
	server->GameStringf("Total gained this game: %u", GetTotalGameExp());
}

void ExpWatcher::TellExpInfo()
{
	char expMessage[128];

	sprintf_s(expMessage, "Expierence gained for zone: %u", GetTotalZoneExp());
	me->Say(expMessage);
	sprintf_s(expMessage, "Expierence gained this game: %u", GetTotalGameExp());
	me->Say(expMessage);
}