#include "boardObjects.h"
#include "pinball.h"

typedef Pinball P;

Flipper::Flipper(const Transform& pose, const FlipperType& flipperType, PxMaterial* material, const Vec3& color, const Fl32& density)
: Box(pose, Vec3(P::board->WallWidth(), P::board->WallHeight(), 0.1f), density, color, material, DynamicActor)
{
	m_type = flipperType;
	m_pose = m_pose * Transform(P::board->Pose().q);
}

Flipper::Flipper(const Flipper& param) : Box(param)
{
	m_joint = param.m_joint;
}

Flipper& Flipper::operator=(const Flipper& param)
{
	if (this == &param)
		return *this;
	else
	{
		m_joint = param.m_joint;
		Box::operator=(param);
		return *this;
	}
}

Flipper::~Flipper()
{

}

void Flipper::Create()
{
	PxRigidDynamic* actor;
	actor = PHYSICS->createRigidDynamic(m_pose);

	m_shape = actor->createShape(PxBoxGeometry(Dimensions()), *m_material);

	m_actor.dynamicActor = actor;;
	m_actor.dynamicActor->userData = &m_color;

	m_actor.dynamicActor->setGlobalPose(m_pose * Transform(Quat(DEG2RAD(90), Vec3(0, 1, 0))));

	Fl32 hingeOffset = 0.f;
	if (m_type == FlipperType::Left)
		hingeOffset = Dimensions().z * 2;
	else
		hingeOffset = -(Dimensions().z * 2);

	m_joint = RevoluteJoint(actor, PxTransform(PxVec3(0.f, 0.f, hingeOffset), PxQuat(DEG2RAD(90), PxVec3(0.f, 0.f, 1.f))),
		nullptr, actor->getGlobalPose() * PxTransform(PxVec3(0.f, 0.f, hingeOffset), PxQuat(DEG2RAD(90), PxVec3(0.f, 0.f, 1.f))));
	// m_joint.SetLimits(45, 45);
}

void Flipper::Flip()
{
	m_actor.dynamicActor->addForce(Vec3(0, 0, 70));
}

void Flipper::SetKinematic(bool isKinematic)
{

}