/*-------------------------------------------------------------------------\
| File: MATERIALCOLLECTION.H												|
| Desc: Declarations for a material collection.								|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#include "globals.h"
#include "Physics.h"

using namespace physx;

struct MaterialCollection
{
	MaterialCollection();
	~MaterialCollection();

	Vec3 wallColor, ballColor, boardColor, plungerColor, spinnerColor, flipperColor, wedgeColor, highBumperColor, lowBumperColor;
	PxMaterial *wallMaterial, *ballMaterial, *boardMaterial, *plungerMaterial, *spinnerMaterial, *flipperMaterial, *wedgeMaterial, *bumperMaterial;
	Fl32 wallDensity, ballDensity, boardDensity, plungerDensity, spinnerDensity, flipperDensity, wedgeDensity, bumperDensity;
};