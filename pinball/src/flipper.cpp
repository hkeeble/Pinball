#include "boardObjects.h"
#include "pinball.h"

typedef Pinball P;

Flipper::Flipper(const Transform& pose, const FlipperType& flipperType, PxMaterial* material, const Vec3& color, const Fl32& density)
: Box(pose, Vec3(P::board->WallWidth(), P::board->WallHeight(), 0.1f), density, color, material, DynamicActor)
{
	m_type = flipperType;

	Quat q = Quat(0); 
	if (flipperType == FlipperType::Left)
		q = Quat(DEG2RAD(-35), Vec3(0, 1, 0));
	else
		q = Quat(DEG2RAD(35), Vec3(0, 1, 0));

	m_pose = m_pose * Transform(P::board->Pose().q) * Transform(q);

	Create();
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
	Log::Write("\t\tCreating Flipper...\n", ENGINE_LOG);

	PxRigidDynamic* actor;
	actor = PHYSICS->createRigidDynamic(m_pose);

	m_shape = actor->createShape(PxBoxGeometry(Dimensions()), *m_material);

	m_actor.dynamicActor = actor;;
	m_actor.dynamicActor->userData = &m_color;

	m_actor.dynamicActor->setGlobalPose(m_pose * Transform(Quat(DEG2RAD(90), Vec3(0, 1, 0))));

	Vec3 hingeOffset = Vec3(0, 0, Dimensions().x/2);
	if (m_type == FlipperType::Left)
		hingeOffset.z = Dimensions().z;
	else
		hingeOffset.z = -Dimensions().z;

	Transform jointPosRight = Transform(hingeOffset, PxQuat(DEG2RAD(-90), PxVec3(0.f, 0.f, 1.f)) * PxQuat(DEG2RAD(180), PxVec3(0.f, 1.f, 0.f)));
	Transform jointPosLeft = Transform(hingeOffset, PxQuat(DEG2RAD(90), PxVec3(0.f, 0.f, 1.f)) * PxQuat(DEG2RAD(180), PxVec3(0.f, 1.f, 0.f)));

	if (m_type == FlipperType::Right)
		m_joint = RevoluteJoint(actor, jointPosRight, nullptr, actor->getGlobalPose() * jointPosRight);
	else
		m_joint = RevoluteJoint(actor, jointPosLeft, nullptr, actor->getGlobalPose() * jointPosLeft);

	m_joint.SetLimits(0, PxPi / 2); // 90 Degree Limits
}

void Flipper::Flip()
{
	m_actor.dynamicActor->wakeUp();
	m_joint.DriveVelocity(flipStrength);
}

void Flipper::Unflip()
{
	m_actor.dynamicActor->wakeUp();
	m_joint.DriveVelocity(-flipStrength);
}