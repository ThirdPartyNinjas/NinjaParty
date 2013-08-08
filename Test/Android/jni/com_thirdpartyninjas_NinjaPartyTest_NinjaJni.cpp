#include <string>
#include <vector>


#include <NinjaParty/AssetManager.hpp>
#include <NinjaParty/SpriteBatch.hpp>
#include <NinjaParty/Texture.hpp>

#include <jni.h>
#include <android/log.h>
#define  LOG_TAG "NinjaPartyTest"
#define  LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#include "com_thirdpartyninjas_NinjaPartyTest_NinjaJni.h"

#define TEST_FACEBOOK

#if defined(TEST_EMPTYGAME)
#include "../../Tests/EmptyGame.hpp"
#elif defined(TEST_SPRITEBATCH)
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
#elif defined(TEST_HTTPREQUEST)
#include "../../Tests/HttpRequest.hpp"
#elif defined(TEST_FACEBOOK)
#include "../../Tests/Facebook.hpp"
#endif

std::string g_apkPath = "";
JavaVM *g_javaVM = nullptr;
Tests::TestGame *game = nullptr;

extern "C"
{
	void FacebookLogin()
	{
		LOGI("C_FacebookLogin");

		JNIEnv *env = nullptr;
		jint envResult = g_javaVM->GetEnv((void**)&env, JNI_VERSION_1_6);
		if(envResult != JNI_OK)
			g_javaVM->AttachCurrentThread(&env, NULL);

		jclass clazz = env->FindClass("com/thirdpartyninjas/NinjaPartyTest/NinjaActivity");
		if(clazz == nullptr)
			throw;

		jmethodID methodId = env->GetStaticMethodID(clazz, "FacebookLogin", "()V");
		env->CallStaticVoidMethod(clazz, methodId);

		if(envResult != JNI_OK)
			g_javaVM->DetachCurrentThread();
	}

	void FacebookLogout()
	{
		LOGI("C_FacebookLogout");

		JNIEnv *env = nullptr;
		jint envResult = g_javaVM->GetEnv((void**)&env, JNI_VERSION_1_6);
		if(envResult != JNI_OK)
			g_javaVM->AttachCurrentThread(&env, NULL);

		jclass clazz = env->FindClass("com/thirdpartyninjas/NinjaPartyTest/NinjaActivity");
		if(clazz == nullptr)
			throw;

		jmethodID methodId = env->GetStaticMethodID(clazz, "FacebookLogout", "()V");
		env->CallStaticVoidMethod(clazz, methodId);

		if(envResult != JNI_OK)
			g_javaVM->DetachCurrentThread();
	}

	bool FacebookIsLoggedIn()
	{
		return false;
		
		LOGI("C_FacebookIsLoggedIn");

		JNIEnv *env = nullptr;
		jint envResult = g_javaVM->GetEnv((void**)&env, JNI_VERSION_1_6);
		if(envResult != JNI_OK)
			g_javaVM->AttachCurrentThread(&env, NULL);

		LOGI("0");

		jclass clazz = env->FindClass("com/thirdpartyninjas/NinjaPartyTest/NinjaActivity");
		if(clazz == nullptr)
		{
			LOGI("1");
			throw;
		}

		LOGI("1a");

		jmethodID methodId = env->GetStaticMethodID(clazz, "FacebookIsLoggedIn", "()Z");
		bool result = env->CallStaticBooleanMethod(clazz, methodId);

		LOGI("2");

		if(envResult != JNI_OK)
			g_javaVM->DetachCurrentThread();

		LOGI("3");

		return result;
	}
}

JNIEXPORT void JNICALL Java_com_thirdpartyninjas_NinjaPartyTest_NinjaJni_init(JNIEnv *env, jclass, jint width, jint height, jstring javaApkPath)
{
	jboolean isCopy;
	const char *nativeApkPath = env->GetStringUTFChars(javaApkPath, &isCopy);
	g_apkPath = nativeApkPath;
	env->ReleaseStringUTFChars(javaApkPath, nativeApkPath);

	env->GetJavaVM(&g_javaVM);

	glEnable(GL_BLEND);
	glViewport(0, 0, width, height);

	LOGI("Init");

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

JNIEXPORT void JNICALL Java_com_thirdpartyninjas_NinjaPartyTest_NinjaJni_destroy(JNIEnv *, jclass)
{
	if(game)
	{
		delete game;
		game = nullptr;
	}

	g_apkPath = "";
}

JNIEXPORT void JNICALL Java_com_thirdpartyninjas_NinjaPartyTest_NinjaJni_setResolution(JNIEnv *, jclass, jint width, jint height)
{
	glViewport(0, 0, width, height);

	if(game)
		game->SetResolution(width, height);
}

JNIEXPORT void JNICALL Java_com_thirdpartyninjas_NinjaPartyTest_NinjaJni_update(JNIEnv *, jclass, jfloat deltaSeconds)
{
	if(game)
	{
		game->Update(deltaSeconds);
	}
}

JNIEXPORT void JNICALL Java_com_thirdpartyninjas_NinjaPartyTest_NinjaJni_draw(JNIEnv *, jclass)
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	if(game)
		game->Draw();
}

JNIEXPORT void JNICALL Java_com_thirdpartyninjas_NinjaPartyTest_NinjaJni_pause(JNIEnv *, jclass)
{
	if(game)
		game->Pause();
}

JNIEXPORT void JNICALL Java_com_thirdpartyninjas_NinjaPartyTest_NinjaJni_resume(JNIEnv *, jclass)
{
	if(game)
		game->Resume();
}

JNIEXPORT void JNICALL Java_com_thirdpartyninjas_NinjaPartyTest_NinjaJni_touchDown(JNIEnv *, jclass, jint id, jint x, jint y)
{
	game->TouchBegan((void*)id, 1, x, y);
}

JNIEXPORT void JNICALL Java_com_thirdpartyninjas_NinjaPartyTest_NinjaJni_touchUp(JNIEnv *, jclass, jint id, jint x, jint y)
{
	game->TouchEnded((void*)id, x, y);
}

JNIEXPORT void JNICALL Java_com_thirdpartyninjas_NinjaPartyTest_NinjaJni_touchMove(JNIEnv *, jclass, jint id, jint x, jint y)
{
	game->TouchMoved((void*)id, x, y);
}

JNIEXPORT void JNICALL Java_com_thirdpartyninjas_NinjaPartyTest_NinjaJni_touchCancel(JNIEnv *, jclass, jint id)
{
	game->TouchCancelled((void*)id);
}

JNIEXPORT void JNICALL Java_com_thirdpartyninjas_NinjaPartyTest_NinjaJni_facebookLogin(JNIEnv *env, jclass, jboolean success, jstring javaAccessToken)
{
	const char *nativeAccessToken = env->GetStringUTFChars(javaAccessToken, nullptr);
	game->FacebookLogin(success, nativeAccessToken);
	env->ReleaseStringUTFChars(javaAccessToken, nativeAccessToken);
}

JNIEXPORT void JNICALL Java_com_thirdpartyninjas_NinjaPartyTest_NinjaJni_facebookLogout(JNIEnv *, jclass)
{
	game->FacebookLogout();
}
