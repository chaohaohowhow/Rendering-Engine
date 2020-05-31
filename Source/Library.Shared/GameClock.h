#pragma once
namespace Library
{
	class GameTime;

	class GameClock
	{
	public:
#pragma region Special Members
		GameClock();
		GameClock(const GameClock&) = default;
		GameClock& operator=(const GameClock&) = default;
		GameClock(GameClock&&) = default;
		GameClock& operator=(GameClock&&) = default;
		~GameClock() = default;
#pragma endregion
		inline const std::chrono::high_resolution_clock::time_point& StartTime() const { return mStartTime; }
		inline const std::chrono::high_resolution_clock::time_point& CurrentTime() const { return mCurrentTime; }
		inline const std::chrono::high_resolution_clock::time_point& LastTime() const { return mLastTime; }

		void Reset();
		void UpdateGameTime(GameTime& gameTime);

	private:
		std::chrono::high_resolution_clock::time_point mStartTime;
		std::chrono::high_resolution_clock::time_point mCurrentTime;
		std::chrono::high_resolution_clock::time_point mLastTime;
	};
}

