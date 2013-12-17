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
		m_joint->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, true);
		m_joint->setRevoluteJointFlag(PxRevoluteJointFlag::eDRIVE_ENABLED, true);
		m_joint->setDriveForceLimit(5);
	}

	void RevoluteJoint::DriveVelocity(Fl32 value)
	{
		m_joint->setDriveVelocity(value);
	}

	Fl32 RevoluteJoint::GetDriveVelocity() const
	{
		return m_joint->getDriveVelocity();
	}

	Fl32 RevoluteJoint::GetAngle() const
	{
		return m_joint->getAngle();
	}

	void RevoluteJoint::SetLimits(Fl32 lower, Fl32 upper)
	{
		m_joint->setRevoluteJointFlag(PxRevoluteJointFlag::eLIMIT_ENABLED, true);
		m_joint->setRevoluteJointFlag(PxRevoluteJointFlag::eDRIVE_FREESPIN, true);
		m_joint->setLimit(PxJointAngularLimitPair(lower, upper));
	}
}