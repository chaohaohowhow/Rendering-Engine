#include "pch.h"
#include "DeferredRenderingDemo.h"


#include "Game.h"
#include "Model.h"
#include "VertexDeclarations.h"
#include "PointLight.h"
#include "Random.h"
#include "Plane.h"
#include "ProxyModel.h"
#include "Camera.h"
#include "ShaderProgram.h"

using namespace std;
using namespace std::string_literals;
using namespace glm;
using namespace Library;

namespace Rendering
{
	RTTI_DEFINITIONS(DeferredRenderingDemo);

	DeferredRenderingDemo::DeferredRenderingDemo(Library::Game& game, std::shared_ptr<Library::Camera> camera) :
		DrawableGameComponent(game, camera)
	{
	}


	DeferredRenderingDemo::~DeferredRenderingDemo()
	{
		glDeleteBuffers(1, &mVBO);
		glDeleteBuffers(1, &mIBO);
		glDeleteVertexArrays(1, &mVAO);
		glDeleteFramebuffers(1, &mGBuffer);
		glDeleteTextures(1, &mColorTexture);
		glDeleteTextures(1, &mPositionTexture);
		glDeleteTextures(1, &mNormalTexture);
		glDeleteTextures(1, &mAlbedoSpecTexture);
	}

	void DeferredRenderingDemo::Initialize()
	{
		// Build shader programs
		vector<ShaderDefinition> shaders;
		shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content\\Shaders\\GBuffer.vert"));
		shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content\\Shaders\\GBuffer.frag"));
		mGBufferPassProgram.BuildProgram(shaders);
		
		shaders.clear();
		shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content\\Shaders\\DeferredRendering.vert"));
		shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content\\Shaders\\DeferredRendering.frag"));
		mPointLightPassProgram.BuildProgram(shaders);

		shaders.clear();
		shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content\\Shaders\\DebugShader.vert"));
		shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content\\Shaders\\DebugShader.frag"));
		mDebugShaderProgram.BuildProgram(shaders);

		// Create frame buffer
		CreateFrameBuffer();

		// Load fountain model, VBO, and IBO
		Model model("Content\\Models\\fountain2.obj.bin");
		auto& mesh = model.Meshes().at(0);
		VertexPositionTextureNormal::CreateVertexBuffer(*mesh, mVBO);
		mesh->CreateIndexBuffer(mIBO);
		mIndexCount = mesh->Indices().size();

		// Loading fountain texture
		mColorTexture = SOIL_load_OGL_texture("Content\\Textures\\fountainBaseColor.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		if (mColorTexture == 0)
		{
			throw runtime_error("SOIL_load_OGL_texture() failed!");
		}
		
		glGenVertexArrays(1, &mVAO);
		mGBufferPassProgram.Initialize(mVAO);
		glBindVertexArray(0);

		// Load sphere model
		Model sphere("Content\\Models\\UnitSphere.obj.bin");
		auto& sphereMesh = sphere.Meshes().at(0);
		VertexPositionTexture::CreateVertexBuffer(*sphereMesh, mSphereVBO);
		sphereMesh->CreateIndexBuffer(mSphereIBO);
		mSphereIndexCount = sphereMesh->Indices().size();

		glGenVertexArrays(1, &mSphereVAO);
		mPointLightPassProgram.Initialize(mSphereVAO);
		glBindVertexArray(0);

		mDebugTranslateLocation = glGetUniformLocation(mDebugShaderProgram.Program(), "Translate");
		if (mDebugTranslateLocation == -1)
		{
			throw runtime_error("glGetUniformLocation() failed!");
		}

		mWorldMatrix = scale(mWorldMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
		// Initialize lights
		for (size_t i = 0; i < PointLightCount; ++i)
		{
			mPointLights.emplace_back(make_unique<PointLight>(*mGame));
			auto& light = mPointLights.back();
			light->SetColor(ColorHelper::RandomColor());
			light->SetPosition(Random::RandomFloat(-10.0f, 10.0f), 0.5f, Random::RandomFloat(-10.0f, 10.0f));
			light->SetRadius(3.0f);
		}

		// Initialize plane
		mPlane = make_unique<Plane>(*mGame, mCamera);
		mPlane->Initialize();
		mPlane->SetWorldMatrix(scale(mPlane->WorldMatrix(), vec3(10.0f, 1.0f, 10.0f)));

		// Initialize proxy model
		mPointLightProxy = make_unique<ProxyModel>(*mGame, mCamera, "Content\\Models\\PointLightProxy.obj.bin", 0.1f, true);
		mPointLightProxy->Initialize();
		glDisable(GL_BLEND);
	}

	void DeferredRenderingDemo::Update(const Library::GameTime& gameTime)
	{
		mWorldMatrix = rotate(mWorldMatrix, gameTime.ElapsedGameTimeSeconds().count() / 2.0f, Vector3Helper::Up);
		UpdateSpecularPower(gameTime);
		UpdateAmbientIntensity(gameTime);
	}

	void DeferredRenderingDemo::Draw(const Library::GameTime& gameTime)
	{
		// Draw to the gBuffer first
		// --------------------------------------------------------------------

		glBindFramebuffer(GL_FRAMEBUFFER, mGBuffer);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);

		glDepthMask(GL_TRUE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_BLEND);

		mGBufferPassProgram.Use();
		// Send Uniforms
		mGBufferPassProgram.Projection() << mCamera->ProjectionMatrix();
		mGBufferPassProgram.View() << mCamera->ViewMatrix();
		mGBufferPassProgram.World() << mWorldMatrix;

		glBindVertexArray(mVAO);
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mColorTexture);
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndexCount), GL_UNSIGNED_INT, 0);

		glUniformMatrix4fv(glGetUniformLocation(mGBufferPassProgram.Program(), "World"), 1, GL_FALSE, value_ptr(mPlane->WorldMatrix()));

		glFrontFace(GL_CCW);
		mPlane->Draw(gameTime);

		glDepthMask(GL_FALSE);

		// lighting pass
		// --------------------------------------------------------------------
		// Configurations
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mPointLightPassProgram.Use();
		// Bind all the textures from the gBuffer
		glBindVertexArray(mSphereVAO);
		glBindBuffer(GL_ARRAY_BUFFER, mSphereVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mSphereIBO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mPositionTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, mAlbedoSpecTexture);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, mNormalTexture);

		// Sending uniforms
		mPointLightPassProgram.CameraPosition() << mCamera->Position();
		mPointLightPassProgram.SpecularPower() << mSpecularPower;
		mPointLightPassProgram.AmbientIntensity() << mAmbientIntensity;
		for (auto& light : mPointLights)
		{
			mat4 world(1);
			scale(world, vec3(light->Radius(), light->Radius(), light->Radius()));
			translate(world, light->Position());
			mPointLightPassProgram.WVP() << mCamera->ViewProjectionMatrix() * world;
			mPointLightPassProgram.PointLightPosition() << light->Position();
			mPointLightPassProgram.PointLightColor() << light->Color();
			mPointLightPassProgram.PointLightRadius() << light->Radius();
			
			// Render sphere
			glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mSphereIndexCount), GL_UNSIGNED_INT, 0);
		}

		// Render Proxy models
		// --------------------------------------------------------------------
		glDisable(GL_BLEND);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, mGBuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // Read from FBO and draw to default buffer
		glBlitFramebuffer(0, 0, mGame->ScreenWidth(), mGame->ScreenHeight(), 0, 0, mGame->ScreenWidth(), mGame->ScreenHeight(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		for (size_t i = 0; i < mPointLights.size(); ++i)
		{
			mPointLightProxy->SetPosition(mPointLights[i]->Position());
			mPointLightProxy->SetColor(mPointLights[i]->Color());
			mPointLightProxy->Update(gameTime);
			mPointLightProxy->Draw(gameTime);
		}
		// Render debug quads
		// --------------------------------------------------------------------
		mDebugShaderProgram.Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mPositionTexture);
		RenderDebug(0);
		glBindTexture(GL_TEXTURE_2D, mAlbedoSpecTexture);
		RenderDebug(1);
		glBindTexture(GL_TEXTURE_2D, mNormalTexture);
		RenderDebug(2);
	}

	void DeferredRenderingDemo::CreateFrameBuffer()
	{
		// Create frame buffer
		glGenFramebuffers(1, &mGBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, mGBuffer);

		// Position texture
		glGenTextures(1, &mPositionTexture);
		glBindTexture(GL_TEXTURE_2D, mPositionTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, mGame->ScreenWidth(), mGame->ScreenHeight(), 0, GL_RGBA, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mPositionTexture, 0);

		// Color + specular color buffer
		glGenTextures(1, &mAlbedoSpecTexture);
		glBindTexture(GL_TEXTURE_2D, mAlbedoSpecTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mGame->ScreenWidth(), mGame->ScreenHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mAlbedoSpecTexture, 0);

		// Normal texture
		glGenTextures(1, &mNormalTexture);
		glBindTexture(GL_TEXTURE_2D, mNormalTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, mGame->ScreenWidth(), mGame->ScreenHeight(), 0, GL_RGBA, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, mNormalTexture, 0);

		// tell OpenGL which color attachments to use
		GLenum attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(3, attachments);

		// Depth buffer
		GLuint RBO;
		glGenRenderbuffers(1, &RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mGame->ScreenWidth(), mGame->ScreenHeight());
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);

		// checking for completeness
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			wchar_t error[] = L"Error: frame buffer not complete";
			OutputDebugStringW(error);
			throw runtime_error("Frame buffer not complete!");
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void DeferredRenderingDemo::RenderQuad()
	{
		if (mQuadVAO == 0)
		{
			float quadVertices[] = {
				// positions        // texture coordinates
				-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
				 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			};
			// setup plane VAO
			glGenVertexArrays(1, &mQuadVAO);
			glGenBuffers(1, &mQuadVBO);
			glBindVertexArray(mQuadVAO);
			glBindBuffer(GL_ARRAY_BUFFER, mQuadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		}
		glBindVertexArray(mQuadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}

	void DeferredRenderingDemo::RenderDebug(size_t index)
	{
		static const float offsetAmount = 0.4f;
		static const mat4 identity(1);
		if (mDebugVAO == 0)
		{
			float quadVertices[] = {
				// positions        // texture 
				-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, 1.0f - offsetAmount, 0.0f, 0.0f, 0.0f,
				-1.0f + offsetAmount, 1.0f, 0.0f, 1.0f, 1.0f,
				-1.0f + offsetAmount, 1.0f - offsetAmount, 0.0f, 1.0f, 0.0f,
			};
			// setup plane VAO
			glGenVertexArrays(1, &mDebugVAO);
			glGenBuffers(1, &mDebugVBO);
			glBindVertexArray(mDebugVAO);
			glBindBuffer(GL_ARRAY_BUFFER, mDebugVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		}
		glBindVertexArray(mDebugVAO);
		mat4 translateMat = translate(identity, vec3(0, -offsetAmount * index, 0));
		glUniformMatrix4fv(mDebugTranslateLocation, 1, GL_FALSE, value_ptr(translateMat));
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}

	void DeferredRenderingDemo::UpdateSpecularPower(const Library::GameTime& gameTime)
	{

		if (glfwGetKey(mGame->Window(), GLFW_KEY_EQUAL) && mSpecularPower < 256.0f)
		{
			mSpecularPower += gameTime.ElapsedGameTime().count() * 0.1f;
		}

		if (glfwGetKey(mGame->Window(), GLFW_KEY_MINUS) && mSpecularPower > 4.0f)
		{
			mSpecularPower -= gameTime.ElapsedGameTime().count() * 0.1f;
		}
	}

	void DeferredRenderingDemo::UpdateAmbientIntensity(const Library::GameTime& gameTime)
	{
		if (glfwGetKey(mGame->Window(), GLFW_KEY_PAGE_UP) && mAmbientIntensity < 1.0f)
		{
			mAmbientIntensity += gameTime.ElapsedGameTimeSeconds().count();
		}

		if (glfwGetKey(mGame->Window(), GLFW_KEY_PAGE_DOWN) && mAmbientIntensity > 0.0f)
		{
			mAmbientIntensity -= gameTime.ElapsedGameTimeSeconds().count();
		}
	}

	void DeferredRenderingDemo::RandomizePointLights()
	{
		for (size_t i = 0; i < mPointLights.size(); ++i)
		{
			auto& light = mPointLights.at(i);
			light->SetColor(ColorHelper::RandomColor());
			light->SetPosition(Random::RandomFloat(-10.0f, 10.0f), 0.5f, Random::RandomFloat(-10.0f, 10.0f));
		}
	}

	float* DeferredRenderingDemo::GetAmbientIntensityAddress()
	{
		return &mAmbientIntensity;
	}

	float* DeferredRenderingDemo::GetSpecularPowerAddress()
	{
		return &mSpecularPower;
	}

}