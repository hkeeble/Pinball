#ifndef _TRIGGERS_H_
#define _TRIGGERS_H_

#include "PxSimulationEventCallback.h"

using namespace physx;

/* Base class for all triggers */
class ScoreCallback : public PxSimulationEventCallback
{
private:
	bool m_isTriggered;
public:
	ScoreCallback();
	virtual ~ScoreCallback();

	virtual void onTrigger(PxTriggerPair* pairs, PxU32 count);
	bool IsTriggered();
};

#endif // _TRIGGERS_H_