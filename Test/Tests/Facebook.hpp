#include <functional>
#include <memory>

#include <NinjaParty/FacebookManager.hpp>
#include <NinjaParty/Game.hpp>

namespace Tests
{
    class TestGame : public NinjaParty::Game
    {
    public:
        TestGame(int screenWidth, int screenHeight)
			: NinjaParty::Game(screenWidth, screenHeight)
        {
        }
        
        void Update(float deltaSeconds)
        {
        }
        
        void Draw()
        {
			if(facebookManager.IsLoggedIn())
			   ClearScreen(NinjaParty::Color::CornflowerBlue);
			else
			   ClearScreen(NinjaParty::Color::Red);
        }
		
		void TouchBegan(void *touchHandle, int tapCount, int x, int y)
		{
			if(x < GetScreenWidth() / 2)
			{
				if(facebookManager.IsLoggedIn())
					facebookManager.RequestInfo();
				else
					facebookManager.Login();
			}
			else
			{
				if(facebookManager.IsLoggedIn())
					facebookManager.Logout();
				else
					facebookManager.Login();
			}
		}
		
		void FacebookLogin(bool success, const std::string &accessToken)
		{
			facebookManager.InjectLogin(success, accessToken);
		}
		
		void FacebookLogout()
		{
			facebookManager.InjectLogout();
		}
		
	private:
		NinjaParty::FacebookManager facebookManager;
    };
}
