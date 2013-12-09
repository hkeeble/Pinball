#ifndef _TRIGGERS_H_
#define _TRIGGERS_H_

#include "PxSimulationEventCallback.h"

using namespace physx;

/* Base class for all triggers */
class Trigger : public PxSimulationEventCallback
{
private:
	bool m_isTriggered;
public:
	Trigger();
	virtual ~Trigger();

	virtual void onTrigger(PxTriggerPair* pairs, PxU32 count);
	bool IsTriggered();
};

/* Various Trigger overrides */
class BallOutOfPlayCallback : public Trigger
{
public:
	virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) override final;
};

class BallEntersPlayCallback : public Trigger
{
public:
	virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) override final;
};

class ScoreCallback : public Trigger
{
public:
	virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) override final;
};

#endif // _TRIGGERS_H_