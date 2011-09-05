#ifndef _BUFFME_H_
#define _BUFFME_H_

#include <vector>

enum BuffStates
{
	STATE_SENDING,
	STATE_WAITINGFORAFFECT,
	STATE_IDLE,
};

class BuffMe
{
	public:
		BuffMe();
		void Start();
		void OnAffect(size_t affectID);
		void OnDisAffect(size_t affectID);
		void OnManaDown();
		void OnTick();
		void NextBuff();
	private:
		void CastCurrentBuff();
		void OnCompletion();

		std::vector<int> buffs;
		std::vector<int> affects;

		size_t currentBuff;
		BuffStates currentState;
		bool needsRebuff;
		int startingSkill;
};

#endif