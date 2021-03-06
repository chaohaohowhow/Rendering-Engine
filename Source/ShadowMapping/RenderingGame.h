#pragma once
#include "Game.h"

#include "GaussianBlur.h"
#include "ShadowMappingDemo.h"

using namespace Library;

namespace Rendering
{

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
		std::shared_ptr<ShadowMappingDemo> mDemo = nullptr;
	};
}

