#include "pch.h"
#include "PointLight.h"

using namespace glm;

namespace Library
{
	RTTI_DEFINITIONS(PointLight);

	PointLight::PointLight(Game& game) :
		Light(game)
	{
	}

	void PointLight::SetPosition(float x, float y, float z)
	{
		mPosition = vec3(x, y, z);
	}
	void PointLight::SetPosition(const glm::vec3& position)
	{
		mPosition = position;
	}
	void PointLight::SetRadius(float radius)
	{
		mRadius = radius;
	}
}