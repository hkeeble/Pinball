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

class WallEntity : public CompoundShapeActor // A board object that contains walls
{
protected:
	// Constants
	const Fl32 m_height;
	const Fl32 m_width;
	const Box m_board;
    const Fl32 m_fallHoleWidth, m_plungerLaneWidth;
public:
	WallEntity(const Box& board, Fl32 fallHoleWidth, Fl32 plungerLaneWidth, Fl32 wallHeight,
			Fl32 wallWidth, int numberOfShapes = 1, Transform pose = IDENTITY_TRANS, Fl32 density = DEFAULT_DENSITY, PxMaterial* material = DEFAULT_MATERIAL,
			Vec3 color = DEFAULT_COLOR, ActorType aType = DEFAULT_ACTOR_TYPE);
	WallEntity(const WallEntity& param);
	virtual WallEntity& operator=(const WallEntity& param);

	virtual ~WallEntity();
};

// Border Defines
#define BDR_SHAPES		5
#define BDR_ID_TOP		0
#define BDR_ID_BTM_LFT	1
#define BDR_ID_BTM_RGT	2
#define BDR_ID_LFT		3
#define BDR_ID_RGT		4

// Inner Wall Defines
#define IW_SHAPES				 1
#define IW_ID_PLUNGE_LN_WALL	 0
#define IW_ID_PLUNGE_LN_WALL_TP	 1
#define IW_ID_PLUNGE_LN_WALL_EXT 2

class Border : public WallEntity
{
public:
	Border(PxMaterial* material, Vec3 color, const Box& board, Fl32 fallHoleWidth, Fl32 plungerLaneWidth, Fl32 wallWidth, Fl32 wallHeight);
	Border(const Border& param);
	virtual Border& operator=(const Border& param);
	virtual ~Border();

	virtual void Create();

	Fl32 Height() const;
	Fl32 Width() const;
};

class InnerWalls : public WallEntity
{
public:
	InnerWalls(PxMaterial* material, Vec3 color, const Box& board, Fl32 fallHoleWidth, Fl32 plungerLaneWidth, Fl32 wallWidth, Fl32 wallHeight);
	InnerWalls(const InnerWalls& param);
	virtual InnerWalls& operator=(const InnerWalls& param);
	virtual ~InnerWalls();

	virtual void Create();
};

class CornerWedge : public Actor
{
private:
	PxConvexMesh* Cook();
public:
	CornerWedge(Transform pose, Vec3 color);
	CornerWedge(const CornerWedge& param);
	virtual CornerWedge& operator=(const CornerWedge& param);
	~CornerWedge();

	virtual void Create();
};
#endif // _BOARD_OBJECTS_H_