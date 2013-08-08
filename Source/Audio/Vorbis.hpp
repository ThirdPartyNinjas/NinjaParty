#ifndef NINJAPARTY_VORBIS_HPP
#define NINJAPARTY_VORBIS_HPP

#include <string>
#include <stdint.h>

struct stb_vorbis;

namespace NinjaParty
{
	class Vorbis
	{
	public:
		~Vorbis();
		
		int ReadSamples(int16_t *samples, int count);
		
		void Reset();
		
		int GetChannels() const { return channels; }
		int GetSampleRate() const { return sampleRate; }
        float GetDuration() const { return duration; }
		
        static Vorbis* FromFile(const std::string &fileName);
        static Vorbis* FromBuffer(unsigned char *buffer, int length);
        
	private:
        Vorbis();
        
		stb_vorbis *stbVorbisData;
		
		int channels;
		int sampleRate;
        float duration;
	};
}

#endif//NINJAPARTY_VORBIS_HPP
