#include <NinjaParty/Font.hpp>
#include <NinjaParty/Vector2.hpp>

namespace NinjaParty
{
	void MeasureString(Font *font, std::string string, NinjaParty::Vector2 &maximum)
	{
		float x = 0, y = 0;

		maximum = Vector2::ZERO;

		for(size_t i=0; i<string.size(); i++)
		{
			if(string[i] == '\n')
			{
				y += font->height;
			}
			else if(string[i] == '\r')
			{
				x = 0;
			}
			else
			{
				CharacterData &cd = (font->characters.count(string[i]) == 1) ? font->characters[string[i]] : font->characters[' '];

				x += cd.advanceX;
			}

			maximum.X() = std::max<float>(maximum.X(), x);
			maximum.Y() = y;
		}
		
		maximum.Y() += font->height;
	}
}
