/*-------------------------------------------------------------------------\
| File: GLOBALS.H															|
| Desc: Provides declarations global defines and type defines				|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include "PxPhysicsAPI.h" // Include PhysX

// Defaults
#define DEFAULT_DENSITY		 0.f
#define DEFAULT_COLOR		 Vec3(0.f, 0.f, 0.f)
#define DEFAULT_MATERIAL	 Physics::PxGetPhysics()->createMaterial(0.0f, 0.0f, 0.0f)
#define DEFAULT_FOV			 60
#define IDENTITY_TRANS		 Transform(PxIdentity)
#define DEFAULT_ACTOR_TYPE	 DynamicActor
#define DEFAULT_GEOMETRY	 PxBoxGeometry(1, 1, 1)->getGeometry()
#define DEFAULT_ORIENTATION  Quat(0, 0, 0)

// Actors
#define MAX_NUM_ACTOR_SHAPES 128

// Default Up Vector
#define UP_VECTOR			Vec3(0, 1, 0)

// Retrieve Physics
#define PHYSICS				Physics::PxGetPhysics()

// Macros
#define StaticCast(data, type) static_cast<type>(data)

// Board Height and Width
#define BRD_WIDTH    1.6f
#define BRD_HEIGHT   BALL_RADIUS*2+0.05f
#define BRD_LENGTH	 3.0f
#define BRD_Z_OFFSET 1.f
#define BRD_ROT		 DEG2RAD(-25)

// Other Board Parameters
#define	FALL_HOLE_WIDTH		BALL_RADIUS*2
#define PLUNGER_LANE_WIDTH	FALL_HOLE_WIDTH

// Ball
#define BALL_RADIUS 0.039375f // 1/16th of an inch

// Border
#define BDR_HEIGHT	0.1f
#define BDR_WIDTH	0.05f

// Glass
#define GLASS_HEIGHT BDR_HEIGHT+(BRD_HEIGHT/2)

// Camera
#define CAMERA_POS	  Vec3(0, 0.7, -4.8)
#define CAMERA_RT_SPD 0.001f
#define CAMERA_MV_SPD 0.1f

// Controls
#define VK_CAMERA_LFT GLUT_KEY_LEFT
#define VK_CAMERA_RGT GLUT_KEY_RIGHT
#define VK_CAMERA_FWD GLUT_KEY_UP
#define VK_CAMERA_BCK GLUT_KEY_DOWN
#define VK_EXIT		27

// Math
#define PI 3.14159265359
#define DEG2RAD(data) data * (PI/180)

// Glass index
#define GLASS_ATR_IDX 1

// Colors
#define BALL_COLOR		Color(0.8, 0.8, 0.8)
#define BOARD_COLOR		Color(0.4, 0.4, 0.4)
#define BORDER_COLOR	Color(0, 0, 0)
#define CLEAR_COLOR		Color(0.5, 1, 1)

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

#endif _GLOBALS_H_