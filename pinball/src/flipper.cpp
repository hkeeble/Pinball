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

	Vec3 hingeOffset = Vec3(Dimensions().x, 0, 0);
	if (m_type == FlipperType::Left)
		hingeOffset.z = Dimensions().z;
	else
		hingeOffset.z = -Dimensions().z;

	m_joint = RevoluteJoint(actor,   Transform(hingeOffset,							 Quat(DEG2RAD(90), Vec3(0.f, 0.f, 1.f))),
							nullptr, actor->getGlobalPose() * Transform(hingeOffset, Quat(DEG2RAD(90), Vec3(0.f, 0.f, 1.f))));
}

void Flipper::Flip()
{
	if (m_type == FlipperType::Left)
		m_joint.DriveVelocity(-80);
	else
		m_joint.DriveVelocity(80);
}

void Flipper::SetKinematic(bool isKinematic)
{

}