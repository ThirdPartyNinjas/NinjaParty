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
			NSString *path = [NSString stringWithCString:fileName.c_str() encoding:NSUTF8StringEncoding];

			GLKTextureInfo *texture = [GLKTextureLoader textureWithContentsOfFile:path options:nil error:nil];
			if(texture == nil)
			{
				return 0;
			}
			
			width = texture.width;
			height = texture.height;
			
			return texture.name;
		}
	}
    
    int LoadTextureFromBuffer(const unsigned char *buffer, int length, int &width, int &height, bool resample)
    {
        NSData *data = [NSData dataWithBytesNoCopy:(void*)buffer
                                            length:length
                                      freeWhenDone:NO];
        
        GLKTextureInfo *texture = [GLKTextureLoader textureWithContentsOfData:data options:nil error:nil];
        
        if(texture == nil)
        {
            return 0;
        }
        
        width = texture.width;
        height = texture.height;
        
        return texture.name;
    }
}
