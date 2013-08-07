#include "BuffMe.h"
#include "../../Includes/D2Client.h"

#include <fstream>

BuffMe::BuffMe()
{
	currentState = STATE_IDLE;
	currentBuff = 0;

	affects.clear();
	buffs.clear();



	// todo: not hardcoded
	affects.push_back(AFFECT_BATTLECOMMAND);
	buffs.push_back(D2S_BATTLECOMMAND);
	
	affects.push_back(AFFECT_BATTLECOMMAND);
	buffs.push_back(D2S_BATTLECOMMAND);

	affects.push_back(AFFECT_BATTLEORDERS);
	buffs.push_back(D2S_BATTLEORDERS);

	affects.push_back(AFFECT_SHOUT);
	buffs.push_back(D2S_SHOUT);

	// shock armor
	affects.push_back(AFFECT_FROZENARMOR);
	buffs.push_back(D2S_FROZENARMOR);

	// Fire shield
	affects.push_back(0x58);
	buffs.push_back(0x32);

	//// eagle eye
	//affects.push_back(0xc9);
	//buffs.push_back(0x16a);



	// No affect if already enchanted!
	affects.push_back(AFFECT_ES_BARBTHUNDERSTORM);
	buffs.push_back(D2S_ES_BARBTHUNDERSTORM);

	// No affect if already enchanted!
	affects.push_back(AFFECT_ENERGYSHIELD);
	buffs.push_back(D2S_ENERGYSHIELD);
}

/// <summary>
/// Starts the buff process with the first buff
/// </summary>
void BuffMe::Start()
{
	currentBuff = 0;
	currentState = STATE_SENDING;
	startingSkill = me->GetSelectedSpell(FALSE);

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

	for each(auto item in affects)
	{
		if(item == affectID)
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
	if(currentBuff >= affects.size() || affects[currentBuff] != affectID)
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
	if(currentBuff >= affects.size() || !me->GetAffection(affects[currentBuff]))
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
	currentBuff++;
	if(currentBuff >= buffs.size())
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
	while(currentBuff < buffs.size() && !me->HaveSpell(buffs[currentBuff]))
	{
		//server->GameStringf("Don't have spell %d", currentBuff);
		currentBuff++;
	}

	if(currentBuff >= buffs.size())
	{
		OnCompletion();
		return;
	}

	currentState = STATE_WAITINGFORAFFECT;
	me->CastNoTarget(buffs[currentBuff], FALSE);
}