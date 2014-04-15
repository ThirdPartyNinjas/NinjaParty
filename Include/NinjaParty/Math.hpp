#ifndef NINJAPARTY_MATH_HPP
#define NINJAPARTY_MATH_HPP

#include "../../Dependencies/Wm5/Wm5Vector2.h"
#include "../../Dependencies/Wm5/Wm5Vector3.h"
#include "../../Dependencies/Wm5/Wm5Vector4.h"

#include "../../Dependencies/Wm5/Wm5Matrix2.h"
#include "../../Dependencies/Wm5/Wm5Matrix3.h"
#include "../../Dependencies/Wm5/Wm5Matrix4.h"

namespace NinjaParty
{
	typedef Wm5::Vector2<float> Vector2;
    typedef Wm5::Vector2<int32_t> IntVector2;
	typedef Wm5::Vector3<float> Vector3;
	typedef Wm5::Vector4<float> Vector4;

	typedef Wm5::Matrix2<float> Matrix2;
	typedef Wm5::Matrix3<float> Matrix3;
	typedef Wm5::Matrix4<float> Matrix4;
    
    Vector2 CatmullRom(const Vector2 &v0, const Vector2 &v1, const Vector2 &v2, const Vector2 &v3, float s);
    Vector2 CatmullRomDerivative(const Vector2 &v0, const Vector2 &v1, const Vector2 &v2, const Vector2 &v3, float s);

    float FindAngle(const Vector2 &vector, const Vector2 &reference);
    float WrapAngle(float value);
    
	Matrix3 CreateScaleMatrix(float x, float y);
	Matrix3 CreateTranslationMatrix(float x, float y);
	Matrix3 CreateRotationMatrix(float r);
    
    Matrix2 CreateRotationMatrix2(float r);
    
    Matrix4 CreateOrthographicProjectionMatrix(float left, float right, float top, float bottom, float near, float far);
}

#endif//NINJAPARTY_MATH_HPP
