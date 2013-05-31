#ifndef NINJAPARTY_TWEEN_HPP
#define NINJAPARTY_TWEEN_HPP

#include <algorithm>

#include "Juggler.hpp"
#include "MathHelpers.hpp"

namespace NinjaParty
{
	enum class TweenFunction
	{
		Linear,
		Quadratic,
		Cubic,
		Quartic,
		Quintic,
		Sinusoidal,
		Circular,
		Back,
		Bounce,
	};
	
	enum class TweenType
	{
		EaseIn,
		EaseOut,
		EaseInOut,
	};
	
	float EaseIn(float time, TweenFunction tweenFunction);
	float EaseOut(float time, TweenFunction tweenFunction);
	float EaseInOut(float time, TweenFunction tweenFunction);
	
	template <typename T>
	class Tween : public Jugglable
	{
	public:
		Tween(T &reference, const T &start, const T& finish, float duration, TweenType type, TweenFunction function)
			: reference(reference), start(start), finish(finish), position(0.0f), duration(duration), function(function)
		{
			switch(type)
			{
				case TweenType::EaseIn:
					this->type = &EaseIn;
					break;
				case TweenType::EaseOut:
					this->type = &EaseOut;
					break;
				case TweenType::EaseInOut:
					this->type = &EaseInOut;
					break;
			}
		}

		virtual void Update(float deltaSeconds)
		{
			position = std::min(duration, position + deltaSeconds);
			reference = Lerp(start, finish, type(position / duration, function));
		}
		
		virtual float GetPosition() const { return position; }
		virtual void SetPosition(float position) { this->position = position; }
		
		virtual bool IsComplete() const { return position >= duration; }
	
	protected:
		T &reference;
		T start;
		T finish;
		float position;
		float duration;
		TweenFunction function;
		float (*type)(float, TweenFunction);
	};
}

#endif//NINJAPARTY_TWEEN_HPP
