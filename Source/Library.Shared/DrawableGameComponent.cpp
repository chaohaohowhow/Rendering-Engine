#include "pch.h"
#include "DrawableGameComponent.h"

namespace Library
{
	RTTI_DEFINITIONS(DrawableGameComponent);

	DrawableGameComponent::DrawableGameComponent(Game& game, std::shared_ptr<Camera> camera) :
		GameComponent(game), mCamera(move(camera))
	{
	}
}