#ifndef _GLOBALS_GLUT_GAME_H_
#define _GLOBALS_GLUT_GAME_H_

#include "PxPhysicsAPI.h" // Include PhysX

// Typedefs
typedef physx::PxVec3		Vec3;
typedef physx::PxVec2		Vec2;
typedef physx::PxTransform	Transform;
typedef physx::PxQuat		Quat;
typedef physx::PxMat44		Mat44;
typedef physx::PxMat33		Mat33;
typedef physx::PxReal		Fl32;

// Retrieve Physics
#define PHYSICS	Physics::PxGetPhysics()

#endif _GLOBALS_GLUT_GAME_H_