#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdexcept>

#include <wav_reader.h>
#include "Wav.hpp"

namespace NinjaParty
{
	Wav::Wav(const std::string &fileName)
	{
		FILE *in;
		char chunk_name[5];
		fmt_chunk_t fmt_chunk;
		
		in = fopen(fileName.c_str(), "rb");
		if(in == nullptr)
		{
			throw std::runtime_error(std::string("Wav loading failed: ") + fileName);
		}
		
		while(1)
		{
			if(read_chars(in, chunk_name, 4) != 0)
				break;
			
			if(strcmp(chunk_name, "RIFF") == 0)
			{
				parse_header(in);
			}
			else if(strcmp(chunk_name, "fmt ") == 0)
			{
				parse_fmt(in, &fmt_chunk);
			}
			else if(strcmp(chunk_name, "data") == 0)
			{ 
				parse_data(in, &fmt_chunk, nullptr, &sampleCount);
				samples.reset(new int16_t[sampleCount]);
				parse_data(in, &fmt_chunk, samples.get(), &sampleCount);
			}
			else
			{
				skip_chunk(in);
				//fclose(in);
				//throw std::runtime_error(std::string("Wav loading failed: ") + fileName);
			}
		}
		
		channels = fmt_chunk.channel_numbers;
		sampleRate = fmt_chunk.sample_rate;
		
		fclose(in);
	}
	
	Wav::~Wav()
	{
	}
}

