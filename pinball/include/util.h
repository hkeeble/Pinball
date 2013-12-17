 /*-------------------------------------------------------------------------\
| File: GLOBALS.H															|
| Desc: Provides declarations global defines and type defines				|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#ifndef _UTILS_H_
#define _UTILS_H_

#include "globals.h" // Include PhysX

// Structs
struct Color
{
	Color(Fl32 R, Fl32 G, Fl32 B) : r(R), g(G), b(B) { }
	~Color() { }
	Fl32 r, g, b;
};

// Controls
#define VK_CAMERA_LFT GLUT_KEY_LEFT
#define VK_CAMERA_RGT GLUT_KEY_RIGHT
#define VK_CAMERA_FWD GLUT_KEY_UP
#define VK_CAMERA_BCK GLUT_KEY_DOWN
#define VK_EXIT		  27

// Glass index
#define GLASS_ATR_IDX 1

#define BALL_RADIUS 0.039375f // 1/16th of an inch

#endif _UTILS_H_