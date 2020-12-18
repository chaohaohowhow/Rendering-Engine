#include "pch.h"
#include "ShadowMappingDemo.h"

#include "DirectionalLight.h"
#include "FirstPersonCamera.h"
#include "Game.h"
#include "HelperMacros.h"
#include "Model.h"
#include "PointLight.h"
#include "VertexDeclarations.h"
#include "ModelMaterial.h"
#include "Plane.h"
#include "ProxyModel.h"
#include "TextureHelper.h"

using namespace std;
using namespace glm;



namespace Library
{
	RTTI_DEFINITIONS(ShadowMappingDemo);

	ShadowMappingDemo::ShadowMappingDemo(Game& game, std::shared_ptr<Camera> camera) :
		DrawableGameComponent(game, move(camera))
	{
	}

	ShadowMappingDemo::~ShadowMappingDemo()
	{
		GLCall(glDeleteBuffers(1, &mVBO));
		GLCall(glDeleteBuffers(1, &mIBO));
		GLCall(glDeleteVertexArrays(1, &mVAO));
		GLCall(glDeleteFramebuffers(1, &mFBO));
		GLCall(glDeleteTextures(1, &mDepthMap));
	}

	void ShadowMappingDemo::Initialize()
	{
		// Build Shader programs
		vector<ShaderDefinition> shaders;
		shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content\\Shaders\\ShadowMapping.vert"));
		shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content\\Shaders\\ShadowMapping.frag"));
		mShaderProgram.BuildProgram(shaders);
		shaders.clear();
		shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content\\Shaders\\DepthShader.vert"));
		shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content\\Shaders\\DepthShader.frag"));
		mDepthShaderProgram.BuildProgram(shaders);
		shaders.clear();
		shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content\\Shaders\\DebugDepthShader.vert"));
		shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content\\Shaders\\DebugDepthShader.frag"));
		mDebugShaderProgram.BuildProgram(shaders);

		// Load the model
		Model model("Content\\Models\\fountain2.obj.bin");

		// Create vertex and index buffer
		auto& mesh = model.Meshes().at(0);
		VertexPositionTextureNormal::CreateVertexBuffer(*mesh, mVBO);
		mesh->CreateIndexBuffer(mIBO);
		mIndexCount = mesh->Indices().size();

		// Loading fountain texture
		mColorTexture = TextureHelper::LoadTexture("Content/Textures/fountainBaseColor.png", GL_REPEAT, GL_LINEAR);

		// Create the vertex array object
		GLCall(glGenVertexArrays(1, &mVAO));
		mShaderProgram.Initialize(mVAO);
		GLCall(glBindVertexArray(0));

		CreateFrameBuffer();

		// Get some uniforms. The other ones are initialized in the shader program
		GLCall(mDirectionalLightDirectionLocation = glGetUniformLocation(mShaderProgram.Program(), "DirectionalLight.Direction"));
		GLCall(mDirectionalLightColorLocation = glGetUniformLocation(mShaderProgram.Program(), "DirectionalLight.Color"));
		GLCall(mPointLightColorLocation = glGetUniformLocation(mShaderProgram.Program(), "PointLight.Color"));
		GLCall(mPointLightPositionLocation = glGetUniformLocation(mShaderProgram.Program(), "PointLight.Position"));
		GLCall(mPointLightRadiusLocation = glGetUniformLocation(mShaderProgram.Program(), "PointLight.Radius"));

		if (mDirectionalLightDirectionLocation == -1 || 
			mDirectionalLightColorLocation == -1 ||
			mPointLightColorLocation == -1 ||
			mPointLightPositionLocation == -1 ||
			mPointLightRadiusLocation == -1)
		{
			throw runtime_error("glGetUniformLocation() did not find uniform locations!");
		}

		// Initialize lights
		mAmbientLight = make_unique<Light>(*mGame);
		mAmbientLight->SetColor(vec4(vec3(0.0f), 1.0f));
		// Directional light
		mDirectionalLight = make_unique<DirectionalLight>(*mGame);
		// Point Light
		mPointLight = make_unique<PointLight>(*mGame);
		mPointLight->SetPosition(-2.0f, 3.0f, -1.0f);
		mPointLight->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		mPointLight->SetRadius(20.0f);

		mWorldMatrix = scale(mWorldMatrix, glm::vec3(0.01f, 0.01f, 0.01f));	
		
		// Initialize plane
		mPlane = make_unique<Plane>(*mGame, mCamera);
		mPlane->Initialize();
		mPlane->SetWorldMatrix(scale(mPlane->WorldMatrix(), vec3(10.0f, 1.0f, 10.0f)));

		mDirectionalLightPosition = vec3(0.0f, 5.0f, 0.0f);
		// Initialize proxy models
		mDirectionalLightProxy = make_unique<ProxyModel>(*mGame, mCamera, "Content\\Models\\DirectionalLightProxy.obj.bin", 0.5f);
		mDirectionalLightProxy->Initialize();
		mDirectionalLightProxy->SetPosition(mDirectionalLightPosition);
		mDirectionalLightProxy->ApplyRotation(rotate(mat4(1), radians(90.0f), Vector3Helper::Up));
		
		mPointLightProxy = make_unique<ProxyModel>(*mGame, mCamera, "Content\\Models\\PointLightProxy.obj.bin", 0.5f);
		mPointLightProxy->Initialize();
		mPointLightProxy->SetPosition(mPointLight->Position());

		mDirectionalLightProjection = ortho(-10.0f, 10.0f, -10.0f, 10.0f, mNearPlane, mFarPlane);
		mDirectionalLightView = lookAt(mDirectionalLightPosition, mDirectionalLightPosition + mDirectionalLight->Direction(), Vector3Helper::Up);
		mDirectionalLightSpaceMatrix = mDirectionalLightProjection * mDirectionalLightView;
	}

	void ShadowMappingDemo::Update(const GameTime& gameTime)
	{
		// Rotates the object
		mWorldMatrix = rotate(mWorldMatrix, gameTime.ElapsedGameTimeSeconds().count() / 2.0f, Vector3Helper::Up);

		UpdateAmbientLight(gameTime);
		UpdateSpecularLight(gameTime);
		UpdateDirectionalLight(gameTime);
		UpdatePointlight(gameTime);

		mPointLightProxy->Update(gameTime);
		mDirectionalLightProxy->Update(gameTime);
	}

	void ShadowMappingDemo::Draw(const GameTime& gameTime)
	{
		// Render scene from light's perspective
		// -----------------------------------------
		GLCall(glCullFace(GL_FRONT));
		mDepthShaderProgram.Use();
		GLCall(glUniformMatrix4fv(glGetUniformLocation(mDepthShaderProgram.Program(), "LightSpaceMatrix"), 1, GL_FALSE, value_ptr(mDirectionalLightSpaceMatrix)));

		GLCall(glViewport(0, 0, mShadowWidth, mShadowHeight));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, mFBO));
		GLCall(glClear(GL_DEPTH_BUFFER_BIT));

		GLCall(glUniformMatrix4fv(glGetUniformLocation(mDepthShaderProgram.Program(), "World"), 1, GL_FALSE, value_ptr(mWorldMatrix)));
		GLCall(glBindVertexArray(mVAO));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, mVBO));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO));
		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, mColorTexture));
		GLCall(glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndexCount), GL_UNSIGNED_INT, 0));

		GLCall(glBindVertexArray(0));

		GLCall(glUniformMatrix4fv(glGetUniformLocation(mDepthShaderProgram.Program(), "World"), 1, GL_FALSE, value_ptr(mPlane->WorldMatrix())));
		mPlane->Draw(gameTime);
		GLCall(glCullFace(GL_BACK));	
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

		// Rendering scene as normal
		// ------------------------------------------------------
		GLCall(glViewport(0, 0, mGame->ScreenWidth(), mGame->ScreenHeight()));
		GLCall(glClearBufferfv(GL_COLOR, 0, value_ptr(ColorHelper::CornflowerBlue)));
		GLCall(glClear(GL_DEPTH_BUFFER_BIT));
		mShaderProgram.Use();

		mShaderProgram.Projection() << mCamera->ProjectionMatrix();
		mShaderProgram.View() << mCamera->ViewMatrix();
		mShaderProgram.World() << mWorldMatrix;
		mShaderProgram.AmbientColor() << mAmbientLight->Color();
		mShaderProgram.CameraPosition() << mCamera->Position();
		mShaderProgram.SpecularColor() << mSpecularColor;
		mShaderProgram.SpecularPower() << mSpecularPower;
		mShaderProgram.LightSpaceMatrix() << mDirectionalLightSpaceMatrix;
		GLCall(glUniform3fv(mDirectionalLightDirectionLocation, 1, value_ptr(-mDirectionalLight->Direction())));
		GLCall(glUniform4fv(mDirectionalLightColorLocation, 1, value_ptr(mDirectionalLight->Color())));
		GLCall(glUniform4fv(mPointLightColorLocation, 1, value_ptr(mPointLight->Color())));
		GLCall(glUniform3fv(mPointLightPositionLocation, 1, value_ptr(mPointLight->Position())));
		static float radius = mPointLight->Radius();
		GLCall(glUniform1fv(mPointLightRadiusLocation, 1, &radius));

		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, mColorTexture));
		GLCall(glActiveTexture(GL_TEXTURE1));
		GLCall(glBindTexture(GL_TEXTURE_2D, mDepthMap));
		GLCall(glBindVertexArray(mVAO));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, mVBO));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO));
		GLCall(glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndexCount), GL_UNSIGNED_INT, 0));

		GLCall(glBindVertexArray(0));
		mShaderProgram.World() << mPlane->WorldMatrix();
		mPlane->Draw(gameTime);

		mPointLightProxy->Draw(gameTime);
		mDirectionalLightProxy->Draw(gameTime);

		mDebugShaderProgram.Use();
		GLCall(glUniform1fv(glGetUniformLocation(mDebugShaderProgram.Program(), "NearPlane"), 1, &mNearPlane));
		GLCall(glUniform1fv(glGetUniformLocation(mDebugShaderProgram.Program(), "FarPlane"), 1, &mFarPlane));
		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, mDepthMap));
		DrawDebugDepthMap();
	}


	void ShadowMappingDemo::CreateFrameBuffer()
	{
		// Create frame buffer
		GLCall(glGenFramebuffers(1, &mFBO));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, mFBO));
		// Attach depth map
		GLCall(glGenTextures(1, &mDepthMap));
		GLCall(glBindTexture(GL_TEXTURE_2D, mDepthMap));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mShadowWidth, mShadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
		GLfloat borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLCall(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor));

		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthMap, 0));
		GLCall(glDrawBuffer(GL_NONE));
		GLCall(glReadBuffer(GL_NONE));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	void ShadowMappingDemo::DrawDebugDepthMap()
	{
			if (mDebugVAO == 0)
			{
				float quadVertices[] = {
					// positions        // texture Coords
					-1.0f, -0.4f, 0.0f, 0.0f, 1.0f,
					-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
					-0.4f, -0.4f, 0.0f, 1.0f, 1.0f,
					-0.4f, -1.0f, 0.0f, 1.0f, 0.0f,
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
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glBindVertexArray(0);
	}

	void ShadowMappingDemo::UpdatePointlight(const GameTime& gameTime)
	{
		float elapsedTime = gameTime.ElapsedGameTimeSeconds().count();
		if (glfwGetKey(mGame->Window(), GLFW_KEY_DELETE) && mPointLight->Color().r > 0.0f)
		{
			auto& lightColor = mPointLight->Color();
			mPointLight->SetColor(lightColor.r - elapsedTime, lightColor.g - elapsedTime, lightColor.b - elapsedTime, 1.0f);
		}
		if (glfwGetKey(mGame->Window(), GLFW_KEY_INSERT) && mPointLight->Color().r < 1.f)
		{
			auto& lightColor = mPointLight->Color();
			mPointLight->SetColor(lightColor.r + elapsedTime, lightColor.g + elapsedTime, lightColor.b + elapsedTime, 1.0f);
		}

		vec3 movementAmount = Vector3Helper::Zero;
		if (glfwGetKey(mGame->Window(), GLFW_KEY_KP_4))
		{
			movementAmount.x = -1.0f;
		}

		if (glfwGetKey(mGame->Window(), GLFW_KEY_KP_6))
		{
			movementAmount.x = 1.0f;
		}

		if (glfwGetKey(mGame->Window(), GLFW_KEY_KP_9))
		{
			movementAmount.y = 1.0f;
		}

		if (glfwGetKey(mGame->Window(), GLFW_KEY_KP_3))
		{
			movementAmount.y = -1.0f;
		}

		if (glfwGetKey(mGame->Window(), GLFW_KEY_KP_8))
		{
			movementAmount.z = -1.0f;
		}

		if (glfwGetKey(mGame->Window(), GLFW_KEY_KP_2))
		{
			movementAmount.z = 1.0f;
		}

		vec3 movement = movementAmount * sLightMovementRate * elapsedTime;
		mPointLight->SetPosition(mPointLight->Position() + movement);
		mPointLightProxy->SetPosition(mPointLight->Position());
	}
	void ShadowMappingDemo::UpdateAmbientLight(const GameTime& gameTime)
	{
		// Change ambient intensity
		static float ambientIntensity = mAmbientLight->Color().r;

		if (glfwGetKey(mGame->Window(), GLFW_KEY_PAGE_UP) && ambientIntensity < 1.0f)
		{
			ambientIntensity += gameTime.ElapsedGameTimeSeconds().count();
			ambientIntensity = std::min(ambientIntensity, 1.0f);

			mAmbientLight->SetColor(vec4(static_cast<vec3>(ambientIntensity), 1.0f));
		}

		if (glfwGetKey(mGame->Window(), GLFW_KEY_PAGE_DOWN) && ambientIntensity > 0.0f)
		{
			ambientIntensity -= gameTime.ElapsedGameTimeSeconds().count();
			ambientIntensity = std::max(ambientIntensity, 0.0f);

			mAmbientLight->SetColor(vec4(static_cast<vec3>(ambientIntensity), 1.0f));
		}
	}
	void ShadowMappingDemo::UpdateSpecularLight(const GameTime& gameTime)
	{
		static float specularIntensity = 1.0f;

		if (glfwGetKey(mGame->Window(), GLFW_KEY_RIGHT_BRACKET) && specularIntensity < 1.0f)
		{
			specularIntensity += gameTime.ElapsedGameTimeSeconds().count();
			specularIntensity = std::min(specularIntensity, 1.0f);

			mSpecularColor = (vec4((vec3)specularIntensity, 1.0f));
		}

		if (glfwGetKey(mGame->Window(), GLFW_KEY_LEFT_BRACKET) && specularIntensity > 0.0f)
		{
			specularIntensity -= gameTime.ElapsedGameTimeSeconds().count();
			specularIntensity = std::max(specularIntensity, 0.0f);

			mSpecularColor = (vec4((vec3)specularIntensity, 1.0f));
		}

		if (glfwGetKey(mGame->Window(), GLFW_KEY_EQUAL) && mSpecularPower < 256.0f)
		{
			mSpecularPower += gameTime.ElapsedGameTime().count() * 0.1f;
		}

		if (glfwGetKey(mGame->Window(), GLFW_KEY_MINUS) && mSpecularPower > 4.0f)
		{
			mSpecularPower -= gameTime.ElapsedGameTime().count() * 0.1f;
		}
	}
	void ShadowMappingDemo::UpdateDirectionalLight(const GameTime& gameTime)
	{
		static float directionalIntensity = 1.0f;
		float elapsedTime = gameTime.ElapsedGameTimeSeconds().count();

		// Update directional light intensity		
		if (glfwGetKey(mGame->Window(), GLFW_KEY_HOME) && directionalIntensity < 1.0f)
		{
			directionalIntensity += elapsedTime;
			directionalIntensity = std::min(directionalIntensity, 1.0f);

			mDirectionalLight->SetColor(vec4((vec3)directionalIntensity, 1.0f));
		}
		if (glfwGetKey(mGame->Window(), GLFW_KEY_END) && directionalIntensity > 0.0f)
		{
			directionalIntensity -= elapsedTime;
			directionalIntensity = std::max(directionalIntensity, 0.0f);

			mDirectionalLight->SetColor(vec4((vec3)directionalIntensity, 1.0f));
		}

		// Rotate directional light
		vec2 rotationAmount = Vector2Helper::Zero;
		if (glfwGetKey(mGame->Window(), GLFW_KEY_LEFT))
		{
			rotationAmount.x += sLightRotationRate.x * elapsedTime;
			mDirectionalLightChanged = true;
		}
		if (glfwGetKey(mGame->Window(), GLFW_KEY_RIGHT))
		{
			rotationAmount.x -= sLightRotationRate.x * elapsedTime;
			mDirectionalLightChanged = true;
		}
		if (glfwGetKey(mGame->Window(), GLFW_KEY_UP))
		{
			rotationAmount.y += sLightRotationRate.y * elapsedTime;
			mDirectionalLightChanged = true;
		}
		if (glfwGetKey(mGame->Window(), GLFW_KEY_DOWN))
		{
			rotationAmount.y -= sLightRotationRate.y * elapsedTime;
			mDirectionalLightChanged = true;
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

		// Change directional light position
		vec3 movementAmount = Vector3Helper::Zero;
		if (glfwGetKey(mGame->Window(), GLFW_KEY_J))
		{
			movementAmount.x = -1.0f;
			mDirectionalLightChanged = true;
		}

		if (glfwGetKey(mGame->Window(), GLFW_KEY_L))
		{
			movementAmount.x = 1.0f;
			mDirectionalLightChanged = true;
		}

		if (glfwGetKey(mGame->Window(), GLFW_KEY_U))
		{
			movementAmount.y = 1.0f;
			mDirectionalLightChanged = true;
		}

		if (glfwGetKey(mGame->Window(), GLFW_KEY_O))
		{
			movementAmount.y = -1.0f;
			mDirectionalLightChanged = true;
		}

		if (glfwGetKey(mGame->Window(), GLFW_KEY_I))
		{
			movementAmount.z = -1.0f;
			mDirectionalLightChanged = true;
		}

		if (glfwGetKey(mGame->Window(), GLFW_KEY_K))
		{
			movementAmount.z = 1.0f;
			mDirectionalLightChanged = true;
		}
		vec3 movement = movementAmount * sLightMovementRate * elapsedTime;
		mDirectionalLightPosition += movement;
		mDirectionalLightProxy->SetPosition(mDirectionalLightPosition);

		if (mDirectionalLightChanged)
		{
			mDirectionalLightView = lookAt(mDirectionalLightPosition, mDirectionalLightPosition + mDirectionalLight->Direction(), Vector3Helper::Up);
			mDirectionalLightSpaceMatrix = mDirectionalLightProjection * mDirectionalLightView;
			mDirectionalLightChanged = false;
		}
	}
}