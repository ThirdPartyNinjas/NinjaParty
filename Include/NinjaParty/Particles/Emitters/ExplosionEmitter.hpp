#ifndef MUDDCONTROL_EXPLOSIONEMITTER_HPP
#define MUDDCONTROL_EXPLOSIONEMITTER_HPP

#include <NinjaParty/Particles/Particle.hpp>
#include <NinjaParty/Particles/Emitters/LimitedEmitter.hpp>

namespace NinjaParty
{
    class Random;
    
    namespace Particles
    {
        struct Particle;
        
        class ExplosionEmitter : public LimitedEmitter
        {
        public:
            ExplosionEmitter(int particlesToEmit, int particlesPerSecond, Random &random, float angle, float minSpeed, float maxSpeed);
            virtual ~ExplosionEmitter();
            
            virtual void Emit(Particle *particles, int count);
            
        private:
            struct impl;
            std::unique_ptr<impl> pimpl;
        };
    }
}

#endif//MUDDCONTROL_EXPLOSIONEMITTER_HPP
