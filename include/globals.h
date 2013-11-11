/*-------------------------------------------------------------------------\
| File: GLOBALS.H															|
| Desc: Provides declarations global defines and type defines				|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#include "PxPhysicsAPI.h" // Include PhysX

// Defaults
#define DEFAULT_DENSITY		 0.f
#define DEFAULT_COLOR		 Vec3(0.f, 0.f, 0.f)
#define DEFAULT_MATERIAL	 Physics::PxGetPhysics()->createMaterial(0.0f, 0.0f, 0.0f)
#define IDENTITY_TRANS		 Transform(PxIdentity)
#define DEFAULT_FOV			 60

// Actors
#define MAX_NUM_ACTOR_SHAPES 128

// Default Up Vector
#define UP_VECTOR			Vec3(0, 1, 0)

// Retrieve Physics
#define PHYSICS				Physics::PxGetPhysics()

typedef physx::PxVec3		Vec3;
typedef physx::PxVec2		Vec2;
typedef physx::PxTransform	Transform;
typedef physx::PxMat44		Mat44;
typedef physx::PxMat33		Mat33;
typedef physx::PxReal		Fl32;
