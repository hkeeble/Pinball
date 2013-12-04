/*-------------------------------------------------------------------------\
| File: BOARDOBJECTS.CPP													|
| Desc: Provides definitions for pinball board objects						|
| Definition File: BOARDOBJECTS.H											|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#include "boardObjects.h"
#include "pinball.h"

typedef Pinball P;

using namespace Physics;

/*-------------------------------------------------------------------------\
|						BOARD DEFINITIONS									 |
\-------------------------------------------------------------------------*/
#pragma region Board
Board::Board(PxMaterial* material, Vec3 color) : Box(Transform((Vec3(0, 0, 0), Quat(DEG2RAD(-25), Vec3(1, 0, 0)))),
												 Vec3(1.6f, 0.05f, 3.0f),
												 1.f,
												 color,
												 material,
												 StaticActor),
												 m_fallHoleWidth(BALL_RADIUS*4),
												 m_plungerLaneWidth(m_fallHoleWidth),
												 m_wallWidth(0.05f),
												 m_wallHeight(0.05f),
												 m_plungerWidth(m_plungerLaneWidth)
{
	Create();
}

Board::~Board()
{

}

Vec3 Board::Top()
{
	return Get().staticActor->getGlobalPose().p + Vec3(0, 0, (Dimensions().z));
}

Vec3 Board::Bottom()
{
	return Get().staticActor->getGlobalPose().p - Vec3(0, 0, (Dimensions().z));
}

Vec3 Board::Right()
{
	return Get().staticActor->getGlobalPose().p - Vec3((Dimensions().x), 0, 0);
}

Vec3 Board::Left()
{
	return Get().staticActor->getGlobalPose().p + Vec3((Dimensions().x), 0, 0); 
}

Vec3 Board::Center()
{
	return Get().staticActor->getGlobalPose().p;
}

Fl32 Board::FallHoleWidth() const
{
	return m_fallHoleWidth;
}

Fl32 Board::PlungerLaneWidth() const
{
	return m_plungerLaneWidth;
}

Fl32 Board::PlungerWidth() const
{
	return m_plungerWidth;
}

Fl32 Board::WallHeight() const
{
	return m_wallHeight;
}

Fl32 Board::WallWidth() const
{
	return m_wallWidth;
}
#pragma endregion
/*-------------------------------------------------------------------------\
|						WALL ENTITY DEFINITIONS								|
\-------------------------------------------------------------------------*/
#pragma region WallEntity
WallEntity::WallEntity(int numberOfShapes, Transform pose, Fl32 density, PxMaterial* material, Vec3 color, ActorType aType)
	: CompoundShapeActor(numberOfShapes, pose, density, material, color, aType), m_height(Pinball::board->WallHeight()), m_width(Pinball::board->WallWidth())
{

}

WallEntity::WallEntity(const WallEntity& param) : CompoundShapeActor(param), m_height(Pinball::board->WallHeight()), m_width(Pinball::board->WallWidth())
{

}

Fl32 WallEntity::Height() const
{
	return m_height;
}

Fl32 WallEntity::Width() const
{
	return m_width;
}
#pragma endregion
/*-------------------------------------------------------------------------\
|						BORDER DEFINITIONS									|
\-------------------------------------------------------------------------*/
#pragma region Border
Border::Border(PxMaterial* material, Vec3 color) : WallEntity(BDR_SHAPES, IDENTITY_TRANS, 1, material, color, StaticActor)
{
	m_density = .1f;

	m_geometrys = new PxGeometryHolder[BDR_SHAPES];

	// Geometries
	PxBoxGeometry geos[BDR_SHAPES];
	geos[BDR_ID_TOP] = PxBoxGeometry(Pinball::board->Dimensions().x, P::board->WallHeight(), P::board->WallWidth());
	
	// Bottom Left and Right Borders (allowing for plunger and fall hole)
	geos[BDR_ID_BTM_RGT] = PxBoxGeometry((Pinball::board->Dimensions().x/2)-P::board->FallHoleWidth()-(P::board->WallWidth()*2), m_height, m_width);
	geos[BDR_ID_BTM_LFT] = PxBoxGeometry((P::board->Dimensions().x/2)-P::board->FallHoleWidth(), m_height, m_width);

	geos[BDR_ID_LFT] = PxBoxGeometry(m_width, m_height, P::board->Dimensions().z);
	geos[BDR_ID_RGT] = PxBoxGeometry(m_width, m_height, P::board->Dimensions().z);

	// Assign Geometries
	m_geometrys[BDR_ID_TOP].box()	  = geos[BDR_ID_TOP];
	m_geometrys[BDR_ID_BTM_RGT].box() = geos[BDR_ID_BTM_RGT];
	m_geometrys[BDR_ID_BTM_LFT].box() = geos[BDR_ID_BTM_LFT];
	m_geometrys[BDR_ID_LFT].box()	  = geos[BDR_ID_LFT];
	m_geometrys[BDR_ID_RGT].box()	  = geos[BDR_ID_RGT];

	// Create Object
	Create();
}

Border::~Border()
{

}

void Border::Create()
{
	PxRigidStatic* rgd = PHYSICS->createRigidStatic(m_pose);
	
	Board* b = const_cast<Board*>(P::board);

	// Determine Poses
	Transform p_top, p_bottom_lft, p_bottom_rgt, p_left, p_right;
	p_top =			Transform(b->Top()	  - Vec3(0,0,m_width));
	p_bottom_lft =  Transform(Vec3(b->Left().x - m_geometrys[BDR_ID_BTM_LFT].box().halfExtents.x, 0, b->Bottom().z));
	p_bottom_rgt =  Transform(Vec3(b->Right().x + m_geometrys[BDR_ID_BTM_RGT].box().halfExtents.x + b->PlungerLaneWidth() + m_width*3, 0, b->Bottom().z));
	p_left	 =		Transform(b->Left()	  - Vec3(m_width, 0, 0));
	p_right  =		Transform(b->Right()  + Vec3(m_width, 0, -m_width));

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
	Transform t = const_cast<Board*>(P::board)->Get().staticActor->getGlobalPose();
	rgd->setGlobalPose(P::board->Pose() * Transform(0, m_height*2, 0));

	m_actor.staticActor = rgd;

	m_actor.staticActor->userData = &m_color;
}
#pragma endregion
/*-------------------------------------------------------------------------\
|						INNER WALL DEFINITIONS								|
\-------------------------------------------------------------------------*/
#pragma region InnerWalls
InnerWalls::InnerWalls(PxMaterial* material, Vec3 color)
	: WallEntity(IW_SHAPES, IDENTITY_TRANS, 1.f, material, color, StaticActor)
{
	m_geometrys = new PxGeometryHolder[IW_SHAPES];

	// Geometrys
	PxBoxGeometry geos[IW_SHAPES];
	geos[IW_ID_PLUNGE_LN_WALL] = PxBoxGeometry(Vec3(m_width, m_height, (P::board->Dimensions().z-(m_width*4) - P::board->PlungerLaneWidth())));
	geos[IW_ID_PLUNGE_LN_WALL_TP] = PxBoxGeometry(Vec3(P::board->Dimensions().x - (P::board->PlungerLaneWidth()*2) - (P::board->WallWidth()*2), m_height, m_width));

	// Flipper Walls
	geos[IW_ID_FLIPPER_WALL_LFT] = PxBoxGeometry(Vec3(P::board->Dimensions().x - P::board->PlungerWidth() * 5 - P::board->WallWidth() - P::board->FallHoleWidth() - 0.1f,
												P::board->WallHeight(),P::board->WallWidth()));
	geos[IW_ID_FLIPPER_WALL_RGT] = PxBoxGeometry(Vec3(P::board->Dimensions().x - P::board->PlungerWidth() * 5 - P::board->WallWidth() - P::board->FallHoleWidth() - 0.1f,
		P::board->WallHeight(), P::board->WallWidth()));

	// Assign Geometrys
	m_geometrys[IW_ID_PLUNGE_LN_WALL].box() = geos[IW_ID_PLUNGE_LN_WALL];
	m_geometrys[IW_ID_PLUNGE_LN_WALL_TP].box() = geos[IW_ID_PLUNGE_LN_WALL_TP];
	m_geometrys[IW_ID_FLIPPER_WALL_LFT].box() = geos[IW_ID_FLIPPER_WALL_LFT];
	m_geometrys[IW_ID_FLIPPER_WALL_RGT].box() = geos[IW_ID_FLIPPER_WALL_RGT];
}

InnerWalls::~InnerWalls()
{

}

void InnerWalls::Create()
{
	PxRigidStatic* rgd = PHYSICS->createRigidStatic(m_pose);
	
	Board* b = const_cast<Board*>(P::board);

	// Determine Poses
	Transform p_pLaneWall, p_pLaneWallTp, p_pLaneWallExt, p_fWallLft, p_fWallRgt;
	p_pLaneWall		= Transform(b->Right()	+ Vec3(b->PlungerLaneWidth() + (m_width*4), 0, b->Bottom().z+m_geometrys[IW_ID_PLUNGE_LN_WALL].box().halfExtents.z));
	p_pLaneWallTp	= Transform(b->Top()	- Vec3(m_width*2, 0, (b->PlungerLaneWidth()*3) + (m_width*6)));
	p_fWallLft		= Transform(b->Bottom() + Vec3(b->FallHoleWidth()+m_geometrys[IW_ID_FLIPPER_WALL_RGT].box().halfExtents.x+.1f, 0, .6f), PxQuat(DEG2RAD(-35), Vec3(0, 1, 0)));
	p_fWallRgt		= Transform(b->Bottom() + Vec3(-(b->FallHoleWidth()+m_geometrys[IW_ID_FLIPPER_WALL_RGT].box().halfExtents.x-.02f), 0, .6f), PxQuat(DEG2RAD(35), Vec3(0, 1, 0)));

	// Create Shapes
	PxShape* pL		   =	rgd->createShape(m_geometrys[IW_ID_PLUNGE_LN_WALL].box(), *m_material); // Plunger Lane Wall
	PxShape* PlT	   = rgd->createShape(m_geometrys[IW_ID_PLUNGE_LN_WALL_TP].box(), *m_material); // Plunger Lane Wall Top
	PxShape* pfWallLft = rgd->createShape(m_geometrys[IW_ID_FLIPPER_WALL_LFT].box(), *m_material); // Flipper Wall Left
	PxShape* pfWallRgt = rgd->createShape(m_geometrys[IW_ID_FLIPPER_WALL_RGT].box(), *m_material); // Flipper Wall Right

	// Set Local Poses
	pL->setLocalPose(p_pLaneWall);
	PlT->setLocalPose(p_pLaneWallTp);
	pfWallLft->setLocalPose(p_fWallLft);
	pfWallRgt->setLocalPose(p_fWallRgt);

	// Set Global Pose
	rgd->setGlobalPose(P::board->Pose() * Transform(0, m_height*2, 0));

	m_actor.staticActor = rgd;

	m_actor.staticActor->userData = &m_color;
}
#pragma endregion