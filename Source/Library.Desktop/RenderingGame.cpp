#include "pch.h"
#include "RenderingGame.h"

#include "Fountain.h"
#include "ColorHelper.h"
#include "FirstPersonCamera.h"
#include "HelperMacros.h"
#include "VectorHelper.h"
#include "ScreenQuad.h"

using namespace std;
using namespace glm;

namespace Rendering
{
	RTTI_DEFINITIONS(RenderingGame);

	RenderingGame::RenderingGame(const std::string& windowTitle) :
		Game(windowTitle)
	{
		mDepthStencilBufferEnabled = true;
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

		// Adding an object
		shared_ptr<GameComponent> fountain = make_shared<Fountain>(*this, camera);
		mComponents.push_back(fountain);
		
		// Adding the post-processing screen quad
		shared_ptr<GameComponent> screenQuad = make_shared<ScreenQuad>(*this, camera);
		mComponents.push_back(screenQuad);

		// TODO: Find a better way to do post-processing draw calls
		Game::Initialize();
		GLuint textureBuffer = fountain->As<Fountain>()->TextureBuffer();
		screenQuad->As<ScreenQuad>()->SetTextureBuffer(textureBuffer);
		camera->SetPosition(0, 6, 10);
		camera->ApplyRotation(rotate(mat4(1), radians(30.0f), Vector3Helper::Left));
	}
	void RenderingGame::Draw(const GameTime& gameTime)
	{
		static const GLfloat one = 1.0f;
		GLCall(glClearBufferfv(GL_COLOR, 0, value_ptr(ColorHelper::CornflowerBlue)));
		GLCall(glClearBufferfv(GL_DEPTH, 0, &one));
		Game::Draw(gameTime);
		glfwSwapBuffers(mWindow);
	}
}