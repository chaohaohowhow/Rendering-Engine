#pragma once
#include "ShaderProgram.h"
namespace Rendering
{
	class DirectionalLightPass : public Library::ShaderProgram
	{
		RTTI_DECLARATIONS(DirectionalLightPass, ShaderProgram);

		SHADER_VARIABLE_DECLARATION(DirectionalLightColor);
		SHADER_VARIABLE_DECLARATION(DirectionalLightDirection);
		SHADER_VARIABLE_DECLARATION(CameraPosition);
		SHADER_VARIABLE_DECLARATION(AmbientIntensity);
		SHADER_VARIABLE_DECLARATION(SpecularPower);

	public:
		DirectionalLightPass();
		virtual void Initialize(GLuint vertexArrayObject) override;

	private:
		enum class VertexAttribute
		{
			Position = 0,
			TextureCoordinate = 1
		};
	};
}

