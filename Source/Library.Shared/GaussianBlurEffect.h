#pragma once
#include "ShaderProgram.h"
namespace Library
{
	class GaussianBlurEffect : public ShaderProgram
	{
		RTTI_DECLARATIONS(GaussianBlurEffect, ShaderProgram);

		SHADER_VARIABLE_DECLARATION(Horizontal);

	public:
		GaussianBlurEffect();

		virtual void Initialize(GLuint vertexArrayObject) override;

	private:
		enum class VertexAttribute
		{
			Position = 0,
			TextureCoordinate = 1,
		};
	};
}

