#include <vector>

#include <NinjaParty/Font.hpp>
#include <NinjaParty/Path.hpp>
#include <NinjaParty/Math.hpp>

namespace NinjaParty
{
	NinjaParty::Vector2 MeasureString(Font *font, const std::string &string)
	{
		NinjaParty::Vector2 measurement;
		MeasureString(font, string, measurement);
		return measurement;
	}
	
	void MeasureString(Font *font, const std::string &string, NinjaParty::Vector2 &measurement)
	{
		float x = 0;

		measurement = Vector2::ZERO;

		for(size_t i=0; i<string.size(); i++)
		{
			if(string[i] == '\n')
			{
				measurement.Y() += font->lineHeight;
			}
			else if(string[i] == '\r')
			{
				measurement.X() = std::max<float>(measurement.X(), x);
				x = 0;
			}
			else
			{
				CharacterData &cd = (font->characters.count(string[i]) == 1) ? font->characters[string[i]] : font->characters[' '];
				x += cd.advanceX;
			}
		}
		
		measurement.X() = std::max<float>(measurement.X(), x);		
		measurement.Y() += font->lineHeight;
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
