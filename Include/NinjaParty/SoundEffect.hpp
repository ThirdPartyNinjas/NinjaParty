#ifndef NINJAPARTY_SOUNDEFFECT_HPP
#define NINJAPARTY_SOUNDEFFECT_HPP

#include <memory>
#include <string>

#include <boost/scoped_array.hpp>

#include "Sound.hpp"

namespace NinjaParty
{
	class SoundEffect : public Sound
	{
	public:
//		SoundEffect(const std::string &fileName, int offset = 0, int length = 0);
//		SoundEffect(int channels, int sampleRate, int16_t *samples, int sampleCount);
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
