#ifndef NINJAPARTY_INCLUDEGL_H
#define NINJAPARTY_INCLUDEGL_H

#if defined _WIN32 || defined _WIN64
    #include <GL3/gl3w.h>
#elif defined __APPLE__
    #include <TargetConditionals.h>

    #if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
        #include <OpenGLES/ES2/gl.h>
        #include <OpenGLES/ES2/glext.h>
    #elif TARGET_OS_MAC
        #include <OpenGL/gl.h>
    #else
        #error Unknown Apple target
    #endif
#elif __ANDROID__
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
#else
    #error Unknown target
#endif

#endif//NINJAPARTY_INCLUDEGL_H
