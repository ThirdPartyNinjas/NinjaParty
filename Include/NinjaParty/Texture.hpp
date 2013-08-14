#ifndef NINJAPARTY_TEXTURE_HPP
#define NINJAPARTY_TEXTURE_HPP

#include <string>

#include <NinjaParty/Color.hpp>
#include <NinjaParty/TextureRegion.hpp>

namespace NinjaParty
{
	enum class TextureFilter
	{
		Linear = 0,
		NearestNeighbor,
	};
	
	class Texture
	{
	public:
		virtual ~Texture();
		
		int GetWidth() const { return width; }
		int GetHeight() const { return height; }
		unsigned int GetTextureId() const { return textureId; }
		
		void SetFilter(TextureFilter filterType);
		
		// Called when OpenGL context is lost.
		// The texture id is invalid, so we don't want glDeleteTextures called
		virtual void Invalidate() { textureId = 0; }

		static Texture* FromOpenGL(unsigned int textureId, int width, int height, TextureFilter textureFilter = TextureFilter::Linear);
		static Texture* FromFile(const std::string &fileName, TextureRegion *textureRegion = nullptr);
		static Texture* FromBuffer(const unsigned char *buffer, int length, TextureRegion *textureRegion = nullptr);
		static Texture* FromColors(const Color *colors, int width, int height, TextureFilter textureFilter = TextureFilter::Linear);
		
	protected:
		Texture();
		
		int width;
		int height;
		
		unsigned int textureId;
		
		TextureFilter filterType;
	};
}

#endif//NINJAPARTY_TEXTURE_HPP
