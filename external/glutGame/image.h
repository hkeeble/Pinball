/*-------------------------------------------------------------------------\
| File: CAMERA.H															|
| Desc: Provides declarations for an image object							|
| Definition File: CAMERA.CPP												|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "globals.h"
#include "gl/glut.h"
#include <string>
#include <array>

namespace GameFramework
{
	class Image
	{
	private:
		unsigned int m_texID;
		std::string m_fileName;

		void cpy(const Image& param);

	public:
		Image();
		Image(const std::string& fileName);
		Image(const Image& param);
		Image& operator=(const Image& param);
		~Image();
		
		void Render();
	};
}

#endif // _IMAGE_H_