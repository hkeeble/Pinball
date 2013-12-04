#ifndef REVOLUTE_JOINT_H
#define REVOLUTE_JOINT_H

#include "PxPhysicsAPI.h"
#include "globals.h"

class RevoluteJoint
{
private:
	PxRevoluteJoint* joint;
public:
	RevoluteJoint(PxRigidActor* actor0, Transform& localFrame0, PxRigidActor* actor1, Transform& localFrame1);
	void DriveVelocity(Fl32 value);
	Fl32 GetDriveVelocity();
	void SetLimits(Fl32 lower, Fl32 upper);
};

#endif // REVOLUTE_JOINT_H