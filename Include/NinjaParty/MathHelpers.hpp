#ifndef NINJAPARTY_MATH_HELPERS_HPP
#define NINJAPARTY_MATH_HELPERS_HPP

namespace NinjaParty
{
	const float PI = 3.14159265358979f;
	const float PIOVERTWO = PI / 2.0f;
	const float PIOVERFOUR = PI / 4.0f;
	const float PIOVEREIGHT = PI / 8.0f;
	
	template <typename T>
	T Lerp(const T &a, const T &b, float t)
	{
		return a + (b - a) * t;
	}
	
	template <typename T>
	bool BoolXor(const T &lhs, const T &rhs)
	{
		return !( lhs && rhs ) && ( lhs || rhs );
	}
}

#endif//NINJAPARTY_MATH_HELPERS_HPP
