#pragma once
#include "Camera.h"
namespace Library
{
	class PerspectiveCamera : public Camera
	{
		RTTI_DECLARATIONS(PerspectiveCamera, Camera);

	public:
		PerspectiveCamera(Game& game, float fieldOfView = sDefaultFieldOfView, float aspectRatio = sDefaultAspectRatio, float nearPlaneDistance = sDefaultNearPlaneDistance, float farPlaneDistance = sDefaultFarPlaneDistance);
		PerspectiveCamera(const PerspectiveCamera&) = default;
		PerspectiveCamera& operator=(const PerspectiveCamera&) = default;
		PerspectiveCamera(PerspectiveCamera&&) = default;
		PerspectiveCamera& operator=(PerspectiveCamera&&) = default;
		virtual ~PerspectiveCamera() = default;

		// Getters and setters
		inline float AspectRatio() const { return mAspectRatio; }
		inline void SetAspectRatio(float aspectRatio) { mAspectRatio = aspectRatio; mProjectionMatrixDataDirty = true; }
		inline float FieldOfView() const { return mFieldOfView; }
		inline void SetFieldOfView(float fieldOfView) { mFieldOfView = fieldOfView; mProjectionMatrixDataDirty = true; }

		virtual void UpdateProjectionMatrix() override;

		inline static const float sDefaultFieldOfView = 45.0f;
		inline static const float sDefaultAspectRatio = 4.0f / 3.0f;
	protected:
		float mFieldOfView;
		float mAspectRatio;
	};
}

