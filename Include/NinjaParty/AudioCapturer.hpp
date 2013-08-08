#ifndef NINJAPARTY_AUDIOCAPTURER_HPP
#define NINJAPARTY_AUDIOCAPTURER_HPP

#include <functional>
#include <memory>

namespace NinjaParty
{
	class AudioCapturer
	{
	public:
		AudioCapturer(int channels, int sampleRate, std::function<void (int16_t*, int)> callback);
		~AudioCapturer();
		
		int GetChannels() const { return channels; }
		int GetSampleRate() const { return sampleRate; }
		
		void CallbackRedirect(int16_t *samples, int count)
		{
			callback(samples, count);
		}
		
	protected:
		int channels;
		int sampleRate;
		std::function<void (int16_t*, int)> callback;
		
		struct impl;
		std::shared_ptr<impl> pimpl;
	};
}

#endif//NINJAPARTY_AUDIOCAPTURER_HPP
