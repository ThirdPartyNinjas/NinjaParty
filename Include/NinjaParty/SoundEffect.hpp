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
		SoundEffect(const std::string &fileName, int offset = 0, int length = 0);
		SoundEffect(int channels, int sampleRate, int16_t *samples, int sampleCount);
		~SoundEffect();
		
		bool GetAudioBuffer(ALuint &audioBuffer);
		
	private:
		ALuint audioBuffer;
	};
}

#endif//NINJAPARTY_SOUNDEFFECT_HPP
