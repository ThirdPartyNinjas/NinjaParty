#include <stdexcept>

#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

#include <OpenAl/al.h>
#include <OpenAl/alc.h>

#include <Storyteller/AudioCapturer.hpp>

namespace Storyteller
{
	struct CaptureThreadData
	{
		ALCdevice *captureDevice;
		boost::function<void (int16_t*, int)> callback;
		bool exitThread;
	};

	struct AudioCapturer::impl
	{
		ALCdevice *device;
		ALCcontext *context;
		boost::thread updateThread;

		CaptureThreadData threadData;

		impl()
			: device(NULL), context(NULL)
		{
			threadData.captureDevice = NULL;
			threadData.exitThread = false;
		}
	};

	static void CaptureThreadProc(CaptureThreadData &data)
	{
		const int maxCaptureBuffer = 9600;
		int16_t sampleBuffer[maxCaptureBuffer];

		while(!data.exitThread)
		{
			boost::this_thread::sleep(boost::posix_time::milliseconds(100));

			ALint samplesAvailable;
			alcGetIntegerv(data.captureDevice, ALC_CAPTURE_SAMPLES, 1, &samplesAvailable);
			alcCaptureSamples(data.captureDevice, sampleBuffer, (samplesAvailable > maxCaptureBuffer) ? maxCaptureBuffer : samplesAvailable);
			data.callback(sampleBuffer, (samplesAvailable > maxCaptureBuffer) ? maxCaptureBuffer : samplesAvailable);
		}
	}

	AudioCapturer::AudioCapturer(int channels, int sampleRate, boost::function<void (int16_t*, int)> callback)
		: channels(channels), sampleRate(sampleRate), callback(callback), pimpl(new impl)
	{
		pimpl->context = alcGetCurrentContext();
		pimpl->device = alcGetContextsDevice(pimpl->context);

		if(alcIsExtensionPresent(pimpl->device, "ALC_EXT_CAPTURE") == AL_FALSE)
		{
			throw std::runtime_error("Device doesn't support capture");
		}

		const char *defaultDevice = alcGetString(NULL, ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER);

		pimpl->threadData.captureDevice = alcCaptureOpenDevice(defaultDevice, sampleRate, channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16, sampleRate / 2);
		if(!pimpl->threadData.captureDevice)
		{
			throw std::runtime_error("Failed to open capture device");
		}

		pimpl->threadData.callback = callback;

		alcCaptureStart(pimpl->threadData.captureDevice);
		pimpl->updateThread = boost::thread(boost::bind(&CaptureThreadProc, boost::ref(pimpl->threadData)));
	}
	
	AudioCapturer::~AudioCapturer()
	{
		pimpl->threadData.exitThread = true;
		pimpl->updateThread.join();

		alcCaptureStop(pimpl->threadData.captureDevice);
		alcCaptureCloseDevice(pimpl->device);
	}
}
