#ifndef NINJAPARTY_ALPHAPARTICLEMODIFIER_HPP
#define NINJAPARTY_ALPHAPARTICLEMODIFIER_HPP

#include <memory>

#include <NinjaParty/Particles/Modifiers/Modifier.hpp>

namespace NinjaParty
{
    namespace Particles
    {
        class AlphaModifier : public Modifier
        {
        public:
            AlphaModifier(float initialAlpha, float finalAlpha);
            virtual ~AlphaModifier();
            
            void Process(float deltaSeconds, Particle *particles, int count);
            
        private:
            struct impl;
            std::unique_ptr<impl> pimpl;
        };
    }
}

#endif//NINJAPARTY_ALPHAPARTICLEMODIFIER_HPP
