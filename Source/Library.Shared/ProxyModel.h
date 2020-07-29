#pragma once
#include "DrawableGameComponent.h"
#include "BasicEffect.h"
#include "VectorHelper.h"
#include "ColorHelper.h"

namespace Library
{
	class Mesh;

	class ProxyModel final : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(ProxyModel, DrawableGameComponent);

	public:
		ProxyModel(Game& game, std::shared_ptr<Camera> camera, const std::string& modelFileName, float scale = 1.0f, bool allowUniformColor = false, GLenum frontFace = GL_CCW);
		ProxyModel(const ProxyModel&) = delete;
		ProxyModel& operator=(const ProxyModel&) = delete;
		ProxyModel(ProxyModel&&) = delete;
		ProxyModel& operator=(ProxyModel&&) = delete;
		~ProxyModel();

		const glm::vec3& Position() const;
		const glm::vec3& Direction() const;
		const glm::vec3& Up() const;
		const glm::vec3& Right() const;

		bool& DisplayWireframe();

		void SetPosition(float x, float y, float z);
		void SetPosition(const glm::vec3& position);
		void SetColor(const glm::vec4& color);

		void ApplyRotation(const glm::mat4& transform);

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

	private:
		glm::mat4 mWorldMatrix{ 1 };
		glm::mat4 mScaleMatrix{ 1 };
		glm::vec4 mColor = ColorHelper::White;
		glm::vec3 mPosition = Vector3Helper::Zero;
		glm::vec3 mDirection = Vector3Helper::In;
		glm::vec3 mUp = Vector3Helper::Up;
		glm::vec3 mRight = Vector3Helper::Right;
		std::string mModelFileName;
		GLuint mVertexArrayObject = 0;
		GLuint mVertexBuffer = 0;
		GLuint mIndexBuffer = 0;
		GLenum mFrontFace = GL_CCW;
		std::size_t mIndexCount = 0;
		bool mDisplayWireframe = true;
		bool mAllowUniformColor;
		BasicEffect mShaderProgram;
	};
}