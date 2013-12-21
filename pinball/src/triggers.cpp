#include "triggers.h"

ScoreCallback::ScoreCallback()
{
	m_isTriggered = false;
}

ScoreCallback::~ScoreCallback()
{

}

void ScoreCallback::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	for (int i = 0; i < count; i++)
	{
		if (pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
			m_isTriggered = true;
		if (pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_LOST)
			m_isTriggered = false;

		PxRigidActor* ball = pairs[i].otherActor;
		if (ball->isRigidDynamic())
		{
			PxRigidDynamic* dyn = (PxRigidDynamic*)ball;
			dyn->addForce(-dyn->getLinearVelocity());
		}
	}
}