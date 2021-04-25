#include "BuffMe.h"
#include "../../Includes/D2Client.h"

#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <regex>

BuffMe::BuffMe()
{
	currentState = STATE_IDLE;
	currentBuffIndex = 0;

	ReadBuffs(".\\plugin\\buffMe_buffs.txt");
}

bool BuffMe::ReadBuffs(const std::string &fileName)
{
	desiredBuffs.clear();

	std::ifstream inFile(fileName.c_str());
	if (!inFile)
	{
		return false;
	}

	while (inFile.good())
	{
		std::string readBuff;
		std::getline(inFile, readBuff);

		if (readBuff.length() <= 0)
		{
			continue;
		}

		if (!isdigit(readBuff[0]))
		{
			continue;
		}

		int buffId;
		int affectId;
		std::string name;

		std::stringstream ss(readBuff);
		ss >> buffId >> affectId;
		ss.ignore(1);
		std::getline(ss, name);

		desiredBuffs.emplace_back(buffId, affectId, name);
	}

	inFile.close();
	return true;
}

/// <summary>
/// Starts the buff process with the first buff
/// </summary>
void BuffMe::Start()
{
	currentBuffIndex = 0;
	currentState = STATE_SENDING;
	startingSkill = me->GetSelectedSpell(FALSE);

	for (auto &item : desiredBuffs)
	{
		server->GameStringf("BuffId = %d | AffectId = %d | Name = %s", item.buffId, item.affectId, item.name.c_str());
	}

	CastCurrentBuff();
}

/// <summary>
/// All buffs have been cast and are applied
/// </summary>
void BuffMe::OnCompletion()
{
	me->SelectSpell(startingSkill, FALSE);
	currentState = STATE_IDLE;
}

/// <summary>
/// The player has lost an affect. If one of the affects is part of one
///   of our buffs then warn the player that they need to rebuff
/// </summary>
/// <param name="affectID">ID of affect</param>
void BuffMe::OnDisAffect(size_t affectID)
{
	if(currentState != STATE_IDLE || needsRebuff)
	{
		return;
	}

	for each(auto &item in desiredBuffs)
	{
		if(item.affectId == affectID)
		{
			server->GameStringf("ÿc3Warningÿc0: ÿc2Rebuff needed");
			needsRebuff = true;
			return;
		}
	}
}

/// <summary>
/// The player has gained an affect. If this is the affect we're looking for
///   then cast the next buff
/// </summary>
/// <param name="affectID">ID of affect</param>
void BuffMe::OnAffect(size_t affectID)
{
	if(currentState != STATE_WAITINGFORAFFECT)
	{
		return;
	}
	
	// Check to see if this is the affect we're waiting on
	if (currentBuffIndex >= desiredBuffs.size())
	{
		return;
	}

	if(desiredBuffs[currentBuffIndex].affectId != affectID)
	{
		return;
	}

	NextBuff();
}

/// <summary>
/// Not all buffs cause an affect if they're already in effect. We check every
///  few ticks to see if that affect is applied after we attempt casting it, if
///  it is then we can continue casting our next buff.
/// </summary>
void BuffMe::OnTick()
{
	if(currentState != STATE_WAITINGFORAFFECT)
	{
		return;
	}

	// Check to see if this is the affect we're waiting on
	if (currentBuffIndex >= desiredBuffs.size())
	{
		return;
	}

	if(!me->GetAffection(desiredBuffs[currentBuffIndex].affectId))
	{
		return;
	}

	NextBuff();
}

/// <summary>
/// Cast the next buff or go to the completion state if no more buffs to cast
/// </summary>
void BuffMe::NextBuff()
{
	currentBuffIndex++;
	if(currentBuffIndex >= desiredBuffs.size())
	{
		OnCompletion();
		return;
	}
	else
	{
		currentState = STATE_SENDING;
		CastCurrentBuff();
	}
}

/// <summary>
/// Casts the current buff
/// </summary>
void BuffMe::CastCurrentBuff()
{
	while(currentBuffIndex < desiredBuffs.size())
	{
		if (me->HaveSpell(desiredBuffs[currentBuffIndex].buffId))
		{
			break;
		}

		//server->GameStringf("Don't have spell %d", currentBuff);
		currentBuffIndex++;
	}

	if(currentBuffIndex >= desiredBuffs.size())
	{
		OnCompletion();
		return;
	}

	currentState = STATE_WAITINGFORAFFECT;
	me->CastNoTarget(desiredBuffs[currentBuffIndex].buffId, FALSE);
}
