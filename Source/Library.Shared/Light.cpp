#include "pch.h"
#include "Light.h"

using namespace std;
using namespace glm;

namespace Library
{
	RTTI_DEFINITIONS(Light);

	Light::Light(Game& game) :
		GameComponent(game)
	{
	}
	void Light::SetColor(float r, float g, float b, float a)
	{
		SetColor(vec4(r, g, b, a));
	}
	void Light::SetColor(const glm::vec4& color)
	{
		mColor = color;
	}
}