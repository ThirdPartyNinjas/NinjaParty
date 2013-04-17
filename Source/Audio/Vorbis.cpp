#include <stdexcept>
#include <stdio.h>

#include "Vorbis.hpp"

namespace NinjaParty
{
//	Vorbis::Vorbis(const std::string &fileName, int offset, int length)
//	{
//		int error;
//		
//		if(offset != 0)
//		{
//			FILE *file = fopen(fileName.c_str(), "rb");
//			fseek(file, offset, SEEK_SET);
//			int error;
//			stbVorbisData = stb_vorbis_open_file_section(file, 1, &error, nullptr, length);
//		}
//		else
//		{
//			stbVorbisData = stb_vorbis_open_filename(const_cast<char*>(fileName.c_str()), &error, nullptr);
//		}
//		if(!stbVorbisData)
//			throw std::runtime_error(std::string("Failed to open Vorbis file: ") + fileName);
//		
//		stb_vorbis_info info = stb_vorbis_get_info(stbVorbisData);
//		channels = info.channels;
//		sampleRate = info.sample_rate;
//        duration = stb_vorbis_stream_length_in_seconds(stbVorbisData);
//	}

    Vorbis::Vorbis()
    {
        stbVorbisData = nullptr;
    }
    
	Vorbis::~Vorbis()
	{
        if(stbVorbisData != nullptr)
            stb_vorbis_close(stbVorbisData);
	}
    
    Vorbis* Vorbis::FromFile(const std::string &fileName)
    {
        int error;
        
        stb_vorbis *stbVorbisData = stb_vorbis_open_filename(const_cast<char*>(fileName.c_str()), &error, nullptr);
        stb_vorbis_info info = stb_vorbis_get_info(stbVorbisData);

        Vorbis *vorbis = new Vorbis();
        vorbis->stbVorbisData = stbVorbisData;
        vorbis->channels = info.channels;
        vorbis->sampleRate = info.sample_rate;
        vorbis->duration = stb_vorbis_stream_length_in_seconds(stbVorbisData);
        
        return vorbis;
    }
	
    Vorbis* Vorbis::FromBuffer(unsigned char *buffer, int length)
    {
        int error;
        
        stb_vorbis *stbVorbisData = stb_vorbis_open_memory(buffer, length, &error, nullptr);
        stb_vorbis_info info = stb_vorbis_get_info(stbVorbisData);
        
        Vorbis *vorbis = new Vorbis();
        vorbis->stbVorbisData = stbVorbisData;
        vorbis->channels = info.channels;
        vorbis->sampleRate = info.sample_rate;
        vorbis->duration = stb_vorbis_stream_length_in_seconds(stbVorbisData);
        
        return vorbis;
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
