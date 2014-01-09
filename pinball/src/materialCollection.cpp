/*-------------------------------------------------------------------------\
| File: MATERIALCOLLECTION.CPP												|
| Desc: Definitions for a material collection.								|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#include "materialCollection.h"

using namespace physx;

MaterialCollection::MaterialCollection()
{
	// Colors
	wallColor		= Vec3(0.f, 0.f, 0.f);
	ballColor		= Vec3(.8f, .8f, .8f);
	boardColor		= Vec3(.4f, .4f, .4f);
	plungerColor	= Vec3(.1f, .1f, 1.f);
	spinnerColor	= Vec3(1.f, .0f, .0f);
	flipperColor	= Vec3(.5f, 0.f, 0.f);
	wedgeColor		= Vec3(.5f, .5f, .5f);
	highBumperColor = Vec3(1, 1, .5f);
	lowBumperColor	= Vec3(0.f, 0.f, 1.f);

	// Densities
	wallDensity		= 1.f;
	ballDensity		= 1.f;
	boardDensity	= 1.f;
	plungerDensity	= 2.f;
	spinnerDensity	= .5f;
	flipperDensity	= .5f;
	wedgeDensity	= 1.f;
	bumperDensity	= 1.f;

	// Materials
	PxMaterial* steel	   = PHYSICS->createMaterial(0.0005f, 0.0004f, 0.397f);
	PxMaterial* wood	   = PHYSICS->createMaterial(0.0012f, 0.0011f, 0.403f);
	PxMaterial* hardRubber = PHYSICS->createMaterial(0.0077f, 0.0076f, 0.928f);

	// Game Materials
	wallMaterial	= steel;
	ballMaterial	= steel;
	boardMaterial	= steel;
	plungerMaterial = steel;
	spinnerMaterial = steel;
	flipperMaterial = steel;
	wedgeMaterial	= steel;
	bumperMaterial  = hardRubber;
}

MaterialCollection::~MaterialCollection()
{
	PX_RELEASE(wallMaterial);
	PX_RELEASE(ballMaterial);
	PX_RELEASE(boardMaterial);
	PX_RELEASE(plungerMaterial);
	PX_RELEASE(spinnerMaterial);
	PX_RELEASE(flipperMaterial);
	PX_RELEASE(wedgeMaterial);
}