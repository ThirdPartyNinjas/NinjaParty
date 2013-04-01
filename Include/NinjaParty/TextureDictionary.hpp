#ifndef NINJAPARTY_TEXTUREDICTIONARY_HPP
#define NINJAPARTY_TEXTUREDICTIONARY_HPP

#include <string>
#include <map>

#include <NinjaParty/TextureRegion.hpp>

namespace NinjaParty
{

class TextureDictionary
{
public:
	static TextureDictionary* FromFile(const std::string &filename);

	bool ContainsTexture(const std::string &textureName);

	TextureRegion& GetRegion(const std::string &file);

private:
	TextureDictionary();

	std::map<std::string, TextureRegion> dictionary;
};

}

#endif//NINJAPARTY_TEXTUREDICTIONARY_HPP
