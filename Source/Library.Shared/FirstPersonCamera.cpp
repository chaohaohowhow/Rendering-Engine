#include "pch.h"
#include "FirstPersonCamera.h"

#include "Game.h"
#include "VectorHelper.h"

using namespace glm;
namespace Library
{
	RTTI_DEFINITIONS(FirstPersonCamera);

	FirstPersonCamera::FirstPersonCamera(Game& game) :
		PerspectiveCamera(game)
	{
	}
	FirstPersonCamera::FirstPersonCamera(Game& game, float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance) :
		PerspectiveCamera(game, fieldOfView, aspectRatio, nearPlaneDistance, farPlaneDistance)
	{
	}
	void FirstPersonCamera::Initialize()
	{
		Camera::Initialize();
		glfwGetCursorPos(mGame->Window(), &mLastCursorX, &mLastCursorY);
	}
	void FirstPersonCamera::Update(const GameTime& gameTime)
	{
		bool positionChanged = false;
		vec2 movementAmount = Vector2Helper::Zero;

		if (glfwGetKey(mGame->Window(), GLFW_KEY_W))
		{
			movementAmount.y = 1.0f;
			positionChanged = true;
		}
		if (glfwGetKey(mGame->Window(), GLFW_KEY_A))
		{
			movementAmount.x = -1.0f;
			positionChanged = true;
		}
		if (glfwGetKey(mGame->Window(), GLFW_KEY_S))
		{
			movementAmount.y = -1.0f;
			positionChanged = true;
		}
		if (glfwGetKey(mGame->Window(), GLFW_KEY_D))
		{
			movementAmount.x = 1.0f;
			positionChanged = true;
		}

		vec2 rotationAmount = Vector2Helper::Zero;
		double x, y;
		glfwGetCursorPos(mGame->Window(), &x, &y);

		if (glfwGetMouseButton(mGame->Window(), GLFW_MOUSE_BUTTON_RIGHT))
		{
			rotationAmount.x = static_cast<float>(mLastCursorX - x) * mMouseSensitivity;
			rotationAmount.y = static_cast<float>(mLastCursorY - y) * mMouseSensitivity;
			positionChanged = true;
		}
		mLastCursorX = x;
		mLastCursorY = y;

		if (positionChanged)
		{
			// Rotating camera
			float elapsedTime = gameTime.ElapsedGameTimeSeconds().count();
			vec2 rotationVector = rotationAmount * mRotationRate * elapsedTime;
			mat4 rotationMatrix = rotate(mat4(1), rotationVector.y, mRight);
			rotationMatrix = rotate(rotationMatrix, rotationVector.x, Vector3Helper::Up);
			ApplyRotation(rotationMatrix);

			// Moving camera
			vec2 movement = movementAmount * mMovementRate * elapsedTime;
			mPosition += mRight * movement.x;
			mPosition += mDirection * movement.y;
		}
		
		Camera::Update(gameTime);
	}
}