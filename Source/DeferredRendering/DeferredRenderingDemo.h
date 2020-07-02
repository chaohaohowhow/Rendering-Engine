#pragma once
#include "ShaderProgram.h"
#include "DrawableGameComponent.h"

namespace Library
{
	class Plane;
	class PointLight;
	class ProxyModel;
}

namespace Rendering
{
	class DeferredRenderingDemo final : public Library::DrawableGameComponent
	{
		RTTI_DECLARATIONS(DeferredRenderingDemo, DrawableGameComponent);

	public:
		DeferredRenderingDemo(Library::Game& game, std::shared_ptr<Library::Camera> camera);
		DeferredRenderingDemo(const DeferredRenderingDemo&) = delete;
		DeferredRenderingDemo(DeferredRenderingDemo&&) = delete;
		DeferredRenderingDemo& operator=(const DeferredRenderingDemo&) = delete;
		DeferredRenderingDemo& operator=(DeferredRenderingDemo&&) = delete;
		~DeferredRenderingDemo();

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;
		void RandomizePointLights();
	private:
		static const size_t PointLightCount = 32;
		glm::mat4 mWorldMatrix{ 1 };

		// For shader program
		GLuint mVAO = 0;
		GLuint mVBO = 0;
		GLuint mIBO = 0;
		GLuint mGBuffer = 0;
		GLuint mColorTexture = 0;
		size_t mIndexCount = 0;
		GLuint mPositionTexture = 0;
		GLuint mNormalTexture = 0;
		GLuint mAlbedoSpecTexture = 0;
		GLuint mQuadVAO = 0;
		GLuint mQuadVBO = 0;
		GLuint mDebugVAO = 0;
		GLuint mDebugVBO = 0;

		GLint mWorldLocation = -1;
		GLint mViewLocation = -1;
		GLint mProjectionLocation = -1;

		GLint mPointLightPositionLocations[PointLightCount] = { -1 };
		GLint mPointLightColorLocations[PointLightCount] = { -1 };
		GLint mCameraPositionLocation = -1;
		GLint mSpecularPowerLocation = -1;
		GLint mAmbientIntensityLocation = -1;
		GLint mDebugTranslateLocation = -1;

		std::unique_ptr<Library::Plane> mPlane;
		std::vector<std::unique_ptr<Library::PointLight>> mPointLights;
		std::unique_ptr<Library::ProxyModel> mPointLightProxy;
		float mSpecularPower = 16.0f;
		float mAmbientIntensity = 0.5f;

		Library::ShaderProgram mGBufferProgram;
		Library::ShaderProgram mShaderProgram;
		Library::ShaderProgram mDebugShaderProgram;

		void CreateFrameBuffer();
		void RenderQuad();
		void RenderDebug(size_t index);
		void UpdateSpecularPower(const Library::GameTime& gameTime);
		void UpdateAmbientIntensity(const Library::GameTime& gameTime);

		enum class VertexAttribute
		{
			Position = 0,
			TextureCoordinate = 1,
			Normal = 2
		};
	};
}

