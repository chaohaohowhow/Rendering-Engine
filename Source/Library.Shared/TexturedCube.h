#pragma once
#include "DrawableGameComponent.h"
#include "ShaderProgram.h"
#include "ColorHelper.h"

namespace Library
{
	class Light;
	class DirectionalLight;
	class PointLight;

	class TexturedCube : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(TexturedCube, DrawableGameComponent);

	public:
		TexturedCube(Game& game, std::shared_ptr<Camera> camera);
		TexturedCube(const TexturedCube&) = delete;
		TexturedCube(TexturedCube&&) = delete;
		TexturedCube& operator=(const TexturedCube&) = delete;
		TexturedCube& operator=(TexturedCube&) = delete;
		~TexturedCube();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;
	private:
		enum class VertexAttribute
		{
			Position = 0,
			TextureCoordinate = 1,
			Normal = 2
		};
		
		glm::mat4 mWorldMatrix{ 1 };
		std::unique_ptr<Light> mAmbientLight;
		std::unique_ptr<DirectionalLight> mDirectionalLight;
		std::unique_ptr<PointLight> mPointLight;
		glm::vec4 mSpecularColor = ColorHelper::White;

		ShaderProgram mShaderProgram;

		GLuint mVertexArrayObject = 0;
		GLuint mVertexBuffer = 0;
		GLuint mIndexBuffer = 0;
		size_t sIndexCount = 0;
		GLuint mTexture = 0;
		float mSpecularPower = 32.0f;

		// Uniforms
		GLint mProjectionLocation = -1;
		GLint mViewLocation = -1;
		GLint mGBufferWorldLocation = -1;
		GLint mAmbientColorLoation = -1;
		GLint mDirectionalLightDirectionLocation = -1;
		GLint mDirectionalLightColorLocation = -1;
		GLint mSpecularColorLocation = -1;
		GLint mSpecularPowerLocation = -1;
		GLint mCameraPositionLocation = -1;
		GLint mPointLightColorLocation = -1;
		GLint mPointLightPositionLocation = -1;
		GLint mPointLightRadiusLocation = -1;
	};
}

