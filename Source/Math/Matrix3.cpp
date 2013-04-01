#include <NinjaParty/Matrix3.hpp>

namespace NinjaParty
{
	Matrix3 CreateScaleMatrix(float x, float y)
	{
		return Matrix3(x, y, 1);
	}
	
	Matrix3 CreateTranslationMatrix(float x, float y)
	{
		return Matrix3(1, 0, x, 0, 1, y, 0, 0, 1);
	}
	
	Matrix3 CreateRotationMatrix(float r)
	{
		return Matrix3(Wm5::Math<float>::Cos(r), -Wm5::Math<float>::Sin(r), 0, Wm5::Math<float>::Sin(r), Wm5::Math<float>::Cos(r), 0, 0, 0, 1);
	}
}
