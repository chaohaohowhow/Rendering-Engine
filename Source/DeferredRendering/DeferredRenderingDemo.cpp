#include "pch.h"
#include "DeferredRenderingDemo.h"


#include "Game.h"
#include "Model.h"
#include "Mesh.h"
#include "ModelMaterial.h"
#include "VertexDeclarations.h"
#include "PointLight.h"
#include "DirectionalLight.h"
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
		for (auto& data : mRenderData)
		{
			glDeleteVertexArrays(1, &data.VAO);
			glDeleteBuffers(1, &data.VBO);
			glDeleteBuffers(1, &data.IBO);
			glDeleteTextures(1, &data.Texture);
		}
		glDeleteFramebuffers(1, &mGBuffer);
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
		shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content\\Shaders\\PointLightPass.vert"));
		shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content\\Shaders\\PointLightPass.frag"));
		mPointLightPassProgram.BuildProgram(shaders);

		shaders.clear();
		shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content\\Shaders\\DirectionalLightPass.vert"));
		shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content\\Shaders\\DirectionalLightPass.frag"));
		mDirectionalLightPassProgram.BuildProgram(shaders);

		shaders.clear();
		shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content\\Shaders\\DebugShader.vert"));
		shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content\\Shaders\\DebugShader.frag"));
		mDebugShaderProgram.BuildProgram(shaders);

		// Create frame buffer
		CreateFrameBuffer();

		InitializeQuad();

		Model model("Content\\Models\\breakfast_room.obj.bin");
		mRenderData.reserve(model.Meshes().size());
		for (auto& mesh : model.Meshes())
		{
			RenderData data;
			glGenVertexArrays(1, &data.VAO);
			glBindVertexArray(data.VAO);
			VertexPositionTextureNormal::CreateVertexBuffer(*mesh, data.VBO);
			mesh->CreateIndexBuffer(data.IBO);
			mGBufferPassProgram.Initialize(data.VAO);
			data.IndexCount = mesh->Indices().size();
			data.DiffuseColor = mesh->GetMaterial()->DiffuseColor();
			auto& textures = mesh->GetMaterial()->Textures();

			if (textures.contains(TextureType::Diffuse))
			{
				string fileName = textures.at(TextureType::Diffuse)[0];
				if (!mFileNameToTextureId.contains(fileName))
				{
					GLuint newTexture = SOIL_load_OGL_texture(("Content\\Textures\\" + fileName).c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
					if (newTexture == 0)
					{
						throw runtime_error("SOIL_load_OGL_texture() failed!");
					}
					mFileNameToTextureId.emplace(fileName, newTexture);
					data.Texture = newTexture;
				}
				else
				{
					data.Texture = mFileNameToTextureId.at(fileName);
				}
			}
			mRenderData.emplace_back(data);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			glGenSamplers(1, &mSampler);
			glSamplerParameteri(mSampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glSamplerParameteri(mSampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		
		mGBufferPassProgram.InitializeUniform();

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
		// Initialize lights
		mPointLights.reserve(mPointLightCount);
		for (size_t i = 0; i < mPointLightCount; ++i)
		{
			mPointLights.emplace_back(make_unique<PointLight>(*mGame));
			auto& light = mPointLights.back();
			light->SetColor(ColorHelper::RandomColor());
			light->SetPosition(Random::RandomFloat(sLightPositionRangeX), Random::RandomFloat(sLightPositionRangeY), Random::RandomFloat(sLightPositionRangeZ));
			light->SetRadius(2.0f);
		}

		mDirectionalLight = make_unique<DirectionalLight>(*mGame);

		// Initialize proxy model
		mPointLightProxy = make_unique<ProxyModel>(*mGame, mCamera, "Content\\Models\\PointLightProxy.obj.bin", 0.1f, true);
		mPointLightProxy->Initialize();
		mDirectionalLightProxy = make_unique<ProxyModel>(*mGame, mCamera, "Content\\Models\\DirectionalLightProxy.obj.bin", 0.3f, false, GL_CW);
		mDirectionalLightProxy->Initialize();
		mDirectionalLightProxy->SetPosition(vec3(0.0f, 5.0f, 0.0f));
		mDirectionalLightProxy->ApplyRotation(rotate(mat4(1), radians(90.0f), Vector3Helper::Up));
	}

	void DeferredRenderingDemo::Update(const Library::GameTime& gameTime)
	{
		UpdateSpecularPower(gameTime);
		UpdateAmbientIntensity(gameTime);
		UpdateDirectionalLight(gameTime);

		mDirectionalLightProxy->Update(gameTime);
	}

	void DeferredRenderingDemo::Draw(const Library::GameTime& gameTime)
	{
		// Draw to the gBuffer first
		// --------------------------------------------------------------------

		glBindFramebuffer(GL_FRAMEBUFFER, mGBuffer);

		glDepthMask(GL_TRUE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_BLEND);

		mGBufferPassProgram.Use();
		// Send Uniforms
		mGBufferPassProgram.Projection() << mCamera->ProjectionMatrix();
		mGBufferPassProgram.View() << mCamera->ViewMatrix();
		mGBufferPassProgram.World() << mWorldMatrix;

		glActiveTexture(GL_TEXTURE0);
		for (auto& data : mRenderData)
		{
			glBindVertexArray(data.VAO);
			glBindBuffer(GL_ARRAY_BUFFER, data.VBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.IBO);
			mGBufferPassProgram.UseTexture() << static_cast<int>(data.Texture != 0);
			if (data.Texture == 0)
			{
				mGBufferPassProgram.DiffuseColor() << data.DiffuseColor;
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, data.Texture);
				glBindSampler(0, mSampler);
			}
			glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(data.IndexCount), GL_UNSIGNED_INT, 0);
		}
		glBindSampler(0, 0);

		glDepthMask(GL_FALSE);

		// Directional light pass
		// --------------------------------------------------------------------
		mDirectionalLightPassProgram.Use();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mPositionTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, mAlbedoSpecTexture);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, mNormalTexture);

		mDirectionalLightPassProgram.CameraPosition() << mCamera->Position();
		mDirectionalLightPassProgram.AmbientIntensity() << mAmbientIntensity;
		mDirectionalLightPassProgram.SpecularPower() << mSpecularPower;
		mDirectionalLightPassProgram.DirectionalLightColor() << mDirectionalLight->Color() * mDirectionalLightIntensity;
		mDirectionalLightPassProgram.DirectionalLightDirection() << -mDirectionalLight->Direction();

		RenderQuad();

		// Point light pass
		// --------------------------------------------------------------------
		// Configurations
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		mPointLightPassProgram.Use();
		glBindVertexArray(mSphereVAO);
		glBindBuffer(GL_ARRAY_BUFFER, mSphereVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mSphereIBO);
		if (mShowSphere)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			mPointLightPassProgram.AmbientIntensity() << 0.1f;
		}
		else
		{
			mPointLightPassProgram.AmbientIntensity() << 0.0f;
		}

		// Sending uniforms
		mPointLightPassProgram.CameraPosition() << mCamera->Position();
		mPointLightPassProgram.SpecularPower() << mSpecularPower;
		mPointLightPassProgram.ScreenSize() << vec2(mGame->ScreenWidth(), mGame->ScreenHeight());
		for (size_t i = 0; i < mPointLightCount; ++i)
		{
			auto& light = mPointLights[i];
			mat4 world(1);
			world = translate(world, light->Position());
			world = scale(world, vec3(light->Radius()));
			mPointLightPassProgram.WVP() << mCamera->ViewProjectionMatrix() * world;
			mPointLightPassProgram.PointLightPosition() << light->Position();
			mPointLightPassProgram.PointLightColor() << light->Color();
			mPointLightPassProgram.PointLightRadius() << light->Radius();
			
			// Render sphere
			glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mSphereIndexCount), GL_UNSIGNED_INT, 0);
		}
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		// Render Proxy models
		// --------------------------------------------------------------------
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, mGBuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // Read from FBO and draw to default buffer
		glBlitFramebuffer(0, 0, mGame->ScreenWidth(), mGame->ScreenHeight(), 0, 0, mGame->ScreenWidth(), mGame->ScreenHeight(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		for (size_t i = 0; i < mPointLightCount; ++i)
		{
			mPointLightProxy->SetPosition(mPointLights[i]->Position());
			mPointLightProxy->SetColor(mPointLights[i]->Color());
			mPointLightProxy->Update(gameTime);
			mPointLightProxy->Draw(gameTime);
		}
		mDirectionalLightProxy->Draw(gameTime);
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

	void DeferredRenderingDemo::InitializeQuad()
	{
		float quadVertices[] = {
			// positions			  // texture coordinates
			-1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &mQuadVAO);
		glGenBuffers(1, &mQuadVBO);
		glBindVertexArray(mQuadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, mQuadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

		mDirectionalLightPassProgram.Initialize(mQuadVAO);
		glBindVertexArray(0);
	}

	void DeferredRenderingDemo::RenderQuad()
	{
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

	void DeferredRenderingDemo::UpdateDirectionalLight(const Library::GameTime& gameTime)
	{
		float elapsedTime = gameTime.ElapsedGameTimeSeconds().count();
		// Rotate directional light
		vec2 rotationAmount = Vector2Helper::Zero;
		if (glfwGetKey(mGame->Window(), GLFW_KEY_LEFT))
		{
			rotationAmount.x += sLightRotationRate.x * elapsedTime;
		}
		if (glfwGetKey(mGame->Window(), GLFW_KEY_RIGHT))
		{
			rotationAmount.x -= sLightRotationRate.x * elapsedTime;
		}
		if (glfwGetKey(mGame->Window(), GLFW_KEY_UP))
		{
			rotationAmount.y += sLightRotationRate.y * elapsedTime;
		}
		if (glfwGetKey(mGame->Window(), GLFW_KEY_DOWN))
		{
			rotationAmount.y -= sLightRotationRate.y * elapsedTime;
		}

		mat4 lightRotationMatrix(1);
		if (rotationAmount.x != 0)
		{
			lightRotationMatrix = rotate(mat4(1), rotationAmount.x, Vector3Helper::Up);
		}

		if (rotationAmount.y != 0)
		{
			lightRotationMatrix = rotate(lightRotationMatrix, rotationAmount.y, mDirectionalLight->Right());
		}

		if (rotationAmount.x != 0.0f || rotationAmount.y != 0.0f)
		{
			mDirectionalLight->ApplyRotation(lightRotationMatrix);
			mDirectionalLightProxy->ApplyRotation(lightRotationMatrix);
		}
	}

	void DeferredRenderingDemo::ToggleShowSphere()
	{
		mShowSphere = !mShowSphere;
	}

	void DeferredRenderingDemo::RandomizePointLights()
	{
		for (size_t i = 0; i < mPointLightCount; ++i)
		{
			auto& light = mPointLights.at(i);
			light->SetColor(ColorHelper::RandomColor());
			light->SetPosition(Random::RandomFloat(sLightPositionRangeX), Random::RandomFloat(sLightPositionRangeY), Random::RandomFloat(sLightPositionRangeZ));
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

	void DeferredRenderingDemo::SetPointLightCount(size_t count)
	{
		mPointLightCount = count;
		if (mPointLightCount > mPointLights.size())
		{
			mPointLights.reserve(mPointLightCount);
			for (size_t i = mPointLights.size(); i < mPointLightCount; ++i)
			{
				mPointLights.emplace_back(make_unique<PointLight>(*mGame));
				auto& light = mPointLights.back();
				light->SetColor(ColorHelper::RandomColor());
				light->SetPosition(Random::RandomFloat(sLightPositionRangeX), Random::RandomFloat(sLightPositionRangeY), Random::RandomFloat(sLightPositionRangeZ));
				light->SetRadius(2.0f);
			}
		}
		RandomizePointLights();
	}

}