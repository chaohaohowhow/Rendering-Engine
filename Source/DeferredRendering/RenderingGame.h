#pragma once
#include "Game.h"

using namespace Library;

namespace Rendering
{
	class DeferredRenderingDemo;

	class RenderingGame final : public Game
	{
		RTTI_DECLARATIONS(RenderingGame, Game);

	public:
		RenderingGame(const std::string& windowTitle);
		~RenderingGame();

	protected:
		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

	private:
		std::shared_ptr<DeferredRenderingDemo> mDemo = nullptr;
		
		void CalculateFrameRate(const GameTime& gameTime);
		
		int mFrameRate = 0;
		int mFrameCount = 0;
		std::chrono::milliseconds mLastTotalTime;
	};
}

