/*-------------------------------------------------------------------------\
| File: BOARDOBJECTS.CPP													|
| Desc: Provides definitions for pinball board objects						|
| Definition File: BOARDOBJECTS.H											|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#include "boardObjects.h"

using namespace Physics;

Border::Border(int numberOfShapes, Transform pose, Fl32 density, PxMaterial* material, Vec3 color, ActorType aType)
		: CompoundShapeActor(numberOfShapes, pose, density, material, color, aType)
{
	m_geometrys = new PxGeometryHolder[4];

	// Geometries
	PxBoxGeometry geos[4];
	geos[BDR_ID_TOP] = PxBoxGeometry(BRD_WIDTH, BDR_HEIGHT, BDR_WIDTH);
	geos[BDR_ID_BTM] = PxBoxGeometry(BRD_WIDTH, BDR_HEIGHT, BDR_WIDTH);
	geos[BDR_ID_LFT] = PxBoxGeometry(BDR_WIDTH, BDR_HEIGHT, BRD_LENGTH);
	geos[BDR_ID_RGT] = PxBoxGeometry(BDR_WIDTH, BDR_HEIGHT, BRD_LENGTH);

	// Assign Geometries
	m_geometrys[BDR_ID_TOP].box() = geos[BDR_ID_TOP];
	m_geometrys[BDR_ID_BTM].box() = geos[BDR_ID_BTM];
	m_geometrys[BDR_ID_LFT].box() = geos[BDR_ID_LFT];
	m_geometrys[BDR_ID_RGT].box() = geos[BDR_ID_RGT];

	// Create Object
	Create();
}
	
Border::Border(const Border& param) : CompoundShapeActor(param)
{
	Create();
}

Border& Border::operator=(const Border& param)
{
	if(this == &param)
		return *this;
	else
	{
		CompoundShapeActor::operator=(param);
		return *this;
	}
}

Border::~Border()
{
	CompoundShapeActor::~CompoundShapeActor();
}

void Border::Create()
{
	PxRigidStatic* dyn = PHYSICS->createRigidStatic(m_pose);
	
	// Determine Poses
	Transform p_top, p_bottom, p_left, p_right;
	p_top	 = PxTransform(PxVec3(0,		  BDR_HEIGHT/2, BRD_LENGTH+BRD_WIDTH));
	p_bottom = PxTransform(PxVec3(0,		  BDR_HEIGHT/2, -BRD_LENGTH-BRD_WIDTH));
	p_left	 = PxTransform(PxVec3(-BRD_WIDTH, BDR_HEIGHT/2, 0));
	p_right  = PxTransform(PxVec3(BRD_WIDTH,  BDR_HEIGHT/2, 0));

	// Create Shapes
	dyn->createShape(m_geometrys[BDR_ID_TOP].box(), *m_material, p_top);
	dyn->createShape(m_geometrys[BDR_ID_BTM].box(), *m_material, p_bottom);
	dyn->createShape(m_geometrys[BDR_ID_LFT].box(), *m_material, p_left);
	dyn->createShape(m_geometrys[BDR_ID_RGT].box(), *m_material, p_right);

	m_actor = dyn;

	m_actor->userData = &m_color;
}