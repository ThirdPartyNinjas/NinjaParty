#ifndef NINJAPARTY_VORBIS_HPP
#define NINJAPARTY_VORBIS_HPP

#include <string>
#include <stdint.h>

#include <stb_vorbis.h>

namespace NinjaParty
{
	class Vorbis
	{
	public:
		Vorbis(const std::string &fileName);
		~Vorbis();
		
		int ReadSamples(int16_t *samples, int count);
		
		void Reset();
		
		int GetChannels() const { return channels; }
		int GetSampleRate() const { return sampleRate; }
        float GetDuration() const { return duration; }
		
	private:
		stb_vorbis *stbVorbisData;
		
		int channels;
		int sampleRate;
        float duration;
	};
}

#endif//NINJAPARTY_VORBIS_HPP
