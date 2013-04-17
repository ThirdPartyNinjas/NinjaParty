#ifndef NINJAPARTY_WAVE_HPP
#define NINJAPARTY_WAVE_HPP

#include <memory>
#include <string>
#include <stdint.h>

namespace NinjaParty
{
	class Wav
	{
	public:
		~Wav();
		
        // interleaved channels
		int16_t* GetSamples() const { return samples.get(); }

        // sample count (per channel)
		int GetSampleCount() const { return sampleCount; }
		int GetSampleRate() const { return sampleRate; }
		int GetChannels() const { return channels; }
		
        static Wav* FromFile(const std::string &fileName);
        static Wav* FromBuffer(unsigned char *buffer, int length);
        
	private:
        Wav();
        
		std::unique_ptr<int16_t[]> samples;
		int sampleCount;
		int channels;
		int sampleRate;
	};
}

#endif//NINJAPARTY_WAVE_HPP
