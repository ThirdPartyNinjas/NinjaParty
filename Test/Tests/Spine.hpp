#include <memory>

#include <NinjaParty/AssetManager.hpp>
#include <NinjaParty/Color.hpp>
#include <NinjaParty/Game.hpp>
#include <NinjaParty/MathHelpers.hpp>
#include <NinjaParty/Random.hpp>
#include <NinjaParty/SpineAnimationPlayer.hpp>
#include <NinjaParty/SpriteBatch.hpp>
#include <NinjaParty/Texture.hpp>
#include <NinjaParty/TextureDictionary.hpp>
#include <NinjaParty/Vector2.hpp>

namespace Tests
{
    class TestGame : public NinjaParty::Game
    {
    public:
        TestGame(int screenWidth, int screenHeight)
            : NinjaParty::Game(screenWidth, screenHeight)
        {
            time = 0;
            inWalk = true;
        }
        
        void LoadContent(const std::string &assetPath, const std::string &assetArchivePath)
        {
            assetManager.reset(new NinjaParty::AssetManager(assetPath, assetArchivePath));
            spriteBatch.reset(new NinjaParty::SpriteBatch(GetScreenWidth(), GetScreenHeight()));
            
            NinjaParty::TextureDictionary *textureDictionary = assetManager->LoadTextureDictionary("Graphics.json");
            texture = assetManager->LoadTexture("Graphics.png");
            NinjaParty::SpineSkeletonData *skeletonData = assetManager->LoadSpineSkeletonData("spineboy.json", textureDictionary);
            spineAnimationPlayer.SetSkeletonData(skeletonData);
            spineAnimationPlayer.SetTexture(texture);
            spineAnimationPlayer.AddAnimation("walk", assetManager->LoadSpineAnimation("walk", skeletonData), true);
            spineAnimationPlayer.AddAnimation("jump", assetManager->LoadSpineAnimation("jump", skeletonData), false);
            spineAnimationPlayer.Start("walk");
        }
        
        void UnloadContent()
        {
        }
        
        void HandleLostGraphicsContext()
        {
            spriteBatch.reset(new NinjaParty::SpriteBatch(GetScreenWidth(), GetScreenHeight()));
            texture = assetManager->LoadTexture("Graphics.png", true);
            spineAnimationPlayer.SetTexture(texture);
        }

        void Update(float deltaSeconds)
        {
            NinjaParty::AnimationEvent animationEvent = spineAnimationPlayer.Update(deltaSeconds);
            
            time += deltaSeconds;
            
            if(time >= 2 || animationEvent.AnimationComplete)
            {
                time = 0;
                if(inWalk)
                    spineAnimationPlayer.Transition("jump", 0.2f);
                else
                    spineAnimationPlayer.Transition("walk", 0.2f);
                inWalk = !inWalk;
            }
        }
        
        void Draw()
        {
            ClearScreen(NinjaParty::Color::Black);
            
            spriteBatch->Begin(NinjaParty::BlendMode::PremultipliedAlpha);
            spineAnimationPlayer.Draw(spriteBatch.get(), NinjaParty::Vector2(GetScreenWidth() / 2, GetScreenHeight() / 2));
            spriteBatch->End();
        }
        
    private:
        std::unique_ptr<NinjaParty::AssetManager> assetManager;
        std::unique_ptr<NinjaParty::SpriteBatch> spriteBatch;
        
        NinjaParty::SpineAnimationPlayer spineAnimationPlayer;
        NinjaParty::Texture *texture;
        
        bool inWalk;
        float time;
    };
}
