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

Plunger::~Plunger()
{

}

void Plunger::Create()
{
	PxRigidDynamic* rgd = PHYSICS->createRigidDynamic(m_pose);

	rgd->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true); // Make Kinematic

	PxShape* shaft = rgd->createShape(m_geometrys[SHAFT].box(), *m_material);

	// Set Global Pose
	rgd->setGlobalPose(P::board->Pose() * Transform(P::board->Right().x + (P::board->WallWidth()*2) + (m_geometrys[SHAFT].box().halfExtents.x*2) - 0.04f, // X
												   P::board->Dimensions().y + m_geometrys[SHAFT].box().halfExtents.y,							  // Y
													P::board->Bottom().z));																				  // Z

	m_actor = rgd;

	m_actor->userData = &m_color;
}