#include <memory>

#include <NinjaParty/AssetManager.hpp>
#include <NinjaParty/Color.hpp>
#include <NinjaParty/Game.hpp>
#include <NinjaParty/MathHelpers.hpp>
#include <NinjaParty/SpriteBatch.hpp>
#include <NinjaParty/Texture.hpp>
#include <NinjaParty/Vector2.hpp>

#if defined(__ANDROID__)
#include <android/log.h>
#define  LOG_TAG "NinjaPartyTest"
#define  LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#endif

// Note: This test is pretty much brute force.
//   Load up all of the assets every update, then free them again.
//   We're loading them two ways from an archive and from files (except on Android where it's archive only)
//   Running it with valgrind or Instruments is great for tracking down memory leaks

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
            this->assetPath = assetPath;
            this->assetArchivePath = assetArchivePath;
        }
        
        void UnloadContent()
        {
        }
        
        void Update(float deltaSeconds)
        {
            try
            {
                // test from archive
                assetManager.reset(new NinjaParty::AssetManager(assetPath, assetArchivePath));
                NinjaParty::Texture *texture = assetManager->LoadTexture("Graphics.png");
                NinjaParty::TextureDictionary *textureDictionary = assetManager->LoadTextureDictionary("Graphics.json");
                assetManager->LoadSong("song.ogg");
                assetManager->LoadSoundEffect("sound.wav");
                assetManager->LoadDeminaAnimation("guy_walk.anim", texture, textureDictionary);
                assetManager->LoadSpineSkeletonData("spineboy.json", textureDictionary);
                assetManager->LoadFont("Arial_40.fnt");
                assetManager->LoadGleedLevel("TestLevel.gleed");
                
#if !defined(__ANDROID__) // android is archive-only
                // test from folder
                assetManager.reset(new NinjaParty::AssetManager(assetPath, ""));
                texture = assetManager->LoadTexture("Graphics.png");
                textureDictionary = assetManager->LoadTextureDictionary("Graphics.json");
                assetManager->LoadSong("song.ogg");
                assetManager->LoadSoundEffect("sound.wav");
                assetManager->LoadDeminaAnimation("guy_walk.anim", texture, textureDictionary);
                assetManager->LoadSpineSkeletonData("spineboy.json", textureDictionary);
                assetManager->LoadFont("Arial_40.fnt");
                assetManager->LoadGleedLevel("TestLevel.gleed");
#endif
            }
            catch(std::exception &exception)
            {
                printf("Caught exception: %s\n", exception.what());
#if defined(__ANDROID__)
                LOGE("Caught exception: %s", exception.what());
#endif
            }
        }
        
        void Draw()
        {
            ClearScreen(NinjaParty::Color::CornflowerBlue);
        }
        
    private:
        std::unique_ptr<NinjaParty::AssetManager> assetManager;
        std::string assetPath;
        std::string assetArchivePath;
    };
}
