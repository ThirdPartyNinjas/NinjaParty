#ifndef NINJAPARTY_SOUNDEFFECT_HPP
#define NINJAPARTY_SOUNDEFFECT_HPP

#include <memory>
#include <string>

#include "Sound.hpp"

namespace NinjaParty
{
	class SoundEffect : public Sound
	{
	public:
		~SoundEffect();
		
		bool GetAudioBuffer(ALuint &audioBuffer);
		
        static SoundEffect* FromFile(const std::string &fileName);
        static SoundEffect* FromBuffer(unsigned char *buffer, int length);
        static SoundEffect* FromSamples(int16_t *samples, int sampleCount, int channels, int sampleRate);

private:
        SoundEffect();
        
		ALuint audioBuffer;
	};
}

#endif//NINJAPARTY_SOUNDEFFECT_HPP
