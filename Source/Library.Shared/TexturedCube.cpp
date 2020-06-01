#include "pch.h"
#include "TexturedCube.h"

#include "Camera.h"
#include "Game.h"
#include "HelperMacros.h"
#include "Light.h"
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

		const VertexPositionTexture vertices[] =
		{
			// Top
			VertexPositionTexture(vec4(-0.5f, +0.5f, +0.5f, 1.0f), vec2(1.0f / 3.0f, 4.0f / 4.0f)),
			VertexPositionTexture(vec4(+0.5f, +0.5f, +0.5f, 1.0f), vec2(2.0f / 3.0f, 4.0f / 4.0f)),
			VertexPositionTexture(vec4(+0.5f, +0.5f, -0.5f, 1.0f), vec2(2.0f / 3.0f, 3.0f / 4.0f)),
			VertexPositionTexture(vec4(-0.5f, +0.5f, -0.5f, 1.0f), vec2(1.0f / 3.0f, 3.0f / 4.0f)),

			// Front
			VertexPositionTexture(vec4(-0.5f, +0.5f, +0.5f, 1.0f), vec2(1.0f / 3.0f, 3.0f / 4.0f)),
			VertexPositionTexture(vec4(+0.5f, +0.5f, +0.5f, 1.0f), vec2(2.0f / 3.0f, 3.0f / 4.0f)),
			VertexPositionTexture(vec4(+0.5f, -0.5f, +0.5f, 1.0f), vec2(2.0f / 3.0f, 2.0f / 4.0f)),
			VertexPositionTexture(vec4(-0.5f, -0.5f, +0.5f, 1.0f), vec2(1.0f / 3.0f, 2.0f / 4.0f)),

			// Bottom
			VertexPositionTexture(vec4(-0.5f, -0.5f, +0.5f, 1.0f), vec2(1.0f / 3.0f, 2.0f / 4.0f)),
			VertexPositionTexture(vec4(+0.5f, -0.5f, +0.5f, 1.0f), vec2(2.0f / 3.0f, 2.0f / 4.0f)),
			VertexPositionTexture(vec4(+0.5f, -0.5f, -0.5f, 1.0f), vec2(2.0f / 3.0f, 1.0f / 4.0f)),
			VertexPositionTexture(vec4(-0.5f, -0.5f, -0.5f, 1.0f), vec2(1.0f / 3.0f, 1.0f / 4.0f)),

			// Back
			VertexPositionTexture(vec4(-0.5f, -0.5f, -0.5f, 1.0f), vec2(1.0f / 3.0f, 1.0f / 4.0f)),
			VertexPositionTexture(vec4(+0.5f, -0.5f, -0.5f, 1.0f), vec2(2.0f / 3.0f, 1.0f / 4.0f)),
			VertexPositionTexture(vec4(+0.5f, +0.5f, -0.5f, 1.0f), vec2(2.0f / 3.0f, 0.0f / 4.0f)),
			VertexPositionTexture(vec4(-0.5f, +0.5f, -0.5f, 1.0f), vec2(1.0f / 3.0f, 0.0f / 4.0f)),

			// Left
			VertexPositionTexture(vec4(-0.5f, +0.5f, -0.5f, 1.0f), vec2(0.0f / 3.0f, 1.0f / 4.0f)),
			VertexPositionTexture(vec4(-0.5f, -0.5f, -0.5f, 1.0f), vec2(1.0f / 3.0f, 1.0f / 4.0f)),
			VertexPositionTexture(vec4(-0.5f, -0.5f, +0.5f, 1.0f), vec2(1.0f / 3.0f, 2.0f / 4.0f)),
			VertexPositionTexture(vec4(-0.5f, +0.5f, +0.5f, 1.0f), vec2(0.0f / 3.0f, 2.0f / 4.0f)),

			//Right
			VertexPositionTexture(vec4(+0.5f, -0.5f, -0.5f, 1.0f), vec2(2.0f / 3.0f, 2.0f / 4.0f)),
			VertexPositionTexture(vec4(+0.5f, +0.5f, -0.5f, 1.0f), vec2(3.0f / 3.0f, 2.0f / 4.0f)),
			VertexPositionTexture(vec4(+0.5f, +0.5f, +0.5f, 1.0f), vec2(3.0f / 3.0f, 1.0f / 4.0f)),
			VertexPositionTexture(vec4(+0.5f, -0.5f, +0.5f, 1.0f), vec2(2.0f / 3.0f, 1.0f / 4.0f))
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
		GLCall(glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Position), 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTexture), (void*)offsetof(VertexPositionTexture, Position)));
		GLCall(glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Position)));
		GLCall(glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::TextureCoordinate), 2, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTexture), (void*)offsetof(VertexPositionTexture, TextureCoordinates)));
		GLCall(glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::TextureCoordinate)));

		GLCall(glBindVertexArray(0));

		// Get uniforms
		GLCall(mProjectionLocation = glGetUniformLocation(mShaderProgram.Program(), "Projection"));
		GLCall(mViewLocation = glGetUniformLocation(mShaderProgram.Program(), "View"));
		GLCall(mWorldLocation = glGetUniformLocation(mShaderProgram.Program(), "World"));
		GLCall(mAmbientColorLoation = glGetUniformLocation(mShaderProgram.Program(), "AmbientColor"));
		if (mProjectionLocation == -1 || mViewLocation == -1 || mWorldLocation == -1 || mAmbientColorLoation == -1)
		{
			throw runtime_error("glGetUniformLocation() did not find uniform location!");
		}

		// Initialize Ambient Light
		mAmbientLight = make_unique<Light>(*mGame);
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

		GLCall(glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndexCount), GL_UNSIGNED_INT, 0));
		GLCall(glBindVertexArray(0));
	}
}