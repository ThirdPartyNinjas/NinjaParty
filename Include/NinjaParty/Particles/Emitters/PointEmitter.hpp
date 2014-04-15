#ifndef NINJAPARTY_POINTPARTICLEEMITTER_HPP
#define NINJAPARTY_POINTPARTICLEEMITTER_HPP

#include <NinjaParty/Particles/Emitters/ConstantRateEmitter.hpp>

namespace NinjaParty
{
    namespace Particles
    {
        class PointEmitter : public ConstantRateEmitter
        {
        public:
            PointEmitter(int maximumParticles);
            virtual ~PointEmitter();
            
        protected:
            virtual void Emit(Particle *particles, int count);
        };
    }
}

#endif//NINJAPARTY_POINTPARTICLEEMITTER_HPP
