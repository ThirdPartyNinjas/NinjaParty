#include <android/log.h>

#include "com_thirdpartyninjas_test_TestJni.h"

#define  LOG_TAG "NinjaPartyTest"
#define  LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

extern "C"
{
	JNIEXPORT void JNICALL Java_com_thirdpartyninjas_test_TestJni_init(JNIEnv *, jobject, jint, jint, jstring);
	JNIEXPORT void JNICALL Java_com_thirdpartyninjas_test_TestJni_update(JNIEnv *, jobject, jfloat);
	JNIEXPORT void JNICALL Java_com_thirdpartyninjas_test_TestJni_draw(JNIEnv *, jobject);
}

JNIEXPORT void JNICALL Java_com_thirdpartyninjas_test_TestJni_init(JNIEnv *, jobject, jint, jint, jstring)
{
	LOGI("init called");
}

JNIEXPORT void JNICALL Java_com_thirdpartyninjas_test_TestJni_update(JNIEnv *, jobject, jfloat)
{
	LOGI("update called");
}

JNIEXPORT void JNICALL Java_com_thirdpartyninjas_test_TestJni_draw(JNIEnv *, jobject)
{
	LOGI("draw called");
}

///*
// * Copyright (C) 2009 The Android Open Source Project
// *
// * Licensed under the Apache License, Version 2.0 (the "License");
// * you may not use this file except in compliance with the License.
// * You may obtain a copy of the License at
// *
// *      http://www.apache.org/licenses/LICENSE-2.0
// *
// * Unless required by applicable law or agreed to in writing, software
// * distributed under the License is distributed on an "AS IS" BASIS,
// * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// * See the License for the specific language governing permissions and
// * limitations under the License.
// */
//
//// OpenGL ES 2.0 code
//
//#include <string>
//
//#include <inttypes.h>
//
//#include <jni.h>
//#include <android/log.h>
//
//#include <GLES2/gl2.h>
//#include <GLES2/gl2ext.h>
//
//#include <stdio.h>
//#include <stdlib.h>
//#include <math.h>
//
//#include <zip.h>
//
//#include <NinjaParty/AssetManager.hpp>
//#include <NinjaParty/SpriteBatch.hpp>
//#include <NinjaParty/Texture.hpp>
//
//#define  LOG_TAG    "libgl2jni"
//#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
//#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
//
//static void printGLString(const char *name, GLenum s) {
//    const char *v = (const char *) glGetString(s);
//    LOGI("GL %s = %s\n", name, v);
//}
//
//static void checkGlError(const char* op) {
//    for (GLint error = glGetError(); error; error
//            = glGetError()) {
//        LOGI("after %s() glError (0x%x)\n", op, error);
//    }
//}
//
//float color = 0.0f;
//
//static const char gVertexShader[] =
//    "attribute vec4 vPosition;\n"
//    "void main() {\n"
//    "  gl_Position = vPosition;\n"
//    "}\n";
//
//static const char gFragmentShader[] =
//    "precision mediump float;\n"
//    "void main() {\n"
//    "  gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
//    "}\n";
//
//GLuint loadShader(GLenum shaderType, const char* pSource) {
//    GLuint shader = glCreateShader(shaderType);
//    if (shader) {
//        glShaderSource(shader, 1, &pSource, NULL);
//        glCompileShader(shader);
//        GLint compiled = 0;
//        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
//        if (!compiled) {
//            GLint infoLen = 0;
//            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
//            if (infoLen) {
//                char* buf = (char*) malloc(infoLen);
//                if (buf) {
//                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
//                    LOGE("Could not compile shader %d:\n%s\n",
//                            shaderType, buf);
//                    free(buf);
//                }
//                glDeleteShader(shader);
//                shader = 0;
//            }
//        }
//    }
//    return shader;
//}
//
//GLuint createProgram(const char* pVertexSource, const char* pFragmentSource) {
//    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
//    if (!vertexShader) {
//        return 0;
//    }
//
//    GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
//    if (!pixelShader) {
//        return 0;
//    }
//
//    GLuint program = glCreateProgram();
//    if (program) {
//        glAttachShader(program, vertexShader);
//        checkGlError("glAttachShader");
//        glAttachShader(program, pixelShader);
//        checkGlError("glAttachShader");
//        glLinkProgram(program);
//        GLint linkStatus = GL_FALSE;
//        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
//        if (linkStatus != GL_TRUE) {
//            GLint bufLength = 0;
//            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
//            if (bufLength) {
//                char* buf = (char*) malloc(bufLength);
//                if (buf) {
//                    glGetProgramInfoLog(program, bufLength, NULL, buf);
//                    LOGE("Could not link program:\n%s\n", buf);
//                    free(buf);
//                }
//            }
//            glDeleteProgram(program);
//            program = 0;
//        }
//    }
//    return program;
//}
//
//GLuint gProgram;
//GLuint gvPositionHandle;
//
//bool setupGraphics(int w, int h) {
//    printGLString("Version", GL_VERSION);
//    printGLString("Vendor", GL_VENDOR);
//    printGLString("Renderer", GL_RENDERER);
//    printGLString("Extensions", GL_EXTENSIONS);
//
//    LOGI("setupGraphics(%d, %d)", w, h);
//    gProgram = createProgram(gVertexShader, gFragmentShader);
//    if (!gProgram) {
//        LOGE("Could not create program.");
//        return false;
//    }
//    gvPositionHandle = glGetAttribLocation(gProgram, "vPosition");
//    checkGlError("glGetAttribLocation");
//    LOGI("glGetAttribLocation(\"vPosition\") = %d\n",
//            gvPositionHandle);
//
//    glViewport(0, 0, w, h);
//    checkGlError("glViewport");
//
//    glEnable(GL_BLEND);
//    return true;
//}
//
//const GLfloat gTriangleVertices[] = { 0.0f, 0.5f, -0.5f, -0.5f,
//        0.5f, -0.5f };
//
//NinjaParty::AssetManager *assetManager;
//NinjaParty::Font *font;
//NinjaParty::SpriteBatch *spriteBatch;
//NinjaParty::Texture *fontTexture;
//NinjaParty::Texture *texture;
//
//void renderFrame()
//{
//    glClearColor(color, color, color, 1.0f);
//    checkGlError("glClearColor");
//    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
//    checkGlError("glClear");
//
//    spriteBatch->Begin();
//    spriteBatch->Draw(texture, NinjaParty::Vector2::ZERO);
//    spriteBatch->DrawString(font, fontTexture, "Test", NinjaParty::Vector2(0, 500), NinjaParty::Color::White);
//    spriteBatch->End();
//}
//
//void update(float deltaSeconds)
//{
//    color += deltaSeconds / 2.0f;
//    while(color > 1.0f)
//        color -= 1.0f;
//}
//
//std::string g_apkPath;
//
//extern "C"
//{
//    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height, jstring apkPath);
//    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_draw(JNIEnv * env, jobject obj);
//    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_update(JNIEnv * env, jobject obj, jfloat deltaSeconds);
//};
//
//JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height, jstring javaApkPath)
//{
//	jboolean isCopy;
//	const char *nativeApkPath = env->GetStringUTFChars(javaApkPath, &isCopy);
//	g_apkPath = nativeApkPath;
//	env->ReleaseStringUTFChars(javaApkPath, nativeApkPath);
//
//    setupGraphics(width, height);
//
//    try
//    {
//		spriteBatch = new NinjaParty::SpriteBatch((int)width, (int)height);
//
//		assetManager = new NinjaParty::AssetManager("assets/", g_apkPath);
//		font = assetManager->LoadFont("Arial_40.fnt");
//		fontTexture = assetManager->LoadTexture("Arial_40.png");
//		texture = assetManager->LoadTexture("Star.png");
//    }
//    catch(std::exception &e)
//    {
//    	LOGE("Exception caught! %s", e.what());
//    }
//}
//
//JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_draw(JNIEnv * env, jobject obj)
//{
//    renderFrame();
//}
//
//JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_update(JNIEnv * env, jobject obj, jfloat deltaSeconds)
//{
//    update(deltaSeconds);
//}
