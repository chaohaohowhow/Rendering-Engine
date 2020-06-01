#include "pch.h"
#include "Camera.h"

using namespace glm;
namespace Library 
{
	RTTI_DEFINITIONS(Camera);

	Camera::Camera(Game& game) :
		GameComponent(game),
		mNearPlaneDistance(sDefaultFarPlaneDistance), mFarPlaneDistance(sDefaultFarPlaneDistance)
	{
	}

	Camera::Camera(Game& game, float nearPlaneDistance, float farPlaneDistance):
		GameComponent(game),
		mNearPlaneDistance(nearPlaneDistance), mFarPlaneDistance(farPlaneDistance)
	{
	}

	void Camera::SetPosition(float x, float y, float z)
	{
		mPosition = vec3(x, y, z);
		mViewMatrixDataDirty = true;
	}

	void Camera::SetPosition(const vec3 position)
	{
		mPosition = position;
		mViewMatrixDataDirty = true;
	}

	void Camera::Reset()
	{
		mPosition = Vector3Helper::Zero;
		mDirection = Vector3Helper::In;
		mUp = Vector3Helper::Up;
		mRight = Vector3Helper::Right;
		mViewMatrixDataDirty = true;

		UpdateViewMatrix();
	}

	void Camera::Initialize()
	{
		UpdateProjectionMatrix();
		Reset();
	}

	void Camera::Update(const GameTime& /*gameTime*/)
	{
		if (mViewMatrixDataDirty)
		{
			UpdateViewMatrix();
		}
	}

	void Camera::UpdateViewMatrix()
	{
		vec3 target = mPosition + mDirection;
		mViewMatrix = lookAt(mPosition, target, mUp);
		mViewMatrixDataDirty = false;
	}

	void Camera::ApplyRotation(const mat4& transform)
	{
		vec4 direction = transform * vec4(mDirection, 0.0f);
		mDirection = (vec3)normalize(direction);

		vec4 up = transform * vec4(mUp, 0.0f);
		mUp = (vec3)normalize(up);

		mRight = cross(mDirection, mUp);
		mUp = cross(mRight, mDirection);
		mViewMatrixDataDirty = true;
	}

}
