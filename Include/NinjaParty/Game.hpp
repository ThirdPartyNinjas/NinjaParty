#ifndef NINJAPARTY_GAME_HPP
#define NINJAPARTY_GAME_HPP

#include <NinjaParty/Color.hpp>
#include <NinjaParty/FacebookManager.hpp>
#include <NinjaParty/ScreenManager.hpp>

namespace NinjaParty
{
	class Game
	{
	public:
		Game(int screenWidth, int screenHeight);
		Game(const Game&) = delete;
		virtual ~Game();
		
		Game& operator=(const Game&) = delete;

		virtual void SetResolution(int screenWidth, int screenHeight)
		{
			this->screenWidth = screenWidth;
			this->screenHeight = screenHeight;
		}

		virtual void LoadContent(const std::string &assetPath = "Assets/", const std::string &assetArchivePath = "");
		virtual void UnloadContent();

		virtual void Update(float deltaSeconds);
		virtual void Draw();
		
		void ClearScreen(Color color);

		void Exit() { exit = true; }
		bool GetExit() const { return exit; }

		int GetScreenWidth() const { return screenWidth; }
		int GetScreenHeight() const { return screenHeight; }

		virtual void Pause() { }
		virtual void Resume() { }

		virtual void HandleLostGraphicsContext() { }

		virtual void TextInput(const std::string &text) { }
		virtual void TextBackspace() { }

		virtual void TouchBegan(void *touchHandle, int tapCount, int x, int y) { }
		virtual void TouchEnded(void *touchHandle, int x, int y) { }
		virtual void TouchMoved(void *touchHandle, int x, int y) { }
		virtual void TouchCancelled(void *touchHandle) { }

		virtual void FacebookLogin(bool success, const std::string &accessToken) { }
		virtual void FacebookLogout() { }

	private:
		bool exit;
		
		int screenWidth;
		int screenHeight;

	protected:
		ScreenManager screenManager;
	};
}

#endif//NINJAPARTY_GAME_HPP
