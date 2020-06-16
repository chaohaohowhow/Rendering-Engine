#pragma once
#include "ScreenQuad.h"
#include "GaussianBlurEffect.h"

namespace Library
{
	struct VertexPositionTexture;

	class GaussianBlur : public ScreenQuad
	{
		RTTI_DECLARATIONS(GaussianBlur, ScreenQuad);

	public:
		GaussianBlur(Game& game, std::shared_ptr<Camera> camera);
		GaussianBlur(const GaussianBlur&) = delete;
		GaussianBlur(GaussianBlur&&) = delete;
		GaussianBlur& operator=(const GaussianBlur&) = delete;
		GaussianBlur& operator=(GaussianBlur&&) = delete;
		~GaussianBlur() = default;

		virtual void Initialize() override;
		virtual void Draw(const GameTime& gameTime) override;

		inline void SetFrameBuffers(std::vector<GLuint>* frameBuffers) { mFrameBuffers = frameBuffers; }
		inline void SetIntermediateColorBuffers(std::vector<GLuint>* intermediateColorBuffer) { mIntermediateColorBuffers = intermediateColorBuffer; }
	private:
		GaussianBlurEffect mShaderProgram;
		const size_t mBlurAmount = 10;
		std::vector<GLuint>* mFrameBuffers = nullptr;
		std::vector<GLuint>* mIntermediateColorBuffers = nullptr;

		bool mHorizontal = true;
		bool mFirstIteration = true;

		static const VertexPositionTexture sScreenVertices[];
		static const uint32_t sScreenIndicies[];
	};
}

