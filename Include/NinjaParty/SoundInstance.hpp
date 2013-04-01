#ifndef NINJAPARTY_SOUNDINSTANCE_HPP
#define NINJAPARTY_SOUNDINSTANCE_HPP

#include "AudioRenderer.hpp"
#include "Sound.hpp"

namespace NinjaParty
{
    class SoundInstance
    {
    public:
        enum class State
        {
            Stopped = 0,
            Playing,
            Paused,
        };
        
        SoundInstance(AudioRenderer *audioRenderer, Sound *sound);
        ~SoundInstance();
        
        void Play(int loopCount, float volume, float fadeInTime = 0);
        void Pause();
        void Resume();
        void Stop(float fadeOutTime = 0);

        State GetState() const { return state; }
        bool IsStopped() const { return state == State::Stopped; }
        
        void Callback(const AudioReference &reference, AudioEventType eventType);
        
    private:
        AudioRenderer *audioRenderer;
        AudioReference audioReference;
        
        Sound *sound;
        
        State state;
    };
}

#endif//NINJAPARTY_SOUNDINSTANCE_HPP
