#ifndef NINJAPARTY_MATH_HELPERS_HPP
#define NINJAPARTY_MATH_HELPERS_HPP

namespace NinjaParty
{
    const float TAU = 6.283185307179586f;
    
	const float PI = 3.14159265358979f;
	const float PIOVERTWO = PI / 2.0f;
	const float PIOVERFOUR = PI / 4.0f;
	const float PIOVEREIGHT = PI / 8.0f;
	const float TWOPI = PI * 2.0f;
    
    template <typename T>
    T Clamp(const T &value, const T &min, const T &max)
    {
        if(value < min)
            return min;
        if(value > max)
            return max;
        return value;
    }

	template <typename T>
	T Lerp(const T &a, const T &b, float t)
	{
		return a + (b - a) * t;
	}
    
    template <typename T>
    T SmoothStep(const T &a, const T &b, float t)
    {
        t = Clamp<T>((t - a) / (b - a), 0, 1);
        return t * t * (3 - 2 * t);
    }
    
    template <typename T>
    T SmootherStep(const T &a, const T &b, float t)
    {
        t = Clamp<T>((t - a) / (b - a), 0, 1);
        return t * t * t * (t * (t * 6 - 15) + 10);
    }
    
//    float smootherstep(float edge0, float edge1, float x)
//    {
//        // Scale, and clamp x to 0..1 range
//        x = clamp((x - edge0)/(edge1 - edge0), 0.0, 1.0);
//        // Evaluate polynomial
//        return x*x*x*(x*(x*6 - 15) + 10);
//    }
	template <typename T>
	bool BoolXor(const T &lhs, const T &rhs)
	{
		return !( lhs && rhs ) && ( lhs || rhs );
	}
}

#endif//NINJAPARTY_MATH_HELPERS_HPP
