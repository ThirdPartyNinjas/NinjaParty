#ifndef NINJAPARTY_SOUND_HPP
#define NINJAPARTY_SOUND_HPP

#include <stdint.h>

#include <NinjaParty/IncludeAL.h>

namespace NinjaParty
{
	class Sound
	{
	public:
		virtual ~Sound() { }

		// Get OpenAL buffer id
		virtual bool GetAudioBuffer(ALuint &audioBuffer) = 0;
		// Return the OpenAL buffer so it can be reused. (Only necessary with streaming audio.)
		virtual void ReturnAudioBuffer(ALuint audioBuffer) { }

		// Get the number of buffers that should be fed to OpenAL at startup (for streaming only)
		virtual int GetBufferCount() { return 1; }
		
		// Called when a song is stopped (or finishes fading) by the AudioRenderer
		virtual void Stop() { }
		
		// Reset to the beginning of the song (streaming)
		virtual void Reset() { }

		int GetChannels() const { return channels; }
		int GetSampleRate() const { return sampleRate; }
        
        float GetDuration() const { return duration; }

		bool IsStreaming() const { return isStreaming; }

	protected:
		bool isStreaming;
		
		int channels;
		int sampleRate;
        
        float duration;
	};
}

#endif//NINJAPARTY_SOUND_HPP
