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
		if (pairs[i].otherActor->isRigidDynamic() && pairs[i].otherActor->getName() == "Ball")
		{
			if (pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{

				PxRigidDynamic* ball = static_cast<PxRigidDynamic*>(pairs[i].otherActor);

				if (pairs[i].triggerActor->getName() == "BumperHigh" || pairs[i].triggerActor->getName() == "BumperLow")
				{
					PxRigidDynamic* bumper = static_cast<PxRigidDynamic*>(pairs[i].triggerActor);

					if (pairs[i].triggerActor->getName() == "BumperHigh")
						Pinball::AddScoreHigh = true;
					else
						Pinball::AddScoreLow = true;

					Transform bumperPose = bumper->getGlobalPose();
					Transform ballPose = ball->getGlobalPose();
					Vec3 dir = bumperPose.p - ballPose.p;
					dir.normalize();

					dir = Vec3(dir.x, dir.y, dir.z);
					Pinball::BounceBall = true;
					Pinball::BallBounceDirection = dir;
				}
				if (pairs[i].triggerActor->getName() == "SpinnerSwitch")
				{
						Pinball::EnableSpinners = true;
				}
			}
		}
	}
}