#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include "PxPhysicsAPI.h"

// Typedefs
typedef physx::PxVec3		Vec3;
typedef physx::PxVec2		Vec2;
typedef physx::PxTransform	Transform;
typedef physx::PxQuat		Quat;
typedef physx::PxMat44		Mat44;
typedef physx::PxMat33		Mat33;
typedef physx::PxReal		Fl32;

/* MACROS */
#define PHYSICS	Physics::PxGetPhysics()

// Release Macro
#define RELEASE(x) if(x != nullptr) delete x

// Multiple Release Macro
#define RELEASE_MULTI(x) if(x != nullptr) delete [] x

// Release macro for PX objects
#define PX_RELEASE(x) if(x != nullptr) x->release()

#define StaticCast(data, type) static_cast<type>(data)

// Math
#define PI 3.14159265359
#define DEG2RAD(data) data * (PI/180)

/* DEFAULTS */
#define UP_VECTOR			Vec3(0, 1, 0)
#define DEFAULT_DENSITY		0.f
#define DEFAULT_COLOR		Vec3(0.f, 0.f, 0.f)
#define DEFAULT_MATERIAL	Physics::PxGetPhysics()->createMaterial(0.0f, 0.0f, 0.0f)
#define DEFAULT_FOV			60
#define IDENTITY_TRANS		Transform(PxIdentity)
#define DEFAULT_ACTOR_TYPE	DynamicActor
#define DEFAULT_GEOMETRY	PxBoxGeometry(1, 1, 1)
#define DEFAULT_ORIENTATION Quat(0.f, 0.f, 0.f, 0.f)

// Max Vertices
#define VERTEX_LIMIT 256

// Convex Mesh Vertices
const Vec3 wedge_verts[] = {
	Vec3(-1, 0, -1),
	Vec3(-1, 0, 1),
	Vec3(1, 0, 1),
	Vec3(1, 1, 1),
	Vec3(1, 1, -1),
	Vec3(1, 0, -1)
};

const Vec3 pyramid_verts[] = {
	Vec3(0, 1, 0),
	Vec3(1, 0, 0),
	Vec3(-1, 0, 0),
	Vec3(0, 0, 1),
	Vec3(0, 0, -1)
};

#endif // _GLOBALS_H_