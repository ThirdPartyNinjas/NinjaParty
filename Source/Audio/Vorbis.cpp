#include <stdexcept>

#include "Vorbis.hpp"

namespace NinjaParty
{
	Vorbis::Vorbis(const std::string &fileName)
	{
		int error;
		
		stbVorbisData = stb_vorbis_open_filename(const_cast<char*>(fileName.c_str()), &error, nullptr);
		if(!stbVorbisData)
			throw std::runtime_error(std::string("Failed to open Vorbis file: ") + fileName);
		
		stb_vorbis_info info = stb_vorbis_get_info(stbVorbisData);
		channels = info.channels;
		sampleRate = info.sample_rate;
        duration = stb_vorbis_stream_length_in_seconds(stbVorbisData);
	}
	
	Vorbis::~Vorbis()
	{
		stb_vorbis_close(stbVorbisData);
	}
	
	int Vorbis::ReadSamples(int16_t *samples, int count)
	{
		return stb_vorbis_get_samples_short_interleaved(stbVorbisData, channels, samples, count);
	}
	
	void Vorbis::Reset()
	{
		stb_vorbis_seek_start(stbVorbisData);
	}
}
