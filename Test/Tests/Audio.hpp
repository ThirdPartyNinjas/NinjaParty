#include <memory>

#include <NinjaParty/AssetManager.hpp>
#include <NinjaParty/AudioRenderer.hpp>
#include <NinjaParty/Color.hpp>
#include <NinjaParty/Game.hpp>
#include <NinjaParty/Song.hpp>
#include <NinjaParty/SoundEffect.hpp>

namespace Tests
{
    class TestGame : public NinjaParty::Game
    {
    public:
        TestGame(int screenWidth, int screenHeight)
            : NinjaParty::Game(screenWidth, screenHeight)
        {
            timer = 0;
        }
        
        void LoadContent(const std::string &assetPath, const std::string &assetArchivePath)
        {
            assetManager.reset(new NinjaParty::AssetManager(assetPath, assetArchivePath));
            audioRenderer.reset(new NinjaParty::AudioRenderer());
            
            soundEffect = assetManager->LoadSoundEffect("sound.wav");
            song = assetManager->LoadSong("song.ogg");
            
            audioRenderer->Play(song, -1, 0.5f);
        }
        
        void UnloadContent()
        {
        }
        
        void Update(float deltaSeconds)
        {
            timer += deltaSeconds;
            
            if(timer >= 2.0f)
            {
                timer -= 2.0f;
                audioRenderer->Play(soundEffect, 0, 1.0f);
            }
        }
        
        void Draw()
        {
            ClearScreen(NinjaParty::Color::Black);
        }
        
    private:
        std::unique_ptr<NinjaParty::AssetManager> assetManager;
        std::unique_ptr<NinjaParty::AudioRenderer> audioRenderer;
        
        float timer;
        
        NinjaParty::Song *song;
        NinjaParty::SoundEffect *soundEffect;
    };
}
