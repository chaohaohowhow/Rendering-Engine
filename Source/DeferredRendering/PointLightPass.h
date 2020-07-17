#pragma once
#include "ShaderProgram.h"

namespace Rendering
{
	class PointLightPass : public Library::ShaderProgram
	{
		RTTI_DECLARATIONS(PointLightPass, ShaderProgram);

		SHADER_VARIABLE_DECLARATION(WVP);
		SHADER_VARIABLE_DECLARATION(PointLightPosition);
		SHADER_VARIABLE_DECLARATION(PointLightColor);
		SHADER_VARIABLE_DECLARATION(PointLightRadius);
		SHADER_VARIABLE_DECLARATION(CameraPosition);
		SHADER_VARIABLE_DECLARATION(SpecularPower);
		SHADER_VARIABLE_DECLARATION(AmbientIntensity);

	public:
		PointLightPass();
		virtual void Initialize(GLuint vertexArrayObject) override;

	private:
		enum class VertexAttribute
		{
			Position = 0,
			TextureCoordinate = 1
		};
	};
}
