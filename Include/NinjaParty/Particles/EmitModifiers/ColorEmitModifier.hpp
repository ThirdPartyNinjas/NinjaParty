#ifndef NINJAPARTY_COLOREMITMODIFIER_HPP
#define NINJAPARTY_COLOREMITMODIFIER_HPP

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
        class ColorEmitModifier : public EmitModifier
        {
        public:
            ColorEmitModifier(Random &random, const std::vector<Color> &colors)
            : random(random), colors(colors)
            {
            }
            
            virtual ~ColorEmitModifier()
            {
            }
            
            void Process(Particle *particles, int count)
            {
                for(int i=0; i<count; i++)
                {
                    Particle &p = particles[i];
                    
                    int index = random.NextInt(colors.size() - 1);
                    p.color = colors[index];
                }
            }
            
        private:
            Random &random;
            std::vector<Color> colors;
        };
    }
}

#endif//NINJAPARTY_COLOREMITMODIFIER_HPP
