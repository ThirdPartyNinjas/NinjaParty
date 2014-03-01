#ifndef NINJAPARTY_ANIMATION_HPP
#define NINJAPARTY_ANIMATION_HPP

#include <string>

#include <NinjaParty/Color.hpp>
#include <NinjaParty/Math.hpp>

namespace NinjaParty
{
	class SpriteBatch;

	struct AnimationEvent
	{
		bool AnimationComplete;
		std::string AnimationEventName;
	};

	class AnimationPlayer
	{
	public:
		AnimationPlayer() : inTransition(false), timeScale(1.0f), currentAnimationName(""), transitionAnimationName("") { }
		virtual ~AnimationPlayer() { }

		virtual void Start(const std::string &name, bool allowRestart = false) = 0;
		virtual void Transition(const std::string &name, float transitionTime) = 0;

		virtual AnimationEvent Update(float deltaSeconds) = 0;
		virtual AnimationEvent SetTime(float seconds) = 0;
		
		virtual void Draw(SpriteBatch *spriteBatch,
						  const Vector2 &position,
						  float rotation = 0,
						  const Vector2 &scale = Vector2::ONE,
						  const Color &tintColor = Color::White,
						  bool flipHorizontal = false,
						  bool flipVertical = false) = 0;

		void SetTimeScale(float timeScale) { this->timeScale = timeScale; }
		bool InTransition() const { return inTransition; }
		std::string GetCurrentAnimationName() const { return currentAnimationName; }
		std::string GetTransitionAnimationName() const { return transitionAnimationName; }

	protected:
		bool inTransition;
		float timeScale;

		std::string currentAnimationName;
		std::string transitionAnimationName;
	};
}

#endif//NINJAPARTY_ANIMATION_HPP
