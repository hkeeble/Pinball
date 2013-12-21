/*-------------------------------------------------------------------------\
| File: LOADIMAGE.H															|
| Desc: Provides declarations for loading images with SOIL					|
| Definition File: CAMERA.CPP												|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#ifndef _LOAD_IMAGE_H_
#define _LOAD_IMAGE_H_

#include <string>
#include "SOIL.h"
#include "log.h"

namespace GameFramework
{
	unsigned int LoadTexture(const std::string& dataPath);
}

#endif // _LOAD_IMAGE_H_