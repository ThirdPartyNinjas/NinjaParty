#ifndef NINJAPARTY_CONSTANTRATEPARTICLEEMITTER_HPP
#define NINJAPARTY_CONSTANTRATEPARTICLEEMITTER_HPP

#include <memory>

#include <NinjaParty/Particles/Emitters/Emitter.hpp>

namespace NinjaParty
{
    namespace Particles
    {
        struct Particle;
        
        class ConstantRateEmitter : public Emitter
        {
        public:
            ConstantRateEmitter(int maximumParticles);
            virtual ~ConstantRateEmitter();
            
            int& ParticlesPerSecond();
            
        protected:
            virtual int Emit(Particle *particles, int max, float deltaSeconds);
            virtual void Emit(Particle *particles, int count) = 0;
            
        private:
            struct impl;
            std::unique_ptr<impl> pimpl;
        };
    }
}

#endif//NINJAPARTY_CONSTANTRATEPARTICLEEMITTER_HPP
