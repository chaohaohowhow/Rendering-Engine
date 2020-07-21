#pragma once
#include "ShaderProgram.h"
#include "DrawableGameComponent.h"
#include "GBufferPass.h"
#include "PointLightPass.h"
#include "DirectionalLightPass.h"

namespace Library
{
	class Plane;
	class PointLight;
	class DirectionalLight;
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
		void ToggleShowSphere();

		float* GetAmbientIntensityAddress();
		float* GetSpecularPowerAddress();
		float mDirectionalLightIntensity = 0.5f;
	private:
		static const size_t PointLightCount = 32;
		glm::mat4 mWorldMatrix{ 1 };

		// For shader program
		GLuint mVAO = 0;
		GLuint mVBO = 0;
		GLuint mIBO = 0;
		size_t mIndexCount = 0;
		GLuint mGBuffer = 0;
		GLuint mColorTexture = 0;

		GLuint mPositionTexture = 0;
		GLuint mNormalTexture = 0;
		GLuint mAlbedoSpecTexture = 0;

		GLuint mQuadVAO = 0;
		GLuint mQuadVBO = 0;
		GLuint mDebugVAO = 0;
		GLuint mDebugVBO = 0;

		GLuint mSphereVAO = 0;
		GLuint mSphereVBO = 0;
		GLuint mSphereIBO = 0;
		size_t mSphereIndexCount = 0;

		GLint mDebugTranslateLocation = -1;

		std::unique_ptr<Library::Plane> mPlane;
		std::vector<std::unique_ptr<Library::PointLight>> mPointLights;
		std::unique_ptr<Library::DirectionalLight> mDirectionalLight;
		std::unique_ptr<Library::ProxyModel> mPointLightProxy;
		std::unique_ptr<Library::ProxyModel> mDirectionalLightProxy;
		float mSpecularPower = 16.0f;
		float mAmbientIntensity = 0.1f;
		bool mShowSphere = false;

		GBufferPass mGBufferPassProgram;
		PointLightPass mPointLightPassProgram;
		DirectionalLightPass mDirectionalLightPassProgram;
		Library::ShaderProgram mDebugShaderProgram;

		void CreateFrameBuffer();
		void InitializeQuad();
		void RenderQuad();
		void RenderDebug(size_t index);
		void UpdateSpecularPower(const Library::GameTime& gameTime);
		void UpdateAmbientIntensity(const Library::GameTime& gameTime);
		void UpdateDirectionalLight(const Library::GameTime& gameTime);

		enum class VertexAttribute
		{
			Position = 0,
			TextureCoordinate = 1,
			Normal = 2
		};

		inline static const glm::vec2 sLightRotationRate{ glm::two_pi<float>(), glm::two_pi<float>() };
	};
}

