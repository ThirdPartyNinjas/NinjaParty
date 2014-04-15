#include <NinjaParty/Math.hpp>
#include <NinjaParty/MathHelpers.hpp>

namespace NinjaParty
{
    Vector2 CatmullRom(const Vector2 &v0, const Vector2 &v1, const Vector2 &v2, const Vector2 &v3, float s)
    {
        NinjaParty::Vector2 a = 0.5f * (-v0 + 3.0f * v1 - 3.0f * v2 + v3);
        NinjaParty::Vector2 b = 0.5f * (2.0f * v0 - 5.0f * v1 + 4.0f * v2 - v3);
        NinjaParty::Vector2 c = 0.5f * (v2 - v0);
        NinjaParty::Vector2 e = 0.5f * (2.0f * v1);
        
        // p(s) = a*s3 + b*s2 + c*s + e
        return a * s * s * s + b * s * s + c * s + e;
    }
    
    Vector2 CatmullRomDerivative(const Vector2 &v0, const Vector2 &v1, const Vector2 &v2, const Vector2 &v3, float s)
    {
        NinjaParty::Vector2 a = 0.5f * (-v0 + 3.0f * v1 - 3.0f * v2 + v3);
        NinjaParty::Vector2 b = 0.5f * (2.0f * v0 - 5.0f * v1 + 4.0f * v2 - v3);
        NinjaParty::Vector2 c = 0.5f * (v2 - v0);

        // dp/ds = 3a*s2 + 2b*s + c
        return 3.0f * a * s * s + 2.0f * b * s + c;
    }

    float FindAngle(const Vector2 &vector, const Vector2 &reference)
    {
        float angle = std::atan2(vector.Y(), vector.X()) - std::atan2(reference.Y(), reference.X());
        
        if(angle < 0)
            angle += 2 * 3.14159f;

        return angle;
    }
    
    float WrapAngle(float value)
    {
        while(value < 0)
            value += TAU;
        while(value > TAU)
            value -= TAU;
        return value;
    }

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
    
    Matrix2 CreateRotationMatrix2(float r)
    {
        return Matrix2(Wm5::Math<float>::Cos(r), -Wm5::Math<float>::Sin(r), Wm5::Math<float>::Sin(r), Wm5::Math<float>::Cos(r));
    }
    
    Matrix4 CreateOrthographicProjectionMatrix(float left, float right, float top, float bottom, float near, float far)
    {
        return Matrix4(2 / (right - left), 0, 0, -(right + left) / (right - left),
                       0, 2 / (top - bottom), 0, -(top + bottom) / (top - bottom),
                       0, 0, -2 / (far - near), -(far + near) / (far - near),
                       0, 0, 0, 1);
    }
}
