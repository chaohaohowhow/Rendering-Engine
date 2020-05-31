#include "pch.h"
#include "RenderingGame.h"

#include "ColoredTriangle.h"
#include "ColorHelper.h"
#include "FirstPersonCamera.h"
#include "HelperMacros.h"
#include "VectorHelper.h"

using namespace std;
using namespace glm;

namespace Rendering
{
	RTTI_DEFINITIONS(RenderingGame);

	RenderingGame::RenderingGame(const std::string& windowTitle) :
		Game(windowTitle)
	{
	}
	void RenderingGame::Initialize()
	{
		// Adding camera
		shared_ptr<Camera> camera = make_shared<FirstPersonCamera>(*this);
		mComponents.push_back(camera);
		
		// Adding escape keyboard control
		AddKeyBoardHandler([&](int key, int /*scanCode*/, int action, int /*mods*/)
			{
				if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
				{
					Exit();
				}
			});

		// Adding the ColoredTriangle
		shared_ptr<GameComponent> triangle = make_shared<ColoredTriangle>(*this, camera);
		mComponents.push_back(triangle);

		Game::Initialize();
		camera->SetPosition(0, 5, 10);
		camera->ApplyRotation(rotate(mat4(1), radians(30.0f), Vector3Helper::Left));
	}
	void RenderingGame::Draw(const GameTime& gameTime)
	{
		GLCall(glClearBufferfv(GL_COLOR, 0, value_ptr(ColorHelper::CornflowerBlue)));
		Game::Draw(gameTime);
		glfwSwapBuffers(mWindow);
	}
}