#pragma once
#include "GameComponent.h"
#include "VectorHelper.h"

namespace Library
{
	class Game;
	class GameTime;

	class Camera : public GameComponent
	{
		RTTI_DECLARATIONS(Camera, GameComponent);

	public:
		// Special Members
		Camera(Game& game);
		Camera(Game& game, float nearPlaneDistance, float farPlaneDistance);
		Camera(const Camera&) = default;
		Camera& operator=(const Camera&) = default;
		Camera(Camera&&) = default;
		Camera& operator=(Camera&&) = default;
		virtual ~Camera() = default;

		// Getters
		inline const glm::vec3& Position() const { return mPosition; }
		inline const glm::vec3& Direction() const { return mDirection; }
		inline const glm::vec3& Up() const { return mUp; }
		inline const glm::vec3& Right() const { return mRight; }
		inline float NearPlaneDistance() const { return mNearPlaneDistance; }
		inline float FarPlaneDistance() const { return mFarPlaneDistance; }
		inline const glm::mat4 ViewMatrix() const { return mViewMatrix; }
		inline const glm::mat4 ProjectionMatrix() const { return mProjectionMatrix; }

		// Setters
		virtual void SetPosition(float x, float y, float z);
		virtual void SetPosition(const glm::vec3 position);

		virtual void Reset();
		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void UpdateViewMatrix();
		virtual void UpdateProjectionMatrix() = 0;
		virtual void ApplyRotation(const glm::mat4& transform);

		inline static const float sDefaultNearPlaneDistance = 0.01f;
		inline static const float sDefaultFarPlaneDistance = 1000.0f;

	protected:
		float mNearPlaneDistance;
		float mFarPlaneDistance;

		// Member variables
		glm::vec3 mPosition = Vector3Helper::Zero;
		glm::vec3 mDirection = Vector3Helper::Forward;
		glm::vec3 mUp = Vector3Helper::Up;
		glm::vec3 mRight = Vector3Helper::Right;
		glm::mat4 mViewMatrix{ 1 };
		glm::mat4 mProjectionMatrix{ 1 };
		bool mViewMatrixDataDirty = true;
		bool mProjectionMatrixDataDirty = true;
	};
}

