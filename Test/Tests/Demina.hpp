#include <memory>

#include <NinjaParty/AssetManager.hpp>
#include <NinjaParty/Color.hpp>
#include <NinjaParty/DeminaAnimationPlayer.hpp>
#include <NinjaParty/Game.hpp>
#include <NinjaParty/MathHelpers.hpp>
#include <NinjaParty/SpriteBatch.hpp>
#include <NinjaParty/Texture.hpp>
#include <NinjaParty/Vector2.hpp>

namespace Tests
{
    class TestGame : public NinjaParty::Game
    {
    public:
        TestGame(int screenWidth, int screenHeight)
        : NinjaParty::Game(screenWidth, screenHeight)
        {
            rotation = 0;
        }
        
        void LoadContent(const std::string &assetPath, const std::string &assetArchivePath)
        {
            assetManager.reset(new NinjaParty::AssetManager(assetPath, assetArchivePath));
            spriteBatch.reset(new NinjaParty::SpriteBatch(GetScreenWidth(), GetScreenHeight()));
            
            NinjaParty::TextureDictionary *textureDictionary = assetManager->LoadTextureDictionary("demina_guy.json");
            texture = assetManager->LoadTexture("demina_guy.png");
            
            deminaPlayer.AddAnimation("Walk", assetManager->LoadDeminaAnimation("guy_walk.anim", texture, textureDictionary));
            deminaPlayer.Start("Walk");
        }
        
        void UnloadContent()
        {
        }
        
        void Update(float deltaSeconds)
        {
            deminaPlayer.Update(deltaSeconds);
        }
        
        void Draw()
        {
            ClearScreen(NinjaParty::Color::Black);
            
            spriteBatch->Begin(NinjaParty::BlendMode::PremultipliedAlpha);
            deminaPlayer.Draw(spriteBatch.get(), NinjaParty::Vector2(GetScreenWidth() / 2, GetScreenHeight() / 2));
            spriteBatch->End();
        }
        
    private:
        std::unique_ptr<NinjaParty::AssetManager> assetManager;
        std::unique_ptr<NinjaParty::SpriteBatch> spriteBatch;
        
        NinjaParty::DeminaAnimationPlayer deminaPlayer;
        
        NinjaParty::Texture *texture;
        float rotation;
    };
}
