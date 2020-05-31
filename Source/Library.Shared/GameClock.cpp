#include "pch.h"
#include "GameClock.h"
#include "GameTime.h"

using namespace std::chrono;

namespace Library
{
	GameClock::GameClock()
	{
		Reset();
	}
	
	void GameClock::Reset()
	{
		mStartTime = high_resolution_clock::now();
		mCurrentTime = mStartTime;
		mLastTime = mCurrentTime;
	}

	void GameClock::UpdateGameTime(GameTime& gameTime)
	{
		mCurrentTime = high_resolution_clock::now();

		gameTime.SetCurrentTime(mCurrentTime);
		gameTime.SetTotalGameTime(duration_cast<milliseconds>(mCurrentTime - mStartTime));
		gameTime.SetElapsedGameTime(duration_cast<milliseconds>(mCurrentTime - mLastTime));
		mLastTime = mCurrentTime;
	}
}