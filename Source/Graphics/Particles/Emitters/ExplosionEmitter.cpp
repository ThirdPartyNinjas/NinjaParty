#include <NinjaParty/Random.hpp>

#include <NinjaParty/Particles/Particle.hpp>
#include <NinjaParty/Particles/Emitters/ExplosionEmitter.hpp>

namespace NinjaParty
{
    namespace Particles
    {
        struct ExplosionEmitter::impl
        {
            impl(Random &random, float angle, float minSpeed, float maxSpeed)
            : random(random), angle(angle), minSpeed(minSpeed), maxSpeed(maxSpeed)
            {
            }
            
            Random &random;
            float angle;
            float minSpeed;
            float maxSpeed;
        };
        
        ExplosionEmitter::ExplosionEmitter(int particlesToEmit, int particlesPerSecond, Random &random, float angle, float minSpeed, float maxSpeed)
        : LimitedEmitter(particlesToEmit, particlesPerSecond), pimpl(new impl(random, angle, minSpeed, maxSpeed))
        {
        }
        
        ExplosionEmitter::~ExplosionEmitter()
        {
        }
        
        void ExplosionEmitter::Emit(Particle *particles, int count)
        {
			for (int i = 0; i < count; i++)
			{
				Particle &p = particles[i];
                
                SetDefaults(p);
                
                float radians = pimpl->random.NextFloat(Rotation() - pimpl->angle / 2, Rotation() + pimpl->angle / 2);
                float speed = pimpl->random.NextFloat(pimpl->minSpeed, pimpl->maxSpeed);
                
                p.velocity = Vector2(std::cos(radians) * speed, std::sin(radians) * speed);
            }
        }
    }
}