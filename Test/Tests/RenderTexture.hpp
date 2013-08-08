#include <memory>

#include <NinjaParty/AssetManager.hpp>
#include <NinjaParty/Color.hpp>
#include <NinjaParty/Game.hpp>
#include <NinjaParty/MathHelpers.hpp>
#include <NinjaParty/RenderTexture.hpp>
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
            
            renderTexture.reset(new NinjaParty::RenderTexture(GetScreenWidth() / 2, GetScreenHeight()));
        }
        
        void UnloadContent()
        {
        }
        
        void HandleLostGraphicsContext()
        {
            spriteBatch.reset(new NinjaParty::SpriteBatch(GetScreenWidth(), GetScreenHeight()));
            
            texture = assetManager->LoadTexture("Star.png", true);
            
            renderTexture.reset(new NinjaParty::RenderTexture(GetScreenWidth() / 2, GetScreenHeight()));
        }        
        
        void Update(float deltaSeconds)
        {
            rotation += deltaSeconds * NinjaParty::PI;
        }
        
        void Draw()
        {
            renderTexture->Bind();
            ClearScreen(NinjaParty::Color::Black);
            
            spriteBatch->Begin();
            spriteBatch->Draw(texture, NinjaParty::Vector2(GetScreenWidth() / 4, GetScreenHeight() / 2), nullptr, NinjaParty::Vector2(0.5f, 0.5f));
            spriteBatch->End();
            
            renderTexture->Unbind();

            ClearScreen(NinjaParty::Color::Black);
            
            spriteBatch->Begin(NinjaParty::BlendMode::PremultipliedAlpha);
            
            spriteBatch->Draw(renderTexture.get(), NinjaParty::Vector2::ZERO);

            spriteBatch->Draw(renderTexture.get(), NinjaParty::Vector2(GetScreenWidth() / 2, GetScreenHeight()), nullptr, NinjaParty::Vector2::ZERO,
                              0, NinjaParty::Color::White, NinjaParty::Vector2(1, -1));
            
            spriteBatch->End();
        }
        
    private:
        std::unique_ptr<NinjaParty::AssetManager> assetManager;
        std::unique_ptr<NinjaParty::SpriteBatch> spriteBatch;
        
        NinjaParty::Texture *texture;
        std::unique_ptr<NinjaParty::RenderTexture> renderTexture;
        float rotation;
    };
}
