#include <map>
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

        void HandleLostGraphicsContext()
        {
            spriteBatch.reset(new NinjaParty::SpriteBatch(GetScreenWidth(), GetScreenHeight()));
            texture = assetManager->LoadTexture("Star.png", true);
        }
        
        void Update(float deltaSeconds)
        {
        }
        
        void Draw()
        {
            ClearScreen(NinjaParty::Color::Black);
            
            spriteBatch->Begin();

            for(auto &iterator : touches)
            {
                spriteBatch->Draw(texture, iterator.second, nullptr, NinjaParty::Vector2(0.5f, 0.5f));
            }

            spriteBatch->End();
        }

        virtual void TouchBegan(void *touchHandle, int tapCount, int x, int y)
        {
            touches[touchHandle] = NinjaParty::Vector2(x, y);
        }

        virtual void TouchEnded(void *touchHandle, int x, int y)
        {
            if(touches.count(touchHandle) != 0)
                touches.erase(touchHandle);
        }

        virtual void TouchMoved(void *touchHandle, int x, int y)
        {
            touches[touchHandle] = NinjaParty::Vector2(x, y);
        }

        virtual void TouchCancelled(void *touchHandle)
        {
            if(touches.count(touchHandle) != 0)
                touches.erase(touchHandle);
        }

    private:
        std::unique_ptr<NinjaParty::AssetManager> assetManager;
        std::unique_ptr<NinjaParty::SpriteBatch> spriteBatch;
        
        NinjaParty::Texture *texture;
        std::map<void*, NinjaParty::Vector2> touches;
    };
}
