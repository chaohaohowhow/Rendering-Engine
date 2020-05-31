#pragma once
#include "RTTI.h"

namespace Library 
{
	class Game;
	class GameTime;

	class GameComponent : public RTTI
	{
		RTTI_DECLARATIONS(GameComponent, RTTI);
	public:
		// Special Members
		GameComponent() = default;
		explicit GameComponent(Game& game);
		GameComponent(const GameComponent&) = default;
		GameComponent& operator=(const GameComponent&) = default;
		GameComponent(GameComponent&&) = default;
		GameComponent& operator=(GameComponent&&) = default;
		virtual ~GameComponent() = default;

		// Getters and Setters
		gsl::not_null<Game*> GetGame() const { return mGame; }
		void SetGame(Game& game) { mGame = &game; }
		bool Enabled() const { return mEnabled; }
		void SetEnabled(bool enabled) { mEnabled = enabled; }

		virtual void Initialize();
		virtual void Update(const GameTime& gameTime);

	protected:
		gsl::not_null<Game*> mGame;
		bool mEnabled = true;
	};
}

