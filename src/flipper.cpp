/*-------------------------------------------------------------------------\
| File: FLIPPER.CPP															|
| Desc: Provides defintions for a pinball flipper							|
| Definition File: BOARDOBJECTS.H											|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#include "boardObjects.h"
#include "pinball.h"

Flipper::Flipper(const Transform& pose, PxMaterial* material, const Vec3& color, const Fl32& density, const Transform& relativeJointPose)
: Wedge			 ( pose,
				   density,
				   color,
				   material,
				   Vec3(.075f, .07f, .04f),
				   DynamicActor )
{
	m_isFlipped = false;
	Create();
	//SetKinematic(true);
	
	// Create Joint
	//m_joint = RevoluteJoint(Get().dynamicActor, Pose() * Transform(Quat(DEG2RAD(90), PxVec3(0.f, 1.f, 0.f))), nullptr, Pose() * relativeJointPose);
}

Flipper::Flipper(const Flipper& param) : Wedge(param)
{

}

Flipper& Flipper::operator=(const Flipper& param)
{
	if (&param == this)
		return *this;
	else
	{
		Wedge::operator=(param);
		return *this;
	}
}

Flipper::~Flipper()
{

}

void Flipper::Flip()
{ 
	m_isFlipped = true;
	m_joint.DriveVelocity(1000.f);
}

void Flipper::SetKinematic(bool isKinematic)
{
	Get().dynamicActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, isKinematic);
}