/*-------------------------------------------------------------------------\
| File: BOARDOBJECTS.H														|
| Desc: Provides declarations for pinball board objects						|
| Definition File: BOARDOBJECTS.CPP											|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#ifndef _BOARD_OBJECTS_H_
#define _BOARD_OBJECTS_H_

#include "Actors.h"
#include "globals.h"

using namespace Physics;

// Border Defines
#define BDR_SHAPES		5
#define BDR_ID_TOP		0
#define BDR_ID_BTM_LFT	1
#define BDR_ID_BTM_RGT	2
#define BDR_ID_LFT		3
#define BDR_ID_RGT		4

// Inner Wall Defines
#define IW_SHAPES				 2
#define IW_ID_PLUNGE_LN_WALL	 0
#define IW_ID_PLUNGE_LN_WALL_TP	 1

// Plunger Defines
#define PSHAPES 1
#define SHAFT	0
#define TIP		1

/* Main Board Class */
class Board : public Box
{
private:
	const Fl32 m_fallHoleWidth;
	const Fl32 m_plungerLaneWidth;
	const Fl32 m_plungerWidth;
	const Fl32 m_wallHeight;
	const Fl32 m_wallWidth;
public:
	Board(PxMaterial* material, Vec3 color);
	~Board();

	Vec3 Top();
	Vec3 Left();
	Vec3 Right();
	Vec3 Bottom();
	Vec3 Center();

	Fl32 WallHeight() const;
	Fl32 WallWidth() const;
	Fl32 FallHoleWidth() const;
	Fl32 PlungerLaneWidth() const;
	Fl32 PlungerWidth() const;
};

/* Abstract Wall Entity class, for borders and inner walls */
class WallEntity : public CompoundShapeActor
{
protected:
	const Fl32 m_width, m_height;
public:
	WallEntity(int numberOfShapes = 1, Transform pose = IDENTITY_TRANS, Fl32 density = DEFAULT_DENSITY, PxMaterial* material = DEFAULT_MATERIAL,
			Vec3 color = DEFAULT_COLOR, ActorType aType = DEFAULT_ACTOR_TYPE);
	WallEntity(const WallEntity& param);

	Fl32 Width() const;
	Fl32 Height() const;
};

/* Represents the board's borders */
class Border : public WallEntity
{
public:
	Border(PxMaterial* material, Vec3 color);
	~Border();
	virtual void Create();
};

/* Compound shape actor containing all inner wall shapes */
class InnerWalls : public WallEntity
{
public:
	InnerWalls(PxMaterial* material, Vec3 color);
	~InnerWalls();
	virtual void Create();
};

/* Compound shape representing a plunger */
class Plunger : public CompoundShapeActor
{
private:
	PxRigidDynamic* m_shaft;
public:
	Plunger(PxMaterial* material, Vec3 color, Fl32 density);
	Plunger(const Plunger& param);
	Plunger& operator=(const Plunger& param);
	~Plunger();
	void SetKinematic(bool isKinematic);
	void SetKinematicTarget(Transform target);
	virtual void Create();
};

/* Convex mesh sactor representing wedges in the corners of the board */
class CornerWedge : public Actor
{
private:
	PxConvexMesh* Cook();
public:
	CornerWedge(Transform pose, Vec3 color);
	~CornerWedge();

	virtual void Create();
};

#endif // _BOARD_OBJECTS_H_