#ifndef NINJAPARTY_GAME_HPP
#define NINJAPARTY_GAME_HPP

#include <memory>

#include <NinjaParty/Color.hpp>
#include <NinjaParty/Event.hpp>
#include <NinjaParty/FacebookManager.hpp>
#include <NinjaParty/ScreenManager.hpp>

/* Events:

 TextInput
 
 ResolutionChanged
 
 LostGraphicsContext
 
 Possible:
 Facebook
 GameCenter
*/

namespace NinjaParty
{
	class Game
	{
	public:
		Game(int screenWidth, int screenHeight);
		Game(const Game&) = delete;
		virtual ~Game();
		
		Game& operator=(const Game&) = delete;

        void PostEvent(const std::shared_ptr<IEvent> &event, EventPriority eventPriority = EventPriority::Normal);
        
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

		virtual void FacebookLogin(bool success, const std::string &accessToken) { }
		virtual void FacebookLogout() { }

        int GetEventCount() const;
        std::shared_ptr<IEvent> GetEvent();
        
	private:
        struct impl;
        std::unique_ptr<impl> pimpl;
        
		bool exit;
		
		int screenWidth;
		int screenHeight;

	protected:
		ScreenManager screenManager;
	};
}

#endif//NINJAPARTY_GAME_HPP
