#ifndef NINJAPARTY_TWEEN_HPP
#define NINJAPARTY_TWEEN_HPP

#include <algorithm>

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
	
	class Tween
	{
	public:
		Tween();
		Tween(float tweenDuration, TweenType tweenType, TweenFunction tweenFunction);
		
		void Reset(float tweenDuration, TweenType tweenType, TweenFunction tweenFunction);
		float Update(float deltaSeconds);
		
		float GetPosition() const { return std::min(duration, position); }
		void SetPosition(float position) { this->position = position; }
		
		bool IsComplete() const { return position >= duration; }
	
	protected:
		float position;
		float duration;
		TweenFunction function;
		float (*type)(float, TweenFunction);
	};
}

#endif//NINJAPARTY_TWEEN_HPP
