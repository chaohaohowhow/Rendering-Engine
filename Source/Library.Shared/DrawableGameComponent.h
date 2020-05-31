#pragma once
#include "GameComponent.h"
namespace Library
{
	class Camera;

	class DrawableGameComponent : public GameComponent
	{
		RTTI_DECLARATIONS(DrawableGameComponent, GameComponent);

	public:
		// Special Members
		DrawableGameComponent() = default;
		explicit DrawableGameComponent(Game& game, std::shared_ptr<Camera> camera = nullptr);
		DrawableGameComponent(const DrawableGameComponent&) = default;
		DrawableGameComponent& operator=(const DrawableGameComponent&) = default;
		DrawableGameComponent(DrawableGameComponent&&) = default;
		DrawableGameComponent& operator=(DrawableGameComponent&&) = default;
		virtual ~DrawableGameComponent() = default;

		// Getters and setters
		inline bool Visible() const { return mVisible; }
		inline void SetVisible(bool visible) { mVisible = visible; }
		inline std::shared_ptr<Camera> GetCamera() const { return mCamera; }
		inline void SetCamera(std::shared_ptr<Camera> camera) { mCamera = std::move(camera); }

		virtual void Draw(const GameTime& gameTime) = 0;

	protected:
		bool mVisible = true;
		std::shared_ptr<Camera> mCamera;
	};
}