#include "pch.h"
#include "ColoredTriangle.h"

#include "Camera.h"
#include "ColorHelper.h"
#include "HelperMacros.h"
#include "VertexDeclarations.h"

using namespace std;
using namespace glm;

namespace Library
{
	RTTI_DEFINITIONS(ColoredTriangle);

	ColoredTriangle::ColoredTriangle(Game& game, std::shared_ptr<Camera> camera) :
		DrawableGameComponent(game, std::move(camera))
	{
	}
	ColoredTriangle::~ColoredTriangle()
	{
		GLCall(glDeleteBuffers(1, &mVertexBuffer));
		GLCall(glDeleteVertexArrays(1, &mVertexArrayObject));
	}
	void ColoredTriangle::Initialize()
	{
		// Build the shader program
		vector<ShaderDefinition> shaders;
		shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content/Shader.vert"));
		shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content/Shader.frag"));
		mShaderProgram.BuildProgram(shaders);

		// Create the vertex buffer
		const VertexPositionColor vertices[] =
		{
			VertexPositionColor(vec4(-0.5f, -0.5f, 0.5f, 1.0f), ColorHelper::Red),
			VertexPositionColor(vec4(0.0f, 0.5f, 0.5f, 1.0f), ColorHelper::Green),
			VertexPositionColor(vec4(0.5f, -0.5f, 0.5f, 1.0f), ColorHelper::Blue)
		};

		GLCall(glGenBuffers(1, &mVertexBuffer));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer));
		GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

		// Create vertex array object
		GLCall(glGenVertexArrays(1, &mVertexArrayObject));
		GLCall(glBindVertexArray(mVertexArrayObject));
		GLCall(glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Position), 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionColor), (void*)offsetof(VertexPositionColor, Position)));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Position));
		GLCall(glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Color), 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionColor), (void*)offsetof(VertexPositionColor, Color)));
		GLCall(glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Color)));

		GLCall(mProjectionLocation = glGetUniformLocation(mShaderProgram.Program(), "Projection"));
		GLCall(mViewLocation = glGetUniformLocation(mShaderProgram.Program(), "View"));

		if (mProjectionLocation == -1 || mViewLocation == -1)
		{
			throw runtime_error("glGetUniformLocation() did not find uniform location!");
		}
		GLCall(glBindVertexArray(0));
	}
	void ColoredTriangle::Draw(const GameTime& /*gameTime*/)
	{
		GLCall(glBindVertexArray(mVertexArrayObject));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer));
		GLCall(glUseProgram(mShaderProgram.Program()));

		GLCall(glUniformMatrix4fv(mProjectionLocation, 1, GL_FALSE, value_ptr(mCamera->ProjectionMatrix())));
		GLCall(glUniformMatrix4fv(mViewLocation, 1, GL_FALSE, value_ptr(mCamera->ViewMatrix())));

		GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));
		GLCall(glBindVertexArray(0));
	}
}