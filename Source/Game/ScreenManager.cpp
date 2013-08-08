#include <algorithm>

#include <NinjaParty/ScreenManager.hpp>
#include <NinjaParty/GameScreen.hpp>

namespace NinjaParty
{
	ScreenManager::ScreenManager(Game *game)
		: game(game)
	{
	}
	
	ScreenManager::~ScreenManager()
	{
	}
	
	void ScreenManager::ReloadGraphics()
	{
		for(auto &iterator : screens)
		{
			iterator->ReloadGraphics();
		}
	}

	void ScreenManager::Update(float deltaSeconds)
	{
		screens.insert(screens.end(), screensToAdd.begin(), screensToAdd.end());
		screensToAdd.clear();
		
		GameScreen *waitingScreen = nullptr;
		bool hasFocus = true;
		bool foundExclusive = false;

		for(auto iterator = screens.rbegin(); iterator != screens.rend(); ++iterator)
		{
			GameScreen *gameScreen = iterator->get();
			
			if((*iterator)->GetScreenState() == ScreenState::Waiting)
			{
				if(waitingScreen == nullptr)
				{
					waitingScreen = gameScreen;
				}
				else
				{
					gameScreen->ExitScreen(false);
				}
			}
			else
			{
				if(gameScreen->IsExclusive())
				{
					if(foundExclusive)
						gameScreen->ExitScreen(true);
					else
						foundExclusive = true;
				}
				
				gameScreen->Update(deltaSeconds, hasFocus);
				hasFocus = false;
			}
		}
		
		foundExclusive = false;
		waitingScreen = nullptr;
		
		auto iterator = screens.begin();
		while(iterator != screens.end())
		{
			if((*iterator)->NeedsRemoved())
			{
				screens.erase(iterator++);
			}
			else
			{
				if((*iterator)->GetScreenState() == ScreenState::Waiting)
				{
					waitingScreen = iterator->get();
				}
				else if((*iterator)->IsExclusive())
				{
					foundExclusive = true;
				}
				++iterator;
			}
		}

		if(waitingScreen != nullptr && !foundExclusive)
		{
			waitingScreen->Update(0, true);
		}
	}

	void ScreenManager::Draw()
	{
		auto end = screens.end();
		for(auto iterator = screens.begin(); iterator != end; ++iterator)
		{
			GameScreen *screen = iterator->get();
			if (screen->GetScreenState() != ScreenState::Waiting)
				(*iterator)->Draw();
		}
	}

	void ScreenManager::AddScreen(std::shared_ptr<GameScreen> gameScreen)
	{
		if (!gameScreen->IsExclusive())
			gameScreen->SetScreenState(ScreenState::TransitionOn);
		else
			gameScreen->SetScreenState(ScreenState::Waiting);
		
		screensToAdd.push_back(gameScreen);
	}
	
	void ScreenManager::AddScreen(GameScreen *gameScreen)
	{
		AddScreen(std::shared_ptr<GameScreen>(gameScreen));
	}
}
