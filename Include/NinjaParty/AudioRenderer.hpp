#ifndef NINJAPARTY_AUDIORENDERER_HPP
#define NINJAPARTY_AUDIORENDERER_HPP

#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include <thread>
#include <stdint.h>

#include <NinjaParty/IncludeAL.h>

// todo: Should this be changed to work on the main thread. If we provide an Update function
// it can be called once per frame, and we could remove all the thread stuff in here.
// Test to figure out which is better.

namespace NinjaParty
{
	enum class AudioEventType
	{
		Complete,
		Looped,
		Error
	};
	
	typedef uint32_t AudioReference;
	typedef std::function<void (const AudioReference&, AudioEventType)> AudioCallback;
	
	class Sound;
	
	enum class AudioNodeState
	{
		Empty,
		Playing,
		InterruptedPlaying,
		Paused,
		StreamEnding
	};
	
	enum class AudioFadeState
	{
		None,
		FadeIn,
		FadeOut,
	};
	
	struct AudioNode
	{
		Sound *sound;
		AudioNodeState state;
		ALuint audioSource;
		int playsRemaining;
		float volume;
		
		float fadeTime;
		float fadeDuration;
		AudioFadeState fadeState;
		
		AudioReference audioReference;
		AudioCallback audioCallback;
	};

	class AudioRenderer
	{
	public:
		AudioRenderer(int maxAudioSources = 32);
		~AudioRenderer();

		AudioReference Play(Sound *sound, int loopCount, float volume, float fadeInTime = 0, AudioCallback callback = nullptr);
		
		// if currently fading in, the fadeOutTime will be adjusted down so that
		// we start at the current faded volume and fade to zero
		void Stop(const AudioReference &audioReference, float fadeOutTime = 0);

		void Pause(const AudioReference &audioReference);
		void Resume(const AudioReference &audioReference);
		
		void BeginAudioInterruption();
		void EndAudioInterruption();

	private:
		friend void* ThreadHelperFunction(void*);
		
		void UpdateThreadFunction();
		void UpdateAudioNode(AudioNode &audioNode, float deltaSeconds);

		std::thread updateThread;
		std::mutex updateMutex;
		
		bool exitThread;

		int maxAudioSources;

		ALCdevice *audioDevice;
		ALCcontext *audioContext;
		
		bool interrupted;

		AudioReference currentAudioReference;

		std::unique_ptr<AudioNode[]> audioNodes;
	};
}

#endif//NINJAPARTY_AUDIORENDERER_HPP
