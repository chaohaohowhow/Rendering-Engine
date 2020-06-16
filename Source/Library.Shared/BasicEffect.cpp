#include "pch.h"
#include "BasicEffect.h"
#include "Mesh.h"
#include "ColorHelper.h"

using namespace glm;
using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(BasicEffect);

	BasicEffect::BasicEffect() :
		SHADER_VARIABLE_INITIALIZATION(WorldViewProjection)
	{
	}

	SHADER_VARIABLE_DEFINITION(BasicEffect, WorldViewProjection)

		void BasicEffect::Initialize(GLuint vertexArrayObject)
	{
		ShaderProgram::Initialize(vertexArrayObject);

		SHADER_VARIABLE_INSTANTIATE(WorldViewProjection)

			glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Position), 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionColor), (void*)offsetof(VertexPositionColor, Position));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Position));

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Color), 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionColor), (void*)offsetof(VertexPositionColor, Color));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Color));
	}
}
