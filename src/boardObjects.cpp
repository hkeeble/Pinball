/*-------------------------------------------------------------------------\
| File: BOARDOBJECTS.CPP													|
| Desc: Provides definitions for pinball board objects						|
| Definition File: BOARDOBJECTS.H											|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#include "boardObjects.h"

namespace Pinball
{
	using namespace Physics;

	Border::Border(int numberOfShapes, Transform pose, Fl32 density, PxMaterial* material, Vec3 color, ActorType aType)
			: CompoundShapeActor(numberOfShapes, pose, density, material, color, aType)
	{

	}
	
	Border::Border(const Border& param) : CompoundShapeActor(param)
	{

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

	}
}