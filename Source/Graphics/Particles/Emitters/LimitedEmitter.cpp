#include <NinjaParty/Particles/Emitters/LimitedEmitter.hpp>

namespace NinjaParty
{
    namespace Particles
    {
        struct LimitedEmitter::impl
        {
            int particlesToEmit;
            int particlesPerSecond;
            int particlesEmitted = 0;
            
            float remainder = 0;
        };
        
        LimitedEmitter::LimitedEmitter(int particlesToEmit, int particlesPerSecond)
        : Emitter(particlesToEmit), pimpl(new impl())
        {
            pimpl->particlesToEmit = particlesToEmit;
            pimpl->particlesPerSecond = particlesPerSecond;
        }
        
        LimitedEmitter::~LimitedEmitter()
        {
        }
        
        bool LimitedEmitter::Completed() const
        {
            return (pimpl->particlesEmitted == pimpl->particlesToEmit) && ActiveParticles() == 0;
        }

        int LimitedEmitter::Emit(Particle *particles, int max, float deltaSeconds)
        {
            float particlesToEmit = deltaSeconds * pimpl->particlesPerSecond + pimpl->remainder;
            
            if(max > pimpl->particlesToEmit - pimpl->particlesEmitted)
                max = pimpl->particlesToEmit - pimpl->particlesEmitted;
            
            if(particlesToEmit > max)
                particlesToEmit = max;
            
            if(max == 0)
                return 0;
            
            pimpl->remainder = particlesToEmit - (int)particlesToEmit;
            
            Emit(particles, particlesToEmit);
            
            pimpl->particlesEmitted += particlesToEmit;
            
            return particlesToEmit;
        }
    }
}
