#include <NinjaParty/GameScreen.hpp>
#include <NinjaParty/ScreenManager.hpp>

namespace NinjaParty
{
	GameScreen::GameScreen(ScreenManager &screenManager)
		: screenManager(screenManager)
	{
		isExclusive = true;
		remove = false;
		transitionOn = transitionOff = false;
		transitionOnTime = transitionOffTime = transitionPosition = 0;
	}
	
	GameScreen::~GameScreen()
	{
	}
	
	void GameScreen::Update(float deltaSeconds, bool hasFocus)
	{
		this->hasFocus = hasFocus;

		switch (screenState)
		{
			case ScreenState::Waiting:
				if(transitionOn)
				{
					transitionPosition = 0;
					screenState = ScreenState::TransitionOn;
				}
				else
				{
					transitionPosition = 1;
					screenState = ScreenState::Active;
				}
				return;

			case ScreenState::TransitionOn:
				transitionPosition += deltaSeconds / transitionOnTime;
				if (transitionPosition >= 1)
				{
					transitionPosition = 1;
					screenState = ScreenState::Active;
				}
				break;

			case ScreenState::TransitionOff:
				transitionPosition -= deltaSeconds / transitionOffTime;
				if(transitionPosition <= 0)
				{
					transitionPosition = 0;
					remove = true;
				}
				break;

			default:
				break;
		}
		
		Update(deltaSeconds);
	}
	
	void GameScreen::ExitScreen(bool allowTransition)
	{
		if (!allowTransition || transitionOff == false)
		{
			remove = true;
		}
		else
		{
			screenState = ScreenState::TransitionOff;
		}
	}
}
