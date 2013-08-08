#ifndef NINJAPARTY_SONG_HPP
#define NINJAPARTY_SONG_HPP

#include <memory>
#include <string>

#include <NinjaParty/Sound.hpp>

namespace NinjaParty
{
	class Vorbis;

	class Song : public Sound
	{
	public:
		~Song();

		bool GetAudioBuffer(ALuint &audioBuffer);
		void ReturnAudioBuffer(ALuint audioBuffer);
		
		int GetBufferCount() { return bufferCount; }
		
		void Stop();
		void Reset();
        
        static Song* FromFile(const std::string &fileName);
        static Song* FromBuffer(unsigned char *buffer, int length);

	private:
        Song(Vorbis *vorbis);
        
		int bufferSize;
		int bufferCount;
		
		struct SongSection
		{
			bool empty;
			ALuint bufferId;
		};
		
        std::unique_ptr<int16_t[]> scratchBuffer;
		std::unique_ptr<Vorbis> vorbis;
		std::unique_ptr<SongSection[]> sections;
	};
}

#endif//NINJAPARTY_SONG_HPP
