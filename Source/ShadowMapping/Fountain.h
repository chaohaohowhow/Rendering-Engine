#pragma once

#include "BloomEffect.h"
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

	class Fountain : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(Fountain, DrawableGameComponent);
	public:
		Fountain(Game& game, std::shared_ptr<Camera> camera);
		Fountain(const Fountain&) = delete;
		Fountain(Fountain&&) = delete;
		Fountain& operator=(const Fountain&) = delete;
		Fountain& operator=(Fountain&) = delete;
		~Fountain();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;
	private:
		glm::mat4 mWorldMatrix{ 1 };
		std::unique_ptr<Light> mAmbientLight;
		std::unique_ptr<DirectionalLight> mDirectionalLight;
		std::unique_ptr<PointLight> mPointLight;
		std::unique_ptr<ProxyModel> mProxyModel;
		std::unique_ptr<Plane> mPlane;
		glm::vec4 mSpecularColor = ColorHelper::White;

		BloomEffect mShaderProgram;

		// For shader program
		GLuint mVAO = 0;
		GLuint mVBO = 0;
		GLuint mIBO = 0;
		size_t mIndexCount = 0;
		GLuint mColorTexture = 0;
		float mSpecularPower = 16.0f;

		// Uniforms
		GLint mDirectionalLightDirectionLocation = -1;
		GLint mDirectionalLightColorLocation = -1;
		GLint mPointLightColorLocation = -1;
		GLint mPointLightPositionLocation = -1;
		GLint mPointLightRadiusLocation = -1;

		inline const static float sLightMovementRate = 10.0f;

		void UpdatePointlight(const GameTime& gameTime);
	};
}

