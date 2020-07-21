#include "pch.h"
#include "PointLightPass.h"
#include "VertexDeclarations.h"

using namespace Library;

namespace Rendering
{
	RTTI_DEFINITIONS(PointLightPass);

	PointLightPass::PointLightPass() :
		SHADER_VARIABLE_INITIALIZATION(WVP),
		SHADER_VARIABLE_INITIALIZATION(PointLightPosition),
		SHADER_VARIABLE_INITIALIZATION(PointLightColor),
		SHADER_VARIABLE_INITIALIZATION(PointLightRadius),
		SHADER_VARIABLE_INITIALIZATION(CameraPosition),
		SHADER_VARIABLE_INITIALIZATION(SpecularPower),
		SHADER_VARIABLE_INITIALIZATION(AmbientIntensity),
		SHADER_VARIABLE_INITIALIZATION(ScreenSize)
	{
	}

	SHADER_VARIABLE_DEFINITION(PointLightPass, WVP);
	SHADER_VARIABLE_DEFINITION(PointLightPass, PointLightPosition);
	SHADER_VARIABLE_DEFINITION(PointLightPass, PointLightColor);
	SHADER_VARIABLE_DEFINITION(PointLightPass, PointLightRadius);
	SHADER_VARIABLE_DEFINITION(PointLightPass, CameraPosition);
	SHADER_VARIABLE_DEFINITION(PointLightPass, SpecularPower);
	SHADER_VARIABLE_DEFINITION(PointLightPass, AmbientIntensity);
	SHADER_VARIABLE_DEFINITION(PointLightPass, ScreenSize);

	void PointLightPass::Initialize(GLuint vertexArrayObject)
	{
		ShaderProgram::Initialize(vertexArrayObject);

		SHADER_VARIABLE_INSTANTIATE(WVP);
		SHADER_VARIABLE_INSTANTIATE(PointLightPosition);
		SHADER_VARIABLE_INSTANTIATE(PointLightColor);
		SHADER_VARIABLE_INSTANTIATE(PointLightRadius);
		SHADER_VARIABLE_INSTANTIATE(CameraPosition);
		SHADER_VARIABLE_INSTANTIATE(SpecularPower);
		SHADER_VARIABLE_INSTANTIATE(AmbientIntensity);
		SHADER_VARIABLE_INSTANTIATE(ScreenSize);

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Position), 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTexture), (void*)offsetof(VertexPositionTexture, Position));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Position));
	}

}