#include "pch.h"
#include "RenderingGame.h"

#include "ShadowMappingDemo.h"
#include "ColorHelper.h"
#include "FirstPersonCamera.h"
#include "HelperMacros.h"
#include "VectorHelper.h"
#include "ScreenQuad.h"
#include "Skybox.h"

using namespace std;
using namespace glm;
using namespace Library;

namespace Rendering
{
	RTTI_DEFINITIONS(RenderingGame);

	RenderingGame::RenderingGame(const std::string& windowTitle) :
		Game(windowTitle)
	{
		mDepthStencilBufferEnabled = true;
	}
	RenderingGame::~RenderingGame()
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


		// Adding an object
		mDemo = make_shared<ShadowMappingDemo>(*this, camera);
		mComponents.push_back(mDemo);

		auto skybox = make_shared<Skybox>(*this, camera, "Content/Textures/Skybox/posx.jpg", "Content/Textures/Skybox/negx.jpg", "Content/Textures/Skybox/posy.jpg", "Content/Textures/Skybox/negy.jpg", "Content/Textures/Skybox/posz.jpg", "Content/Textures/Skybox/negz.jpg", 100.0f);
		mComponents.push_back(skybox);

		Game::Initialize();

		camera->SetPosition(0, 6, 10);
		camera->ApplyRotation(rotate(mat4(1), radians(30.0f), Vector3Helper::Left));
	}
	void RenderingGame::Update(const GameTime& gameTime)
	{
		Game::Update(gameTime);
	}
	void RenderingGame::Draw(const GameTime& gameTime)
	{
		// Rendering the scene onto a render target
		static const GLfloat one = 1.0f;
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		GLCall(glClearBufferfv(GL_COLOR, 0, value_ptr(ColorHelper::CornflowerBlue)));
		GLCall(glClearBufferfv(GL_DEPTH, 0, &one));

		Game::Draw(gameTime);

		glfwSwapBuffers(mWindow);
	}
}