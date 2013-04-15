#import <string>

#import <GLKit/GLKit.h>
#import <OpenGLES/ES2/gl.h>

#import <NinjaParty/LoadTexture.hpp>


namespace NinjaParty
{
	extern "C"
	{
		int LoadTexture(const std::string &fileName, int &width, int &height)
		{
			GLuint textureId;
			
			glGenTextures(1, &textureId);
			glBindTexture(GL_TEXTURE_2D, textureId);
			
			NSString *path = [NSString stringWithCString:fileName.c_str() encoding:NSUTF8StringEncoding];

			GLKTextureInfo *texture = [GLKTextureLoader textureWithContentsOfFile:path options:nil error:nil];
			if(texture == nil)
			{
				return 0;
			}
			
			width = texture.width;
			height = texture.height;
			textureId = texture.name;
			
			return textureId;
		}
        
        // FIXME: If we want to use this file, we need to add in
        // LoadTextureFromBuffer
	}
}
