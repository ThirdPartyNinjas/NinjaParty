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
            NinjaParty::SoundEffect *soundEffect = assetManager->LoadSoundEffect("sound.wav");
            NinjaParty::DeminaAnimation *deminaAnimation = assetManager->LoadDeminaAnimation("guy_walk.anim", texture, textureDictionary);
            NinjaParty::SpineSkeletonData *spineSkeletonData = assetManager->LoadSpineSkeletonData("spineboy.json", textureDictionary);
            NinjaParty::Font *font = assetManager->LoadFont("Arial_40.fnt");
            NinjaParty::GleedLevel *gleedLevel = assetManager->LoadGleedLevel("TestLevel.gleed");

            assetManager.reset(new NinjaParty::AssetManager(assetPath, ""));
            
            // test from folder
#if !defined(__ANDROID__) // android is archive-only
            texture = assetManager->LoadTexture("Graphics.png");
            textureDictionary = assetManager->LoadTextureDictionary("Graphics.json");
            song = assetManager->LoadSong("song.ogg");
            soundEffect = assetManager->LoadSoundEffect("sound.wav");
            deminaAnimation = assetManager->LoadDeminaAnimation("guy_walk.anim", texture, textureDictionary);
            spineSkeletonData = assetManager->LoadSpineSkeletonData("spineboy.json", textureDictionary);
            font = assetManager->LoadFont("Arial_40.fnt");
            gleedLevel = assetManager->LoadGleedLevel("TestLevel.gleed");
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
