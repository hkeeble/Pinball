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
|						WALL ENTITY DEFINITIONS								|
\-------------------------------------------------------------------------*/
#pragma region WallEntity
WallEntity::WallEntity(const Box& board, Fl32 fallHoleWidth, Fl32 plungerLaneWidth, Fl32 wallHeight, Fl32 wallWidth, int numberOfShapes, Transform pose,
					   Fl32 density, PxMaterial* material, Vec3 color, ActorType aType)
						: CompoundShapeActor(numberOfShapes, pose, density, material, color, aType),
						m_height(wallHeight), m_width(wallWidth), m_fallHoleWidth(fallHoleWidth), m_plungerLaneWidth(plungerLaneWidth), m_board(board)
{

}

WallEntity::WallEntity(const WallEntity& param) : CompoundShapeActor(param), 
	m_height(param.m_height), m_width(param.m_width), m_fallHoleWidth(param.m_fallHoleWidth), m_plungerLaneWidth(param.m_plungerLaneWidth), m_board(param.m_board)
{

}

WallEntity& WallEntity::operator=(const WallEntity& param)
{
	if(this == &param)
		return *this;
	else
	{
		CompoundShapeActor::operator=(param);
		return *this;
	}
}

WallEntity::~WallEntity()
{

}
#pragma endregion
/*-------------------------------------------------------------------------\
|						BORDER DEFINITIONS									|
\-------------------------------------------------------------------------*/
#pragma region Border
Border::Border(PxMaterial* material, Vec3 color, const Box& board, Fl32 fallHoleWidth, Fl32 plungerLaneWidth, Fl32 wallHeight, Fl32 wallWidth)
	: WallEntity(board, fallHoleWidth, plungerLaneWidth, wallHeight, wallWidth, BDR_SHAPES, IDENTITY_TRANS, 1, material, color, StaticActor)
{
	m_density = .1f;

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
	
Border::Border(const Border& param) : WallEntity(param)
{
	Create();
}

Border& Border::operator=(const Border& param)
{
	if(this == &param)
		return *this;
	else
	{
		WallEntity::operator=(param);
		return *this;
	}
}

Border::~Border()
{

}

void Border::Create()
{
	PxRigidStatic* rgd = PHYSICS->createRigidStatic(m_pose);
	
	// Determine Poses
	Transform p_top, p_bottom_lft, p_bottom_rgt, p_left, p_right;
	p_top = Transform(Vec3(0, 0, m_board.Dimensions().z));

	p_bottom_lft = PxTransform	(PxVec3 (m_board.Dimensions().x/2+m_fallHoleWidth+m_width,  0, -m_board.Dimensions().z));
	p_bottom_rgt = Transform	(Vec3	((-m_board.Dimensions().x/2)+(m_width*2),    0, -m_board.Dimensions().z));

	p_left	 = Transform(Vec3(-m_board.Dimensions().x, 0, 0));
	p_right  = Transform(Vec3(m_board.Dimensions().x,  0, 0));

	// Create Shapes
	PxShape* sT = rgd->createShape(m_geometrys[BDR_ID_TOP].box(), *m_material);
	PxShape* sBl = rgd->createShape(m_geometrys[BDR_ID_BTM_LFT].box(), *m_material);
	PxShape* sBr = rgd->createShape(m_geometrys[BDR_ID_BTM_RGT].box(), *m_material);
	PxShape* sL = rgd->createShape(m_geometrys[BDR_ID_LFT].box(), *m_material);
	PxShape* sR = rgd->createShape(m_geometrys[BDR_ID_RGT].box(), *m_material);

	// Set Local Poses
	sT->setLocalPose(p_top);
	sBl->setLocalPose(p_bottom_lft);
	sBr->setLocalPose(p_bottom_rgt);
	sL->setLocalPose(p_left);
	sR->setLocalPose(p_right);
	
	// Set Global Pose
	Transform t = const_cast<Box&>(m_board).Get()->getGlobalPose();
	rgd->setGlobalPose(Transform(Vec3(t.p.x, m_height+(m_board.Dimensions().y/2), t.p.z), t.q));

	m_actor = rgd;

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
InnerWalls::InnerWalls(PxMaterial* material, Vec3 color, const Box& board, Fl32 fallHoleWidth, Fl32 plungerLaneWidth, Fl32 wallWidth, Fl32 wallHeight)
	: WallEntity(board, fallHoleWidth, plungerLaneWidth, wallWidth, wallHeight, IW_SHAPES, IDENTITY_TRANS, 1.f, material, color, StaticActor)
{
	m_geometrys = new PxGeometryHolder[IW_SHAPES];

	// Geometrys
	PxBoxGeometry geos[IW_SHAPES];
	geos[IW_ID_PLUNGE_LN_WALL]		= PxBoxGeometry(Vec3(m_width, m_height, (m_board.Dimensions().z-(m_width*2) - plungerLaneWidth)));

	// Assign Geometrys
	m_geometrys[IW_ID_PLUNGE_LN_WALL].box() = geos[IW_ID_PLUNGE_LN_WALL];
}

InnerWalls::InnerWalls(const InnerWalls& param) : WallEntity(param)
{
	Create();
}

InnerWalls& InnerWalls::operator=(const InnerWalls& param)
{
	if(this == &param)
		return *this;
	else
	{
		WallEntity::operator=(param);
		return *this;
	}
}

InnerWalls::~InnerWalls()
{

}

void InnerWalls::Create()
{
	PxRigidStatic* rgd = PHYSICS->createRigidStatic(m_pose);
	
	// Determine Poses
	Transform p_pLaneWall;
	p_pLaneWall = Transform(-m_board.Dimensions().x+(m_width*2)+(m_plungerLaneWidth*2), 0, -m_width*2);

	// Create Shapes
	PxShape* pL = rgd->createShape(m_geometrys[IW_ID_PLUNGE_LN_WALL].box(), *m_material);

	// Set Local Poses
	pL->setLocalPose(p_pLaneWall);

	// Set Global Pose
	Transform t = const_cast<Box&>(m_board).Get()->getGlobalPose();
	rgd->setGlobalPose(Transform(Vec3(t.p.x, m_height+(m_board.Dimensions().y/2), t.p.z), t.q));

	m_actor = rgd;

	m_actor->userData = &m_color;
}
#pragma endregion
/*-------------------------------------------------------------------------\
|						CORNER WEDGE DEFINITIONS							|
\-------------------------------------------------------------------------*/
#pragma region CornerWedge
CornerWedge::CornerWedge(Transform pose, Vec3 color) : Actor(pose, 1.f, StaticActor)
{

}

CornerWedge::CornerWedge(const CornerWedge& param) : Actor(param)
{

}

CornerWedge& CornerWedge::operator=(const CornerWedge& param)
{
	if(this == &param)
		return *this;
	else
	{
		Actor::operator=(param);
		return *this;
	}
}

CornerWedge::~CornerWedge()
{

}

PxConvexMesh* CornerWedge::Cook()
{
	PxConvexMeshDesc convexDesc;

	return NULL;
}

void CornerWedge::Create()
{

}
#pragma endregion