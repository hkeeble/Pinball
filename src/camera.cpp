/*-------------------------------------------------------------------------\
| File: CAMERA.CPP															|
| Desc: Provides definitions for a camera object							|
| Declaration File: CAMERA.H												|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#include "camera.h"

Camera::Camera()
{
	Up = UP_VECTOR;
	LookAt = Vec3(0);
	EyePos = Vec3(0);
	FOV = DEFAULT_FOV;
}

Camera::Camera(Vec3 up, Vec3 lookAt, Vec3 eyePos, Fl32 Fov)
{
	Up = up;
	LookAt = lookAt;
	EyePos = eyePos;
	FOV = Fov;
}

Camera::~Camera()
{

}