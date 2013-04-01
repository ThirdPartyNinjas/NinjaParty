#ifndef NINJAPARTY_MATRIX3_HPP
#define NINJAPARTY_MATRIX3_HPP

#include <Wm5Matrix3.h>

namespace NinjaParty
{	
	typedef Wm5::Matrix3<float> Matrix3;

	Matrix3 CreateScaleMatrix(float x, float y);
	Matrix3 CreateTranslationMatrix(float x, float y);
	Matrix3 CreateRotationMatrix(float r);
}

#endif//NINJAPARTY_MATRIX3_HPP
