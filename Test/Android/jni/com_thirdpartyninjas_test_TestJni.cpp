#include <string>

#include <android/log.h>
#include <jni.h>

#include <NinjaParty/AssetManager.hpp>
#include <NinjaParty/SpriteBatch.hpp>
#include <NinjaParty/Texture.hpp>

#include "com_thirdpartyninjas_test_TestJni.h"

#define  LOG_TAG "NinjaPartyTest"
#define  LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#define TEST_SPINE

#ifdef TEST_SPRITEBATCH
#include "../../Tests/SpriteBatch.hpp"
#elif defined(TEST_SPINE)
#include "../../Tests/Spine.hpp"
#elif defined(TEST_DEMINA)
#include "../../Tests/Demina.hpp"
#elif defined(TEST_RENDERTEXTURE)
#include "../../Tests/RenderTexture.hpp"
#elif defined(TEST_AUDIO)
#include "../../Tests/Audio.hpp"
#elif defined(TEST_GLEED)
#include "../../Tests/Gleed.hpp"
#elif defined(TEST_TOUCH)
#include "../../Tests/Touch.hpp"
#elif defined(TEST_ASSETS)
#include "../../Tests/Assets.hpp"
#endif

std::string g_apkPath = "";
Tests::TestGame *game = nullptr;

JNIEXPORT void JNICALL Java_com_thirdpartyninjas_test_TestJni_init(JNIEnv *env, jclass, jint width, jint height, jstring javaApkPath)
{
	jboolean isCopy;
	const char *nativeApkPath = env->GetStringUTFChars(javaApkPath, &isCopy);
	g_apkPath = nativeApkPath;
	env->ReleaseStringUTFChars(javaApkPath, nativeApkPath);

	glEnable(GL_BLEND);
	glViewport(0, 0, width, height);

	try
	{
		if(game == nullptr)
		{
			game = new Tests::TestGame(width, height);
			game->LoadContent("assets/", g_apkPath);
		}
		else
		{
			game->HandleLostGraphicsContext();
		}
	}
	catch(std::exception &exception)
	{
		LOGE("Exception caught: %s", exception.what());
	}
}

JNIEXPORT void JNICALL Java_com_thirdpartyninjas_test_TestJni_update(JNIEnv *, jclass, jfloat deltaSeconds)
{
	if(game)
		game->Update(deltaSeconds);
}

JNIEXPORT void JNICALL Java_com_thirdpartyninjas_test_TestJni_draw(JNIEnv *, jclass)
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	if(game)
		game->Draw();
}

JNIEXPORT void JNICALL Java_com_thirdpartyninjas_test_TestJni_pause(JNIEnv *, jclass)
{
	if(game)
		game->Pause();
}

JNIEXPORT void JNICALL Java_com_thirdpartyninjas_test_TestJni_resume(JNIEnv *, jclass)
{
	if(game)
		game->Resume();
}

JNIEXPORT void JNICALL Java_com_thirdpartyninjas_test_TestJni_touchDown(JNIEnv *, jclass, jint id, jint x, jint y)
{
	game->TouchBegan((void*)id, 1, x, y);
}

JNIEXPORT void JNICALL Java_com_thirdpartyninjas_test_TestJni_touchUp(JNIEnv *, jclass, jint id, jint x, jint y)
{
	game->TouchEnded((void*)id, x, y);
}

JNIEXPORT void JNICALL Java_com_thirdpartyninjas_test_TestJni_touchMove(JNIEnv *, jclass, jint id, jint x, jint y)
{
	game->TouchMoved((void*)id, x, y);
}

JNIEXPORT void JNICALL Java_com_thirdpartyninjas_test_TestJni_touchCancel(JNIEnv *, jclass, jint id)
{
	game->TouchCancelled((void*)id);
}
