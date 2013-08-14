#include <memory>

#include <NinjaParty/AssetManager.hpp>
#include <NinjaParty/Color.hpp>
#include <NinjaParty/Font.hpp>
#include <NinjaParty/Game.hpp>
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
        }
        
        void LoadContent(const std::string &assetPath, const std::string &assetArchivePath)
        {
			assetManager.reset(new NinjaParty::AssetManager(assetPath, ""));
			spriteBatch.reset(new NinjaParty::SpriteBatch(GetScreenWidth(), GetScreenHeight()));
			
			font = assetManager->LoadFont("Arial_40.fnt");
			fontTexture = assetManager->LoadTexture("Arial_40.png");
			
			std::unique_ptr<NinjaParty::Color[]> colors(new NinjaParty::Color[1]);
			for(int i=0; i<1; i++)
			{
				colors[i] = NinjaParty::Color(1, 1, 0, 0.5f);
			}
			
			texture.reset(NinjaParty::Texture::FromColors(colors.get(), 1, 1));
        }
        
        void UnloadContent()
        {
        }
        
        void Update(float deltaSeconds)
        {
        }
        
        void Draw()
        {
			std::string string = "abcdefghijklmnopqrstuvwxyz";
            ClearScreen(NinjaParty::Color::CornflowerBlue);
			spriteBatch->Begin();
			spriteBatch->DrawString(font, fontTexture, string, NinjaParty::Vector2(GetScreenWidth() / 2, GetScreenHeight() / 2), NinjaParty::Vector2(0.5f, 0.5f));
		
			spriteBatch->Draw(texture.get(), NinjaParty::Vector2(GetScreenWidth() / 2, GetScreenHeight() / 2), nullptr, NinjaParty::Vector2(0.5f, 0.5f), 0, NinjaParty::Color::White, NinjaParty::MeasureString(font, string));
			spriteBatch->End();
        }
        
    private:
        std::unique_ptr<NinjaParty::AssetManager> assetManager;
		std::unique_ptr<NinjaParty::SpriteBatch> spriteBatch;
		
		NinjaParty::Font *font;
		NinjaParty::Texture *fontTexture;

		std::unique_ptr<NinjaParty::Texture> texture;
    };
}
