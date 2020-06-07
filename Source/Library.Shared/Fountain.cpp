#include "pch.h"
#include "Fountain.h"

#include "DirectionalLight.h"
#include "FirstPersonCamera.h"
#include "Game.h"
#include "HelperMacros.h"
#include "Model.h"
#include "PointLight.h"
#include "VertexDeclarations.h"
#include "ModelMaterial.h"

using namespace std;
using namespace glm;

namespace Library
{
	RTTI_DEFINITIONS(Fountain);

	Fountain::Fountain(Game& game, std::shared_ptr<Camera> camera) :
		DrawableGameComponent(game, move(camera))
	{
	}

	Fountain::~Fountain()
	{
		GLCall(glDeleteBuffers(1, &mVBO));
		GLCall(glDeleteBuffers(1, &mIBO));
		GLCall(glDeleteVertexArrays(1, &mVAO));
		GLCall(glDeleteFramebuffers(1, &mFBO));
	}

	void Fountain::Initialize()
	{
		// Build Shader programs
		vector<ShaderDefinition> shaders;
		shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content/Shader.vert"));
		shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content/Shader.frag"));
		mShaderProgram.BuildProgram(shaders);

		// Load the model
		Model model("Content/Models/fountain2.obj", true);

		// Create vertex and index buffer
		auto mesh = model.Meshes().at(0);
		VertexPositionTexture::CreateVertexBuffer(*mesh, mVBO);
		mesh->CreateIndexBuffer(mIBO);
		mIndexCount = mesh->Indices().size();

		// Create frame buffer
		GLCall(glGenFramebuffers(1, &mFBO));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, mFBO));
		// Color attachment texture
		GLCall(glGenTextures(1, &mTextureBuffer));
		GLCall(glBindTexture(GL_TEXTURE_2D, mTextureBuffer));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mGame->ScreenWidth(), mGame->ScreenHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTextureBuffer, 0));
		// render buffer object for depth and stencil
		GLuint RBO;
		GLCall(glGenRenderbuffers(1, &RBO));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, RBO));
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mGame->ScreenWidth(), mGame->ScreenHeight()));
		GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO));
		// Check if complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			throw runtime_error("Frame buffer is not complete!");
		}
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

		// Loading fountain textures
		mColorTexture = SOIL_load_OGL_texture("Content\\Textures\\fountainBaseColor.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		mNormalMap = SOIL_load_OGL_texture("Content\\Textures\\fountainNormal.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		if (mColorTexture == 0 || mNormalMap == 0)
		{
			throw runtime_error("SOIL_load_OGL_texture() failed!");
		}

		// Create the vertex array object
		GLCall(glGenVertexArrays(1, &mVAO));
		mShaderProgram.Initialize(mVAO);
		GLCall(glBindVertexArray(0));

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
		mAmbientLight->SetColor(vec4(static_cast<vec3>(0.2f), 1.0f));
		// Directional light
		mDirectionalLight = make_unique<DirectionalLight>(*mGame);
		mDirectionalLight->ApplyRotation(rotate(mat4(1), radians(-30.0f), mDirectionalLight->Right()));
		mDirectionalLight->ApplyRotation(rotate(mat4(1), radians(30.0f), Vector3Helper::Up));
		mDirectionalLight->FlipDirection();
		// Point Light
		mPointLight = make_unique<PointLight>(*mGame);
		mPointLight->SetPosition(-0.5f, 1.5f, -1.0f);

		mWorldMatrix = scale(mWorldMatrix, glm::vec3(0.01f, 0.01f, 0.01f));		
	}

	void Fountain::Update(const GameTime& gameTime)
	{
		// Rotates the object
		mWorldMatrix = rotate(mWorldMatrix, gameTime.ElapsedGameTimeSeconds().count() / 2.0f, Vector3Helper::Up);

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

		if (glfwGetKey(mGame->Window(), GLFW_KEY_HOME) && mSpecularPower < 256.0f)
		{
			mSpecularPower += gameTime.ElapsedGameTime().count() * 0.1f;
		}

		if (glfwGetKey(mGame->Window(), GLFW_KEY_END) && mSpecularPower > 4.0f)
		{
			mSpecularPower -= gameTime.ElapsedGameTime().count() * 0.1f;
		}
	}

	void Fountain::Draw(const GameTime& /*gameTime*/)
	{
		mShaderProgram.Use();

		mShaderProgram.Projection() << mCamera->ProjectionMatrix();
		mShaderProgram.View() << mCamera->ViewMatrix();
		mShaderProgram.World() << mWorldMatrix;
		mShaderProgram.AmbientColor() << mAmbientLight->Color();
		mShaderProgram.CameraPosition() << mCamera->Position();
		mShaderProgram.SpecularColor() << mSpecularColor;
		mShaderProgram.SpecularPower() << mSpecularPower;

		GLCall(glUniform3fv(mDirectionalLightDirectionLocation, 1, value_ptr(mDirectionalLight->Direction())));
		GLCall(glUniform4fv(mDirectionalLightColorLocation, 1, value_ptr(mDirectionalLight->Color())));
		GLCall(glUniform4fv(mPointLightColorLocation, 1, value_ptr(mPointLight->Color())));
		GLCall(glUniform3fv(mPointLightPositionLocation, 1, value_ptr(mPointLight->Position())));
		static float radius = mPointLight->Radius();
		GLCall(glUniform1fv(mPointLightRadiusLocation, 1, &radius));

		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, mFBO));
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glDepthFunc(GL_LEQUAL));

		GLCall(glEnable(GL_CULL_FACE));
		GLCall(glFrontFace(GL_CCW));

		static const GLfloat one = 1.0f;
		GLCall(glClearBufferfv(GL_COLOR, 0, value_ptr(ColorHelper::CornflowerBlue)));
		GLCall(glClearBufferfv(GL_DEPTH, 0, &one));

		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, mColorTexture));
		GLCall(glActiveTexture(GL_TEXTURE1));
		GLCall(glBindTexture(GL_TEXTURE_2D, mNormalMap));
		GLCall(glBindVertexArray(mVAO));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, mVBO));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO));
		GLCall(glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndexCount), GL_UNSIGNED_INT, 0));

		GLCall(glBindVertexArray(0));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	

}