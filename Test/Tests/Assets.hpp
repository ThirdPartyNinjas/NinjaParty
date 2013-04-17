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
            // test from archive
            assetManager.reset(new NinjaParty::AssetManager(assetPath, assetArchivePath));
            NinjaParty::Texture *texture = assetManager->LoadTexture("Graphics.png");
            NinjaParty::TextureDictionary *textureDictionary = assetManager->LoadTextureDictionary("Graphics.json");
            NinjaParty::Song *song = assetManager->LoadSong("song.ogg");
            
            // haven't gotten this far
            // todo:
            NinjaParty::DeminaAnimation *deminaAnimation = assetManager->LoadDeminaAnimation("guy_walk.json", texture, textureDictionary);
            NinjaParty::SpineSkeletonData *spineSkeletonData = assetManager->LoadSpineSkeletonData("spineboy.json", textureDictionary);
            NinjaParty::Font *font = assetManager->LoadFont("Arial_40.json");
            NinjaParty::GleedLevel *gleedLevel = assetManager->LoadGleedLevel("TestLevel.gleed");
            NinjaParty::SoundEffect *soundEffect = assetManager->LoadSoundEffect("sound.wav");
            
            // test from folder
#if !defined(__ANDROID__)
            assetManager.reset(new NinjaParty::AssetManager(assetPath, ""));
//            texture = assetManager->LoadTexture("Graphics.png");
//            textureDictionary = assetManager->LoadTextureDictionary("Graphics.json");
//            assetManager->LoadDeminaAnimation("guy_walk.json", texture, textureDictionary);
//            assetManager->LoadSpineSkeletonData("spineboy.json", textureDictionary);
//            assetManager->LoadFont("Arial_40.json");
//            assetManager->LoadGleedLevel("TestLevel.gleed");
//            assetManager->LoadSong("song.ogg");
//            assetManager->LoadSoundEffect("sound.wav");
#endif
        }
        
        void UnloadContent()
        {
        }
        
        void Update(float deltaSeconds)
        {
        }
        
        void Draw()
        {
            ClearScreen(NinjaParty::Color::CornflowerBlue);
        }
        
    private:
        std::unique_ptr<NinjaParty::AssetManager> assetManager;
    };
}
