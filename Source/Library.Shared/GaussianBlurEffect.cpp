#include "pch.h"
#include "GaussianBlurEffect.h"

#include "HelperMacros.h"
#include "VertexDeclarations.h"

namespace Library
{
	RTTI_DEFINITIONS(GaussianBlurEffect);

	GaussianBlurEffect::GaussianBlurEffect() :
		SHADER_VARIABLE_INITIALIZATION(Horizontal)
	{
	}

	SHADER_VARIABLE_DEFINITION(GaussianBlurEffect, Horizontal);

	void GaussianBlurEffect::Initialize(GLuint vertexArrayObject)
	{
		ShaderProgram::Initialize(vertexArrayObject);

		SHADER_VARIABLE_INSTANTIATE(Horizontal);

		GLCall(glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Position), 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTexture), (void*)offsetof(VertexPositionTexture, Position)));
		GLCall(glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Position)));

		GLCall(glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::TextureCoordinate), 2, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTexture), (void*)offsetof(VertexPositionTexture, TextureCoordinates)));
		GLCall(glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::TextureCoordinate)));
	}
}