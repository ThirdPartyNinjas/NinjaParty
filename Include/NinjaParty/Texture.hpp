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
		
		static Texture* FromFile(const std::string &fileName, TextureRegion *textureRegion = nullptr);
		static Texture* FromBuffer(const unsigned char *buffer, int length, TextureRegion *textureRegion = nullptr);
		
	protected:
		Texture();
		
		int width;
		int height;
		
		unsigned int textureId;
		
		TextureFilter filterType;
	};

	typedef std::pair<Texture*, TextureRegion> TexturePair;
}

#endif//NINJAPARTY_TEXTURE_HPP
