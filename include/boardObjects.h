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
#define BDR_ID_TOP	0
#define BDR_ID_BTM	1
#define BDR_ID_LFT	2
#define BDR_ID_RGT	3
#define BDR_HEIGHT	0.1f
#define BDR_WIDTH	0.005f

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

#endif // _BOARD_OBJECTS_H_