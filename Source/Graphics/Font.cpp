#include <vector>

#include <NinjaParty/Font.hpp>
#include <NinjaParty/Path.hpp>
#include <NinjaParty/Vector2.hpp>

namespace NinjaParty
{
	void MeasureString(Font *font, const std::string &string, NinjaParty::Vector2 &maximum)
	{
		float x = 0, y = 0;
		float maxHeight = 0;

		maximum = Vector2::ZERO;

		for(size_t i=0; i<string.size(); i++)
		{
			if(string[i] == '\n')
			{
				y += font->lineHeight;
				maxHeight = 0;
			}
			else if(string[i] == '\r')
			{
				x = 0;
			}
			else
			{
				CharacterData &cd = (font->characters.count(string[i]) == 1) ? font->characters[string[i]] : font->characters[' '];

				x += cd.advanceX;
				maxHeight = std::max<float>(maxHeight, cd.height);
			}

			maximum.X() = std::max<float>(maximum.X(), x);
			maximum.Y() = y;
		}
		
		maximum.Y() += maxHeight;//font->height;
	}
	
	std::string WordWrapString(Font *font, const std::string &string, float width)
	{
		std::vector<std::string> words;
		Path::split(words, string, " ");
		float lineWidth = 0;
		float spaceWidth = font->characters[' '].advanceX;
		std::string result;

		for(std::string &word : words)
		{
			Vector2 size;
			MeasureString(font, word, size);

			lineWidth += size.X();
			if(lineWidth >= width)
			{
				result += "\r\n";
				lineWidth = size.X();
			}
			result += word;
			result += " ";
			lineWidth += spaceWidth;
		}

		return result;
	}
}
