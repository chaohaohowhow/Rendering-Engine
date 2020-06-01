#include "pch.h"
#include "SpotLight.h"

using namespace glm;

namespace Library
{
	RTTI_DEFINITIONS(SpotLight);

	SpotLight::SpotLight(Game& game) :
		PointLight(game)
	{
	}
	void SpotLight::SetInnerAngle(float value)
	{
		mInnerAngle = value;
	}
	void SpotLight::SetOuterAngle(float value)
	{
		mOuterAngle = value;
	}
	void SpotLight::ApplyRotation(const glm::mat4& transform)
	{
		vec4 direction = transform * vec4(mDirection, 0.0f);
		mDirection = static_cast<vec3>(normalize(direction));

		vec4 up = transform * vec4(mUp, 0.0f);
		mUp = static_cast<vec3>(normalize(up));

		mRight = cross(mDirection, mUp);
		mUp = cross(mRight, mDirection);
	}
}