/*-------------------------------------------------------------------------\
| File: FLIPPER.CPP															|
| Desc: Provides defintions for a pinball flipper							|
| Definition File: BOARDOBJECTS.H											|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#include "boardObjects.h"
#include "pinball.h"

Flipper::Flipper(Transform pose, PxMaterial* material, Vec3 color, Fl32 density)
: Wedge			 ( pose,
				   density,
				   color,
				   material,
				   Vec3(.075f, .07f, .03f),
				   DynamicActor )
{
	isFlipped = false;
	Create();
	SetKinematic(true);
	Get().dynamicActor->setGlobalPose(Get().dynamicActor->getGlobalPose() * Transform(Pinball::board->Get().staticActor->getGlobalPose().q));
}

Flipper::Flipper(const Flipper& param) : Wedge(param)
{

}

Flipper& Flipper::operator=(const Flipper& param)
{
	if (&param == this)
		return *this;
	else
	{
		Wedge::operator=(param);
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

void Flipper::SetKinematic(bool isKinematic)
{
	Get().dynamicActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, isKinematic);
}