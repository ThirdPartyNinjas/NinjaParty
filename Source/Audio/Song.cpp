#include <stdexcept>
#include <assert.h>

#include <NinjaParty/Song.hpp>

#include "Vorbis.hpp"

namespace NinjaParty
{
	Song::Song(const std::string &fileName, int offset, int length)
	{
		isStreaming = true;
		
		vorbis.reset(new Vorbis(fileName, offset, length));
		
		channels = vorbis->GetChannels();
		sampleRate = vorbis->GetSampleRate();
		
		this->bufferCount = 3;
		this->bufferSize = sampleRate * channels / 10;

		sections.reset(new SongSection[bufferCount]);		
		for(int i=0; i<bufferCount; i++)
		{
			alGenBuffers(1, &sections[i].bufferId);
			sections[i].empty = true;
			// todo: check for OpenAL errors
		}
        
        scratchBuffer.reset(new int16_t[bufferSize]);
        duration = vorbis->GetDuration();
	}
	
	Song::~Song()
	{
		for(int i=0; i<bufferCount; i++)
		{
			alDeleteBuffers(1, &sections[i].bufferId);
		}
	}
	
	bool Song::GetAudioBuffer(ALuint &audioBuffer)
	{
		for(int i=0; i<bufferCount; i++)
		{
			if(sections[i].empty)
			{
				int count = vorbis->ReadSamples(scratchBuffer.get(), bufferSize);
				if(count == 0)
				{
					vorbis->Reset();
					return false;
				}
				ALsizei format = (channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
				
				alBufferData(sections[i].bufferId, format, scratchBuffer.get(), count * sizeof(int16_t) * channels, sampleRate);
				sections[i].empty = false;
				audioBuffer = sections[i].bufferId;
				
				return true;
			}
		}
		
		throw std::runtime_error("Song error, no free buffers");
	}
	
	void Song::ReturnAudioBuffer(ALuint audioBufferId)
	{
		for(int i=0; i<bufferCount; i++)
		{
			if(sections[i].bufferId == audioBufferId)
			{
				sections[i].empty = true;
				break;
			}
		}
	}
	
	void Song::Stop()
	{
		for(int i=0; i<bufferCount; i++)
		{
			sections[i].empty = true;
		}
		
		Reset();
	}
	void Song::Reset()
	{
		vorbis->Reset();
	}
}
