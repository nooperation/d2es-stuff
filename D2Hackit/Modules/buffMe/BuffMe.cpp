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
	currentBuffIndex = 0;
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
void BuffMe::Start(bool useChat)
{
	ReadBuffs(".\\plugin\\buffMe_buffs.txt");

	this->useChat = useChat;
	currentBuffIndex = 0;
	currentState = STATE_SENDING;
	startingSkill = me->GetSelectedSpell(FALSE);

	CastCurrentBuff();
}

/// <summary>
/// All buffs have been cast and are applied
/// </summary>
void BuffMe::RestoreOriginalSkill()
{
	currentState = STATE_WAITINGFORRESTORESKILL;

	buffCastTime = std::chrono::system_clock::now();
	me->SelectSpell(startingSkill, FALSE);
}

void BuffMe::OnCompletion()
{
	currentState = STATE_WAITINGFORRESTORESKILL;

	buffCastTime = std::chrono::system_clock::now();
	me->SelectSpell(startingSkill, FALSE);
	currentState = STATE_IDLE;

	if (useChat)
	{
		me->Say("ÿc5BuffMeÿc0: Done");
	}
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
			if(useChat)
			{
				me->Say("ÿc5BuffMeÿc0: Rebuff needed");
			}

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
	if(currentState == STATE_WAITINGFORAFFECT)
	{
		// Check to see if this is the affect we're waiting on
		if (currentBuffIndex >= desiredBuffs.size())
		{
			return;
		}

		const auto& desiredBuff = desiredBuffs[currentBuffIndex];

		if (desiredBuff.affectId < 0)
		{
			// Negative affect ID's are just the number of milliseconds to wait after casting before considering it as complete

			const auto targetTime = buffCastTime + std::chrono::milliseconds(-desiredBuff.affectId);
			const auto now = std::chrono::system_clock::now();

			if (now < targetTime)
			{
				return;
			}
		}
		else
		{
			if (!me->GetAffection(desiredBuff.affectId))
			{
				return;
			}
		}

		NextBuff();
	}
	else if (currentState == STATE_WAITINGFORRESTORESKILL)
	{
		const auto targetTime = buffCastTime + std::chrono::milliseconds(50);
		const auto now = std::chrono::system_clock::now();

		if (now >= targetTime) 
		{
			OnCompletion();
			return;
		}
	}


}

/// <summary>
/// Cast the next buff or go to the completion state if no more buffs to cast
/// </summary>
void BuffMe::NextBuff()
{
	currentBuffIndex++;
	if(currentBuffIndex >= desiredBuffs.size())
	{
		RestoreOriginalSkill();
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
			// numCharges will equal 0 when we actually have a spell. Selecting a spell we only have charges for
			//  will disconnect you due to an invalid select spell packet (d2hackit problem, complex item parsing problem)
			const auto numCharges = me->GetSpellChargesReal(desiredBuffs[currentBuffIndex].buffId);
			if (numCharges == 0)
			{
				break;
			}
			else
			{
				//server->GameStringf("Skipping %s: It's granted via item charges", desiredBuffs[currentBuffIndex].name.c_str());
			}
		}
		else
		{
			//server->GameStringf("Skipping %s: we don't have it", desiredBuffs[currentBuffIndex].name.c_str());
		}

		currentBuffIndex++;
	}

	if(currentBuffIndex >= desiredBuffs.size())
	{
		RestoreOriginalSkill();
		return;
	}

	currentState = STATE_WAITINGFORAFFECT;

	buffCastTime = std::chrono::system_clock::now();
	me->CastNoTarget(desiredBuffs[currentBuffIndex].buffId, FALSE);
}
