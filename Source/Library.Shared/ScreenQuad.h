#pragma once
#include "DrawableGameComponent.h"

#include "ShaderProgram.h"
namespace Library
{
	struct VertexPositionTexture;

	class ScreenQuad : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(ScreenQuad, DrawableGameComponent);

	public:
		ScreenQuad(Game& game, std::shared_ptr<Camera> camera);
		ScreenQuad(const ScreenQuad&) = delete;
		ScreenQuad(ScreenQuad&&) = delete;
		ScreenQuad& operator=(const ScreenQuad&) = delete;
		ScreenQuad& operator=(ScreenQuad&&) = delete;
		~ScreenQuad();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

		inline void SetTextureBuffer(GLuint textureColorBuffer) { mTextureBuffer = textureColorBuffer; }
		inline void SetBloomBlur(GLuint bloomBlur) { mBloomBlurTexture = bloomBlur; }
	protected:
		GLuint mVAO = 0;
		GLuint mVBO = 0;
		GLuint mIBO = 0;
		static const size_t mIndexCount = 6;
		GLuint mTextureBuffer = 0;
		GLuint mBloomBlurTexture = 0;


	private:
		ShaderProgram mShaderProgram;
		static const VertexPositionTexture sScreenVertices[];
		static const uint32_t sScreenIndicies[];

		enum class VertexAttribute
		{
			Position = 0,
			TextureCoordinate = 1,
		};
	};
}
