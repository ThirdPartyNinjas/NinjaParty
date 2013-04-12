#include <memory>

#include <NinjaParty/AssetManager.hpp>
#include <NinjaParty/Color.hpp>
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
            
            texture = assetManager->LoadTexture("Star.png");
        }
        
        void UnloadContent()
        {
        }
        
        void Update(float deltaSeconds)
        {
            rotation += deltaSeconds * NinjaParty::PI;
        }
        
        void Draw()
        {
            ClearScreen(NinjaParty::Color::Black);
            
            spriteBatch->Begin();
            spriteBatch->Draw(texture, NinjaParty::Vector2::ZERO, nullptr, NinjaParty::Vector2::ZERO, rotation, NinjaParty::Color(0, 1, 0, 1));
            spriteBatch->Draw(texture, NinjaParty::Vector2(GetScreenWidth() / 2, GetScreenHeight() / 2), nullptr, NinjaParty::Vector2(0.5f, 0.5f), rotation, NinjaParty::Color(1, 1, 1, 0.5f));
            spriteBatch->Draw(texture, NinjaParty::Vector2(GetScreenWidth(), GetScreenHeight()), nullptr, NinjaParty::Vector2(1.0f, 1.0f), rotation, NinjaParty::Color(1, 0, 0, 1));
            spriteBatch->End();
        }
        
    private:
        std::unique_ptr<NinjaParty::AssetManager> assetManager;
        std::unique_ptr<NinjaParty::SpriteBatch> spriteBatch;
        
        NinjaParty::Texture *texture;
        float rotation;
    };
}
