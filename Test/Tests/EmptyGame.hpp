#include <memory>

#include <NinjaParty/Color.hpp>
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
            ClearScreen(NinjaParty::Color::CornflowerBlue);
        }
        
    private:
		
    };
}
