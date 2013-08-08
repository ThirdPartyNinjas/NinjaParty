#ifndef NINJAPARTY_GAMESCREEN_HPP
#define NINJAPARTY_GAMESCREEN_HPP

namespace NinjaParty
{
	class ScreenManager;

	enum class ScreenState
	{
		Waiting,
		TransitionOn,
		Active,
		TransitionOff,
	};

	class GameScreen
	{
	public:
		GameScreen(ScreenManager &screenManager);
		GameScreen(const GameScreen&) = delete;
		virtual ~GameScreen();
		
		virtual void ReloadGraphics() { }

		GameScreen& operator=(const GameScreen&) = delete;

		bool IsExclusive() const { return isExclusive; }
		bool HasFocus() const { return hasFocus; }

		float GetTransitionOnTime() const { return transitionOnTime; }
		float GetTransitionOffTime() const { return transitionOffTime; }
		float GetTransitionPosition() const { return transitionPosition; }

		ScreenState GetScreenState() const { return screenState; }
		void SetScreenState(ScreenState state) { screenState = state; }
		ScreenManager& GetScreenManager() const { return screenManager; }

		void Update(float deltaSeconds, bool hasFocus);

		void ExitScreen(bool allowTransition);

		bool NeedsRemoved() const { return remove; }

		virtual void Update(float deltaSeconds) = 0;
		virtual void Draw() { }

	protected:
		bool hasFocus;
		bool isExclusive;

		bool remove;

		bool transitionOn;
		bool transitionOff;
		float transitionOnTime;
		float transitionOffTime;
		float transitionPosition;

		ScreenState screenState;
		ScreenManager &screenManager;
	};
}

#endif//NINJAPARTY_GAMESCREEN_HPP
