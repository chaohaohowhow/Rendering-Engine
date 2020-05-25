#include "pch.h"
#include "Camera.h"

namespace Library {
	Library::Camera::Camera(GLfloat moveSpeed, GLfloat turnSpeed) :
		mMovementSpeed(moveSpeed), mTurnSpeed(turnSpeed)
	{
	}

	glm::vec3& Library::Camera::GetRight()
	{
		return mRight;
	}

	glm::vec3& Library::Camera::GetFront()
	{
		return mFront;
	}

	glm::vec3& Library::Camera::GetPosition()
	{
		return mPosition;
	}

	void Library::Camera::KeyControl(bool* keys)
	{
		if (keys[GLFW_KEY_W])
		{
			mPosition += mFront * mMovementSpeed;
		}
		if (keys[GLFW_KEY_S])
		{
			mPosition -= mFront * mMovementSpeed;
		}
		if (keys[GLFW_KEY_A])
		{
			mPosition -= mRight * mMovementSpeed;
		}
		if (keys[GLFW_KEY_D])
		{
			mPosition += mRight * mMovementSpeed;
		}
	}
	void Camera::MouseControl(GLfloat xChange, GLfloat yChange)
	{
		xChange *= mTurnSpeed;
		yChange *= mTurnSpeed;

		mYaw += xChange;
		mPitch += yChange;

		mPitch = std::clamp(mPitch, -89.0f, 89.0f);

		Update();
	}
	glm::mat4 Camera::CalculateViewMatrix()
	{
		return glm::lookAt(mPosition, mPosition + mFront, mUp);
	}
	void Camera::Update()
	{
		mFront = glm::normalize(
			glm::vec3(
				cos(glm::radians(mYaw)) * cos(glm::radians(mPitch)),
				sin(glm::radians(mPitch)),
				sin(glm::radians(mYaw)) * cos(glm::radians(mPitch))
			));
		mRight = glm::normalize(glm::cross(mFront, mWorldUp));
		mUp = glm::normalize(glm::cross(mRight, mFront));
	}
}
