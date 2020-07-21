#include "pch.h"
#include "DirectionalLightPass.h"

#include "VertexDeclarations.h"

using namespace Library;

namespace Rendering
{
	RTTI_DEFINITIONS(DirectionalLightPass);

	DirectionalLightPass::DirectionalLightPass() :
		SHADER_VARIABLE_INITIALIZATION(DirectionalLightColor),
		SHADER_VARIABLE_INITIALIZATION(DirectionalLightDirection), 
		SHADER_VARIABLE_INITIALIZATION(CameraPosition),
		SHADER_VARIABLE_INITIALIZATION(AmbientIntensity), 
		SHADER_VARIABLE_INITIALIZATION(SpecularPower)
	{
	}

	SHADER_VARIABLE_DEFINITION(DirectionalLightPass, DirectionalLightColor);
	SHADER_VARIABLE_DEFINITION(DirectionalLightPass, DirectionalLightDirection);
	SHADER_VARIABLE_DEFINITION(DirectionalLightPass, CameraPosition);
	SHADER_VARIABLE_DEFINITION(DirectionalLightPass, AmbientIntensity);
	SHADER_VARIABLE_DEFINITION(DirectionalLightPass, SpecularPower);

	void DirectionalLightPass::Initialize(GLuint vertexArrayObject)
	{
		ShaderProgram::Initialize(vertexArrayObject);

		SHADER_VARIABLE_INSTANTIATE(DirectionalLightColor);
		SHADER_VARIABLE_INSTANTIATE(DirectionalLightDirection);
		SHADER_VARIABLE_INSTANTIATE(CameraPosition);
		SHADER_VARIABLE_INSTANTIATE(AmbientIntensity);
		SHADER_VARIABLE_INSTANTIATE(SpecularPower);

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Position), 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTexture), (void*)offsetof(VertexPositionTexture, Position));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Position));

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::TextureCoordinate), 2, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTexture), (void*)offsetof(VertexPositionTexture, TextureCoordinates));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::TextureCoordinate));
	}
}