#ifndef NINJAPARTY_INCLUDEAL_H
#define NINJAPARTY_INCLUDEAL_H

#if defined _WIN32 || defined _WIN64
    #error
#elif defined __APPLE__
	#include <OpenAl/al.h>
	#include <OpenAl/alc.h>
#elif __ANDROID__
	#include <AL/al.h>
	#include <AL/alc.h>
#else
    #error Unknown target
#endif

#endif//NINJAPARTY_INCLUDEAL_H
