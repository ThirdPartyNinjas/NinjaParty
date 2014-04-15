#include <NinjaParty/MathHelpers.hpp>
#include <NinjaParty/Random.hpp>
#include <NinjaParty/Particles/Particle.hpp>
#include <NinjaParty/Particles/Emitters/CircleEmitter.hpp>

namespace NinjaParty
{
    namespace Particles
    {
        struct CircleEmitter::impl
        {
            impl(Random &random, float radius, bool edge)
            : random(random), radius(radius), edge(edge)
            {
            }
            
            Random &random;
            float radius;
            bool edge;
        };
        
        CircleEmitter::CircleEmitter(int maximumParticles, Random &random, float radius, bool edge)
        : ConstantRateEmitter(maximumParticles), pimpl(new impl(random, radius, edge))
        {
        }
        
        CircleEmitter::~CircleEmitter()
        {
        }
        
        void CircleEmitter::Emit(Particle *particles, int count)
        {
            Random &random = pimpl->random;
            bool &edge = pimpl->edge;
            float &radius = pimpl->radius;
            
            for(int i=0; i<count; i++)
            {
                float radiusMultiplier = edge ? radius : radius * random.NextFloat();
                float radians = random.NextFloat(-NinjaParty::PI, NinjaParty::PI);
                NinjaParty::Vector2 angle = NinjaParty::Vector2(std::sin(radians), std::cos(radians));
                
                Particle &p = particles[i];
                
                SetDefaults(p);
                
                p.position = Position() + NinjaParty::Vector2(angle.X() * radiusMultiplier, angle.Y() * radiusMultiplier);
            }
        }
    }
}