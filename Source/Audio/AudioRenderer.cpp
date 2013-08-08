#include <stdexcept>
#include <assert.h>

#include <functional>

#include <NinjaParty/AudioRenderer.hpp>
#include <NinjaParty/Sound.hpp>

namespace NinjaParty
{
	void* ThreadHelperFunction(void *param)
	{
		AudioRenderer *audioRenderer = static_cast<AudioRenderer*>(param);
		audioRenderer->UpdateThreadFunction();
		return nullptr;
	}
	
	AudioRenderer::AudioRenderer(int maxAudioSources)
	{
		assert(maxAudioSources > 0);
		
		this->maxAudioSources = maxAudioSources;
		
		audioDevice = alcOpenDevice(nullptr);
		if(!audioDevice)
			throw std::runtime_error("Failed to open Audio device");
		
		audioContext = alcCreateContext(audioDevice, nullptr);
		// todo: check for openal error
		
		alcMakeContextCurrent(audioContext);
		// todo: check for openal error
		
		alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);
		
		audioNodes.reset(new AudioNode[maxAudioSources]);
		for(int i=0; i<maxAudioSources; i++)
		{
			audioNodes[i].sound = nullptr;
			audioNodes[i].state = AudioNodeState::Empty;
			
			alGenSources(1, &audioNodes[i].audioSource);
			// todo: check for openal error
			
			alSourcef(audioNodes[i].audioSource, AL_REFERENCE_DISTANCE, 25.0f);
			alSourcef(audioNodes[i].audioSource, AL_MAX_DISTANCE, 150.0f);
			alSourcef(audioNodes[i].audioSource, AL_ROLLOFF_FACTOR, 6.0f);
		}
		
		currentAudioReference = 1;
		
		interrupted = false;
		exitThread = false;
		updateThread = std::thread(std::bind(&AudioRenderer::UpdateThreadFunction, this));
	}
	
	AudioRenderer::~AudioRenderer()
	{
		exitThread = true;
		updateThread.join();
		
		for(int i=0; i<maxAudioSources; i++)
		{
			alSourceStop(audioNodes[i].audioSource);
			alDeleteSources(1, &audioNodes[i].audioSource);
		}
		
		alcDestroyContext(audioContext);
		alcCloseDevice(audioDevice);
	}
	
	AudioReference AudioRenderer::Play(Sound *sound, int loopCount, float volume, float fadeInTime, AudioCallback audioCallback)
	{
		updateMutex.lock();
		
		sound->Reset();
		
		for(int i=0; i<maxAudioSources; i++)
		{
			if(audioNodes[i].state == AudioNodeState::Empty)
			{
				audioNodes[i].sound = sound;
				audioNodes[i].state = AudioNodeState::Playing;
				audioNodes[i].audioCallback = audioCallback;
				audioNodes[i].audioReference = ++currentAudioReference;
				audioNodes[i].playsRemaining = loopCount;
				audioNodes[i].fadeTime = 0;
				audioNodes[i].fadeDuration = fadeInTime;
				audioNodes[i].volume = volume;

				alSourcei(audioNodes[i].audioSource, AL_BUFFER, 0);

				if(sound->IsStreaming())
				{
					for(int j=0; j<sound->GetBufferCount(); j++)
					{
						ALuint bufferId;

						if(!sound->GetAudioBuffer(bufferId))
							break;
						alSourceQueueBuffers(audioNodes[i].audioSource, 1, &bufferId);
					}
				}
				else
				{
					ALuint bufferId;

					sound->GetAudioBuffer(bufferId);
					alSourcei(audioNodes[i].audioSource, AL_BUFFER, bufferId);
				}
				
				if(fadeInTime > 0)
				{
					audioNodes[i].fadeState = AudioFadeState::FadeIn;
					alSourcef(audioNodes[i].audioSource, AL_GAIN, 0);
				}
				else
				{
					audioNodes[i].fadeState = AudioFadeState::None;
					alSourcef(audioNodes[i].audioSource, AL_GAIN, volume);
				}
				
				alSourcePlay(audioNodes[i].audioSource);
				break;
			}
		}

		updateMutex.unlock();
		
		return currentAudioReference;
	}
	
	void AudioRenderer::Stop(const AudioReference &audioReference, float fadeOutTime)
	{
		updateMutex.lock();
		
		for(int i=0; i<maxAudioSources; i++)
		{
			if(audioNodes[i].audioReference == audioReference)
			{
				if(audioNodes[i].state != AudioNodeState::Empty)
				{
					if(fadeOutTime == 0)
					{
						alSourceStop(audioNodes[i].audioSource);
						audioNodes[i].sound->Stop();
						audioNodes[i].state = AudioNodeState::Empty;
						audioNodes[i].sound = nullptr;
						
						if(audioNodes[i].audioCallback != nullptr)
						{
							audioNodes[i].audioCallback(audioNodes[i].audioReference, AudioEventType::Complete);
						}
					}
					else if(audioNodes[i].fadeState != AudioFadeState::FadeOut)
					{
						if(audioNodes[i].fadeState == AudioFadeState::FadeIn)
						{
							float currentPosition = audioNodes[i].fadeTime / audioNodes[i].fadeDuration;
							audioNodes[i].fadeTime = fadeOutTime * (1 - currentPosition);
						}
						else
						{
							audioNodes[i].fadeTime = 0;
						}
						
						audioNodes[i].fadeState = AudioFadeState::FadeOut;
						audioNodes[i].fadeDuration = fadeOutTime;
					}
				}
				break;
			}
		}
		
		updateMutex.unlock();
	}

	void AudioRenderer::Pause(const AudioReference &audioReference)
	{
		updateMutex.lock();
		
		for(int i=0; i<maxAudioSources; i++)
		{
			if(audioNodes[i].audioReference == audioReference)
			{
				if(audioNodes[i].state == AudioNodeState::Playing)
				{
					alSourcePause(audioNodes[i].audioSource);
					audioNodes[i].state = AudioNodeState::Paused;
				}
				break;
			}
		}
		
		updateMutex.unlock();
	}
	
	void AudioRenderer::Resume(const AudioReference &audioReference)
	{
		updateMutex.lock();
		
		for(int i=0; i<maxAudioSources; i++)
		{
			if(audioNodes[i].audioReference == audioReference)
			{
				if(audioNodes[i].state == AudioNodeState::Paused)
				{
					alSourcePlay(audioNodes[i].audioSource);
					audioNodes[i].state = AudioNodeState::Playing;
				}
				break;
			}
		}
		
		updateMutex.unlock();
	}

	void AudioRenderer::BeginAudioInterruption()
	{
		updateMutex.lock();

		interrupted = true;
		
		for(int i=0; i<maxAudioSources; i++)
		{
			AudioNode &audioNode = audioNodes[i];
			
			if(audioNode.state == AudioNodeState::Playing)
			{
				alSourcePause(audioNode.audioSource);
				audioNode.state = AudioNodeState::InterruptedPlaying;
			}
		}
		
		alcMakeContextCurrent(nullptr);
		alcSuspendContext(audioContext);

		updateMutex.unlock();
	}
	
	void AudioRenderer::EndAudioInterruption()
	{
		updateMutex.lock();

		interrupted = false;
		alcMakeContextCurrent(audioContext);
		alcProcessContext(audioContext);
		
		for(int i=0; i<maxAudioSources; i++)
		{
			AudioNode &audioNode = audioNodes[i];
			
			if(audioNode.state == AudioNodeState::InterruptedPlaying)
			{
				alSourcePlay(audioNode.audioSource);
				audioNode.state = AudioNodeState::Playing;
			}
		}

		updateMutex.unlock();
	}
	
	void AudioRenderer::UpdateThreadFunction()
	{
		auto lastTime = std::chrono::high_resolution_clock::now();
		
		while(!exitThread)
		{
			auto currentTime = std::chrono::high_resolution_clock::now();
			float deltaSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count() / 1000.0f;
			lastTime = currentTime;

			if(!interrupted)
			{
				updateMutex.lock();

				for(int i=0; i<maxAudioSources; i++)
				{
					if(audioNodes[i].state != AudioNodeState::Empty)
					{
						UpdateAudioNode(audioNodes[i], deltaSeconds);
					}
				}
				updateMutex.unlock();
			}

			// Note: This sleep time might need to be modified
			std::this_thread::sleep_for(std::chrono::milliseconds(16));
		}
	}

	void AudioRenderer::UpdateAudioNode(AudioNode &audioNode, float deltaSeconds)
	{
		if(audioNode.sound->IsStreaming())
		{
			if(audioNode.state == AudioNodeState::Paused)
			{
			}
			else if(audioNode.state == AudioNodeState::Playing)
			{
				// perform a sanity test and verify that we're still playing
				// it's possible that something stole our clock cycles and we didn't
				// refill the buffers in time, and it stopped
				ALint sourceState;
				alGetSourcei(audioNode.audioSource, AL_SOURCE_STATE, &sourceState);
				
				if(sourceState != AL_PLAYING)
				{
					alSourcePlay(audioNode.audioSource);
				}
				
				ALint processed;
				alGetSourcei(audioNode.audioSource, AL_BUFFERS_PROCESSED, &processed);
					
				while(processed-- > 0)
				{
					ALuint freeBuffer;
					alSourceUnqueueBuffers(audioNode.audioSource, 1, &freeBuffer);
					audioNode.sound->ReturnAudioBuffer(freeBuffer);
					
					if(audioNode.sound->GetAudioBuffer(freeBuffer))
					{
						alSourceQueueBuffers(audioNode.audioSource, 1, &freeBuffer);
					}
					else
					{
						if(audioNode.playsRemaining != 0)
						{
							if(audioNode.playsRemaining != -1)
								audioNode.playsRemaining--;
							audioNode.sound->Reset();
							audioNode.sound->GetAudioBuffer(freeBuffer);
							alSourceQueueBuffers(audioNode.audioSource, 1, &freeBuffer);
							
							if(audioNode.audioCallback != nullptr)
							{
								audioNode.audioCallback(audioNode.audioReference, AudioEventType::Looped);
							}
						}
						else
						{
							audioNode.state = AudioNodeState::StreamEnding;
						}
					}
				}
			}
			else if(audioNode.state == AudioNodeState::StreamEnding)
			{
				ALint sourceState;
				alGetSourcei(audioNode.audioSource, AL_SOURCE_STATE, &sourceState);
				
				if(sourceState != AL_PLAYING)
				{
					audioNode.state = AudioNodeState::Empty;
					audioNode.sound = nullptr;
					
					if(audioNode.audioCallback != nullptr)
					{
						audioNode.audioCallback(audioNode.audioReference, AudioEventType::Complete);
					}
				}
			}
			else
			{
				// unknown audio state - error
				audioNode.state = AudioNodeState::Empty;
				audioNode.sound = nullptr;

				alSourceStop(audioNode.audioSource);

				if(audioNode.audioCallback != nullptr)
				{
					audioNode.audioCallback(audioNode.audioReference, AudioEventType::Error);
				}
			}
		}
		else
		{
			ALint sourceState;
			alGetSourcei(audioNode.audioSource, AL_SOURCE_STATE, &sourceState);
			
			if(sourceState != AL_PLAYING && sourceState != AL_PAUSED)
			{
				if(audioNode.playsRemaining != 0)
				{
					if(audioNode.playsRemaining != -1)
						audioNode.playsRemaining--;
					alSourcePlay(audioNode.audioSource);
					
					if(audioNode.audioCallback != nullptr)
					{
						audioNode.audioCallback(audioNode.audioReference, AudioEventType::Looped);
					}
				}
				else
				{
					audioNode.state = AudioNodeState::Empty;
					audioNode.sound = nullptr;
					
					if(audioNode.audioCallback != nullptr)
					{
						audioNode.audioCallback(audioNode.audioReference, AudioEventType::Complete);
					}
				}
			}
		}
		
		if(audioNode.state != AudioNodeState::Empty)
		{
			if(audioNode.fadeState == AudioFadeState::FadeIn)
			{
				audioNode.fadeTime += deltaSeconds;
				if(audioNode.fadeTime > audioNode.fadeDuration)
				{
					audioNode.fadeState = AudioFadeState::None;
					alSourcef(audioNode.audioSource, AL_GAIN, audioNode.volume);
				}
				else
				{
					alSourcef(audioNode.audioSource, AL_GAIN, audioNode.volume * audioNode.fadeTime / audioNode.fadeDuration);
				}
			}
			else if(audioNode.fadeState == AudioFadeState::FadeOut)
			{
				audioNode.fadeTime += deltaSeconds;
				if(audioNode.fadeTime > audioNode.fadeDuration)
				{
					alSourceStop(audioNode.audioSource);
					audioNode.sound->Stop();
					audioNode.state = AudioNodeState::Empty;
					audioNode.sound = nullptr;
					
					if(audioNode.audioCallback != nullptr)
					{
						audioNode.audioCallback(audioNode.audioReference, AudioEventType::Complete);
					}
				}
				else
				{
					alSourcef(audioNode.audioSource, AL_GAIN, audioNode.volume * (1 - audioNode.fadeTime / audioNode.fadeDuration));
				}
			}
		}
	}
}
