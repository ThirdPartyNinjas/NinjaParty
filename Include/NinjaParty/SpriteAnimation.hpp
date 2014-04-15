#ifndef NINJAPARTY_SPRITEANIMATION_HPP
#define NINJAPARTY_SPRITEANIMATION_HPP

#include <vector>

#include <NinjaParty/TextureRegion.hpp>

namespace NinjaParty
{
    class Texture;
    
	struct SpriteAnimation
	{
		Texture *texture;
		std::vector<TextureRegion> frames;
		float frameSeconds;
		bool loop;
	};
}

#endif//NINJAPARTY_SPRITEANIMATION_HPP
