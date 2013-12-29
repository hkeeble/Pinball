#include "loadImage.h"

namespace GameFramework
{
	unsigned int LoadTexture(const std::string& dataPath)
	{
		glEnable(GL_TEXTURE_2D);

		std::string fPath = GetCurrentDir(); // Get current directory, SOIL doesn't seem to work from executable directory

		fPath.append("img\\" + dataPath);

		unsigned int tex_2d = SOIL_load_OGL_texture
			(
			fPath.c_str(),
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
			);

		if (tex_2d == 0)
		{
			std::string s = "\n\n---- SOIL ERROR ----\nFilename: " + fPath;
			s.append("\nSOIL Message: ");
			s.append(SOIL_last_result());
			s.append("\n\n");

			Log::Write(s.c_str(), ENGINE_LOG);
		}

		glBindTexture(GL_TEXTURE_2D, tex_2d);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		glDisable(GL_TEXTURE_2D);

		return tex_2d;
	}
}