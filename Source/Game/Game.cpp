#include <deque>

#include <NinjaParty/IncludeGL.h>

#include <NinjaParty/Game.hpp>

namespace NinjaParty
{
    struct Game::impl
    {
        std::deque<std::shared_ptr<IEvent>> eventQueue;
    };
    
	Game::Game(int screenWidth, int screenHeight)
		: pimpl(new impl()), exit(false), screenWidth(screenWidth), screenHeight(screenHeight), screenManager(this)
	{
	}
	
	Game::~Game()
	{
	}
    
    void Game::PostEvent(const std::shared_ptr<IEvent> &event, NinjaParty::EventPriority eventPriority)
    {
        if(eventPriority == EventPriority::High)
            pimpl->eventQueue.push_front(event);
        else
            pimpl->eventQueue.push_back(event);
    }
	
	void Game::LoadContent(const std::string &assetPath, const std::string &assetArchivePath)
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
	
	void Game::ClearScreen(Color color)
	{
		glClearColor(color.R(), color.G(), color.B(), color.A());
		glClear(GL_COLOR_BUFFER_BIT);
	}
    
    int Game::GetEventCount() const
    {
        return pimpl->eventQueue.size();
    }
    
    std::shared_ptr<IEvent> Game::GetEvent()
    {
        if(pimpl->eventQueue.empty())
            return nullptr;
        
        auto event = pimpl->eventQueue.front();
        pimpl->eventQueue.pop_front();
        return event;
    }
}
