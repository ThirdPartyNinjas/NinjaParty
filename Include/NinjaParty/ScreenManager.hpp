#ifndef NINJAPARTY_SCREENMANAGER_HPP
#define NINJAPARTY_SCREENMANAGER_HPP

#include <list>
#include <memory>

namespace NinjaParty
{
	class Game;
	class GameScreen;
	
	class ScreenManager
	{
	public:
		ScreenManager(Game*);
		~ScreenManager();
		
		void Update(float deltaSeconds);
		
		void Draw();
		
		void AddScreen(GameScreen *gameScreen);
		void AddScreen(std::shared_ptr<GameScreen> gameScreen);

		Game* GetGame()
		{ 
			return game; 
		}
		
	private:
		void RemoveScreen(GameScreen *gameScreen);
		
	private:
		std::list<std::shared_ptr<GameScreen> > screens;
		std::list<std::shared_ptr<GameScreen> > screensToAdd;

		Game *game;
	};
}

#endif//NINJAPARTY_SCREENMANAGER_HPP
