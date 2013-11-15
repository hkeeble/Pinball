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

class Border : public CompoundShapeActor
{
public:
	Border(int numberOfShapes = 1, Transform pose = IDENTITY_TRANS, Fl32 density = DEFAULT_DENSITY, PxMaterial* material = DEFAULT_MATERIAL,
		Vec3 color = DEFAULT_COLOR, ActorType aType = DEFAULT_ACTOR_TYPE);
	Border(const Border& param);
	virtual Border& operator=(const Border& param);
	virtual ~Border();

	virtual void Create();
};

// Inner Wall Defines
#define IW_SHAPES				 3
#define IW_ID_PLUNGE_LN_WALL	 0
#define IW_ID_PLUNGE_LN_WALL_TP	 1
#define IW_ID_PLUNGE_LN_WALL_EXT 2

class InnerWalls : public CompoundShapeActor
{
public:
	InnerWalls(int numberOfShapes = 1, Transform pose = IDENTITY_TRANS, Fl32 density = DEFAULT_DENSITY, PxMaterial* material = DEFAULT_MATERIAL,
		Vec3 color = DEFAULT_COLOR, ActorType aType = DEFAULT_ACTOR_TYPE);
	InnerWalls(const Border& param);
	virtual InnerWalls& operator=(const Border& param);
	virtual ~InnerWalls();

	virtual void Create();
};

#endif // _BOARD_OBJECTS_H_