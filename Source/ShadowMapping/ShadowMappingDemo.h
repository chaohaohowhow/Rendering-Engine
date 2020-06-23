#pragma once

#include "ShadowMappingEffect.h"
#include "DrawableGameComponent.h"
#include "ColorHelper.h"

namespace Library
{
	class Light;
	class DirectionalLight;
	class PointLight;
	class ModelMaterial;
	class Plane;
	class ProxyModel;

	class ShadowMappingDemo : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(ShadowMappingDemo, DrawableGameComponent);
	public:
		ShadowMappingDemo(Game& game, std::shared_ptr<Camera> camera);
		ShadowMappingDemo(const ShadowMappingDemo&) = delete;
		ShadowMappingDemo(ShadowMappingDemo&&) = delete;
		ShadowMappingDemo& operator=(const ShadowMappingDemo&) = delete;
		ShadowMappingDemo& operator=(ShadowMappingDemo&) = delete;
		~ShadowMappingDemo();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

		const GLsizei mShadowWidth = 1024;
		const GLsizei mShadowHeight = 1024;
	private:
		glm::mat4 mWorldMatrix{ 1 };
		std::unique_ptr<Light> mAmbientLight;
		std::unique_ptr<DirectionalLight> mDirectionalLight;
		std::unique_ptr<PointLight> mPointLight;
		std::unique_ptr<ProxyModel> mDirectionalLightProxy;
		std::unique_ptr<ProxyModel> mPointLightProxy;
		std::unique_ptr<Plane> mPlane;
		glm::vec4 mSpecularColor = ColorHelper::White;

		ShadowMappingEffect mShaderProgram;
		ShaderProgram mDepthShaderProgram;
		ShaderProgram mDebugShaderProgram;
		// Shadow mapping related variables
		const float mNearPlane = 0.1f;
		const float mFarPlane = 20.0f;
		glm::vec3 mDirectionalLightPosition;
		glm::mat4 mDirectionalLightProjection;
		glm::mat4 mDirectionalLightView;
		glm::mat4 mDirectionalLightSpaceMatrix;
		bool mDirectionalLightChanged = true;

		// For shader program
		GLuint mVAO = 0;
		GLuint mVBO = 0;
		GLuint mIBO = 0;
		GLuint mFBO = 0;
		GLuint mDepthMap = 0;
		size_t mIndexCount = 0;
		GLuint mColorTexture = 0;
		float mSpecularPower = 16.0f;

		// Uniforms
		GLint mDirectionalLightDirectionLocation = -1;
		GLint mDirectionalLightColorLocation = -1;
		GLint mPointLightColorLocation = -1;
		GLint mPointLightPositionLocation = -1;
		GLint mPointLightRadiusLocation = -1;

		inline static const float sLightMovementRate = 10.0f;
		inline static const glm::vec2 sLightRotationRate{ glm::two_pi<float>(), glm::two_pi<float>() };

		void UpdateAmbientLight(const GameTime& gameTime);
		void UpdateSpecularLight(const GameTime& gameTime);
		void UpdateDirectionalLight(const GameTime& gameTime);
		void UpdatePointlight(const GameTime& gameTime);

		void CreateFrameBuffer();
		void DrawDebugDepthMap();
		GLuint mDebugVAO = 0;
		GLuint mDebugVBO = 0;
	};
}

