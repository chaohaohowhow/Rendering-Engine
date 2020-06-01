#pragma once
#include "DrawableGameComponent.h"
#include "ShaderProgram.h"

namespace Library
{
	class TRex : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(TRex, DrawableGameComponent);

	public:
		TRex(Game& game, std::shared_ptr<Camera> camera);
		TRex(const TRex&) = delete;
		TRex(TRex&&) = delete;
		TRex& operator=(const TRex&) = delete;
		TRex& operator=(TRex&) = delete;
		~TRex();

		virtual void Initialize() override;
		//virtual void Draw(const GameTime& gameTime) override;

	private:
		enum class VertexAttribute
		{
			Position,
			TextureCoordinate
		};

		ShaderProgram mShaderProgram;
		GLuint mVAO = 0;
		GLuint mVBO = 0;
		GLuint mIBO = 0;
		size_t mIndexCount = 0;

		GLint mProjectionLocation = -1;
		GLint mViewLocation = -1;
	};
}

