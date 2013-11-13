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

namespace Pinball
{
	using namespace Physics;
	
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
}

#endif // _BOARD_OBJECTS_H_