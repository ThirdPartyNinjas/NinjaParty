#include <NinjaParty/Random.hpp>

#include <NinjaParty/Particles/Particle.hpp>
#include <NinjaParty/Particles/Emitters/ConeEmitter.hpp>

namespace NinjaParty
{
    namespace Particles
    {
        struct ConeEmitter::impl
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
        
        ConeEmitter::ConeEmitter(int maximumParticles, Random &random, float angle, float minSpeed, float maxSpeed)
        : ConstantRateEmitter(maximumParticles), pimpl(new impl(random, angle, minSpeed, maxSpeed))
        {
            
        }
        
        ConeEmitter::~ConeEmitter()
        {
        }
        
        void ConeEmitter::Emit(Particle *particles, int count)
        {
			float radians = pimpl->random.NextFloat(Rotation() - pimpl->angle / 2, Rotation() + pimpl->angle / 2);
            
			for (int i = 0; i < count; i++)
			{
				Particle &p = particles[i];
                
                SetDefaults(p);
                
                float speed = pimpl->random.NextFloat(pimpl->minSpeed, pimpl->maxSpeed);
                
                p.velocity = Vector2(std::cos(radians) * speed, std::sin(radians) * speed);
            }
        }
    }
}