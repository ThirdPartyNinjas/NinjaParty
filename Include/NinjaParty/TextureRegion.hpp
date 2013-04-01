#ifndef NINJAPARTY_TEXTUREREGION_HPP
#define NINJAPARTY_TEXTUREREGION_HPP

#include <NinjaParty/Vector2.hpp>
#include <NinjaParty/Rectangle.hpp>

namespace NinjaParty
{
	struct TextureRegion
	{
		Rectangle bounds;
		Vector2 origin;
		Vector2 input;
		bool rotated;
	};
}

#endif//NINJAPARTY_TEXTUREREGION_HPP
