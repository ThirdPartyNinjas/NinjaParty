#ifndef NINJAPARTY_FONT_HPP
#define NINJAPARTY_FONT_HPP

#include <algorithm>
#include <map>
#include <stdint.h>
#include <string>

#include <NinjaParty/Vector2.hpp>

namespace NinjaParty
{
	struct CharacterData
	{
		int16_t x, y;
		uint16_t width, height;
		int16_t offsetX, offsetY;
		int16_t advanceX;
	};

	struct Font
	{
		std::map<int16_t, CharacterData> characters;
		int height;
	};
	
	void MeasureString(Font *font, std::string string, NinjaParty::Vector2 &maximum);
}

#endif//NINJAPARTY_FONT_HPP
