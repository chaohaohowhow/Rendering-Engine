#pragma once
#include "ShaderProgram.h"

namespace Library
{
	class ShadowMappingEffect : public ShaderProgram
	{
		RTTI_DECLARATIONS(ShadowMappingEffect, ShaderProgram);

		SHADER_VARIABLE_DECLARATION(Projection);
		SHADER_VARIABLE_DECLARATION(View);
		SHADER_VARIABLE_DECLARATION(World);
		SHADER_VARIABLE_DECLARATION(AmbientColor);
		SHADER_VARIABLE_DECLARATION(CameraPosition);
		SHADER_VARIABLE_DECLARATION(SpecularColor);
		SHADER_VARIABLE_DECLARATION(SpecularPower);
		SHADER_VARIABLE_DECLARATION(LightSpaceMatrix);

	public:
		ShadowMappingEffect();

		virtual void Initialize(GLuint vertexArrayObject) override;

	private:
		enum class VertexAttribute
		{
			Position = 0,
			TextureCoordinate = 1,
			Normal = 2
		};
	};
}

