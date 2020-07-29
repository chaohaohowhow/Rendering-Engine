#pragma once
#include "ShaderProgram.h"
namespace Rendering
{
	class GBufferPass : public Library::ShaderProgram
	{
		RTTI_DECLARATIONS(GBufferPass, ShaderProgram);

		SHADER_VARIABLE_DECLARATION(Projection);
		SHADER_VARIABLE_DECLARATION(View);
		SHADER_VARIABLE_DECLARATION(World);
		SHADER_VARIABLE_DECLARATION(UseTexture);
		SHADER_VARIABLE_DECLARATION(DiffuseColor);

	public:
		GBufferPass();
		virtual void Initialize(GLuint vertexArrayObject) override;
		void InitializeUniform();

	private:
		enum class VertexAttribute
		{
			Position = 0,
			TextureCoordinate = 1,
			Normal = 2
		};
	};
}

