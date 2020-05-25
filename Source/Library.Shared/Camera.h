#pragma once
namespace Library
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(GLfloat moveSpeed, GLfloat turnSpeed);
		Camera(const Camera&) = default;
		Camera(Camera&&) = default;
		Camera& operator=(const Camera&) = default;
		Camera& operator=(Camera&&) = default;
		~Camera() = default;

		glm::vec3& GetRight();
		glm::vec3& GetFront();
		glm::vec3& GetPosition();
		void KeyControl(bool* keys);
		void MouseControl(GLfloat xChange, GLfloat yChange);
		glm::mat4 CalculateViewMatrix();
	private:
		glm::vec3 mPosition = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 mFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 mUp = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 mRight = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 mWorldUp = mUp;
		GLfloat mPitch = 0.0f;
		GLfloat mYaw = -90.0f;

		GLfloat mMovementSpeed = 0.1f;
		GLfloat mTurnSpeed = 0.1f;

		void Update();
	};
}

