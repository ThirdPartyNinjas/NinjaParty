#include <functional>
#include <memory>

#include <NinjaParty/Game.hpp>
#include <NinjaParty/HttpRequestManager.hpp>

namespace Tests
{
    class TestGame : public NinjaParty::Game
    {
    public:
        TestGame(int screenWidth, int screenHeight)
			: NinjaParty::Game(screenWidth, screenHeight)
        {
			NinjaParty::HttpRequestManager::Instance()->PerformGet("http://www.google.com", std::bind(&TestGame::HttpCallback, this, std::placeholders::_1, std::placeholders::_2));
        }
        
        void Update(float deltaSeconds)
        {
        }
        
        void Draw()
        {
            ClearScreen(NinjaParty::Color::CornflowerBlue);
        }
        
    private:
		void HttpCallback(bool success, const std::string &response)
		{
			printf("Response:\n\n%s\n", response.c_str());
		}		
    };
}
