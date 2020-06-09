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
		~RenderingGame();

	protected:
		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

	private:
		std::shared_ptr<ScreenQuad> mScreenQuad = nullptr;

		GLuint mFBO;
		std::vector<GLuint> mColorBuffers{ 2 };
		std::vector<GLuint> mIntermediateFBOs{ 2 };
		std::vector<GLuint> mIntermediateColorBuffers{ 2 };

		void CreateFrameBuffers();
	};
}

