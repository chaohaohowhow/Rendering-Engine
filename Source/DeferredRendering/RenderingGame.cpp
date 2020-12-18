#include "pch.h"
#include "RenderingGame.h"

#include "DeferredRenderingDemo.h"
#include "ColorHelper.h"
#include "FirstPersonCamera.h"
#include "HelperMacros.h"
#include "VectorHelper.h"
#include "Skybox.h"
#include "ImGuiComponent.h"

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
		camera->SetPosition(0.0f, 0.0f, 2.0f);
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
		mDemo = make_shared<DeferredRenderingDemo>(*this, camera);
		mComponents.push_back(mDemo);

		auto skybox = make_shared<Skybox>(*this, camera, "Content/Textures/Skybox/posx.jpg", "Content/Textures/Skybox/negx.jpg", "Content/Textures/Skybox/posy.jpg", "Content/Textures/Skybox/negy.jpg", "Content/Textures/Skybox/posz.jpg", "Content/Textures/Skybox/negz.jpg", 100.0f);
		mComponents.push_back(skybox);

		auto imGui = make_shared<ImGuiComponent>(*this);
		mComponents.push_back(imGui);

		auto helpTextImGuiRenderBlock = make_shared<ImGuiComponent::RenderBlock>([this]()
			{
				size_t numLights = mDemo->GetPointLightCount();
				stringstream sstream;
				sstream << "Current frame rate: " << setprecision(4);
				sstream << mFrameRate << "frames/s";
				ImGui::SetNextWindowPos(ImVec2(400, 0), ImGuiCond_Appearing);
				ImGui::Begin("Controls");

				ImGui::Text(sstream.str().c_str());
				ImGui::Text("Camera (WASD + Right-Click-Mouse-Look)");
				ImGui::Text("Use arrow keys to control directional light");
				ImGui::Separator();
				if (ImGui::CollapsingHeader("Controls"))
				{
					ImGui::PushItemWidth(200);
					ImGui::SliderFloat("Ambient Intensity", mDemo->GetAmbientIntensityAddress(), 0.0f, 1.0f);
					ImGui::SliderFloat("Directional Light Intensity", &mDemo->mDirectionalLightIntensity, 0.0f, 1.0f);
					ImGui::SliderFloat("Specular Power", mDemo->GetSpecularPowerAddress(), 4.0f, 256.0f);
					ImGui::SliderInt("Light Count", reinterpret_cast<int*>(&numLights), 1, 800);

					if (ImGui::Button("Randomize light"))
						mDemo->RandomizePointLights();
					ImGui::SameLine();
					if (ImGui::Button("Toggle sphere"))
						mDemo->ToggleShowSphere();
				}
				ImGui::End();

				if (numLights != mDemo->GetPointLightCount())
				{
					mDemo->SetPointLightCount(numLights);
				}
			});
		imGui->AddRenderBlock(helpTextImGuiRenderBlock);

		Game::Initialize();

		camera->SetPosition(0, 6, 9);
		camera->ApplyRotation(rotate(mat4(1), radians(30.0f), Vector3Helper::Left));
	}
	void RenderingGame::Update(const GameTime& gameTime)
	{
		Game::Update(gameTime);
		CalculateFrameRate(gameTime);
	}
	void RenderingGame::Draw(const GameTime& gameTime)
	{
		// Rendering the scene onto a render target
		static const GLfloat one = 1.0f;
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		GLCall(glClearBufferfv(GL_COLOR, 0, value_ptr(ColorHelper::CornflowerBlue)));
		GLCall(glClearBufferfv(GL_DEPTH, 0, &one));
		GLCall(glViewport(0, 0, mScreenWidth, mScreenHeight));

		Game::Draw(gameTime);

		glfwSwapBuffers(mWindow);
	}
	void RenderingGame::CalculateFrameRate(const GameTime& gameTime)
	{
		if((mGameTime.TotalGameTime() - mLastTotalTime) >= 1s)
		{
			mLastTotalTime = gameTime.TotalGameTime();
			mFrameRate = mFrameCount;
			mFrameCount = 0;
		}
		++mFrameCount;
	}
}