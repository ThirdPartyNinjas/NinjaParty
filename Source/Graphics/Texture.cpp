#include <stdexcept>

#include <NinjaParty/IncludeGL.h>
#include <NinjaParty/LoadTexture.hpp>
#include <NinjaParty/Texture.hpp>
#include <NinjaParty/TextureRegion.hpp>
#include <NinjaParty/Vector2.hpp>


namespace NinjaParty
{
	Texture::Texture()
	: textureId(0), filterType(TextureFilter::Linear)
	{
	}
	
	Texture::~Texture()
	{
		if(textureId != 0)
			glDeleteTextures(1, &textureId);
	}

	void Texture::SetFilter(TextureFilter filterType)
	{
		glBindTexture(GL_TEXTURE_2D, textureId);
		
		switch(filterType)
		{
			default:
			case TextureFilter::Linear:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				break;
				
			case TextureFilter::NearestNeighbor:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				break;
		}
	}

	Texture* Texture::FromOpenGL(unsigned int textureId, int width, int height, TextureFilter textureFilter)
	{
		Texture *texture = new Texture();
		texture->textureId = textureId;
		texture->width = width;
		texture->height = height;
		texture->SetFilter(textureFilter);
		return texture;
	}

	Texture* Texture::FromFile(const std::string &fileName, TextureRegion *textureRegion)
	{
		int width, height;
		int textureId = LoadTexture(fileName, width, height);
		
		if(textureId == 0)
			throw std::runtime_error(std::string("Failed to load image: ") + fileName);
		
		Texture *texture = new Texture();
		texture->textureId = static_cast<unsigned int>(textureId);
		texture->width = width;
		texture->height = height;
		
		if(textureRegion != nullptr)
		{
			textureRegion->bounds.x = textureRegion->bounds.y = 0;
            textureRegion->originTopLeft = Vector2::ZERO;
            textureRegion->originBottomRight = Vector2(1.0f, 1.0f);
			textureRegion->originCenter = Vector2(0.5f, 0.5f);
			textureRegion->bounds.width = texture->GetWidth();
			textureRegion->bounds.height = texture->GetHeight();
            
            // todo: input
            
			textureRegion->rotated = false;
		}
		return texture;
	}

	Texture* Texture::FromBuffer(const unsigned char *buffer, int length, TextureRegion *textureRegion)
	{
		int width, height;
		int textureId = LoadTextureFromBuffer(buffer, length, width, height);
		
		Texture *texture = new Texture();
		texture->textureId = static_cast<unsigned int>(textureId);
		texture->width = width;
		texture->height = height;
		
		if(textureRegion != 0)
		{
			textureRegion->bounds.x = textureRegion->bounds.y = 0;
            textureRegion->originTopLeft = Vector2::ZERO;
            textureRegion->originBottomRight = Vector2(1.0f, 1.0f);
			textureRegion->originCenter = Vector2(0.5f, 0.5f);
			textureRegion->bounds.width = texture->GetWidth();
			textureRegion->bounds.height = texture->GetHeight();
            
            // todo: input
            
			textureRegion->rotated = false;
		}
		return texture;
	}
	
	Texture* Texture::FromColors(const Color *colors, int width, int height, TextureFilter textureFilter)
	{
		GLuint textureId;
		
		std::unique_ptr<uint8_t[]> buffer(new uint8_t[width * height * 4]);
		
		for(int y=0; y<height; y++)
		{
			for(int x=0; x<width; x++)
			{
				buffer[y * (width * 4) + (x * 4) + 0] = (uint8_t)(colors[y * width + x].R() * 255.0f);
				buffer[y * (width * 4) + (x * 4) + 1] = (uint8_t)(colors[y * width + x].G() * 255.0f);
				buffer[y * (width * 4) + (x * 4) + 2] = (uint8_t)(colors[y * width + x].B() * 255.0f);
				buffer[y * (width * 4) + (x * 4) + 3] = (uint8_t)(colors[y * width + x].A() * 255.0f);
			}
		}
		
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer.get());
		
		Texture *texture = new Texture();
		texture->textureId = textureId;
		texture->width = width;
		texture->height = height;
		texture->SetFilter(textureFilter);
		
		return texture;
	}
	
}
