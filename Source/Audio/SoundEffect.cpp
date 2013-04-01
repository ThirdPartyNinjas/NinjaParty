#include <NinjaParty/SoundEffect.hpp>

#include "Wav.hpp"

namespace NinjaParty
{
	SoundEffect::SoundEffect(const std::string &fileName)
	{
		isStreaming = false;
		
		Wav wav(fileName);
		
		alGenBuffers(1, &audioBuffer);
		
		alBufferData(audioBuffer, (wav.GetChannels() == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16,
                     wav.GetSamples(), wav.GetSampleCount() * 2, wav.GetSampleRate());
		
		channels = wav.GetChannels();
		sampleRate = wav.GetSampleRate();
        duration = (wav.GetSampleCount() / channels) / static_cast<float>(sampleRate);
	}
	
	SoundEffect::SoundEffect(int channels, int sampleRate, int16_t *samples, int sampleCount)
	{
		isStreaming = false;
		
		alGenBuffers(1, &audioBuffer);
		
		alBufferData(audioBuffer, (channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, samples, sampleCount * 2, sampleRate);
		
		this->channels = channels;
		this->sampleRate = sampleRate;
	}

	SoundEffect::~SoundEffect()
	{
		alDeleteBuffers(1, &audioBuffer);
	}
	
	bool SoundEffect::GetAudioBuffer(ALuint &audioBuffer)
	{
		audioBuffer = this->audioBuffer;
		return true;
	}
}
