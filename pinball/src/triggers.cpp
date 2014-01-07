#include "pinball.h"

ScoreCallback::ScoreCallback() : SimulationEventCallback()
{

}

ScoreCallback::~ScoreCallback()
{
	SimulationEventCallback::SimulationEventCallback();
}

void ScoreCallback::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	for (int i = 0; i < count; i++)
	{
		if (pairs[i].otherActor->isRigidDynamic())
		{
			PxRigidActor* ball = pairs[i].otherActor;

			if (pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
				Pinball::AddScore(100);
		}
	}
}