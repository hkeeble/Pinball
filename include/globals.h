 /*-------------------------------------------------------------------------\
| File: GLOBALS.H															|
| Desc: Provides declarations global defines and type defines				|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include "PxPhysicsAPI.h" // Include PhysX

// Typedefs
typedef physx::PxVec3		Vec3;
typedef physx::PxVec2		Vec2;
typedef physx::PxTransform	Transform;
typedef physx::PxQuat		Quat;
typedef physx::PxMat44		Mat44;
typedef physx::PxMat33		Mat33;
typedef physx::PxReal		Fl32;

// Structs
struct Color
{
	Color(Fl32 R, Fl32 G, Fl32 B) : r(R), g(G), b(B) { }
	~Color() { }
	Fl32 r, g, b;
};

// Defaults
#define DEFAULT_DENSITY		 0.f
#define DEFAULT_COLOR		 Vec3(0.f, 0.f, 0.f)
#define DEFAULT_MATERIAL	 Physics::PxGetPhysics()->createMaterial(0.0f, 0.0f, 0.0f)
#define DEFAULT_FOV			 60
#define IDENTITY_TRANS		 Transform(PxIdentity)
#define DEFAULT_ACTOR_TYPE	 DynamicActor
#define DEFAULT_GEOMETRY	 PxBoxGeometry(1, 1, 1)->getGeometry()
#define DEFAULT_ORIENTATION  Quat(0, 0, 0)

// Default Up Vector
#define UP_VECTOR Vec3(0, 1, 0)

// Retrieve Physics
#define PHYSICS	Physics::PxGetPhysics()

// Macros
#define StaticCast(data, type) static_cast<type>(data)

// Controls
#define VK_CAMERA_LFT GLUT_KEY_LEFT
#define VK_CAMERA_RGT GLUT_KEY_RIGHT
#define VK_CAMERA_FWD GLUT_KEY_UP
#define VK_CAMERA_BCK GLUT_KEY_DOWN
#define VK_EXIT		  27

// Math
#define PI 3.14159265359
#define DEG2RAD(data) data * (PI/180)

// Glass index
#define GLASS_ATR_IDX 1

#define BALL_RADIUS 0.039375f // 1/16th of an inch

#endif _GLOBALS_H_