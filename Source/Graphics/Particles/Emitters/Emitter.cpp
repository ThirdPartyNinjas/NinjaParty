#include <NinjaParty/Particles/Particle.hpp>
#include <NinjaParty/Particles/Emitters/Emitter.hpp>
#include <NinjaParty/Particles/EmitModifiers/EmitModifier.hpp>
#include <NinjaParty/Particles/Modifiers/Modifier.hpp>
#include <NinjaParty/SpriteBatch.hpp>
#include <NinjaParty/SpriteAnimation.hpp>
#include <NinjaParty/Texture.hpp>

namespace NinjaParty
{
    namespace Particles
    {
        struct Emitter::impl
        {
            std::unique_ptr<Particle[]> particles;
            int maximumParticles;
            int activeParticles = 0;
            
            Texture *texture = nullptr;
            TextureRegion textureRegion;
            SpriteAnimation *spriteAnimation = nullptr;
            
            ParticleBlendMode blendMode = ParticleBlendMode::Additive;
            
            Vector2 position = NinjaParty::Vector2::ZERO;
            float rotation = 0;
            
            NinjaParty::Vector2 defaultVelocity = NinjaParty::Vector2::ZERO;
            NinjaParty::Vector2 defaultScale = NinjaParty::Vector2::ONE;
            NinjaParty::Color defaultColor = NinjaParty::Color::White;
            float defaultLifetime = 1.0f;
            float defaultRotationSpeed = 0.0f;
            
            bool paused = false;
            bool terminated = false;
            
            std::vector<std::shared_ptr<Modifier>> modifiers;
            std::vector<std::shared_ptr<EmitModifier>> emitModifiers;
        };
        
        Emitter::Emitter(int maximumParticles)
        : pimpl(new impl)
        {
            pimpl->particles.reset(new Particle[maximumParticles]);
            pimpl->maximumParticles = maximumParticles;
        }
        
        Emitter::~Emitter()
        {
        }
        
        void Emitter::Update(float deltaSeconds)
        {
            int &activeParticles = pimpl->activeParticles;
            int &maximumParticles = pimpl->maximumParticles;
            
            for(int particleIndex = 0; particleIndex < activeParticles; ++particleIndex)
            {
                Particle &p = pimpl->particles[particleIndex];
                p.time += deltaSeconds;
                
                if(p.time >= p.lifetime)
                {
                    std::swap(p, pimpl->particles[activeParticles - 1]);
                    particleIndex--;
                    activeParticles--;
                    continue;
                }
                
                p.position += p.velocity * deltaSeconds;
                p.angle += p.rotationSpeed * deltaSeconds;
            }
            
            if(!Terminated() && !Paused())
            {
                int count = Emit(&pimpl->particles[activeParticles], maximumParticles - activeParticles, deltaSeconds);
                
                if(count > 0)
                {
                    for(auto p : pimpl->emitModifiers)
                    {
                        p->Process(&pimpl->particles[activeParticles], count);
                    }
                    
                    activeParticles += count;
                }
            }
            
            for(auto p : pimpl->modifiers)
            {
                p->Process(deltaSeconds, pimpl->particles.get(), activeParticles);
            }
        }
        
        void Emitter::Prewarm(float seconds)
        {
            while(seconds > 0)
            {
                float amount = std::min(1 / 30.0f, seconds);
                seconds -= amount;
                Update(amount);
            }
        }
        
        void Emitter::Draw(SpriteBatch *spriteBatch)
        {
            if(pimpl->spriteAnimation == nullptr)
            {
                for(int particleIndex = 0; particleIndex < pimpl->activeParticles; ++particleIndex)
                {
                    Particle &p = pimpl->particles[particleIndex];
                    
                    NinjaParty::Color color = p.color;
                    
                    if(pimpl->blendMode == ParticleBlendMode::Additive)
                        color.A() = 0;
                    
                    spriteBatch->Draw(pimpl->texture, pimpl->textureRegion, p.position, Vector2(0.5f, 0.5f), p.angle, color, p.scale);
                }
            }
            else
            {
                int maxFrame = pimpl->spriteAnimation->frames.size() - 1;
                
                for(int particleIndex = 0; particleIndex < pimpl->activeParticles; ++particleIndex)
                {
                    Particle &p = pimpl->particles[particleIndex];
                    
                    int frame = pimpl->spriteAnimation->frameSeconds / p.time;
                    TextureRegion &spriteRegion = (frame <= maxFrame) ? pimpl->spriteAnimation->frames[frame] : pimpl->spriteAnimation->frames[maxFrame];
                    
                    NinjaParty::Color color = p.color;
                    
                    if(pimpl->blendMode == ParticleBlendMode::Additive)
                        color.A() = 0;
                    
                    spriteBatch->Draw(pimpl->texture, spriteRegion, p.position, Vector2(0.5f, 0.5f), p.angle, color, p.scale);
                }
            }
        }
        
        void Emitter::SetTexture(Texture *texture, TextureRegion *textureRegion)
        {
            pimpl->texture = texture;
            
            if(textureRegion == nullptr)
            {
                pimpl->textureRegion.bounds.x = pimpl->textureRegion.bounds.y = 0;
                pimpl->textureRegion.bounds.width = texture->GetWidth();
                pimpl->textureRegion.bounds.height = texture->GetHeight();
                pimpl->textureRegion.originTopLeft = Vector2::ZERO;
                pimpl->textureRegion.originCenter = Vector2(0.5f, 0.5f);
                pimpl->textureRegion.originBottomRight = Vector2::ONE;
                pimpl->textureRegion.input.x = 0;
                pimpl->textureRegion.input.y = 0;
                pimpl->textureRegion.input.width = texture->GetWidth();
                pimpl->textureRegion.input.height = texture->GetHeight();
                pimpl->textureRegion.rotated = false;
            }
            else
            {
                pimpl->textureRegion = *textureRegion;
            }
        }
        
        void Emitter::SetSpriteAnimation(SpriteAnimation *spriteAnimation)
        {
            pimpl->spriteAnimation = spriteAnimation;
        }
        
        void Emitter::SetBlendMode(ParticleBlendMode blendMode)
        {
            pimpl->blendMode = blendMode;
        }
        
        Vector2& Emitter::Position()
        {
            return pimpl->position;
        }
        
        float& Emitter::Rotation()
        {
            return pimpl->rotation;
        }
        
        NinjaParty::Vector2& Emitter::DefaultVelocity()
        {
            return pimpl->defaultVelocity;
        }
        
        NinjaParty::Vector2& Emitter::DefaultScale()
        {
            return pimpl->defaultScale;
        }
        
        NinjaParty::Color& Emitter::DefaultColor()
        {
            return pimpl->defaultColor;
        }
        
        float& Emitter::DefaultLifetime()
        {
            return pimpl->defaultLifetime;
        }
        
        float& Emitter::DefaultRotationSpeed()
        {
            return pimpl->defaultRotationSpeed;
        }
        
        void Emitter::SetDefaults(Particle &p)
        {
            p.position = Position();
            p.angle = Rotation();
            
            p.velocity = pimpl->defaultVelocity;
            p.scale = pimpl->defaultScale;
            p.color = pimpl->defaultColor;
            p.time = 0;
            p.lifetime = pimpl->defaultLifetime;
            p.rotationSpeed = pimpl->defaultRotationSpeed;
        }
        
        int Emitter::ActiveParticles() const
        {
            return pimpl->activeParticles;
        }
        
        void Emitter::AddModifier(std::shared_ptr<Modifier> modifier)
        {
            pimpl->modifiers.push_back(modifier);
        }
        
        void Emitter::AddEmitModifier(std::shared_ptr<EmitModifier> emitModifier)
        {
            pimpl->emitModifiers.push_back(emitModifier);
        }
        
        void Emitter::Pause()
        {
            pimpl->paused = true;
        }
        
        void Emitter::Unpause()
        {
            pimpl->paused = false;
        }
        
        bool Emitter::Paused() const
        {
            return pimpl->paused;
        }
        
        void Emitter::Terminate()
        {
            pimpl->terminated = true;
        }
        
        bool Emitter::Terminated() const
        {
            return pimpl->terminated;
        }
        
        bool Emitter::Completed() const
        {
            return (pimpl->terminated && pimpl->activeParticles == 0);
        }
    }
}
