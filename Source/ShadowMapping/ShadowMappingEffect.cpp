#include "pch.h"
#include "ShadowMappingEffect.h"

#include "HelperMacros.h"
#include "VertexDeclarations.h"

namespace Library
{
	RTTI_DEFINITIONS(ShadowMappingEffect);

	ShadowMappingEffect::ShadowMappingEffect() :
		SHADER_VARIABLE_INITIALIZATION(Projection), SHADER_VARIABLE_INITIALIZATION(View),
		SHADER_VARIABLE_INITIALIZATION(World), SHADER_VARIABLE_INITIALIZATION(AmbientColor),
		SHADER_VARIABLE_INITIALIZATION(CameraPosition), SHADER_VARIABLE_INITIALIZATION(SpecularColor),
		SHADER_VARIABLE_INITIALIZATION(SpecularPower), SHADER_VARIABLE_INITIALIZATION(LightSpaceMatrix)
	{
	}

	SHADER_VARIABLE_DEFINITION(ShadowMappingEffect, Projection);
	SHADER_VARIABLE_DEFINITION(ShadowMappingEffect, View);
	SHADER_VARIABLE_DEFINITION(ShadowMappingEffect, World);
	SHADER_VARIABLE_DEFINITION(ShadowMappingEffect, AmbientColor);
	SHADER_VARIABLE_DEFINITION(ShadowMappingEffect, CameraPosition);
	SHADER_VARIABLE_DEFINITION(ShadowMappingEffect, SpecularColor);
	SHADER_VARIABLE_DEFINITION(ShadowMappingEffect, SpecularPower);
	SHADER_VARIABLE_DEFINITION(ShadowMappingEffect, LightSpaceMatrix);
	


	void ShadowMappingEffect::Initialize(GLuint vertexArrayObject)
	{
		ShaderProgram::Initialize(vertexArrayObject);

		SHADER_VARIABLE_INSTANTIATE(Projection);
		SHADER_VARIABLE_INSTANTIATE(View);
		SHADER_VARIABLE_INSTANTIATE(World);
		SHADER_VARIABLE_INSTANTIATE(AmbientColor);
		SHADER_VARIABLE_INSTANTIATE(CameraPosition);
		SHADER_VARIABLE_INSTANTIATE(SpecularColor);
		SHADER_VARIABLE_INSTANTIATE(SpecularPower);
		SHADER_VARIABLE_INSTANTIATE(LightSpaceMatrix);

		GLCall(glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Position), 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormal), (void*)offsetof(VertexPositionTextureNormal, Position)));
		GLCall(glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Position)));

		GLCall(glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::TextureCoordinate), 2, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormal), (void*)offsetof(VertexPositionTextureNormal, TextureCoordinates)));
		GLCall(glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::TextureCoordinate)));

		GLCall(glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Normal), 3, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormal), (void*)offsetof(VertexPositionTextureNormal, Normal)));
		GLCall(glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Normal)));
	}
}