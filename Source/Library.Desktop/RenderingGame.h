#pragma once
#include "Game.h"

#include "ScreenQuad.h"

using namespace Library;

namespace Rendering
{

	class RenderingGame final : public Game
	{
		RTTI_DECLARATIONS(RenderingGame, Game);

	public:
		RenderingGame(const std::string& windowTitle);

	protected:
		virtual void Initialize() override;
		virtual void Draw(const GameTime& gameTime) override;
	};
}

