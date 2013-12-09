#include "triggers.h"


Trigger::Trigger()
{
	m_isTriggered = false;
}

Trigger::~Trigger()
{

}

void Trigger::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	for (int i = 0; i < count; i++)
	{
		if (pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
			m_isTriggered = true;
		if (pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_LOST)
			m_isTriggered = false;
	}
}

void BallOutOfPlayCallback::onTrigger(PxTriggerPair* pairs, PxU32 count)
{

}

void BallEntersPlayCallback::onTrigger(PxTriggerPair* pairs, PxU32 count)
{

}

void ScoreCallback::onTrigger(PxTriggerPair* pairs, PxU32 count)
{

}