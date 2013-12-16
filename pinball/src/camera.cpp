/*-------------------------------------------------------------------------\
| File: CAMERA.CPP															|
| Desc: Provides definitions for a camera object							|
| Declaration File: CAMERA.H												|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#include "camera.h"
#include "GL/glut.h"

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

void Camera::Update()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(	EyePos.x,	EyePos.y,	EyePos.z,
				LookAt.x,	LookAt.y,	LookAt.z,
				Up.x,		Up.y,		Up.z	);
}

Camera::~Camera()
{

}