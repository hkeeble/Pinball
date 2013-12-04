#include "RevoluteJoint.h"

namespace Physics
{
	RevoluteJoint::RevoluteJoint()
	{
		m_joint = nullptr;
	}

	RevoluteJoint::RevoluteJoint(PxRigidActor* actor0, Transform& localFrame0, PxRigidActor* actor1, Transform& localFrame1)
	{
		m_joint = PxRevoluteJointCreate(*PHYSICS, actor0, localFrame0, actor1, localFrame1);
	}

	void RevoluteJoint::DriveVelocity(Fl32 value)
	{
		m_joint->setDriveVelocity(value);
		m_joint->setRevoluteJointFlag(PxRevoluteJointFlag::eDRIVE_ENABLED, true);
	}

	Fl32 RevoluteJoint::GetDriveVelocity() const
	{
		return m_joint->getDriveVelocity();
	}

	void RevoluteJoint::SetLimits(Fl32 lower, Fl32 upper)
	{
		m_joint->setLimit(PxJointAngularLimitPair(lower, upper));
		m_joint->setRevoluteJointFlag(PxRevoluteJointFlag::eLIMIT_ENABLED, true);
	}
}