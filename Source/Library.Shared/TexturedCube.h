#pragma once
#include "DrawableGameComponent.h"
#include "ShaderProgram.h"

namespace Library
{
	class Light;

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
			TextureCoordinate = 1
		};
		
		glm::mat4 mWorldMatrix{ 1 };
		std::unique_ptr<Light> mAmbientLight;

		ShaderProgram mShaderProgram;

		GLuint mVertexArrayObject = 0;
		GLuint mVertexBuffer = 0;
		GLuint mIndexBuffer = 0;
		size_t mIndexCount = 0;
		GLuint mTexture = 0;

		GLint mProjectionLocation = -1;
		GLint mViewLocation = -1;
		GLint mWorldLocation = -1;
		GLint mAmbientColorLoation = -1;
	};
}

