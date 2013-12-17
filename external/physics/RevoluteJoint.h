#ifndef REVOLUTE_JOINT_H
#define REVOLUTE_JOINT_H

#include "globals.h"
#include "Physics.h"

namespace Physics
{
	using namespace physx;

	class RevoluteJoint
	{
	private:
		PxRevoluteJoint* m_joint;
	public:
		RevoluteJoint();
		RevoluteJoint(PxRigidActor* actor0, Transform& localFrame0, PxRigidActor* actor1, Transform& localFrame1);
		void DriveVelocity(Fl32 value);
		Fl32 GetDriveVelocity() const;
		void SetLimits(Fl32 lower, Fl32 upper);
	};
}
#endif // REVOLUTE_JOINT_H