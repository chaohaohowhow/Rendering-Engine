#include "pch.h"
#include "RenderingGame.h"

#include "Fountain.h"
#include "ColorHelper.h"
#include "FirstPersonCamera.h"
#include "HelperMacros.h"
#include "VectorHelper.h"
#include "ScreenQuad.h"
#include "Skybox.h"

using namespace std;
using namespace glm;

namespace Rendering
{
	RTTI_DEFINITIONS(RenderingGame);

	RenderingGame::RenderingGame(const std::string& windowTitle) :
		Game(windowTitle)
	{
		mDepthStencilBufferEnabled = true;
		mColorBuffers.resize(2);
		mIntermediateFBOs.resize(2);
		mIntermediateColorBuffers.resize(2);
	}
	RenderingGame::~RenderingGame()
	{
		GLCall(glDeleteFramebuffers(1, &mFBO));
		GLCall(glDeleteFramebuffers(2, &mIntermediateFBOs[0]));
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

		CreateFrameBuffers();

		// Adding an object
		shared_ptr<Fountain> fountain = make_shared<Fountain>(*this, camera);
		mComponents.push_back(fountain);

		auto skybox = make_shared<Skybox>(*this, camera, "Content/Textures/Skybox/posx.jpg", "Content/Textures/Skybox/negx.jpg", "Content/Textures/Skybox/posy.jpg", "Content/Textures/Skybox/negy.jpg", "Content/Textures/Skybox/posz.jpg", "Content/Textures/Skybox/negz.jpg", 100.0f);
		mComponents.push_back(skybox);

		// Post-processing screen quad
		mScreenQuad = make_shared<ScreenQuad>(*this, camera);

		// Gaussian Blur effect
		mGaussianBlur = make_shared<GaussianBlur>(*this, camera);

		Game::Initialize();
		mScreenQuad->Initialize();
		mScreenQuad->SetTextureBuffer(mColorBuffers[0]);
		mScreenQuad->SetBloomBlur(mIntermediateColorBuffers[1]);

		mGaussianBlur->Initialize();
		mGaussianBlur->SetTextureBuffer(mColorBuffers[1]);
		mGaussianBlur->SetFrameBuffers(&mIntermediateFBOs);
		mGaussianBlur->SetIntermediateColorBuffers(&mIntermediateColorBuffers);
		camera->SetPosition(0, 6, 10);
		camera->ApplyRotation(rotate(mat4(1), radians(30.0f), Vector3Helper::Left));
	}
	void RenderingGame::Update(const GameTime& gameTime)
	{
		Game::Update(gameTime);
		mScreenQuad->Update(gameTime);
	}
	void RenderingGame::Draw(const GameTime& gameTime)
	{
		// Rendering the scene onto a render target
		static const GLfloat one = 1.0f;
		GLCall(glClearBufferfv(GL_COLOR, 0, value_ptr(ColorHelper::CornflowerBlue)));
		GLCall(glClearBufferfv(GL_DEPTH, 0, &one));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		GLCall(glClearBufferfv(GL_COLOR, 0, value_ptr(ColorHelper::CornflowerBlue)));
		GLCall(glClearBufferfv(GL_DEPTH, 0, &one));

		Game::Draw(gameTime);

		// Do a 2-pass Gaussian Blur
		//mGaussianBlur->Draw(gameTime);

		// Draw the render target
		//mScreenQuad->Draw(gameTime);
		glfwSwapBuffers(mWindow);
	}
	void RenderingGame::CreateFrameBuffers()
	{
		// Create frame buffer
		GLCall(glGenFramebuffers(1, &mFBO));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, mFBO));
		// Color attachment textures
		GLCall(glGenTextures(2, &mColorBuffers[0]));
		for (size_t i = 0; i < 2; ++i)
		{
			GLCall(glBindTexture(GL_TEXTURE_2D, mColorBuffers[i]));
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, ScreenWidth(), ScreenHeight(), 0, GL_RGBA, GL_FLOAT, nullptr));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)); 
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
			GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, static_cast<GLenum>(GL_COLOR_ATTACHMENT0 + i), GL_TEXTURE_2D, mColorBuffers[i], 0));
		}
		// render buffer object for depth and stencil tests
		GLuint RBO;
		GLCall(glGenRenderbuffers(1, &RBO));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, RBO));
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, ScreenWidth(), ScreenHeight()));
		GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO));
		// Tell OpenGL what color attachments to draw to
		GLuint colorAttachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		GLCall(glDrawBuffers(2, colorAttachments));
		// Check if complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			throw runtime_error("Frame buffer is not complete!");
		}
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

		// Intermediate frame buffer
		GLCall(glGenFramebuffers(2, &mIntermediateFBOs[0]));
		GLCall(glGenTextures(2, &mIntermediateColorBuffers[0]));
		for (size_t i = 0; i < 2; ++i)
		{
			GLCall(glBindFramebuffer(GL_FRAMEBUFFER, mIntermediateFBOs[i]));
			GLCall(glBindTexture(GL_TEXTURE_2D, mIntermediateColorBuffers[i]));
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, ScreenWidth(), ScreenHeight(), 0, GL_RGBA, GL_FLOAT, nullptr));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
			GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, static_cast<GLenum>(GL_COLOR_ATTACHMENT0 + i), GL_TEXTURE_2D, mIntermediateColorBuffers[i], 0));
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			{
				throw runtime_error("Intermediate frame buffer not complete!");
			}
		}
	}
}