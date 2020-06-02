#include "pch.h"
#include "TexturedCube.h"

#include "Camera.h"
#include "DirectionalLight.h"
#include "Game.h"
#include "HelperMacros.h"
#include "Light.h"
#include "PointLight.h"
#include "VectorHelper.h"
#include "VertexDeclarations.h"

using namespace std;
using namespace glm;

namespace Library
{
	RTTI_DEFINITIONS(TexturedCube);

	TexturedCube::TexturedCube(Game& game, std::shared_ptr<Camera> camera) :
		DrawableGameComponent(game, std::move(camera))
	{
	}
	TexturedCube::~TexturedCube()
	{
		GLCall(glDeleteBuffers(1, &mVertexBuffer));
		GLCall(glDeleteBuffers(1, &mIndexBuffer));
		GLCall(glDeleteVertexArrays(1, &mVertexArrayObject));
	}
	void TexturedCube::Initialize()
	{
		vector<ShaderDefinition> shaders;
		shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content/Shader.vert"));
		shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content/Shader.frag"));
		mShaderProgram.BuildProgram(shaders);

		const VertexPositionTextureNormal vertices[] =
		{
			// Top
			VertexPositionTextureNormal(vec4(-0.5f, +0.5f, +0.5f, 1.0f), vec2(1.0f / 3.0f, 4.0f / 4.0f), Vector3Helper::Up),
			VertexPositionTextureNormal(vec4(+0.5f, +0.5f, +0.5f, 1.0f), vec2(2.0f / 3.0f, 4.0f / 4.0f), Vector3Helper::Up),
			VertexPositionTextureNormal(vec4(+0.5f, +0.5f, -0.5f, 1.0f), vec2(2.0f / 3.0f, 3.0f / 4.0f), Vector3Helper::Up),
			VertexPositionTextureNormal(vec4(-0.5f, +0.5f, -0.5f, 1.0f), vec2(1.0f / 3.0f, 3.0f / 4.0f), Vector3Helper::Up),

			// Front
			VertexPositionTextureNormal(vec4(-0.5f, +0.5f, +0.5f, 1.0f), vec2(1.0f / 3.0f, 3.0f / 4.0f), Vector3Helper::Out),
			VertexPositionTextureNormal(vec4(+0.5f, +0.5f, +0.5f, 1.0f), vec2(2.0f / 3.0f, 3.0f / 4.0f), Vector3Helper::Out),
			VertexPositionTextureNormal(vec4(+0.5f, -0.5f, +0.5f, 1.0f), vec2(2.0f / 3.0f, 2.0f / 4.0f), Vector3Helper::Out),
			VertexPositionTextureNormal(vec4(-0.5f, -0.5f, +0.5f, 1.0f), vec2(1.0f / 3.0f, 2.0f / 4.0f), Vector3Helper::Out),

			// Bottom
			VertexPositionTextureNormal(vec4(-0.5f, -0.5f, +0.5f, 1.0f), vec2(1.0f / 3.0f, 2.0f / 4.0f), Vector3Helper::Down),
			VertexPositionTextureNormal(vec4(+0.5f, -0.5f, +0.5f, 1.0f), vec2(2.0f / 3.0f, 2.0f / 4.0f), Vector3Helper::Down),
			VertexPositionTextureNormal(vec4(+0.5f, -0.5f, -0.5f, 1.0f), vec2(2.0f / 3.0f, 1.0f / 4.0f), Vector3Helper::Down),
			VertexPositionTextureNormal(vec4(-0.5f, -0.5f, -0.5f, 1.0f), vec2(1.0f / 3.0f, 1.0f / 4.0f), Vector3Helper::Down),

			// Back
			VertexPositionTextureNormal(vec4(-0.5f, -0.5f, -0.5f, 1.0f), vec2(1.0f / 3.0f, 1.0f / 4.0f), Vector3Helper::In),
			VertexPositionTextureNormal(vec4(+0.5f, -0.5f, -0.5f, 1.0f), vec2(2.0f / 3.0f, 1.0f / 4.0f), Vector3Helper::In),
			VertexPositionTextureNormal(vec4(+0.5f, +0.5f, -0.5f, 1.0f), vec2(2.0f / 3.0f, 0.0f / 4.0f), Vector3Helper::In),
			VertexPositionTextureNormal(vec4(-0.5f, +0.5f, -0.5f, 1.0f), vec2(1.0f / 3.0f, 0.0f / 4.0f), Vector3Helper::In),

			// Left
			VertexPositionTextureNormal(vec4(-0.5f, +0.5f, -0.5f, 1.0f), vec2(0.0f / 3.0f, 1.0f / 4.0f), Vector3Helper::Left),
			VertexPositionTextureNormal(vec4(-0.5f, -0.5f, -0.5f, 1.0f), vec2(1.0f / 3.0f, 1.0f / 4.0f), Vector3Helper::Left),
			VertexPositionTextureNormal(vec4(-0.5f, -0.5f, +0.5f, 1.0f), vec2(1.0f / 3.0f, 2.0f / 4.0f), Vector3Helper::Left),
			VertexPositionTextureNormal(vec4(-0.5f, +0.5f, +0.5f, 1.0f), vec2(0.0f / 3.0f, 2.0f / 4.0f), Vector3Helper::Left),

			//Right
			VertexPositionTextureNormal(vec4(+0.5f, -0.5f, -0.5f, 1.0f), vec2(2.0f / 3.0f, 2.0f / 4.0f), Vector3Helper::Right),
			VertexPositionTextureNormal(vec4(+0.5f, +0.5f, -0.5f, 1.0f), vec2(3.0f / 3.0f, 2.0f / 4.0f), Vector3Helper::Right),
			VertexPositionTextureNormal(vec4(+0.5f, +0.5f, +0.5f, 1.0f), vec2(3.0f / 3.0f, 1.0f / 4.0f), Vector3Helper::Right),
			VertexPositionTextureNormal(vec4(+0.5f, -0.5f, +0.5f, 1.0f), vec2(2.0f / 3.0f, 1.0f / 4.0f), Vector3Helper::Right)
		};

		// Vertex buffer
		GLCall(glGenBuffers(1, &mVertexBuffer));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer));
		GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

		const int indicies[] =
		{
			0, 2, 1,
			0, 3, 2,

			4, 6, 5,
			4, 7, 6,

			8, 10, 9,
			8, 11, 10,

			12, 14, 13,
			12, 15, 14,

			16, 18, 17,
			16, 19, 18,

			20, 22, 21, 
			20, 23, 22
		};

		mIndexCount = size(indicies);

		// Index buffer
		GLCall(glGenBuffers(1, &mIndexBuffer));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * mIndexCount, indicies, GL_STATIC_DRAW));

		// Vertex array
		GLCall(glGenVertexArrays(1, &mVertexArrayObject));
		GLCall(glBindVertexArray(mVertexArrayObject));

		// Loading Texture
		mTexture = SOIL_load_OGL_texture("Content\\Textures\\Cube.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB);
		if (mTexture == 0)
		{
			throw runtime_error("SOIL_load_OGL_texture() failed!");
		}
		
		// Attribute Pointers
		GLCall(glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Position), 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormal), (void*)offsetof(VertexPositionTextureNormal, Position)));
		GLCall(glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Position)));
		GLCall(glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::TextureCoordinate), 2, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormal), (void*)offsetof(VertexPositionTextureNormal, TextureCoordinates)));
		GLCall(glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::TextureCoordinate)));
		GLCall(glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Normal), 3, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormal), (void*)offsetof(VertexPositionTextureNormal, Normal)));
		GLCall(glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Normal)));

		GLCall(glBindVertexArray(0));

		// Get uniforms
		GLCall(mProjectionLocation = glGetUniformLocation(mShaderProgram.Program(), "Projection"));
		GLCall(mViewLocation = glGetUniformLocation(mShaderProgram.Program(), "View"));
		GLCall(mWorldLocation = glGetUniformLocation(mShaderProgram.Program(), "World"));
		GLCall(mAmbientColorLoation = glGetUniformLocation(mShaderProgram.Program(), "AmbientColor"));
		GLCall(mDirectionalLightDirectionLocation = glGetUniformLocation(mShaderProgram.Program(), "DirectionalLight.Direction"));
		GLCall(mDirectionalLightColorLocation = glGetUniformLocation(mShaderProgram.Program(), "DirectionalLight.Color"));
		GLCall(mCameraPositionLocation = glGetUniformLocation(mShaderProgram.Program(), "CameraPosition"));
		GLCall(mSpecularColorLocation = glGetUniformLocation(mShaderProgram.Program(), "SpecularColor"));
		GLCall(mSpecularPowerLocation = glGetUniformLocation(mShaderProgram.Program(), "SpecularPower"));
		GLCall(mPointLightColorLocation = glGetUniformLocation(mShaderProgram.Program(), "PointLight.Color"));
		GLCall(mPointLightPositionLocation = glGetUniformLocation(mShaderProgram.Program(), "PointLight.Position"));
		GLCall(mPointLightRadiusLocation = glGetUniformLocation(mShaderProgram.Program(), "PointLight.Radius"));
			if (mProjectionLocation == -1 ||
				mViewLocation == -1 ||
				mWorldLocation == -1 ||
				mAmbientColorLoation == -1 ||
				mDirectionalLightDirectionLocation == -1 ||
				mDirectionalLightColorLocation == -1 ||
				mCameraPositionLocation == -1 ||
				mSpecularColorLocation == -1 ||
				mSpecularPowerLocation == -1 ||
				mPointLightColorLocation == -1 ||
				mPointLightPositionLocation == -1 ||
				mPointLightRadiusLocation == -1)
			{
				throw runtime_error("glGetUniformLocation() did not find uniform location!");
			}

		// Initialize lights
		mAmbientLight = make_unique<Light>(*mGame);
		mAmbientLight->SetColor(vec4(static_cast<vec3>(0.2f), 1.0f));
		// Directional light
		mDirectionalLight = make_unique<DirectionalLight>(*mGame);
		mDirectionalLight->ApplyRotation(rotate(mat4(1), radians(-30.0f), mDirectionalLight->Right()));
		mDirectionalLight->ApplyRotation(rotate(mat4(1), radians(30.0f), Vector3Helper::Up));
		// Point Light
		mPointLight = make_unique<PointLight>(*mGame);
		mPointLight->SetPosition(-0.5f, 1.5f, -1.0f);
	}
	void TexturedCube::Update(const GameTime& gameTime)
	{
		// Rotates the cube
		mWorldMatrix = rotate(mWorldMatrix, gameTime.ElapsedGameTimeSeconds().count(), Vector3Helper::Up);

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
	void TexturedCube::Draw(const GameTime& /*gameTime*/)
	{
		// Bind buffers and vertex array
		GLCall(glBindVertexArray(mVertexArrayObject));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer));
		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, mTexture));

		// Use shader program
		GLCall(glUseProgram(mShaderProgram.Program()));
		
		// Set uniforms
		GLCall(glUniformMatrix4fv(mProjectionLocation, 1, GL_FALSE, value_ptr(mCamera->ProjectionMatrix())));
		GLCall(glUniformMatrix4fv(mViewLocation, 1, GL_FALSE, value_ptr(mCamera->ViewMatrix())));
		GLCall(glUniformMatrix4fv(mWorldLocation, 1, GL_FALSE, value_ptr(mWorldMatrix)));
		GLCall(glUniform4fv(mAmbientColorLoation, 1, value_ptr(mAmbientLight->Color())));
		GLCall(glUniform3fv(mDirectionalLightDirectionLocation, 1, value_ptr(mDirectionalLight->Direction())));
		GLCall(glUniform4fv(mDirectionalLightColorLocation, 1, value_ptr(mDirectionalLight->Color())));
		GLCall(glUniform3fv(mCameraPositionLocation, 1, value_ptr(mCamera->Position())));
		GLCall(glUniform4fv(mSpecularColorLocation, 1, value_ptr(mSpecularColor)));
		GLCall(glUniform1fv(mSpecularPowerLocation, 1, &mSpecularPower));
		GLCall(glUniform4fv(mPointLightColorLocation, 1, value_ptr(mPointLight->Color())));
		GLCall(glUniform3fv(mPointLightPositionLocation, 1, value_ptr(mPointLight->Position())));
		static float radius = mPointLight->Radius();
		GLCall(glUniform1fv(mPointLightRadiusLocation, 1, &radius));

		GLCall(glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndexCount), GL_UNSIGNED_INT, 0));
		GLCall(glBindVertexArray(0));
	}
}