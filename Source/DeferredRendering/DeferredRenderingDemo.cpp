#include "pch.h"
#include "DeferredRenderingDemo.h"

#include "HelperMacros.h"
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
		GLCall(glDeleteBuffers(1, &mVBO));
		GLCall(glDeleteBuffers(1, &mIBO));
		GLCall(glDeleteVertexArrays(1, &mVAO));
		GLCall(glDeleteFramebuffers(1, &mFBO));
		GLCall(glDeleteTextures(1, &mColorTexture));
		GLCall(glDeleteTextures(1, &mPositionTexture));
		GLCall(glDeleteTextures(1, &mNormalTexture));
		GLCall(glDeleteTextures(1, &mAlbedoSpecTexture));
	}

	void DeferredRenderingDemo::Initialize()
	{
		// Build shader programs
		vector<ShaderDefinition> shaders;
		shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content\\Shaders\\GBuffer.vert"));
		shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content\\Shaders\\GBuffer.frag"));
		mGBufferProgram.BuildProgram(shaders);
		
		shaders.clear();
		shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content\\Shaders\\DeferredRendering.vert"));
		shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content\\Shaders\\DeferredRendering.frag"));
		mShaderProgram.BuildProgram(shaders);

		// Create frame buffer
		CreateFrameBuffer();
		// Load fountain model, VBO, and IBO
		Model model("Content\\Models\\fountain2.obj", true);

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
		
		// Create VAO
		GLCall(glGenVertexArrays(1, &mVAO));
		GLCall(glBindVertexArray(mVAO));
		GLCall(glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Position), 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormal), (void*)offsetof(VertexPositionTextureNormal, Position)));
		GLCall(glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Position)));

		GLCall(glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::TextureCoordinate), 2, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormal), (void*)offsetof(VertexPositionTextureNormal, TextureCoordinates)));
		GLCall(glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::TextureCoordinate)));

		GLCall(glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Normal), 3, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormal), (void*)offsetof(VertexPositionTextureNormal, Normal)));
		GLCall(glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Normal)));

		// Get uniform locations
		GLCall(mGBufferWorldLocation = glGetUniformLocation(mGBufferProgram.Program(), "World"));
		GLCall(mViewLocation = glGetUniformLocation(mGBufferProgram.Program(), "View"));
		GLCall(mProjectionLocation = glGetUniformLocation(mGBufferProgram.Program(), "Projection")); 
		for (size_t i = 0; i < PointLightCount; ++i)
		{
			GLCall(mPointLightPositionLocations[i] = glGetUniformLocation(mShaderProgram.Program(), ("PointLights[" + to_string(i) + "].Position").c_str()));
			GLCall(mPointLightColorLocations[i] = glGetUniformLocation(mShaderProgram.Program(), ("PointLights[" + to_string(i) + "].Color").c_str()));
		}
		GLCall(mCameraPositionLocation = glGetUniformLocation(mShaderProgram.Program(), "CameraPosition"));
		GLCall(mShaderWorldLocation = glGetUniformLocation(mShaderProgram.Program(), "World"));
		GLCall(glBindVertexArray(0));

		mWorldMatrix = scale(mWorldMatrix, glm::vec3(0.01f, 0.01f, 0.0f));
		// Initialize lights
		for (size_t i = 0; i < PointLightCount; ++i)
		{
			mPointLights.emplace_back(make_unique<PointLight>(*mGame));
			auto& light = mPointLights.back();
			light->SetColor(ColorHelper::RandomColor());
			light->SetPosition(Random::RandomFloat(-10.0f, 10.0f), 1.0f, Random::RandomFloat(-10.0f, 10.0f));
		}

		// Initialize plane
		mPlane = make_unique<Plane>(*mGame, mCamera);
		mPlane->Initialize();
		mPlane->SetWorldMatrix(scale(mPlane->WorldMatrix(), vec3(10.0f, 1.0f, 10.0f)));

		// Initialize proxy model
		mPointLightProxy = make_unique<ProxyModel>(*mGame, mCamera, "Content\\Models\\PointLightProxy.obj", 0.1f, true);
		mPointLightProxy->Initialize();
	}

	void DeferredRenderingDemo::Update(const Library::GameTime& gameTime)
	{
		mWorldMatrix = rotate(mWorldMatrix, gameTime.ElapsedGameTimeSeconds().count() / 2.0f, Vector3Helper::Up);
		mPointLightProxy->Update(gameTime);
	}

	void DeferredRenderingDemo::Draw(const Library::GameTime& gameTime)
	{
		// Draw to the gBuffer first
		// --------------------------------------------------------------------
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, mFBO));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		// Send Uniforms
		mGBufferProgram.Use();
		GLCall(glUniformMatrix4fv(mGBufferWorldLocation, 1, GL_FALSE, value_ptr(mWorldMatrix)));
		GLCall(glUniformMatrix4fv(mViewLocation, 1, GL_FALSE, value_ptr(mCamera->ViewMatrix())));
		GLCall(glUniformMatrix4fv(mProjectionLocation, 1, GL_FALSE, value_ptr(mCamera->ProjectionMatrix())));

		GLCall(glBindVertexArray(mVAO));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, mVBO));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO));
		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, mColorTexture));
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		GLCall(glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndexCount), GL_UNSIGNED_INT, 0));

		GLCall(glUniformMatrix4fv(glGetUniformLocation(mGBufferProgram.Program(), "World"), 1, GL_FALSE, value_ptr(mPlane->WorldMatrix())));
		mPlane->Draw(gameTime);

		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		// Render scene with the lighting pass
		// --------------------------------------------------------------------
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		mShaderProgram.Use();
		// Bind all the textures from the gBuffer
		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, mPositionTexture));
		GLCall(glActiveTexture(GL_TEXTURE1));
		GLCall(glBindTexture(GL_TEXTURE_2D, mAlbedoSpecTexture));
		GLCall(glActiveTexture(GL_TEXTURE2));
		GLCall(glBindTexture(GL_TEXTURE_2D, mNormalTexture));
		// Send uniforms
		for (size_t i = 0; i < mPointLights.size(); ++i)
		{
			GLCall(glUniform3fv(mPointLightPositionLocations[i], 1, value_ptr(mPointLights[i]->Position())));
			GLCall(glUniform3fv(mPointLightColorLocations[i], 1, value_ptr(mPointLights[i]->Color())));
		}
		GLCall(glUniform3fv(mCameraPositionLocation, 1, value_ptr(mCamera->Position())));
		GLCall(glUniformMatrix4fv(mShaderWorldLocation, 1, GL_FALSE, value_ptr(mWorldMatrix)));
		RenderQuad();

		// Render Proxy models
		// --------------------------------------------------------------------
		GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, mFBO));
		GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0)); // Read from FBO and draw to default buffer
		GLCall(glBlitFramebuffer(0, 0, mGame->ScreenWidth(), mGame->ScreenHeight(), 0, 0, mGame->ScreenWidth(), mGame->ScreenHeight(), GL_DEPTH_BUFFER_BIT, GL_NEAREST));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

		for (size_t i = 0; i < mPointLights.size(); ++i)
		{
			mPointLightProxy->SetPosition(mPointLights[i]->Position());
			mPointLightProxy->SetColor(mPointLights[i]->Color());
			mPointLightProxy->Update(gameTime);
			mPointLightProxy->Draw(gameTime);
		}
	}

	void DeferredRenderingDemo::CreateFrameBuffer()
	{
		// Create frame buffer
		GLCall(glGenFramebuffers(1, &mFBO));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, mFBO));

		// Position texture
		GLCall(glGenTextures(1, &mPositionTexture));
		GLCall(glBindTexture(GL_TEXTURE_2D, mPositionTexture));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, mGame->ScreenWidth(), mGame->ScreenHeight(), 0, GL_RGBA, GL_FLOAT, nullptr));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mPositionTexture, 0));

		// Color + specular color buffer
		GLCall(glGenTextures(1, &mAlbedoSpecTexture));
		GLCall(glBindTexture(GL_TEXTURE_2D, mAlbedoSpecTexture));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mGame->ScreenWidth(), mGame->ScreenHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mAlbedoSpecTexture, 0));

		// Normal texture
		GLCall(glGenTextures(1, &mNormalTexture));
		GLCall(glBindTexture(GL_TEXTURE_2D, mNormalTexture));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, mGame->ScreenWidth(), mGame->ScreenHeight(), 0, GL_RGBA, GL_FLOAT, nullptr));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, mNormalTexture, 0));

		// tell OpenGL which color attachments to use
		GLenum attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		GLCall(glDrawBuffers(3, attachments));

		// Depth buffer
		GLuint RBO;
		GLCall(glGenRenderbuffers(1, &RBO));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, RBO));
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mGame->ScreenWidth(), mGame->ScreenHeight()));
		GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO));

		// checking for completeness
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			wchar_t error[] = L"Error: frame buffer not complete";
			OutputDebugStringW(error);
			throw runtime_error("Frame buffer not complete!");
		}
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
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
			GLCall(glGenVertexArrays(1, &mQuadVAO));
			GLCall(glGenBuffers(1, &mQuadVBO));
			GLCall(glBindVertexArray(mQuadVAO));
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, mQuadVBO));
			GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW));
			GLCall(glEnableVertexAttribArray(0));
			GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
			GLCall(glEnableVertexAttribArray(1));
			GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))));
		}
		GLCall(glBindVertexArray(mQuadVAO));
		GLCall(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
		GLCall(glBindVertexArray(0));
	}

}