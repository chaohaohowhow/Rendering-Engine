#pragma once
#include "Light.h"
#include "VectorHelper.h"
namespace Library
{
	class DirectionalLight : public Light
	{
		RTTI_DECLARATIONS(DirectionalLight, Light);

	public:
		DirectionalLight(Game& game);
		DirectionalLight(const DirectionalLight&) = default;
		DirectionalLight(DirectionalLight&&) = default;
		DirectionalLight& operator=(const DirectionalLight&) = default;
		DirectionalLight& operator=(DirectionalLight&&) = default;
		virtual ~DirectionalLight() = default;

		inline const glm::vec3& Direction() const { return mDirection; }
		inline const glm::vec3& Up() const { return mUp; }
		inline const glm::vec3& Right() const { return mRight; }

		void ApplyRotation(const glm::mat4 transform);
		void FlipDirection();
	private:
		glm::vec3 mDirection = Vector3Helper::In;
		glm::vec3 mUp = Vector3Helper::Up;
		glm::vec3 mRight = Vector3Helper::Right;
	};
}

