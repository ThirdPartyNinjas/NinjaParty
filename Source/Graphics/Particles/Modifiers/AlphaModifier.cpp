#include <NinjaParty/MathHelpers.hpp>

#include <NinjaParty/Particles/Particle.hpp>
#include <NinjaParty/Particles/Modifiers/AlphaModifier.hpp>

namespace NinjaParty
{
    namespace Particles
    {
        struct AlphaModifier::impl
        {
            float initialAlpha;
            float finalAlpha;
        };
        
        AlphaModifier::AlphaModifier(float initialAlpha, float finalAlpha)
        : pimpl(new impl())
        {
            pimpl->initialAlpha = initialAlpha;
            pimpl->finalAlpha = finalAlpha;
        }
        
        AlphaModifier::~AlphaModifier()
        {
        }
        
        void AlphaModifier::Process(float deltaSeconds, Particle *particles, int count)
        {
            float &initialAlpha = pimpl->initialAlpha;
            float &finalAlpha = pimpl->finalAlpha;
            
            for(int i=0; i<count; i++)
            {
                Particle &p = particles[i];
                
                float newAlpha = Lerp(initialAlpha, finalAlpha, p.time / p.lifetime);
                
                float multiplier = newAlpha / p.color.A();
                
                p.color.R() *= multiplier;
                p.color.G() *= multiplier;
                p.color.B() *= multiplier;
                p.color.A() = newAlpha;
            }
        }
    }
}
