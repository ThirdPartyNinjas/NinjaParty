#include <NinjaParty/Particles/Emitters/ConstantRateEmitter.hpp>

namespace NinjaParty
{
    namespace Particles
    {
        struct ConstantRateEmitter::impl
        {
            int particlesPerSecond;
            float remainder;
        };
        
        ConstantRateEmitter::ConstantRateEmitter(int maximumParticles)
        : Emitter(maximumParticles), pimpl(new impl())
        {
            pimpl->remainder = 0;
        }
        
        ConstantRateEmitter::~ConstantRateEmitter()
        {
        }
        
        int& ConstantRateEmitter::ParticlesPerSecond()
        {
            return pimpl->particlesPerSecond;
        }
        
        int ConstantRateEmitter::Emit(Particle *particles, int max, float deltaSeconds)
        {
            float particlesToEmit = deltaSeconds * pimpl->particlesPerSecond + pimpl->remainder;
            
            if(particlesToEmit > max)
                particlesToEmit = max;
            
            pimpl->remainder = particlesToEmit - (int)particlesToEmit;
            
            Emit(particles, particlesToEmit);
            
            return particlesToEmit;
        }
    }
}
