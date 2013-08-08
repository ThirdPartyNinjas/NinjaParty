#include <memory>

#include <NinjaParty/AssetManager.hpp>
#include <NinjaParty/Color.hpp>
#include <NinjaParty/Game.hpp>
#include <NinjaParty/MathHelpers.hpp>
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
            rotation = 0;
        }
        
        void LoadContent(const std::string &assetPath, const std::string &assetArchivePath)
        {
            assetManager.reset(new NinjaParty::AssetManager(assetPath, assetArchivePath));
            spriteBatch.reset(new NinjaParty::SpriteBatch(GetScreenWidth(), GetScreenHeight()));
            
            texture = assetManager->LoadTexture("regiontest.png");
            textureDictionary = assetManager->LoadTextureDictionary("regiontest.json");
            textureRegion = textureDictionary->GetRegion("arrow.png");
        }
        
        void UnloadContent()
        {
        }
        
        void HandleLostGraphicsContext()
        {
            spriteBatch.reset(new NinjaParty::SpriteBatch(GetScreenWidth(), GetScreenHeight()));
            texture = assetManager->LoadTexture("regiontest.png", true);
        }

        void Update(float deltaSeconds)
        {
            rotation += deltaSeconds * NinjaParty::PI;
        }
        
        void Draw()
        {
            ClearScreen(NinjaParty::Color::Black);
            
            spriteBatch->Begin();
            
            spriteBatch->Draw(texture, textureRegion, NinjaParty::Vector2::ZERO, NinjaParty::Vector2::ZERO);
            spriteBatch->Draw(texture, textureRegion, NinjaParty::Vector2(GetScreenWidth() / 2, GetScreenHeight() / 2), NinjaParty::Vector2(0.5f, 0.5f), rotation);
            spriteBatch->Draw(texture, textureRegion, NinjaParty::Vector2(GetScreenWidth(), GetScreenHeight()), NinjaParty::Vector2::ONE);
            
            spriteBatch->End();
        }
        
    private:
        std::unique_ptr<NinjaParty::AssetManager> assetManager;
        std::unique_ptr<NinjaParty::SpriteBatch> spriteBatch;
        
        NinjaParty::Texture *texture;
        NinjaParty::TextureDictionary *textureDictionary;
        NinjaParty::TextureRegion textureRegion;
        float rotation;
    };
}
