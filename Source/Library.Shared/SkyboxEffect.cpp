#include "pch.h"
#include "SkyboxEffect.h"
#include "Mesh.h"

using namespace glm;
using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(SkyboxEffect);

	SkyboxEffect::SkyboxEffect() :
		SHADER_VARIABLE_INITIALIZATION(WorldViewProjection)
	{
	}

	SHADER_VARIABLE_DEFINITION(SkyboxEffect, WorldViewProjection)

		void SkyboxEffect::Initialize(GLuint vertexArrayObject)
	{
		ShaderProgram::Initialize(vertexArrayObject);

		SHADER_VARIABLE_INSTANTIATE(WorldViewProjection)

			glVertexAttribPointer(VertexAttributePosition, 4, GL_FLOAT, GL_FALSE, sizeof(VertexPosition), (void*)offsetof(VertexPosition, Position));
		glEnableVertexAttribArray(VertexAttributePosition);
	}
}