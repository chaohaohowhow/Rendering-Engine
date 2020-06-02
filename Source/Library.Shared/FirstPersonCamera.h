#pragma once
#include "PerspectiveCamera.h"

namespace Library
{
	class FirstPersonCamera : public PerspectiveCamera
	{
		RTTI_DECLARATIONS(FirstPersonCamera, PerspectiveCamera);

	public:
		// Special members
		FirstPersonCamera(Game& game);
		FirstPersonCamera(Game& game, float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance);
		FirstPersonCamera(const FirstPersonCamera&) = delete;
		FirstPersonCamera& operator=(const FirstPersonCamera&) = delete;
		FirstPersonCamera(FirstPersonCamera&&) = delete;
		FirstPersonCamera& operator=(FirstPersonCamera&&) = delete;
		virtual ~FirstPersonCamera() = default;

		// Getters and setters
		inline float MouseSensitivity() const { return mMouseSensitivity; }
		inline float RotationRate() const { return mRotationRate; }
		inline float MovementRate() const { return mMovementRate; }
		inline void SetMouseSensitivity(float mouseSensitivity) { mMouseSensitivity = mouseSensitivity; }
		inline void SetRotationRate(float rotationRate) { mRotationRate = rotationRate; }
		inline void SetMovementRate(float movementRate) { mMovementRate = movementRate; }

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		
		inline static const float sDefaultMouseSensitivity = 10.0f;
		inline static const float sDefaultRotationRate = glm::radians(1.0f);
		inline static const float sDefaultMovementRate = 10.f;
	protected:
		float mMouseSensitivity = sDefaultMouseSensitivity;
		float mRotationRate = sDefaultRotationRate;
		float mMovementRate = sDefaultMovementRate;
	private:
		double mLastCursorX = 0.0;
		double mLastCursorY = 0.0;
	};
}

