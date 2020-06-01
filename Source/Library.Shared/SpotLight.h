#pragma once
#include "PointLight.h"
namespace Library
{
	class SpotLight : public PointLight
	{
		RTTI_DECLARATIONS(SpotLight, PointLight);

	public:
		SpotLight(Game& game);
		SpotLight(const SpotLight&) = default;
		SpotLight(SpotLight&&) = default;
		SpotLight& operator=(const SpotLight&) = default;
		SpotLight& operator=(SpotLight&&) = default;
		virtual ~SpotLight() = default;

		inline const glm::vec3& Direction() const { return mDirection; }
		inline const glm::vec3& Up() const { return mUp; }
		inline const glm::vec3& Right() const { return mRight; }

		inline float InnerAngle() const { return mInnerAngle; }
		inline float OuterAngle() const { return mOuterAngle; }

		void SetInnerAngle(float value);
		void SetOuterAngle(float value);

		void ApplyRotation(const glm::mat4& transform);

		inline static const float DefaultInnerAngle = 0.75f;
		inline static const float DefaultOuterAngle = 0.25f;
	protected:
		glm::vec3 mDirection = Vector3Helper::In;
		glm::vec3 mUp = Vector3Helper::Up;
		glm::vec3 mRight = Vector3Helper::Right;
		float mInnerAngle = DefaultInnerAngle;
		float mOuterAngle = DefaultOuterAngle;
	};
}

