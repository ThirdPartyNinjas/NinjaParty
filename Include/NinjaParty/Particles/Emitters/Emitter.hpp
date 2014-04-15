#ifndef NINJAPARTY_PARTICLEEMITTER_HPP
#define NINJAPARTY_PARTICLEEMITTER_HPP

#include <memory>

#include <NinjaParty/Color.hpp>
#include <NinjaParty/Math.hpp>
#include <NinjaParty/TextureRegion.hpp>

namespace NinjaParty
{
    class SpriteBatch;
    class SpriteAnimation;
    class Texture;

    namespace Particles
    {
        class EmitModifier;
        class Modifier;
        
        struct Particle;
        
        enum class ParticleBlendMode
        {
            Additive = 0,
            Alpha,
        };
        
        class Emitter
        {
        public:
            Emitter(int maximumParticles);
            virtual ~Emitter();
            
            void Update(float deltaSeconds);
            void Prewarm(float seconds);
            
            void Draw(SpriteBatch *spriteBatch);
            
            void SetTexture(Texture *texture, TextureRegion *textureRegion);
            void SetSpriteAnimation(SpriteAnimation *spriteAnimation);
            
            void SetBlendMode(ParticleBlendMode blendMode);
            
            Vector2& Position();
            float& Rotation();
            
            NinjaParty::Vector2& DefaultVelocity();
            NinjaParty::Vector2& DefaultScale();
            NinjaParty::Color& DefaultColor();
            float& DefaultLifetime();
            float& DefaultRotationSpeed();
            
            int ActiveParticles() const;
            
            void SetDefaults(Particle &p);
            
            void AddModifier(std::shared_ptr<Modifier> modifier);
            void AddEmitModifier(std::shared_ptr<EmitModifier> emitModifier);

            void Pause();
            void Unpause();
            
            bool Paused() const;
            
            void Terminate();
            bool Terminated() const;
            
            virtual bool Completed() const;

        protected:
            virtual int Emit(Particle *particles, int max, float deltaSeconds) = 0;

        private:
            struct impl;
            std::unique_ptr<impl> pimpl;
        };
    }
}

#endif//NINJAPARTY_PARTICLEEMITTER_HPP
