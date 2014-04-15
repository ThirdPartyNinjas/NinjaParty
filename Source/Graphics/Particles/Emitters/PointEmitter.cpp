#include <NinjaParty/Particles/Particle.hpp>
#include <NinjaParty/Particles/Emitters/PointEmitter.hpp>

namespace NinjaParty
{
    namespace Particles
    {
        PointEmitter::PointEmitter(int maximumParticles)
        : ConstantRateEmitter(maximumParticles)
        {
        }
        
        PointEmitter::~PointEmitter()
        {
        }
        
        void PointEmitter::Emit(Particle *particles, int count)
        {
            for(int i=0; i<count; i++)
            {
                Particle &p = particles[i];
                
                SetDefaults(p);
            }
        }
    }
}
