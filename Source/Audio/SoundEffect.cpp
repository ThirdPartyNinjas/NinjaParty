#include <NinjaParty/Random.hpp>
#include <NinjaParty/SoundEffect.hpp>

#include "Wav.hpp"

namespace NinjaParty
{
    SoundEffect::SoundEffect()
    {
    }
    
	SoundEffect::~SoundEffect()
	{
		alDeleteBuffers(1, &audioBuffer);
	}

    SoundEffect* SoundEffect::FromFile(const std::string &fileName)
    {
        std::unique_ptr<Wav> wav(Wav::FromFile(fileName));
        return FromSamples(wav->GetSamples(), wav->GetSampleCount(), wav->GetChannels(), wav->GetSampleRate());
    }
    
    SoundEffect* SoundEffect::FromBuffer(unsigned char *buffer, int length)
    {
        std::unique_ptr<Wav> wav(Wav::FromBuffer(buffer, length));
        return FromSamples(wav->GetSamples(), wav->GetSampleCount(), wav->GetChannels(), wav->GetSampleRate());
    }
    
    SoundEffect* SoundEffect::FromSamples(int16_t *samples, int sampleCount, int channels, int sampleRate)
    {
        SoundEffect *soundEffect = new SoundEffect();
        
		alGenBuffers(1, &soundEffect->audioBuffer);
		alBufferData(soundEffect->audioBuffer, (channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, samples, sampleCount * channels * 2, sampleRate);

		soundEffect->isStreaming = false;
		soundEffect->channels = channels;
		soundEffect->sampleRate = sampleRate;
        soundEffect->duration = sampleCount / static_cast<float>(sampleRate);
        
        return soundEffect;
    }

	bool SoundEffect::GetAudioBuffer(ALuint &audioBuffer)
	{
		audioBuffer = this->audioBuffer;
		return true;
	}
}
