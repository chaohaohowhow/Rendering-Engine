#pragma once
#include "DrawableGameComponent.h"
#include "ShaderProgram.h"

namespace Library
{
	class ColoredTriangle final : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(ColoredTriangle, DrawableGameComponent);

	public:
		ColoredTriangle(Game& game, std::shared_ptr<Camera> camera);
		ColoredTriangle(const ColoredTriangle&) = delete;
		ColoredTriangle(ColoredTriangle&&) = delete;
		ColoredTriangle& operator=(const ColoredTriangle&) = delete;
		ColoredTriangle& operator=(ColoredTriangle&&) = delete;
		~ColoredTriangle();

		virtual void Initialize() override;
		virtual void Draw(const GameTime& gameTime) override;

	private:
		enum class VertexAttribute
		{
			Position = 0,
			Color = 1
		};

		ShaderProgram mShaderProgram;
		GLuint mVertexArrayObject = 0;
		GLuint mVertexBuffer = 0;
		GLint mProjectionLocation = -1;
		GLint mViewLocation = -1;
	};
}

