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

	public:
		GBufferPass();
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

