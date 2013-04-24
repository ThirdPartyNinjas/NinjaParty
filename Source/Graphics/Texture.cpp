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
}
