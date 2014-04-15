#ifndef NINJAPARTY_CIRCLEEMITTER_HPP
#define NINJAPARTY_CIRCLEEMITTER_HPP

#include <memory>

#include <NinjaParty/Particles/Emitters/ConstantRateEmitter.hpp>

namespace NinjaParty
{
    class Random;
    
    namespace Particles
    {
        struct Particle;
        
        class CircleEmitter: public ConstantRateEmitter
        {
        public:
            CircleEmitter(int maximumParticles, Random &random, float radius, bool edge = false);
            virtual ~CircleEmitter();

            virtual void Emit(Particle *particles, int count);
            
        private:
            struct impl;
            std::unique_ptr<impl> pimpl;
        };
    }
}

#endif//NINJAPARTY_CIRCLEEMITTER_HPP
