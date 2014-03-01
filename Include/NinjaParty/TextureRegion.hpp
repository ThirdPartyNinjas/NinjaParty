#ifndef NINJAPARTY_TEXTUREREGION_HPP
#define NINJAPARTY_TEXTUREREGION_HPP

#include <NinjaParty/Math.hpp>
#include <NinjaParty/Rectangle.hpp>

namespace NinjaParty
{
	struct TextureRegion
	{
		Rectangle bounds;	// the pixel position/dimensions of the trimmed texture
        Rectangle input;	// x and y are amount trimmed from top-left, w and h are the input width and height

		Vector2 originTopLeft;
		Vector2 originBottomRight;
		Vector2 originCenter;

		bool rotated;
	};
}

#endif//NINJAPARTY_TEXTUREREGION_HPP
