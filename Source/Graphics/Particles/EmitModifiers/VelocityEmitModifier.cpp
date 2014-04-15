#include <NinjaParty/Math.hpp>
#include <NinjaParty/Random.hpp>
#include <NinjaParty/Particles/Particle.hpp>
#include <NinjaParty/Particles/EmitModifiers/VelocityEmitModifier.hpp>

namespace NinjaParty
{
    namespace Particles
    {
        struct VelocityEmitModifier::impl
        {
            impl(NinjaParty::Random &random, float minX, float maxX, float minY, float maxY)
            : random(random), minX(minX), maxX(maxX), minY(minY), maxY(maxY)
            {
            }

            NinjaParty::Random &random;
            float minX, maxX;
            float minY, maxY;
        };
        
        VelocityEmitModifier::VelocityEmitModifier(NinjaParty::Random &random, float minX, float maxX, float minY, float maxY)
        : pimpl(new impl(random, minX, maxX, minY, maxY))
        {
        }
        
        VelocityEmitModifier::~VelocityEmitModifier()
        {
        }
        
        void VelocityEmitModifier::Process(Particle *particles, int count)
        {
            NinjaParty::Random &random = pimpl->random;
            float &minX = pimpl->minX;
            float &maxX = pimpl->maxX;
            float &minY = pimpl->minY;
            float &maxY = pimpl->maxY;
            
            for(int i=0; i<count; i++)
            {
                Particle &p = particles[i];
                
                p.velocity = NinjaParty::Vector2(random.NextFloat(minX, maxX), random.NextFloat(minY, maxY));
            }
        }
    }
}