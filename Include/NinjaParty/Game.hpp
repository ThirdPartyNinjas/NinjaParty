#ifndef NINJAPARTY_GAME_HPP
#define NINJAPARTY_GAME_HPP

#include <NinjaParty/Color.hpp>
#include <NinjaParty/Keyboard.hpp>
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

		virtual void BeginAudioInterruption() { }
		virtual void EndAudioInterruption() { }

		virtual void HandleLostGraphicsContext() { }
        
		virtual void TouchBegan(void *touchHandle, int tapCount, int x, int y) { }
		virtual void TouchEnded(void *touchHandle, int x, int y) { }
		virtual void TouchMoved(void *touchHandle, int x, int y) { }
		virtual void TouchCancelled(void *touchHandle) { }

        virtual void HandleTap(int x, int y) { }
        
        void KeyEvent(Key key, KeyState keyState);
        KeyboardState GetKeyboardState() const { return keyboardState; }

	private:
		bool exit;
		
		int screenWidth;
		int screenHeight;

	protected:
		ScreenManager screenManager;
        
        KeyboardState keyboardState;
	};
}

#endif//NINJAPARTY_GAME_HPP
