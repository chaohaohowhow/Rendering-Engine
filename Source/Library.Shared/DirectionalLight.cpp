#include "pch.h"
#include "DirectionalLight.h"

using namespace glm;

namespace Library
{
	RTTI_DEFINITIONS(DirectionalLight);
	DirectionalLight::DirectionalLight(Game& game) :
		Light(game)
	{
	}
	void DirectionalLight::ApplyRotation(const glm::mat4 transform)
	{
		vec4 direction = transform * vec4(mDirection, 0.0f);
		mDirection = static_cast<vec3>(normalize(direction));

		vec4 up = transform * vec4(mUp, 0.0f);
		mUp = static_cast<vec3>(normalize(up));

		mRight = cross(mDirection, mUp);
		mUp = cross(mRight, mDirection);
	}
	void DirectionalLight::FlipDirection()
	{
		mDirection = -mDirection;
		mRight = -mRight;
		mUp = -mUp;
	}
}