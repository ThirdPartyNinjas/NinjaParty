#ifndef NINJAPARTY_PARTICLEMODIFIER_HPP
#define NINJAPARTY_PARTICLEMODIFIER_HPP

namespace NinjaParty
{
    namespace Particles
    {
        struct Particle;
        
        class Modifier
        {
        public:
            virtual void Process(float deltaSeconds, Particle *particles, int count) = 0;
            virtual ~Modifier() { }
        };
    }
}

#endif//NINJAPARTY_PARTICLEMODIFIER_HPP
