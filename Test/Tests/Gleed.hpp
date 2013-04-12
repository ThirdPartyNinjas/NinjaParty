#include <memory>

#include <NinjaParty/AssetManager.hpp>
#include <NinjaParty/Game.hpp>
#include <NinjaParty/GleedLevel.hpp>

namespace Tests
{
    // note: This test just currently dumps a bunch of stuff to stdout with printf (is that enough?)
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
            
            gleedLevel = assetManager->LoadGleedLevel("TestLevel.gleed");
            
            for(NinjaParty::GleedLayer &layer : gleedLevel->layers)
            {
                printf("Layer, %s\n", layer.name.c_str());
                for(auto &item : layer.items)
                {
                    switch(item->GetItemType())
                    {
                        case NinjaParty::GleedItemType::Texture:
                            printf("Texture, %s\n", item->name.c_str());
                            break;
                        case NinjaParty::GleedItemType::Path:
                            printf("Path, %s\n", item->name.c_str());
                            break;
                        case NinjaParty::GleedItemType::Circle:
                            printf("Circle, %s\n", item->name.c_str());
                            break;
                        case NinjaParty::GleedItemType::Rectangle:
                            printf("Rectangle, %s\n", item->name.c_str());
                            break;
                    }
                }
            }
        }
        
        void UnloadContent()
        {
        }
        
        void Update(float deltaSeconds)
        {
        }
        
        void Draw()
        {
            ClearScreen(NinjaParty::Color::Black);
        }
        
    private:
        std::unique_ptr<NinjaParty::AssetManager> assetManager;
        
        NinjaParty::GleedLevel *gleedLevel;
    };
}
