#include "pch.h"
#include "GameComponent.h"

namespace Library
{
	RTTI_DEFINITIONS(GameComponent);

	GameComponent::GameComponent(Game& game) :
		mGame(&game), mEnabled(true)
	{
	}

	void GameComponent::Initialize()
	{
		// This function is left blank intentionally
	}
	void GameComponent::Update(const GameTime& /*gameTime*/)
	{
		// This function is left blank intentionally
	}
}