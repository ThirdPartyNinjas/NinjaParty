#include <string>

#include <stb_image.h>

#include <NinjaParty/IncludeGL.h>
#include <NinjaParty/LoadTexture.hpp>

namespace NinjaParty
{
	extern "C"
	{
		int LoadTexture(const std::string &fileName, int &width, int &height)
		{
			GLuint textureId;
			
			int w, h, bpp;
            
            unsigned char *data = stbi_load(fileName.c_str(), &w, &h, &bpp, 4);
            
            // todo: throw exception if failed
            
			glGenTextures(1, &textureId);
			glBindTexture(GL_TEXTURE_2D, textureId);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			
            width = w;
			height = h;
			
            stbi_image_free(data);
            
			return textureId;
		}
	}
}
