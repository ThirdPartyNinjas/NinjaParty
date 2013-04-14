#ifndef NINJAPARTY_WAVE_HPP
#define NINJAPARTY_WAVE_HPP

#include <string>
#include <stdint.h>
#include <boost/scoped_array.hpp>

namespace NinjaParty
{
	class Wav
	{
	public:
		Wav(const std::string &fileName, int offset = 0, int length = 0);
		~Wav();
		
		int16_t* GetSamples() const { return &samples[0]; }
		int GetSampleCount() const { return sampleCount; }
		int GetSampleRate() const { return sampleRate; }
		int GetChannels() const { return channels; }
		
	private:
		boost::scoped_array<int16_t> samples;
		int sampleCount;
		int channels;
		int sampleRate;
	};
}

#endif//NINJAPARTY_WAVE_HPP
