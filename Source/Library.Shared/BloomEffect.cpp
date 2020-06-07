#include "pch.h"
#include "BloomEffect.h"

#include "HelperMacros.h"
#include "VertexDeclarations.h"

namespace Library
{
	RTTI_DEFINITIONS(BloomEffect);

	BloomEffect::BloomEffect() :
		SHADER_VARIABLE_INITIALIZATION(Projection), SHADER_VARIABLE_INITIALIZATION(View),
		SHADER_VARIABLE_INITIALIZATION(World), SHADER_VARIABLE_INITIALIZATION(AmbientColor),
		SHADER_VARIABLE_INITIALIZATION(CameraPosition), SHADER_VARIABLE_INITIALIZATION(SpecularColor),
		SHADER_VARIABLE_INITIALIZATION(SpecularPower)
	{
	}

	SHADER_VARIABLE_DEFINITION(BloomEffect, Projection);
	SHADER_VARIABLE_DEFINITION(BloomEffect, View);
	SHADER_VARIABLE_DEFINITION(BloomEffect, World);
	SHADER_VARIABLE_DEFINITION(BloomEffect, AmbientColor);
	SHADER_VARIABLE_DEFINITION(BloomEffect, CameraPosition);
	SHADER_VARIABLE_DEFINITION(BloomEffect, SpecularColor);
	SHADER_VARIABLE_DEFINITION(BloomEffect, SpecularPower);


	void BloomEffect::Initialize(GLuint vertexArrayObject)
	{
		ShaderProgram::Initialize(vertexArrayObject);

		SHADER_VARIABLE_INSTANTIATE(Projection);
		SHADER_VARIABLE_INSTANTIATE(View);
		SHADER_VARIABLE_INSTANTIATE(World);
		SHADER_VARIABLE_INSTANTIATE(AmbientColor);
		SHADER_VARIABLE_INSTANTIATE(CameraPosition);
		SHADER_VARIABLE_INSTANTIATE(SpecularColor);
		SHADER_VARIABLE_INSTANTIATE(SpecularPower);

		GLCall(glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Position), 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTexture), (void*)offsetof(VertexPositionTexture, Position)));
		GLCall(glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Position)));

		GLCall(glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::TextureCoordinate), 2, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTexture), (void*)offsetof(VertexPositionTexture, TextureCoordinates)));
		GLCall(glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::TextureCoordinate)));
	}
}