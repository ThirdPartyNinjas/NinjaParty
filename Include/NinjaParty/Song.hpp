#ifndef NINJAPARTY_SONG_HPP
#define NINJAPARTY_SONG_HPP

#include <memory>
#include <string>

#include <boost/scoped_array.hpp>

#include <NinjaParty/Sound.hpp>

namespace NinjaParty
{
	class Vorbis;

	class Song : public Sound
	{
	public:
		Song(const std::string &fileName, int offset = 0, int length = 0);
		~Song();

		bool GetAudioBuffer(ALuint &audioBuffer);
		void ReturnAudioBuffer(ALuint audioBuffer);
		
		int GetBufferCount() { return bufferCount; }
		
		void Stop();
		void Reset();

	private:
		int bufferSize;
		int bufferCount;
		
		struct SongSection
		{
			bool empty;
			ALuint bufferId;
		};
		
        std::unique_ptr<int16_t[]> scratchBuffer;
		std::unique_ptr<Vorbis> vorbis;
		boost::scoped_array<SongSection> sections;
	};
}

#endif//NINJAPARTY_SONG_HPP
