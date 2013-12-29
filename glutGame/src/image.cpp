#include "image.h"
#include "loadImage.h"

namespace GameFramework
{
	Image::Image(const std::string& fileName)
	{
		m_fileName = fileName;
		m_texID = LoadTexture(m_fileName);
	}

	Image::Image()
	{
		m_fileName = "";
		m_texID = 0;
	}

	Image::Image(const Image& param)
	{
		cpy(param);
	}

	Image& Image::operator=(const Image& param)
	{
		if (&param == this)
			return *this;
		else
		{
			cpy(param);
			return *this;
		}
	}

	void Image::cpy(const Image& param)
	{
		m_fileName = param.m_fileName;
		m_texID = LoadTexture(m_fileName);
	}

	Image::~Image()
	{

	}

	void Image::Render()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0, 1, 1, 0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);

		glGetError(); // Clear previous errors

		glBindTexture(GL_TEXTURE_2D, m_texID);

		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

		glBegin(GL_QUADS);
		glTexCoord2i(0, 1);
		glVertex2i(0, 0);

		glTexCoord2i(1, 1);
		glVertex2i(1, 0);

		glTexCoord2i(1, 0);
		glVertex2i(1, 1);

		glTexCoord2i(0, 0);
		glVertex2i(0, 1);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);

		GLenum err = glGetError();
		if (err != GL_NO_ERROR)
			std::cout << gluErrorString(err) << std::endl;

		glEnable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);
	}
}