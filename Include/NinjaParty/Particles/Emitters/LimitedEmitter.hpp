#ifndef NINJAPARTY_LIMITEDEMITTER_HPP
#define NINJAPARTY_LIMITEDEMITTER_HPP

#include <memory>

#include <NinjaParty/Particles/Emitters/Emitter.hpp>

namespace NinjaParty
{
    namespace Particles
    {
        struct Particle;
        
        class LimitedEmitter : public Emitter
        {
        public:
            LimitedEmitter(int particlesToEmit, int particlesPerSecond);
            virtual ~LimitedEmitter();
            
            virtual bool Completed() const;

        protected:
            virtual int Emit(Particle *particles, int max, float deltaSeconds);
            virtual void Emit(Particle *particles, int count) = 0;
            
        private:
            struct impl;
            std::unique_ptr<impl> pimpl;
        };
    }
}

#endif//NINJAPARTY_LIMITEDEMITTER_HPP
