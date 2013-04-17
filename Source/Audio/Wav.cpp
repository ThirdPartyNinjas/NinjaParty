#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <stdexcept>

#include "Wav.hpp"

namespace NinjaParty
{
    Wav::Wav()
    {
    }
    
	Wav::~Wav()
	{
	}
    
    static int8_t read_int8(FILE *file)
    {
        int i = getc(file);
        if(i == EOF)
            throw std::runtime_error("Wav read failed: Unexpected end of file found");
        return static_cast<int8_t>(i);
    }
    
    static void read_int8_array(FILE *file, int8_t *buffer, int count)
    {
        for(int i=0; i<count; i++)
            buffer[i] = read_int8(file);
    }

    static uint8_t read_uint8(FILE *file)
    {
        int i = getc(file);
        if(i == EOF)
            throw std::runtime_error("Wav read failed: Unexpected end of file found");
        return static_cast<uint8_t>(i);
    }

    static int16_t read_int16(FILE *file)
    {
        int16_t i = read_int8(file);
        i += static_cast<int16_t>(read_int8(file)) << 8;
        return i;
    }
    
    static uint16_t read_uint16(FILE *file)
    {
        uint16_t i = read_uint8(file);
        i += static_cast<uint16_t>(read_uint8(file)) << 8;
        return i;
    }

    static uint32_t read_uint32(FILE *file)
    {
        uint32_t i = read_uint8(file);
        i += static_cast<uint32_t>(read_uint8(file)) << 8;
        i += static_cast<uint32_t>(read_uint8(file)) << 16;
        i += static_cast<uint32_t>(read_uint8(file)) << 24;
        return i;
    }

    static int8_t read_int8(unsigned char **position, unsigned char *end)
    {
        if(*position == end)
            throw std::runtime_error("Wav read failed: Unexpected end of file found");
        int8_t i = static_cast<int8_t>(**position);
        (*position)++;
        return i;
    }
    
    static void read_int8_array(unsigned char **position, unsigned char *end, int8_t *buffer, int count)
    {
        for(int i=0; i<count; i++)
            buffer[i] = read_int8(position, end);
    }
    
    static uint8_t read_uint8(unsigned char **position, unsigned char *end)
    {
        if(*position == end)
            throw std::runtime_error("Wav read failed: Unexpected end of file found");
        uint8_t i = static_cast<uint8_t>(**position);
        (*position)++;
        return i;
    }
    
    static int16_t read_int16(unsigned char **position, unsigned char *end)
    {
        int16_t i = read_int8(position, end);
        i += static_cast<int16_t>(read_int8(position, end)) << 8;
        return i;
    }
    
    static uint16_t read_uint16(unsigned char **position, unsigned char *end)
    {
        uint16_t i = read_uint8(position, end);
        i += static_cast<uint16_t>(read_uint8(position, end)) << 8;
        return i;
    }
    
    static uint32_t read_uint32(unsigned char **position, unsigned char *end)
    {
        uint32_t i = read_uint8(position, end);
        i += static_cast<uint32_t>(read_uint8(position, end)) << 8;
        i += static_cast<uint32_t>(read_uint8(position, end)) << 16;
        i += static_cast<uint32_t>(read_uint8(position, end)) << 24;
        return i;
    }
    
    struct fmt_chunk_t
    {
        unsigned short int format_type;
        unsigned short int channel_numbers;
        unsigned int sample_rate;
        unsigned int bytes_per_second;
        unsigned short int bytes_per_sample;
        unsigned short int bits_per_sample;
    };
    
    Wav* Wav::FromFile(const std::string &fileName)
    {
        FILE *file = fopen(fileName.c_str(), "rb");
        if(file == nullptr)
            throw std::runtime_error(std::string("Failed to open wav file: ") + fileName);
        
        fseek(file, 0, SEEK_END);
        long fileLength = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        fmt_chunk_t fmtChunk;
        bool fmtChunkValid = false;
        std::unique_ptr<int16_t[]> samples;
        uint32_t sampleCount = 0;
        
        do
        {
            char chunkName[5];
            read_int8_array(file, (int8_t*)chunkName, 4);
            chunkName[4] = '\0';
            
            if(strcmp(chunkName, "RIFF") == 0)
            {
                uint32_t length = read_uint32(file);
                char riffType[5];
                read_int8_array(file, (int8_t*)riffType, 4);
                riffType[4] = '\0';
                
                if(length != fileLength - 8 || strcmp(riffType, "WAVE") != 0)
                    throw std::runtime_error(std::string("Parse error in wav file: ") + fileName);
            }
            else if(strcmp(chunkName, "fmt ") == 0)
            {
                uint32_t length = read_uint32(file);
                long nextPosition = ftell(file) + length;

                fmtChunk.format_type = read_uint16(file);
                fmtChunk.channel_numbers = read_uint16(file);
                fmtChunk.sample_rate = read_uint32(file);
                fmtChunk.bytes_per_second = read_uint32(file);
                fmtChunk.bytes_per_sample = read_uint16(file);
                fmtChunk.bits_per_sample = read_uint16(file);
                fmtChunkValid = true;
                fseek(file, nextPosition, SEEK_SET);
            }
            else if(strcmp(chunkName, "data") == 0)
            {
                if(fmtChunkValid == false)
                    throw std::runtime_error(std::string("Parse error in wav file: ") + fileName);
                
                uint32_t length = read_uint32(file);
                samples.reset(new int16_t[length / 2]);
                int16_t *position = samples.get();
                
                if(fmtChunk.bits_per_sample == 16)
                {
                    sampleCount = length / 2 / fmtChunk.channel_numbers;
                    
                    for(int i=0; i<length / 2; i++)
                    {
                        *position++ = read_int16(file);
                    }
                }
                else if(fmtChunk.bits_per_sample == 8)
                {
                    sampleCount = length / fmtChunk.channel_numbers;

                    for(int i=0; i<length; i++)
                    {
                        *position++ = static_cast<int16_t>(read_int8(file)) << 8;
                    }
                }
                
				break;
            }
            else
            {
                uint32_t length = read_uint32(file);
                fseek(file, length, SEEK_CUR);
            }
        } while(true);
        
        Wav *wav = new Wav();
        wav->channels = fmtChunk.channel_numbers;
        wav->sampleCount = sampleCount;
        wav->sampleRate = fmtChunk.sample_rate;
        wav->samples = std::move(samples);
        return wav;
    }

    Wav* Wav::FromBuffer(unsigned char *buffer, int bufferLength)
    {
        unsigned char *position = buffer;
        unsigned char *end = buffer + bufferLength;
        
        fmt_chunk_t fmtChunk;
        bool fmtChunkValid = false;
        std::unique_ptr<int16_t[]> samples;
        uint32_t sampleCount = 0;
        
        do
        {
            char chunkName[5];
            read_int8_array(&position, end, (int8_t*)chunkName, 4);
            chunkName[4] = '\0';
            
            if(strcmp(chunkName, "RIFF") == 0)
            {
                uint32_t length = read_uint32(&position, end);
                char riffType[5];
                read_int8_array(&position, end, (int8_t*)riffType, 4);
                riffType[4] = '\0';
                
                if(length != bufferLength - 8 || strcmp(riffType, "WAVE") != 0)
                    throw std::runtime_error("Wav buffer parse error");
            }
            else if(strcmp(chunkName, "fmt ") == 0)
            {
                uint32_t length = read_uint32(&position, end);
                unsigned char *nextPosition = position + length;
                
                fmtChunk.format_type = read_uint16(&position, end);
                fmtChunk.channel_numbers = read_uint16(&position, end);
                fmtChunk.sample_rate = read_uint32(&position, end);
                fmtChunk.bytes_per_second = read_uint32(&position, end);
                fmtChunk.bytes_per_sample = read_uint16(&position, end);
                fmtChunk.bits_per_sample = read_uint16(&position, end);
                fmtChunkValid = true;
                
                position = nextPosition;
            }
            else if(strcmp(chunkName, "data") == 0)
            {
                if(fmtChunkValid == false)
                    throw std::runtime_error("Wav buffer parse error");
                
                uint32_t length = read_uint32(&position, end);
                samples.reset(new int16_t[length / 2]);
                int16_t *p = samples.get();
                
                if(fmtChunk.bits_per_sample == 16)
                {
                    sampleCount = length / 2 / fmtChunk.channel_numbers;
                    
                    for(int i=0; i<length / 2; i++)
                    {
                        *p++ = read_int16(&position, end);
                    }
                }
                else if(fmtChunk.bits_per_sample == 8)
                {
                    sampleCount = length / fmtChunk.channel_numbers;
                    
                    for(int i=0; i<length; i++)
                    {
                        *p++ = static_cast<int16_t>(read_int8(&position, end)) << 8;
                    }
                }
                
				break;
            }
            else
            {
                uint32_t length = read_uint32(&position, end);
                position += length;
            }
        } while(true);
        
        Wav *wav = new Wav();
        wav->channels = fmtChunk.channel_numbers;
        wav->sampleCount = sampleCount;
        wav->sampleRate = fmtChunk.sample_rate;
        wav->samples = std::move(samples);
        return wav;
    }
}

