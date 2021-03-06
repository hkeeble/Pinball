/*-------------------------------------------------------------------------\
| File: BOARDOBJECTS.CPP													|
| Desc: Provides definitions for pinball board objects						|
| Definition File: BOARDOBJECTS.H											|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#include "boardObjects.h"
#include "util.h"
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
	SetTexture("boardTexture.png");
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
InnerWalls::InnerWalls(PxMaterial* material, const Vec3& color)
	: WallEntity(IW_SHAPES, IDENTITY_TRANS, 1.f, material, color, StaticActor)
{
	m_geometrys = new PxGeometryHolder[IW_SHAPES];

	// Geometrys
	PxBoxGeometry geos[IW_SHAPES];
	geos[IW_ID_PLUNGE_LN_WALL] = PxBoxGeometry(Vec3(m_width, m_height, (P::board->Dimensions().z-(m_width*4) - P::board->PlungerLaneWidth())));
	geos[IW_ID_PLUNGE_LN_WALL_TP] = PxBoxGeometry(Vec3(P::board->Dimensions().x - (P::board->PlungerLaneWidth()*2) - (P::board->WallWidth()*2), m_height, m_width));

	geos[IW_ID_SWITCH_WALL_LFT] = PxBoxGeometry(Vec3(m_width, m_height, (P::board->Dimensions().z / 2) - 0.85f));
	geos[IW_ID_SWITCH_WALL_RGT] = PxBoxGeometry(Vec3(m_width, m_height, (P::board->Dimensions().z / 2) - 0.85f));

	// Assign Geometrys
	m_geometrys[IW_ID_PLUNGE_LN_WALL].box()		= geos[IW_ID_PLUNGE_LN_WALL];
	m_geometrys[IW_ID_PLUNGE_LN_WALL_TP].box()	= geos[IW_ID_PLUNGE_LN_WALL_TP];
	m_geometrys[IW_ID_SWITCH_WALL_LFT].box()	= geos[IW_ID_SWITCH_WALL_LFT];
	m_geometrys[IW_ID_SWITCH_WALL_RGT].box()	= geos[IW_ID_SWITCH_WALL_RGT];
}

InnerWalls::~InnerWalls()
{

}

void InnerWalls::Create()
{
	PxRigidStatic* rgd = PHYSICS->createRigidStatic(m_pose);
	
	Board* b = const_cast<Board*>(P::board);

	// Determine Poses
	Transform p_pLaneWall, p_pLaneWallTp, p_switchWallLft, p_switchWallRgt;

	p_pLaneWall		= Transform(b->Right()	+ Vec3(b->PlungerLaneWidth() + (m_width*4), 0, b->Bottom().z+m_geometrys[IW_ID_PLUNGE_LN_WALL].box().halfExtents.z));
	p_pLaneWallTp = Transform(b->Top() - Vec3(m_width * 2, 0, (b->PlungerLaneWidth() * 3) + (m_width * 6)));

	p_switchWallRgt = Transform(b->Right() + Vec3(0.7f, 0.f, (b->Dimensions().z / 2) - 2.9f));
	p_switchWallLft = Transform(b->Left() - Vec3(0.4f, 0.f, (b->Dimensions().z / 2) - 0.1f));

	// Create Shapes
	PxShape* pL		   = rgd->createShape(m_geometrys[IW_ID_PLUNGE_LN_WALL].box(), *m_material); // Plunger Lane Wall
	PxShape* PlT	   = rgd->createShape(m_geometrys[IW_ID_PLUNGE_LN_WALL_TP].box(), *m_material); // Plunger Lane Wall Top
	PxShape* sL		   = rgd->createShape(m_geometrys[IW_ID_SWITCH_WALL_LFT].box(), *m_material); // Plunger Lane Wall
	PxShape* sR		   = rgd->createShape(m_geometrys[IW_ID_SWITCH_WALL_RGT].box(), *m_material); // Plunger Lane Wall Top

	// Set Local Poses
	pL->setLocalPose(p_pLaneWall);
	PlT->setLocalPose(p_pLaneWallTp);
	sL->setLocalPose(p_switchWallLft);
	sR->setLocalPose(p_switchWallRgt);

	// Set Global Pose
	rgd->setGlobalPose(P::board->Pose() * Transform(0, m_height*2, 0));

	m_actor.staticActor = rgd;

	m_actor.staticActor->userData = &m_color;
}
#pragma endregion
/*-------------------------------------------------------------------------\
|						SPINNER DEFINITIONS								    |
\-------------------------------------------------------------------------*/
Spinner::Spinner(const Transform& pose, PxMaterial* material, const Vec3& color, const Fl32& density, const SpinnerType& type) : CompoundShapeActor(1, pose, density, material, color)
{
	m_spinnerType = type;

	m_geometrys = new PxGeometryHolder[3];

	// Geometrys
	PxBoxGeometry geos[5];
	geos[S_CENTER] = PxBoxGeometry(0.05f, P::board->WallHeight(), 0.05f);
	geos[S_LEFT] = PxBoxGeometry(0.2f, P::board->WallHeight(), 0.05f);
	geos[S_RIGHT] = PxBoxGeometry(0.2f, P::board->WallHeight(), 0.05f);

	// Assign Geometrys
	m_geometrys[S_CENTER].box() = geos[S_CENTER];
	m_geometrys[S_LEFT].box() = geos[S_LEFT];
	m_geometrys[S_RIGHT].box() = geos[S_RIGHT];
}

Spinner::Spinner(const Spinner& param) : CompoundShapeActor(param)
{
	m_joint = param.m_joint;
}

Spinner& Spinner::operator=(const Spinner& param)
{
	if (&param == this)
		return *this;
	else
	{
		CompoundShapeActor::operator=(param);
		m_joint = param.m_joint;
		return *this;
	}
}

Spinner::~Spinner()
{

}

void Spinner::Create()
{
	PxRigidDynamic* dyn = PHYSICS->createRigidDynamic(m_pose);

	// Create Shapes
	dyn->createShape(m_geometrys[S_CENTER].box(), *m_material);
	PxShape* left = dyn->createShape(m_geometrys[S_LEFT].box(), *m_material);
	PxShape* right = dyn->createShape(m_geometrys[S_RIGHT].box(), *m_material);

	// Set Local Poses
	left->setLocalPose(Transform(Vec3(-.25f, .0f, .0f)));
	right->setLocalPose(Transform(Vec3(.25f, .0f, .0f)));

	// Set Rotation
	dyn->setGlobalPose(m_pose * P::board->Pose() * Transform(Quat(DEG2RAD(35), Vec3(0, 1, 0))));

	m_actor.dynamicActor = dyn;

	m_actor.dynamicActor->userData = &m_color;

	Transform jointPos = Transform(Vec3(0), PxQuat(DEG2RAD(-90), PxVec3(0.f, 0.f, 1.f)) * PxQuat(DEG2RAD(180), PxVec3(0.f, 1.f, 0.f)));

	m_joint = RevoluteJoint(dyn, jointPos, nullptr, dyn->getGlobalPose() * jointPos);
	SetKinematic(true);
}

void Spinner::SetKinematic(bool isKinematic)
{
	m_actor.dynamicActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, isKinematic);
}

void Spinner::Toggle()
{
	if (m_actor.dynamicActor->getRigidBodyFlags().isSet(PxRigidBodyFlag::eKINEMATIC) == true)
	{
		SetKinematic(false);
		Get().dynamicActor->wakeUp();
		if (m_spinnerType == SpinnerType::CLOCKWISE)
			m_joint.DriveVelocity(m_drvSpeed);
		else
			m_joint.DriveVelocity(-m_drvSpeed);
	}
	else
	{
		m_joint.DriveVelocity(0);
		SetKinematic(true);
	}
}