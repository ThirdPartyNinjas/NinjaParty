#ifndef NINJAPARTY_SPRITEANIMATION_HPP
#define NINJAPARTY_SPRITEANIMATION_HPP

#include <map>
#include <string>
#include <vector>

#include <NinjaParty/AnimationPlayer.hpp>
#include <NinjaParty/Texture.hpp>

namespace NinjaParty
{
	struct SpriteAnimation
	{
		Texture *texture;
		std::vector<TextureRegion> frames;
		float frameSeconds;
		bool loop;
	};
	
	class SpriteAnimationPlayer : public AnimationPlayer
	{
	public:
		SpriteAnimationPlayer();
		virtual ~SpriteAnimationPlayer();

		void AddAnimation(const std::string &name, SpriteAnimation *spriteAnimation);
		
		void Start(const std::string &name, bool allowRestart = false);
		void Transition(const std::string &name, float transitionTime);

		AnimationEvent Update(float deltaSeconds);
		AnimationEvent SetTime(float seconds);

		void Draw(SpriteBatch *spriteBatch,
				  const Vector2 &position,
				  float rotation = 0,
				  const Vector2 &scale = Vector2::ONE,
				  const Color &tintColor = Color::White,
				  bool flipHorizontal = false,
				  bool flipVertical = false);

	protected:
		std::map<std::string, SpriteAnimation*> animations;

		SpriteAnimation *currentAnimation;
		float currentAnimationTime;
		int currentFrame;
	};
}

#endif//NINJAPARTY_SPRITEANIMATION_HPP
