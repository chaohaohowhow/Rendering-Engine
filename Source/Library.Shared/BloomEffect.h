#pragma once
#include "ShaderProgram.h"

namespace Library
{
	class BloomEffect : public ShaderProgram
	{
		RTTI_DECLARATIONS(BloomEffect, ShaderProgram);

		SHADER_VARIABLE_DECLARATION(Projection);
		SHADER_VARIABLE_DECLARATION(View);
		SHADER_VARIABLE_DECLARATION(World);
		SHADER_VARIABLE_DECLARATION(AmbientColor);
		SHADER_VARIABLE_DECLARATION(CameraPosition);
		SHADER_VARIABLE_DECLARATION(SpecularColor);
		SHADER_VARIABLE_DECLARATION(SpecularPower);

	public:
		BloomEffect();

		virtual void Initialize(GLuint vertexArrayObject) override;

	private:
		enum class VertexAttribute
		{
			Position = 0,
			TextureCoordinate = 1,
		};
	};
}

