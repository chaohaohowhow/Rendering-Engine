#pragma once
#include "DrawableGameComponent.h"
#include "BasicEffect.h"

namespace Library
{
	class Mesh;

	class Skybox final : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(Skybox, DrawableGameComponent);

	public:
		Skybox(Game& game, std::shared_ptr<Camera> camera, const std::string& posXFilename, const std::string& negXFilename, const std::string& posYFilename, const std::string& negYFilename, const std::string& posZFilename, const std::string& negZFilename, float scale);
		Skybox(const Skybox&) = delete;
		Skybox& operator=(const Skybox&) = delete;
		Skybox(Skybox&&) = delete;
		Skybox& operator=(Skybox&&) = delete;
		~Skybox();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

	private:
		glm::mat4 mWorldMatrix{ 0 };
		glm::mat4 mScaleMatrix;

		std::string mPosXFilename;
		std::string mNegXFilename;
		std::string mPosYFilename;
		std::string mNegYFilename;
		std::string mPosZFilename;
		std::string mNegZFilename;

		GLuint mVertexArrayObject = 0;
		GLuint mVertexBuffer = 0;
		GLuint mIndexBuffer = 0;
		std::size_t mIndexCount = 0;
		GLuint mSkyboxTexture = 0;
		BasicEffect mShaderProgram;
	};
}

