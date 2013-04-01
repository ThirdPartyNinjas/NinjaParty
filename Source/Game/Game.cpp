#include <NinjaParty/IncludeGL.h>

#include <NinjaParty/Game.hpp>

namespace NinjaParty
{
	Game::Game(int screenWidth, int screenHeight)
		: exit(false), screenWidth(screenWidth), screenHeight(screenHeight), screenManager(this)
	{
	}
	
	Game::~Game()
	{
	}
	
	void Game::LoadContent()
	{
	}
	
	void Game::UnloadContent()
	{
	}
	
	void Game::Update(float deltaSeconds)
	{
		screenManager.Update(deltaSeconds);
	}
	
	void Game::Draw()
	{
        ClearScreen(NinjaParty::Color::Black);
        
		screenManager.Draw();
	}
	
//	void Game::BeginAudioInterruption()
//	{
//		
//	}
//	
//	void Game::EndAudioInterruption()
//	{
//		
//	}
	
	void Game::ClearScreen(Color color)
	{
		glClearColor(color.R(), color.G(), color.B(), color.A());
		glClear(GL_COLOR_BUFFER_BIT);
	}
}
