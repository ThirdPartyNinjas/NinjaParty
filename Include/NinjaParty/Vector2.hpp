#ifndef NINJAPARTY_VECTOR2_HPP
#define NINJAPARTY_VECTOR2_HPP

#include <Wm5Vector2.h>

namespace NinjaParty
{	
	typedef Wm5::Vector2<float> Vector2;
    
    Vector2 CatmullRom(const Vector2 &v0, const Vector2 &v1, const Vector2 &v2, const Vector2 &v3, float t);
}

#endif//NINJAPARTY_VECTOR2_HPP
