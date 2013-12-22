#include "image.h"
#include "loadImage.h"

namespace GameFramework
{
	static const Vertex2 verts[4] = {
		Vertex2(0.f, 0.f),
		Vertex2(1.f, 0.f),
		Vertex2(1.f, 1.f),
		Vertex2(0.f, 1.f)
	};

	static const TextureCoord texCoords[4] = {
		TextureCoord(0.f, 1.f),
		TextureCoord(1.f, 1.f),
		TextureCoord(1.f, 0.f),
		TextureCoord(0.f, 0.f)
	};

	static const unsigned int indices[4] = {
		0, 1, 2, 3
	};

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
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_VERTEX_ARRAY);
		glEnable(GL_TEXTURE_COORD_ARRAY);

		glGetError();

		glBindTexture(GL_TEXTURE_2D, m_texID);

		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

		glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
		glVertexPointer(3, GL_FLOAT, 0, verts);
		glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, indices);

		glBindTexture(GL_TEXTURE_2D, 0);

		GLenum err = glGetError();
		if (err != GL_NO_ERROR)
			std::cout << gluErrorString(err) << std::endl;

		glEnable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_VERTEX_ARRAY);
		glDisable(GL_TEXTURE_COORD_ARRAY);
	}
}