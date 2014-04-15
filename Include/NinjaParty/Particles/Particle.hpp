#ifndef NINJAPARTY_PARTICLE_HPP
#define NINJAPARTY_PARTICLE_HPP

#include <NinjaParty/Color.hpp>
#include <NinjaParty/Math.hpp>

namespace NinjaParty
{
    namespace Particles
    {
        struct Particle
        {
            Vector2 position;
            Vector2 velocity;
            Vector2 scale;
            Color color;
            float time;
            float lifetime;
            float angle;
            float rotationSpeed;
        };
    }
}

#endif//NINJAPARTY_PARTICLE_HPP
