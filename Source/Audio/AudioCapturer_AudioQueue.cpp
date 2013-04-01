#include <stdexcept>
#include <AudioToolbox/AudioToolbox.h>

#include <NinjaParty/AudioCapturer.hpp>

namespace NinjaParty
{
	struct AudioCapturer::impl
	{
		AudioQueueRef audioQueue;
	};

	static void AudioInputCallback(void *inUserData, AudioQueueRef inAQ, AudioQueueBufferRef inBuffer,
								   const AudioTimeStamp *inStartTime, UInt32 inNumPackets, const AudioStreamPacketDescription *inPacketDesc);
	
	AudioCapturer::AudioCapturer(int channels, int sampleRate, std::function<void (int16_t*, int)> callback)
		: channels(channels), sampleRate(sampleRate), callback(callback), pimpl(new impl)
	{
		AudioStreamBasicDescription streamDescription;
		
		memset(&streamDescription, 0, sizeof(streamDescription));
		
		streamDescription.mSampleRate = sampleRate;
		streamDescription.mChannelsPerFrame = channels;
		streamDescription.mFormatID = kAudioFormatLinearPCM;
		streamDescription.mFormatFlags = kLinearPCMFormatFlagIsSignedInteger | kLinearPCMFormatFlagIsPacked;
		streamDescription.mBitsPerChannel = 16;
		streamDescription.mBytesPerPacket = streamDescription.mBytesPerFrame =
			(streamDescription.mBitsPerChannel / 8) * streamDescription.mChannelsPerFrame;
		streamDescription.mFramesPerPacket = 1;
		streamDescription.mReserved = 0;
		
		OSStatus result = AudioQueueNewInput(&streamDescription, &AudioInputCallback, this, nullptr, nullptr, 0, &pimpl->audioQueue);
		if(result != 0)
		{
			throw std::runtime_error("AudioCapturer failed to initialize. (AudioQueueNewInput failed.)");
		}

		for (int i = 0; i < 10; ++i)
		{
			AudioQueueBufferRef buffer;
			AudioQueueAllocateBuffer(pimpl->audioQueue, 2048, &buffer);
			AudioQueueEnqueueBuffer(pimpl->audioQueue, buffer, 0, nullptr);
		}
		
		result = AudioQueueStart(pimpl->audioQueue, nullptr);
		
	}
	
	AudioCapturer::~AudioCapturer()
	{
		AudioQueueStop(pimpl->audioQueue, true);
		AudioQueueDispose(pimpl->audioQueue, true);
	}

	static void AudioInputCallback(void *inUserData, AudioQueueRef inAQ, AudioQueueBufferRef inBuffer,
		const AudioTimeStamp *inStartTime, UInt32 inNumPackets, const AudioStreamPacketDescription *inPacketDesc)
	{
		AudioCapturer *ac = static_cast<AudioCapturer*>(inUserData);
		ac->CallbackRedirect(static_cast<int16_t*>(inBuffer->mAudioData), inBuffer->mAudioDataByteSize / sizeof(int16_t));
		
		AudioQueueEnqueueBuffer(inAQ, inBuffer, 0, nullptr);
	}
}
