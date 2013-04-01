#ifndef NINJAPARTY_GAME_HPP
#define NINJAPARTY_GAME_HPP

#include <NinjaParty/Color.hpp>
//#include <NinjaParty/ScreenManager.hpp>

namespace NinjaParty
{
	class Game
	{
	public:
		Game(int screenWidth, int screenHeight);
		Game(const Game&) = delete;
		virtual ~Game();
		
		Game& operator=(const Game&) = delete;

		virtual void LoadContent();
		virtual void UnloadContent();

		virtual void Update(float deltaSeconds);
		virtual void Draw();
		
//		virtual void BeginAudioInterruption();
//		virtual void EndAudioInterruption();

		void ClearScreen(Color color);

		void Exit() { exit = true; }
		bool GetExit() const { return exit; }

		int GetScreenWidth() const { return screenWidth; }
		int GetScreenHeight() const { return screenHeight; }
		
	private:
		bool exit;
		
		int screenWidth;
		int screenHeight;

	protected:
//		ScreenManager screenManager;
	};
}

#endif//NINJAPARTY_GAME_HPP
