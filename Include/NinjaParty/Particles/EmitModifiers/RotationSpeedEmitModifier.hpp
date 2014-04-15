#ifndef NINJAPARTY_ROTATIONSPEEDEMITMODIFIER_HPP
#define NINJAPARTY_ROTATIONSPEEDEMITMODIFIER_HPP

#include <iterator>
#include <vector>

#include <NinjaParty/Color.hpp>
#include <NinjaParty/Random.hpp>

#include <NinjaParty/Particles/Particle.hpp>
#include <NinjaParty/Particles/EmitModifiers/EmitModifier.hpp>

namespace NinjaParty
{
    namespace Particles
    {
        class RotationSpeedEmitModifier : public EmitModifier
        {
        public:
            RotationSpeedEmitModifier(Random &random, float min, float max)
            : random(random), min(min), max(max)
            {
            }
            
            virtual ~RotationSpeedEmitModifier()
            {
            }
            
            void Process(Particle *particles, int count)
            {
                for(int i=0; i<count; i++)
                {
                    Particle &p = particles[i];
                    
                    p.rotationSpeed = random.NextFloat(min, max);
                }
            }
            
        private:
            Random &random;
            float min, max;
        };
    }
}

#endif//NINJAPARTY_ROTATIONSPEEDEMITMODIFIER_HPP
