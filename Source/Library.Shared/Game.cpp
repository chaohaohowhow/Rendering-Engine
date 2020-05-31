#include "pch.h"
#include "Game.h"

#include "DrawableGameComponent.h"
#include "HelperMacros.h"

using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(Game);

	Game::Game(const std::string& windowTitle) :
		mWindowTitle(windowTitle), mScreenWidth(sDefaultWidth), mScreenHeight(sDefaultHeight)
	{
	}

	void Game::Run()
	{
		sInternalInstance = this;

		InitializeWindow();
		InitializeOpenGL();
		Initialize();

		mGameClock.Reset();

		while (!glfwWindowShouldClose(mWindow))
		{
			mGameClock.UpdateGameTime(mGameTime);
			Update(mGameTime);
			Draw(mGameTime);

			glfwPollEvents();
		}

		ShutDown();
	}

	void Game::Exit()
	{
		glfwSetWindowShouldClose(mWindow, GL_TRUE);
	}
	void Game::Initialize()
	{
		for (shared_ptr<GameComponent>& component : mComponents)
		{
			component->Initialize();
		}
	}
	void Game::Update(const GameTime& gameTime)
	{
		for (shared_ptr<GameComponent>& component : mComponents)
		{
			if (component->Enabled())
			{
				component->Update(gameTime);
			}
		}
	}
	void Game::Draw(const GameTime& gameTime)
	{
		// TODO: find a better way to draw game component than doing dynamic cast
		for (shared_ptr<GameComponent>& component : mComponents)
		{
			DrawableGameComponent* drawableGameComponent = component->As<DrawableGameComponent>();
			if (drawableGameComponent != nullptr && drawableGameComponent->Visible())
			{
				drawableGameComponent->Draw(gameTime);
			}
		}
	}
	void Game::InitializeWindow()
	{
		if (!glfwInit())
		{
			throw runtime_error("glfwInit() failed!");
		}

		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		mWindow = glfwCreateWindow(mScreenWidth, mScreenHeight, mWindowTitle.c_str(), mIsFullScreen ? monitor : nullptr, nullptr);
		if (mWindow == nullptr)
		{
			ShutDown();
			throw runtime_error("glfwCreateWindow() failed!");
		}

		if (!mIsFullScreen)
		{
			CenterWindow(monitor);
		}
	}
	void Game::InitializeOpenGL()
	{
		glfwMakeContextCurrent(mWindow);

		if (glewInit() != GLEW_NO_ERROR)
		{
			throw runtime_error("gl3wInit() failed!");
		}

		GLCall(glGetIntegerv(GL_MAJOR_VERSION, &mMajorVersion));
		GLCall(glGetIntegerv(GL_MINOR_VERSION, &mMajorVersion));

		if (mDepthStencilBufferEnabled)
		{
			GLCall(glEnable(GL_DEPTH_TEST));
			GLCall(glDepthFunc(GL_LEQUAL));
		}

		GLCall(glViewport(0, 0, mScreenWidth, mScreenHeight));

		glfwSetKeyCallback(mWindow, Game::OnKey);
	}
	void Game::ShutDown()
	{
		mComponents.clear();
		mComponents.shrink_to_fit();
		mKeyboardHandlers.clear();
		glfwDestroyWindow(mWindow);
		glfwTerminate();
	}
	void Game::OnKey(GLFWwindow* /*window*/, int key, int scanCode, int action, int mods)
	{
		for (const auto& handler : sInternalInstance->mKeyboardHandlers)
		{
			handler.second(key, scanCode, action, mods);
		}
	}
	void Game::CenterWindow(GLFWmonitor* monitor)
	{
		assert(monitor != nullptr);

		const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);
		if (videoMode == nullptr)
		{
			throw runtime_error("Could not retrieve video mode!");
		}

		int x, y;
		glfwGetMonitorPos(monitor, &x, &y);

		int centerX = x + (videoMode->width - mScreenWidth) / 2;
		int centerY = y + (videoMode->height - mScreenHeight) / 2;

		glfwSetWindowPos(mWindow, centerX, centerY);
	}
}