#pragma once
#include "RTTI.h"
#include "GameClock.h"
#include "GameComponent.h"
#include "GameTime.h"

namespace Library
{
	class Game : public RTTI
	{
		RTTI_DECLARATIONS(Game, RTTI);

	public:
		using KeyboardHandler = std::function<void(int, int, int, int)>;

		// Special Members
		Game(const std::string& windowTitle);
		Game(const Game&) = delete;
		Game& operator=(const Game&) = delete;
		Game(Game&&) = delete;
		Game& operator=(Game&&) = delete;
		virtual ~Game() = default;

		// Getters
		inline GLFWwindow* Window() const { return mWindow; }
		inline bool DepthStencilBufferEnabled() const { return mDepthStencilBufferEnabled; }
		inline const std::string& WindowTitle() const { return mWindowTitle; }
		inline int ScreenWidth() const { return mScreenWidth; }
		inline int ScreenHeight() const { return mScreenHeight; }
		inline float AspectRatio() const { return static_cast<float>(mScreenWidth) / mScreenHeight; }
		inline bool IsFullScreen() const { return mIsFullScreen; }
		inline const std::vector<std::shared_ptr<GameComponent>>& Components() const { return mComponents; }
		//const ServiceContainer& Services() const;

		// Game Essentials
		virtual void Run();
		virtual void Exit();
		virtual void Initialize();
		virtual void Update(const GameTime& gameTime);
		virtual void Draw(const GameTime& gameTime);

		inline void AddKeyBoardHandler(KeyboardHandler handler) { mKeyboardHandlers[&handler] = handler; }
		inline void RemoveKeyboardHandler(KeyboardHandler handler) { mKeyboardHandlers.erase(&handler); }
	protected:
		virtual void InitializeWindow();
		virtual void InitializeOpenGL();
		virtual void ShutDown();

		inline static const std::uint32_t sDefaultWidth = 1920;
		inline static const std::uint32_t sDefaultHeight = 1440;

		// Member variables
		std::string mWindowTitle;
		GLFWwindow* mWindow = nullptr;
		std::uint32_t mScreenWidth;
		std::uint32_t mScreenHeight;
		bool mIsFullScreen = false;
		GLint mMajorVersion = 0;
		GLint mMinorVersion = 0;
		bool mDepthStencilBufferEnabled = false;
		GameClock mGameClock;
		GameTime mGameTime;
		std::vector<std::shared_ptr<GameComponent>> mComponents;
		// ServiceContainer mServices;
		std::map<KeyboardHandler*, KeyboardHandler> mKeyboardHandlers;

	private:
		inline static Game* sInternalInstance = nullptr;
		static void OnKey(GLFWwindow* window, int key, int scanCode, int action, int mods);
		void CenterWindow(GLFWmonitor* monitor);
	};
}

