/*-------------------------------------------------------------------------\
| File: BOARDOBJECTS.CPP													|
| Desc: Provides definitions for pinball board objects						|
| Definition File: BOARDOBJECTS.H											|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#include "boardObjects.h"

using namespace Physics;

/*-------------------------------------------------------------------------\
|						BORDER DEFINITIONS									|
\-------------------------------------------------------------------------*/
Border::Border(int numberOfShapes, Transform pose, Fl32 density, PxMaterial* material, Vec3 color, ActorType aType)
		: CompoundShapeActor(numberOfShapes, pose, density, material, color, aType)
{
	m_geometrys = new PxGeometryHolder[BDR_SHAPES];

	// Geometries
	PxBoxGeometry geos[BDR_SHAPES];
	geos[BDR_ID_TOP] = PxBoxGeometry(BRD_WIDTH, BDR_HEIGHT, BDR_WIDTH);

	// Bottom Left and Right Borders (allowing for plunger and fall hole)
	geos[BDR_ID_BTM_RGT] = PxBoxGeometry((BRD_WIDTH/2)-FALL_HOLE_WIDTH-PLUNGER_LANE_WIDTH, BDR_HEIGHT, BDR_WIDTH);
	geos[BDR_ID_BTM_LFT] = PxBoxGeometry((BRD_WIDTH/2)-FALL_HOLE_WIDTH, BDR_HEIGHT, BDR_WIDTH);

	geos[BDR_ID_LFT] = PxBoxGeometry(BDR_WIDTH, BDR_HEIGHT, BRD_LENGTH);
	geos[BDR_ID_RGT] = PxBoxGeometry(BDR_WIDTH, BDR_HEIGHT, BRD_LENGTH);

	// Assign Geometries
	m_geometrys[BDR_ID_TOP].box()	  = geos[BDR_ID_TOP];
	m_geometrys[BDR_ID_BTM_RGT].box() = geos[BDR_ID_BTM_RGT];
	m_geometrys[BDR_ID_BTM_LFT].box() = geos[BDR_ID_BTM_LFT];
	m_geometrys[BDR_ID_LFT].box()	  = geos[BDR_ID_LFT];
	m_geometrys[BDR_ID_RGT].box()	  = geos[BDR_ID_RGT];

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
	Transform p_top, p_bottom_lft, p_bottom_rgt, p_left, p_right;
	p_top = Transform(Vec3(0, BDR_HEIGHT/2, BRD_LENGTH));

	p_bottom_lft = PxTransform(PxVec3(BRD_WIDTH/2+FALL_HOLE_WIDTH, BDR_HEIGHT/2, -BRD_LENGTH));
	p_bottom_rgt = Transform(Vec3((-BRD_WIDTH/2)+PLUNGER_LANE_WIDTH, BDR_HEIGHT/2, -BRD_LENGTH));

	p_left	 = Transform(Vec3(-BRD_WIDTH, BDR_HEIGHT/2, 0));
	p_right  = Transform(Vec3(BRD_WIDTH,  BDR_HEIGHT/2, 0));

	// Create Shapes
	PxShape* sT = dyn->createShape(m_geometrys[BDR_ID_TOP].box(), *m_material);
	PxShape* sBl = dyn->createShape(m_geometrys[BDR_ID_BTM_RGT].box(), *m_material);
	PxShape* sBr = dyn->createShape(m_geometrys[BDR_ID_BTM_RGT].box(), *m_material);
	PxShape* sL = dyn->createShape(m_geometrys[BDR_ID_LFT].box(), *m_material);
	PxShape* sR = dyn->createShape(m_geometrys[BDR_ID_RGT].box(), *m_material);

	// Set Local Poses
	sT->setLocalPose(p_top);
	sBl->setLocalPose(p_bottom_lft);
	sBr->setLocalPose(p_bottom_rgt);
	sL->setLocalPose(p_left);
	sR->setLocalPose(p_right);

	// Set Rotation
	dyn->setGlobalPose(Transform(Vec3(0, 0, BRD_Z_OFFSET), Quat(BRD_ROT, Vec3(1, 0, 0))));

	m_actor = dyn;

	m_actor->userData = &m_color;
}

/*-------------------------------------------------------------------------\
|						INNER WALL DEFINITIONS								|
\-------------------------------------------------------------------------*/
InnerWalls::InnerWalls(int numberOfShapes = 1, Transform pose = IDENTITY_TRANS, Fl32 density = DEFAULT_DENSITY, PxMaterial* material = DEFAULT_MATERIAL,
		Vec3 color = DEFAULT_COLOR, ActorType aType = DEFAULT_ACTOR_TYPE)
{
	m_geometrys = new PxGeometryHolder[IW_SHAPES];

	//// Geometrys
	//PxBoxGeometry geos[IW_SHAPES];
	//geos[IW_ID_PLUNGE_LN_WALL]		= 
	//geos[IW_ID_PLUNGE_LN_WALL_TP]	= 
	//geos[IW_ID_PLUNGE_LN_WALL_EXT]	= 

	//// Assign Geometrys


}

InnerWalls::InnerWalls(const Border& param)
{

}

InnerWalls& InnerWalls::operator=(const Border& param)
{

}

InnerWalls::~InnerWalls()
{

}

void InnerWalls::Create()
{

}