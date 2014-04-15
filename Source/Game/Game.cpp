#include <algorithm>
#include <chrono>
#include <deque>
#include <map>
#include <thread>
#include <vector>

#include <NinjaParty/Event.hpp>
#include <NinjaParty/IncludeGL.h>
#include <NinjaParty/Game.hpp>

namespace NinjaParty
{
    struct Game::impl
    {
        std::deque<std::shared_ptr<IEvent>> eventQueue;
        std::mutex eventMutex;
        std::chrono::high_resolution_clock::time_point lastTime;
    };
    
	Game::Game(int screenWidth, int screenHeight)
		: pimpl(new impl()), exit(false), screenWidth(screenWidth), screenHeight(screenHeight)
	{
        pimpl->lastTime = std::chrono::high_resolution_clock::now();
	}
	
	Game::~Game()
	{
	}
    
    void Game::PostEvent(const std::shared_ptr<IEvent> &event, NinjaParty::EventPriority eventPriority)
    {
        switch(eventPriority)
        {
            case EventPriority::Normal:
            {
                std::lock_guard<std::mutex> lock(pimpl->eventMutex);
                pimpl->eventQueue.push_back(event);
                break;
            }
                
            case EventPriority::High:
            {
                std::lock_guard<std::mutex> lock(pimpl->eventMutex);
                pimpl->eventQueue.push_front(event);
                break;
            }
                
            case EventPriority::Immediate:
            {
                EventBroadcaster::Instance().Broadcast(event);
                break;
            }
        }
    }
        
	void Game::LoadContent(const std::string &assetPath, const std::string &assetArchivePath)
	{
	}
	
	void Game::UnloadContent()
	{
	}
	
	void Game::UpdateFrame()
	{
        pimpl->eventMutex.lock();
        
        std::deque<std::shared_ptr<IEvent>> eventQueue(pimpl->eventQueue);
        pimpl->eventQueue.clear();
        
        pimpl->eventMutex.unlock();
        
        while(!eventQueue.empty())
        {
            auto &event = eventQueue.front();
            EventBroadcaster::Instance().Broadcast(event);
            eventQueue.pop_front();
        }
        
        EventBroadcaster::Instance().Execute();
        
        std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
        int ms = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - pimpl->lastTime).count();
        float deltaSeconds = ms / 1000.0f;
        pimpl->lastTime = currentTime;
        
        Update(deltaSeconds);
	}

	void Game::DrawFrame()
	{
        ClearScreen(NinjaParty::Color::Black);
        
        Draw();
	}
	
	void Game::ClearScreen(Color color)
	{
		glClearColor(color.R(), color.G(), color.B(), color.A());
		glClear(GL_COLOR_BUFFER_BIT);
	}
}
