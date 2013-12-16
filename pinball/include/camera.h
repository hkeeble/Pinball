/*-------------------------------------------------------------------------\
| File: CAMERA.H															|
| Desc: Provides declarations for a camera object							|
| Definition File: CAMERA.CPP												|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "globals.h"

class Camera
{
public:
	Camera();
	Camera(Vec3 up, Vec3 lookAt, Vec3 eyePos, Fl32 FOV);
	~Camera();
	void Update();
	Vec3 Up, LookAt, EyePos;
	Fl32 FOV;
};

#endif // _CAMERA_H_