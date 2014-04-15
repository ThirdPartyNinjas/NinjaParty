#ifndef NINJAPARTY_CONEEMITTER_HPP
#define NINJAPARTY_CONEEMITTER_HPP

#include <memory>

#include <NinjaParty/Particles/Emitters/ConstantRateEmitter.hpp>

namespace NinjaParty
{
    class Random;
    
    namespace Particles
    {
        struct Particle;
        
        class ConeEmitter: public ConstantRateEmitter
        {
        public:
            ConeEmitter(int maximumParticles, Random &random, float angle, float minSpeed, float maxSpeed);
            virtual ~ConeEmitter();
            
            virtual void Emit(Particle *particles, int count);
            
        private:
            struct impl;
            std::unique_ptr<impl> pimpl;
        };
    }
}

#endif//NINJAPARTY_CONEEMITTER_HPP
