//
//  main.m
//  Test
//
//  Created by Jesse Chounard on 3/30/13.
//  Copyright (c) 2013 Third Party Ninjas LLC. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include <SDL.h>
#include <SDL_opengl.h>

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
#elif defined(TEST_ASSETS)
#include "../../Tests/Assets.hpp"
#endif

int main(int argc, char *argv[])
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	NSString *resourcePath = [[NSBundle mainBundle] resourcePath];
	[[NSFileManager defaultManager] changeCurrentDirectoryPath:resourcePath];
	[pool drain];
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        exit(-1);
    }
    
    // todo: figure this out
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    
    SDL_Window *window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(1);
    
    SDL_Event evt;
    bool run = true;
    
    Tests::TestGame game(1280, 720);
    game.LoadContent("Assets/", "Assets/Assets.zip");

    float frequency = SDL_GetPerformanceFrequency();
    uint64_t previousTime = SDL_GetPerformanceCounter();
    
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    
    while (run)
    {
        while (SDL_PollEvent(&evt))
        {
            if(evt.type == SDL_QUIT)
            {
                run = false;
            }
        }
        
        uint64_t currentTime = SDL_GetPerformanceCounter();
        float deltaSeconds = static_cast<float>(currentTime - previousTime) / frequency;
        previousTime = currentTime;
        
        game.Update(deltaSeconds);
        game.Draw();
        
        SDL_GL_SwapWindow(window);
        SDL_Delay(1);
    }

    game.UnloadContent();
    
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
