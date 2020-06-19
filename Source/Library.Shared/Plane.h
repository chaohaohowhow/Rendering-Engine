#pragma once
#include "DrawableGameComponent.h"
namespace Library
{
	struct VertexPositionTextureNormal;

	class Plane : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(Plane, DrawableGameComponent);

	public:
		Plane(Game& game, std::shared_ptr<Camera> camera, bool hasNormal = true);
		Plane(const Plane&) = delete;
		Plane(Plane&&) = delete;
		Plane& operator=(const Plane&) = delete;
		Plane& operator=(Plane&&) = delete;
		~Plane();

		virtual void Initialize() override;
		virtual void Draw(const GameTime& gameTime) override;

		inline const glm::mat4& WorldMatrix() const { return mWorldMatrix; }
		inline void SetWorldMatrix(const glm::mat4& worldMatrix) { mWorldMatrix = worldMatrix; }
	private:
		static const VertexPositionTextureNormal sPlaneVertices[];
		static const GLuint sPlaneIndicies[];
		static const size_t sIndexCount = 6;

		GLuint mVAO = 0;
		GLuint mVBO = 0;
		GLuint mIBO = 0;
		glm::mat4 mWorldMatrix{ 1 };
		bool mHasNormal;
		GLuint mTexture = 0;

		enum class VertexAttribute
		{
			Position = 0,
			TextureCoordinate = 1,
			Normal = 2
		};
	};
}

