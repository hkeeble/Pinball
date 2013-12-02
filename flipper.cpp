/*-------------------------------------------------------------------------\
| File: FLIPPER.CPP															|
| Desc: Provides defintions for a pinball flipper							|
| Definition File: BOARDOBJECTS.H											|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#include "boardObjects.h"

Flipper::Flipper(PxMaterial* material, Vec3 color, Fl32 density)
: ConvexMeshActor( IDENTITY_TRANS,
				   density,
				   color,
				   material,
				   Vec3(1, 1, 1),
				   DynamicActor )
{
	isFlipped = false;
}

Flipper::Flipper(const Flipper& param) : ConvexMeshActor(param)
{

}

Flipper& Flipper::operator=(const Flipper& param)
{
	if (&param == this)
		return *this;
	else
	{
		ConvexMeshActor::operator=(param);
		return *this;
	}
}

Flipper::~Flipper()
{

}

void Flipper::Flip()
{ 
	isFlipped = true;
}