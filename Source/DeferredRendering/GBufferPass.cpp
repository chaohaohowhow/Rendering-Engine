#include "pch.h"
#include "GBufferPass.h"
#include "VertexDeclarations.h"

using namespace Library;

namespace Rendering
{
	RTTI_DEFINITIONS(GBufferPass);

	GBufferPass::GBufferPass() :
		SHADER_VARIABLE_INITIALIZATION(Projection), SHADER_VARIABLE_INITIALIZATION(View),
		SHADER_VARIABLE_INITIALIZATION(World), SHADER_VARIABLE_INITIALIZATION(UseTexture),
		SHADER_VARIABLE_INITIALIZATION(DiffuseColor)
	{
	}

	SHADER_VARIABLE_DEFINITION(GBufferPass, Projection);
	SHADER_VARIABLE_DEFINITION(GBufferPass, View);
	SHADER_VARIABLE_DEFINITION(GBufferPass, World);
	SHADER_VARIABLE_DEFINITION(GBufferPass, UseTexture);
	SHADER_VARIABLE_DEFINITION(GBufferPass, DiffuseColor);

	void GBufferPass::Initialize(GLuint /*vertexArrayObject*/)
	{
		//ShaderProgram::Initialize(vertexArrayObject);

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Position), 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormal), (void*)offsetof(VertexPositionTextureNormal, Position));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Position));

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::TextureCoordinate), 2, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormal), (void*)offsetof(VertexPositionTextureNormal, TextureCoordinates));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::TextureCoordinate));

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Normal), 3, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormal), (void*)offsetof(VertexPositionTextureNormal, Normal));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Normal));
	}
	void GBufferPass::InitializeUniform()
	{
		SHADER_VARIABLE_INSTANTIATE(Projection);
		SHADER_VARIABLE_INSTANTIATE(View);
		SHADER_VARIABLE_INSTANTIATE(World);
		SHADER_VARIABLE_INSTANTIATE(UseTexture);
		SHADER_VARIABLE_INSTANTIATE(DiffuseColor);
	}
}