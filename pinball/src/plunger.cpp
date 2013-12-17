/*-------------------------------------------------------------------------\
| File: PLUNGER.CPP															|
| Desc: Provides defintions for a pinball plunger							|
| Definition File: PLUNGER.H												|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#include "boardObjects.h"
#include "pinball.h"

typedef Pinball P;

Plunger::Plunger(PxMaterial* material, const Vec3& color, const Fl32& density)
	: CompoundShapeActor
		(PSHAPES,
		IDENTITY_TRANS,
		density,
		material,
		color,
		DynamicActor)
{
	m_geometrys = new PxGeometryHolder[PSHAPES];

	// Geometries
	PxBoxGeometry geos[PSHAPES];
	geos[SHAFT] = PxBoxGeometry(P::board->PlungerLaneWidth() - (P::board->WallWidth()/2) - 0.04f, P::board->WallHeight(), P::board->Dimensions().z / 5);

	// Assign Geometries
	m_geometrys[SHAFT].box() = geos[SHAFT];

	// Create Object
	Create();
}

Plunger::Plunger(const Plunger& param) : CompoundShapeActor(param)
{
	m_shaft = PHYSICS->createRigidDynamic(param.m_shaft->getGlobalPose());
	m_shaft->setRigidDynamicFlags(param.m_shaft->getRigidBodyFlags());
}

Plunger& Plunger::operator=(const Plunger& param)
{
	if(&param == this)
		return *this;
	else
	{
		CompoundShapeActor::operator=(param);
		PX_RELEASE(m_shaft);
		m_shaft = PHYSICS->createRigidDynamic(param.m_shaft->getGlobalPose());
		m_shaft->setRigidDynamicFlags(param.m_shaft->getRigidBodyFlags());
		return *this;
	}
}

Plunger::~Plunger()
{
	PX_RELEASE(m_shaft);
}

void Plunger::Create()
{
	m_shaft = PHYSICS->createRigidDynamic(m_pose);

	PxShape* shaft = m_shaft->createShape(m_geometrys[SHAFT].box(), *m_material);

	// Set Global Pose
	m_shaft->setGlobalPose(P::board->Pose() * Transform(P::board->Right().x + P::board->WallWidth() + (m_geometrys[SHAFT].box().halfExtents.x) + 0.06f, // X
												   P::board->Dimensions().y + m_geometrys[SHAFT].box().halfExtents.y,							  // Y
													P::board->Bottom().z + 0.5f));																				  // Z
	m_initialPose = m_shaft->getGlobalPose();

	m_actor.dynamicActor = m_shaft;

	m_actor.dynamicActor->userData = &m_color;
}

void Plunger::SetKinematic(bool isKinematic)
{
	m_shaft->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, isKinematic);
}

/* Sets a relative kinematic target */
void Plunger::SetKinematicTarget(Transform target)
{
	Transform t = m_shaft->getGlobalPose();
	m_shaft->setKinematicTarget(t * target);
}

void Plunger::Reset()
{
	m_actor.dynamicActor->setGlobalPose(m_initialPose);
	m_ready = true;
	SetKinematic(true);
}

void Plunger::SetReady(bool isReady)
{
	m_ready = isReady;
}

bool Plunger::IsReady()
{
	return m_ready;
}