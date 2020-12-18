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
		float mDirectionalLightIntensity = 0.2f;

		inline size_t GetPointLightCount() const { return mPointLightCount; }
		void SetPointLightCount(size_t count);

		inline GLuint GetPositionTexture() const { return mPositionTexture; }
		inline GLuint GetAlbedoSpecTexture() const { return mAlbedoSpecTexture; }
		inline GLuint GetNormalTexture() const { return mNormalTexture; }
	private:
		struct RenderData 
		{
			GLuint VAO = 0;
			GLuint VBO = 0;
			GLuint IBO = 0;
			GLuint IndexCount = 0;
			glm::vec3 DiffuseColor { 0 };
			GLuint Texture = 0;
		};
		size_t mPointLightCount = 32;
		glm::mat4 mWorldMatrix{ 1 };

		// For shader program
		std::vector<RenderData> mRenderData;
		std::map<std::string, GLuint> mFileNameToTextureId;
		GLuint mGBuffer = 0;
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

		//std::unique_ptr<Library::Plane> mPlane;
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
		inline static const glm::vec2 sLightPositionRangeX{ -6.0f, 5.0f };
		inline static const glm::vec2 sLightPositionRangeY{ 0.0f, 7.0f };
		inline static const glm::vec2 sLightPositionRangeZ{ -4.0f, 5.0f };

	};
}

