/*-------------------------------------------------------------------------\
| File: BOARDOBJECTS.CPP													|
| Desc: Provides definitions for pinball board objects						|
| Definition File: BOARDOBJECTS.H											|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#include "boardObjects.h"
#include "pinball.h"

using namespace Physics;

/*-------------------------------------------------------------------------\
|						BORDER DEFINITIONS									|
\-------------------------------------------------------------------------*/
#pragma region Border
Border::Border(PxMaterial* material, Vec3 color, const Box& board, Fl32 fallHoleWidth, Fl32 plungerLaneWidth)
		: CompoundShapeActor(BDR_SHAPES, IDENTITY_TRANS, 1, material, color, StaticActor),
		m_height(0.1f), m_width(0.05f), m_fallHoleWidth(fallHoleWidth), m_plungerLaneWidth(plungerLaneWidth), m_board(board)
{
	m_geometrys = new PxGeometryHolder[BDR_SHAPES];

	// Geometries
	PxBoxGeometry geos[BDR_SHAPES];
	geos[BDR_ID_TOP] = PxBoxGeometry(m_board.Dimensions().x, m_height, m_width);

	// Bottom Left and Right Borders (allowing for plunger and fall hole)
	geos[BDR_ID_BTM_RGT] = PxBoxGeometry((m_board.Dimensions().x/2)-fallHoleWidth-plungerLaneWidth, m_height, m_width);
	geos[BDR_ID_BTM_LFT] = PxBoxGeometry((m_board.Dimensions().x/2)-fallHoleWidth, m_height, m_width);

	geos[BDR_ID_LFT] = PxBoxGeometry(m_width, m_height, m_board.Dimensions().z);
	geos[BDR_ID_RGT] = PxBoxGeometry(m_width, m_height, m_board.Dimensions().z);

	// Assign Geometries
	m_geometrys[BDR_ID_TOP].box()	  = geos[BDR_ID_TOP];
	m_geometrys[BDR_ID_BTM_RGT].box() = geos[BDR_ID_BTM_RGT];
	m_geometrys[BDR_ID_BTM_LFT].box() = geos[BDR_ID_BTM_LFT];
	m_geometrys[BDR_ID_LFT].box()	  = geos[BDR_ID_LFT];
	m_geometrys[BDR_ID_RGT].box()	  = geos[BDR_ID_RGT];

	// Create Object
	Create();
}
	
Border::Border(const Border& param) : CompoundShapeActor(param),
	m_height(0.1f), m_width(0.05f), m_fallHoleWidth(param.m_fallHoleWidth), m_plungerLaneWidth(param.m_plungerLaneWidth), m_board(param.m_board)
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
	p_top = Transform(Vec3(0, m_height+(m_board.Dimensions().y), m_board.Dimensions().z));

	p_bottom_lft = PxTransform	(PxVec3 (m_board.Dimensions().x/2+m_fallHoleWidth+m_width,  m_height+(m_board.Dimensions().y), -m_board.Dimensions().z));
	p_bottom_rgt = Transform	(Vec3	((-m_board.Dimensions().x/2)+m_plungerLaneWidth,    m_height+(m_board.Dimensions().y), -m_board.Dimensions().z));

	p_left	 = Transform(Vec3(-m_board.Dimensions().x, m_height+(m_board.Dimensions().y), 0));
	p_right  = Transform(Vec3(m_board.Dimensions().x,  m_height+(m_board.Dimensions().y), 0));

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
	dyn->setGlobalPose(const_cast<Box&>(m_board).Get()->getGlobalPose());

	m_actor = dyn;

	m_actor->userData = &m_color;
}

Fl32 Border::Height() const
{
	return m_height;
}

Fl32 Border::Width() const
{
	return m_width;
}
#pragma endregion
/*-------------------------------------------------------------------------\
|						INNER WALL DEFINITIONS								|
\-------------------------------------------------------------------------*/
#pragma region InnerWalls
InnerWalls::InnerWalls(int numberOfShapes, Transform pose, Fl32 density, PxMaterial* material, Vec3 color, ActorType aType)
{
	m_geometrys = new PxGeometryHolder[IW_SHAPES];

	//// Geometrys
	//PxBoxGeometry geos[IW_SHAPES];
	//geos[IW_ID_PLUNGE_LN_WALL]		= 
	//geos[IW_ID_PLUNGE_LN_WALL_TP]	= 
	//geos[IW_ID_PLUNGE_LN_WALL_EXT]	= 

	//// Assign Geometrys
	//m_geometrys[IW_ID_PLUNGE_LN_WALL].any() = geos[IW_ID_PLUNGE_LN_WALL];
	//m_geometrys[IW_ID_PLUNGE_LN_WALL_TP].any() = geos[IW_ID_PLUNGE_LN_WALL_TP];
	//m_geometrys[IW_ID_PLUNGE_LN_WALL_EXT].any() = geos[IW_ID_PLUNGE_LN_WALL_EXT];
}

InnerWalls::InnerWalls(const Border& param)
{

}

InnerWalls& InnerWalls::operator=(const Border& param)
{
	return *this;
}

InnerWalls::~InnerWalls()
{

}

void InnerWalls::Create()
{

}
#pragma endregion