#include <NinjaParty/SoundInstance.hpp>

namespace NinjaParty
{
    SoundInstance::SoundInstance(AudioRenderer *audioRenderer, Sound *sound)
        : audioRenderer(audioRenderer), sound(sound), state(State::Stopped)
    {
    }
    
    SoundInstance::~SoundInstance()
    {
    }
    
    void SoundInstance::SetSound(NinjaParty::Sound *sound)
    {
        if(state != State::Stopped)
            throw std::runtime_error("SoundInstance::SetSound cannot set sound when not stopped");
        
        this->sound = sound;
    }
    
    void SoundInstance::Play(int loopCount, float volume, float fadeInTime)
    {
        using namespace std::placeholders;
        
        audioReference = audioRenderer->Play(sound, loopCount, volume, fadeInTime, std::bind(&SoundInstance::Callback, this, _1, _2));
        state = State::Playing;
    }
    
    void SoundInstance::Pause()
    {
        if(state != State::Stopped)
        {
            audioRenderer->Pause(audioReference);
            state = State::Paused;
        }
    }

    void SoundInstance::Resume()
    {
        if(state == State::Paused)
        {
            audioRenderer->Resume(audioReference);
            state = State::Playing;
        }
    }
    
    void SoundInstance::Stop(float fadeOutTime)
    {
        if(state != State::Stopped)
        {
            audioRenderer->Stop(audioReference, fadeOutTime);
        }
    }
    
    void SoundInstance::Callback(const AudioReference &reference, AudioEventType eventType)
    {
        if(eventType == AudioEventType::Complete)
        {
            state = State::Stopped;
        }
    }
}
