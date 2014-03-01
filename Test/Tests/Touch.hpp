#include <map>
#include <memory>

#include <NinjaParty/AssetManager.hpp>
#include <NinjaParty/Color.hpp>
#include <NinjaParty/Game.hpp>
#include <NinjaParty/GestureEvents.hpp>
#include <NinjaParty/MathHelpers.hpp>
#include <NinjaParty/SpriteBatch.hpp>
#include <NinjaParty/Texture.hpp>
#include <NinjaParty/TouchEvents.hpp>
#include <NinjaParty/Math.hpp>

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
            texture = assetManager->LoadTexture("Star.png");//, true);
        }
        
        void Update(float deltaSeconds)
        {
            while(GetEventCount() > 0)
            {
                using namespace NinjaParty;
                
                auto event = GetEvent();
                
                EventId eventId = event->GetId();
                
                if(eventId == TouchBeganEvent::GetEventId())
                {
                    auto e = std::dynamic_pointer_cast<TouchBeganEvent>(event);
                    touches[e->Handle()] = Vector2(e->X(), e->Y());
                }
                else if(eventId == TouchMovedEvent::GetEventId())
                {
                    auto e = std::dynamic_pointer_cast<TouchMovedEvent>(event);
                    touches[e->Handle()] = Vector2(e->X(), e->Y());
                }
                else if(eventId == TouchEndedEvent::GetEventId())
                {
                    auto e = std::dynamic_pointer_cast<TouchEndedEvent>(event);
                    if(touches.count(e->Handle()) != 0)
                        touches.erase(e->Handle());
                }
                else if(eventId == TouchCancelledEvent::GetEventId())
                {
                    auto e = std::dynamic_pointer_cast<TouchCancelledEvent>(event);
                    if(touches.count(e->Handle()) != 0)
                        touches.erase(e->Handle());
                }
                else if(eventId == GesturePanEvent::GetEventId())
                {
                    auto e = std::dynamic_pointer_cast<GesturePanEvent>(event);
                    printf("Pan Event - Offset: (%d, %d)\n", e->X(), e->Y());
                }
                else if(eventId == GesturePinchEvent::GetEventId())
                {
                    auto e = std::dynamic_pointer_cast<GesturePinchEvent>(event);
                    printf("Pinch Event - Position: (%d, %d) Scale: %f\n", e->X(), e->Y(), e->Scale());
                }
                else if(eventId == GestureRotationEvent::GetEventId())
                {
                    auto e = std::dynamic_pointer_cast<GestureRotationEvent>(event);
                    printf("Rotation Event - Radians: %f\n", e->Radians());
                }
                else if(eventId == GestureSwipeEvent::GetEventId())
                {
                    auto e = std::dynamic_pointer_cast<GestureSwipeEvent>(event);
                    printf("Swipe Event - Direction: ");
                
                    switch(e->Direction())
                    {
                        case SwipeDirection::Up:
                            printf("Up\n");
                            break;
                        case SwipeDirection::Down:
                            printf("Down\n");
                            break;
                        case SwipeDirection::Left:
                            printf("Left\n");
                            break;
                        case SwipeDirection::Right:
                            printf("Right\n");
                            break;
                    }
                }
                else if(eventId == GestureTapEvent::GetEventId())
                {
                    auto e = std::dynamic_pointer_cast<GestureTapEvent>(event);
                    printf("Tap Event - Position: (%d, %d) Touches: %d\n", e->X(), e->Y(), e->Touches());
                }
            }
        }
        
        void Draw()
        {
            NinjaParty::Game::Draw();
            
            ClearScreen(NinjaParty::Color::Black);
            
            spriteBatch->Begin();

            for(auto &iterator : touches)
            {
                spriteBatch->Draw(texture, iterator.second, nullptr, NinjaParty::Vector2(0.5f, 0.5f));
            }

            spriteBatch->End();
        }

    private:
        std::unique_ptr<NinjaParty::AssetManager> assetManager;
        std::unique_ptr<NinjaParty::SpriteBatch> spriteBatch;
        
        NinjaParty::Texture *texture;
        std::map<void*, NinjaParty::Vector2> touches;
    };
}
