#include "pch.h"
#include "PerspectiveCamera.h"

namespace Library
{
	RTTI_DEFINITIONS(PerspectiveCamera);
	PerspectiveCamera::PerspectiveCamera(Game& game, float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance) :
		Camera(game, nearPlaneDistance, farPlaneDistance),
		mFieldOfView(fieldOfView), mAspectRatio(aspectRatio)
	{
	}

	void PerspectiveCamera::UpdateProjectionMatrix()
	{
		if (mProjectionMatrixDataDirty)
		{
			mProjectionMatrix = glm::perspective(mFieldOfView, mAspectRatio, mNearPlaneDistance, mFarPlaneDistance);
			mProjectionMatrixDataDirty = false;
		}
	}
}