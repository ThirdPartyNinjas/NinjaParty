#include <algorithm>
#include <cmath>

#include <NinjaParty/Tween.hpp>
#include <NinjaParty/MathHelpers.hpp>

namespace NinjaParty
{
	const float BackConstant = 1.70158f;
	
	float EaseIn(float time, TweenFunction tweenFunction)
	{
		switch (tweenFunction)
		{
			case TweenFunction::Linear:
				return time;
				
			case TweenFunction::Quadratic:
				return time * time;
				
			case TweenFunction::Cubic:
				return time * time * time;
				
			case TweenFunction::Quartic:
				return time * time * time * time;
				
			case TweenFunction::Quintic:
				return time * time * time * time * time;
				
			case TweenFunction::Sinusoidal:
				return 1 - std::cos(time * PIOVERTWO);
				
			case TweenFunction::Circular:
				return -(std::sqrt(1 - time * time) - 1);
				
			case TweenFunction::Back:
				return time * time * ((BackConstant + 1) * time - BackConstant);
				
			case TweenFunction::Bounce:
				return 1 - EaseOut(1 - time, TweenFunction::Bounce);
				
			default:
				return 0;
		}
	}

	float EaseOut(float time, TweenFunction tweenFunction)
	{
		switch (tweenFunction)
		{
			case TweenFunction::Linear:
				return time;
				
			case TweenFunction::Quadratic:
				return -1 * (std::pow(time - 1, 2) - 1);
				
			case TweenFunction::Cubic:
				return (std::pow(time - 1, 3) + 1);
				
			case TweenFunction::Quartic:
				return -1 * (std::pow(time - 1, 4) - 1);
				
			case TweenFunction::Quintic:
				return std::pow(time - 1, 5) + 1;
				
			case TweenFunction::Sinusoidal:
				return std::sin(time * PIOVERTWO);
				
			case TweenFunction::Circular:
				return std::sqrt(1 - (time -= 1) * time);
				
			case TweenFunction::Back:
				time -= 1;
				return (time * time * ((BackConstant + 1) * time + BackConstant) + 1);
				
			case TweenFunction::Bounce:
				if (time < 1 / 2.75f)
					return (7.5625f * time * time);
				else if (time < 2 / 2.75)
					return (7.5625f * (time -= (1.5f / 2.75f)) * time + 0.75f);
				else if (time < 2.5 / 2.75)
					return (7.5625f * (time -= (2.25f / 2.75f)) * time + 0.9375f);
				else
					return (7.5625f * (time -= (2.625f / 2.75f)) * time + 0.984375f);
				
			default:
				return 0;
		}
	}

	float EaseInOut(float time, TweenFunction tweenFunction)
	{
		switch (tweenFunction)
		{
			case TweenFunction::Linear:
				return time;
				
			case TweenFunction::Quadratic:
				time *= 2;
				if (time < 1) return EaseIn(time, TweenFunction::Quadratic) / 2;
				return (float)(-0.5 * (std::pow(time - 2, 2) + -2));
				
			case TweenFunction::Cubic:
				time *= 2;
				if (time < 1) return EaseIn(time, TweenFunction::Cubic) / 2;
				return (float)(0.5 * (std::pow(time - 2, 3) + 2));
				
			case TweenFunction::Quartic:
				time *= 2;
				if (time < 1) return EaseIn(time, TweenFunction::Quartic) / 2;
				return (float)(-0.5 * (std::pow(time - 2, 4) + -2));
				
			case TweenFunction::Quintic:
				time *= 2;
				if (time < 1) return EaseIn(time, TweenFunction::Quintic) / 2;
				return (float)(0.5 * (std::pow(time - 2, 5) + 2));
				
			case TweenFunction::Sinusoidal:
				return -1 * 0.5f * (float)(std::cos(PI * time) - 1);
				
			case TweenFunction::Circular:
				time *= 2;
				if (time < 1)
					return -0.5f * ((float)std::sqrt(1 - time * time) - 1);
				time -= 2;
				return 0.5f * ((float)std::sqrt(1 - time * time) + 1);
				
			case TweenFunction::Back:
				if ((time *= 2) < 1)
					return 0.5f * (time * time * (((BackConstant * (1.525f)) + 1) * time - BackConstant * 1.525f));
				return 0.5f * ((time -= 2) * time * (((BackConstant * (1.525f)) + 1) * time + BackConstant * 1.525f) + 2);
				
			case TweenFunction::Bounce:
				if (time < 0.5f)
					return EaseIn(time * 2, TweenFunction::Bounce) * 0.5f;
				return EaseOut(time * 2 - 1.0f, TweenFunction::Bounce) * 0.5f + 0.5f;
				
			default:
				return 0;
		}
	}
}
