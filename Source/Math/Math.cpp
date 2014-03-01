#include <NinjaParty/Math.hpp>

namespace NinjaParty
{
    Vector2 CatmullRom(const Vector2 &v0, const Vector2 &v1, const Vector2 &v2, const Vector2 &v3, float t)
    {
        return 0.5f *
        ((2.0f * v1) +
         (-v0 + v2) * t +
         (2.0f * v0 - 5.0f * v1 + 4.0f * v2 - v3) * t * t +
         (-v0 + 3.0f * v1 - 3.0f * v2 + v3) * t * t * t);
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
    
    Matrix4 CreateOrthographicProjectionMatrix(float left, float right, float top, float bottom, float near, float far)
    {
//        float *p = &projectionMatrix[0][0];
//		*p++ = 2 / (right - left);
//		*p++ = 0;
//		*p++ = 0;
//		*p++ = -(right + left) / (right - left);
//		
//		*p++ = 0;
//		*p++ = 2 / (top - bottom);
//		*p++ = 0;
//		*p++ = -(top + bottom) / (top - bottom);
//		
//		*p++ = 0;
//		*p++ = 0;
//		*p++ = -2 / (far - near);
//		*p++ = -(far + near) / (far - near);
//		
//		*p++ = 0;
//		*p++ = 0;
//		*p++ = 0;
//		*p++ = 1;
        
        return Matrix4(2 / (right - left), 0,                  0,                 -(right + left) / (right - left),
                       0,                  2 / (top - bottom), 0,                 -(top + bottom) / (top - bottom),
                       0,                  0,                  -2 / (far - near), -(far + near) / (far - near),
                       0,                  0,                  0,                 1);
    }
}
