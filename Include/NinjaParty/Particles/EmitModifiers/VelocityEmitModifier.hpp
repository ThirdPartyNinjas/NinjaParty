#ifndef NINJAPARTY_VELOCITYPARTICLEEMITMODIFIER_HPP
#define NINJAPARTY_VELOCITYPARTICLEEMITMODIFIER_HPP

#include <memory>

#include <NinjaParty/Particles/EmitModifiers/EmitModifier.hpp>

namespace NinjaParty
{
    class Random;
    
    namespace Particles
    {
        struct Particle;
        
        class VelocityEmitModifier : public EmitModifier
        {
        public:
            VelocityEmitModifier(Random &random, float minX, float maxX, float minY, float maxY);
            virtual ~VelocityEmitModifier();
            
            void Process(Particle *particles, int count);
            
        private:
            struct impl;
            std::unique_ptr<impl> pimpl;
        };
    }
}

#endif//NINJAPARTY_VELOCITYPARTICLEEMITMODIFIER_HPP
