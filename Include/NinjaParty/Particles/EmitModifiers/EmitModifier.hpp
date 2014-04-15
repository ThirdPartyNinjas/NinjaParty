#ifndef NINJAPARTY_PARTICLEEMITMODIFIER_HPP
#define NINJAPARTY_PARTICLEEMITMODIFIER_HPP

namespace NinjaParty
{
    namespace Particles
    {
        struct Particle;
        
        class EmitModifier
        {
        public:
            virtual void Process(Particle *particles, int count) = 0;
            virtual ~EmitModifier() { }
        };
    }
}

#endif//NINJAPARTY_PARTICLEEMITMODIFIER_HPP
