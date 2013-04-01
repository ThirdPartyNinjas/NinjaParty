#include <NinjaParty/Vector2.hpp>

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
}
