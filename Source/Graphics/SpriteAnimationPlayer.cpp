#include <NinjaParty/SpriteAnimationPlayer.hpp>
#include <NinjaParty/SpriteBatch.hpp>

namespace NinjaParty
{
	SpriteAnimationPlayer::SpriteAnimationPlayer()
		: currentAnimation(nullptr), currentAnimationTime(0), currentFrame(0)
	{
	}

	SpriteAnimationPlayer::~SpriteAnimationPlayer()
	{
	}

	void SpriteAnimationPlayer::AddAnimation(std::string const &name, NinjaParty::SpriteAnimation *spriteAnimation)
	{
		animations[name] = spriteAnimation;

		if(animations.size() == 1)
		{
			currentAnimation = spriteAnimation;
			currentAnimationName = name;
			currentAnimationTime = 0;
			currentFrame = 0;
		}
	}

	void SpriteAnimationPlayer::Start(const std::string &name, bool allowRestart)
	{
		if(!allowRestart && name == currentAnimationName)
		{
			return;
		}

		currentAnimationName = name;
		currentAnimation = animations[name];
		currentAnimationTime = 0;
	}

	void SpriteAnimationPlayer::Transition(const std::string &name, float transitionTime)
	{
		Start(name, false);
	}

	AnimationEvent SpriteAnimationPlayer::Update(float deltaSeconds)
	{
		deltaSeconds *= timeScale;
		
		AnimationEvent animationEvent;
		animationEvent.AnimationComplete = false;
		
		currentAnimationTime += deltaSeconds;
		
		float animationLength = currentAnimation->frames.size() * currentAnimation->frameSeconds;
		while(currentAnimationTime >= animationLength)
		{
			if(currentAnimation->loop)
			{
				currentAnimationTime -= animationLength;
			}
			else
			{
				currentAnimationTime = animationLength - currentAnimation->frameSeconds;
				animationEvent.AnimationComplete = true;
			}
		}
		
		currentFrame = static_cast<int>(currentAnimationTime / currentAnimation->frameSeconds);
		
		// todo: if current frame is not the same as started, check for animation triggers
		
		return animationEvent;
	}

	AnimationEvent SpriteAnimationPlayer::SetTime(float seconds)
	{
		AnimationEvent animationEvent;
		animationEvent.AnimationComplete = false;
		
		currentAnimationTime = seconds;
		
		float animationLength = currentAnimation->frames.size() * currentAnimation->frameSeconds;
		while(currentAnimationTime >= animationLength)
		{
			if(currentAnimation->loop)
			{
				currentAnimationTime -= animationLength;
			}
			else
			{
				currentAnimationTime = animationLength - currentAnimation->frameSeconds;
				animationEvent.AnimationComplete = true;
			}
		}
		
		if(currentAnimationTime < 0)
			currentAnimationTime = 0;
		
		currentFrame = static_cast<int>(currentAnimationTime / currentAnimation->frameSeconds);
		
		// todo: if current frame is not the same as started, check for animation triggers
		
		return animationEvent;
	}

	void SpriteAnimationPlayer::Draw(SpriteBatch *spriteBatch, const Vector2 &position, float rotation,
		const Vector2 &scale, const Color &tintColor, bool flipHorizontal, bool flipVertical)
	{
		// todo: flips?
		spriteBatch->Draw(TexturePair(currentAnimation->texture, currentAnimation->frames[currentFrame]), position, rotation, tintColor, scale);
	}
}
