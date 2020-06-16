#include "pch.h"
#include "ScreenQuad.h"

#include "VertexDeclarations.h"
#include "HelperMacros.h"
#include "Game.h"

using namespace std;
using namespace glm;

namespace Library
{
	RTTI_DEFINITIONS(ScreenQuad);

	const VertexPositionTexture ScreenQuad::sScreenVertices[] =
	{
		VertexPositionTexture(vec4(-1.0f, -1.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),
		VertexPositionTexture(vec4(-1.0f, +1.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f)),
		VertexPositionTexture(vec4(+1.0f, -1.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f)),
		VertexPositionTexture(vec4(+1.0f, +1.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f))
	};;

	const uint32_t ScreenQuad::sScreenIndicies[] =
	{
		0, 2, 1,
		3, 1, 2
	};

	ScreenQuad::ScreenQuad(Game& game, std::shared_ptr<Camera> camera) :
		DrawableGameComponent(game, move(camera))
	{
	}

	ScreenQuad::~ScreenQuad()
	{
		GLCall(glDeleteBuffers(1, &mVBO));
		GLCall(glDeleteBuffers(1, &mIBO));
		GLCall(glDeleteVertexArrays(1, &mVAO));
	}

	void ScreenQuad::Initialize()
	{
		// Build shader programs
		vector<ShaderDefinition> screenShaders;
		screenShaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content/ScreenShader.vert"));
		screenShaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content/ScreenShader.frag"));
		mShaderProgram.BuildProgram(screenShaders);

		// Vertex Buffer
		GLCall(glGenBuffers(1, &mVBO));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, mVBO));
		GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(sScreenVertices), sScreenVertices, GL_STATIC_DRAW));

		// Index buffer
		GLCall(glGenBuffers(1, &mIBO));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * mIndexCount, sScreenIndicies, GL_STATIC_DRAW));

		// Vertex array
		GLCall(glGenVertexArrays(1, &mVAO));
		GLCall(glBindVertexArray(mVAO));

		// Attribute pointers
		GLCall(glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Position), 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTexture), (void*)offsetof(VertexPositionTexture, Position)));
		GLCall(glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Position)));
		GLCall(glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::TextureCoordinate), 2, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTexture), (void*)offsetof(VertexPositionTexture, TextureCoordinates)));
		GLCall(glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::TextureCoordinate)));
	}

	void ScreenQuad::Update(const GameTime& /*gameTime*/)
	{
	}

	void ScreenQuad::Draw(const GameTime& /*gameTime*/)
	{
		GLCall(glDisable(GL_DEPTH_TEST));
		mShaderProgram.Use();

		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		GLCall(glBindVertexArray(mVAO));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, mVBO));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO));
		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, mTextureBuffer));
		GLCall(glActiveTexture(GL_TEXTURE1));
		GLCall(glBindTexture(GL_TEXTURE_2D, mBloomBlurTexture));
		GLCall(glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndexCount), GL_UNSIGNED_INT, 0));
		GLCall(glBindVertexArray(0));
	}

}