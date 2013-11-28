/*-------------------------------------------------------------------------\
| File: PLUNGER.CPP															|
| Desc: Provides defintions for a pinball plunger							|
| Definition File: PLUNGER.H												|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#include "boardObjects.h"
#include "pinball.h"

typedef Pinball P;

Plunger::Plunger(PxMaterial* material, Vec3 color, Fl32 density)
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
	geos[SHAFT] = PxBoxGeometry(P::board->PlungerWidth()/2, P::board->WallHeight() - 0.01f, P::board->Dimensions().z/5);

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
		if(m_shaft)
			m_shaft->release();
		m_shaft = PHYSICS->createRigidDynamic(param.m_shaft->getGlobalPose());
		m_shaft->setRigidDynamicFlags(param.m_shaft->getRigidBodyFlags());
	}
}

Plunger::~Plunger()
{
	if(m_shaft)
		m_shaft->release();
}

void Plunger::Create()
{
	m_shaft = PHYSICS->createRigidDynamic(m_pose);

	//m_shaft->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true); // Make Kinematic

	PxShape* shaft = m_shaft->createShape(m_geometrys[SHAFT].box(), *m_material);

	// Set Global Pose
	m_shaft->setGlobalPose(P::board->Pose() * Transform(P::board->Right().x + (P::board->WallWidth()*2) + (m_geometrys[SHAFT].box().halfExtents.x*2) - 0.04f, // X
												   P::board->Dimensions().y + m_geometrys[SHAFT].box().halfExtents.y,							  // Y
													P::board->Bottom().z));																				  // Z

	m_actor = m_shaft;

	m_actor->userData = &m_color;
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