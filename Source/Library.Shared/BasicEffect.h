#pragma once

#include "ShaderProgram.h"
#include "VertexDeclarations.h"

namespace Library
{
	class Mesh;

	class BasicEffect final : public ShaderProgram
	{
		RTTI_DECLARATIONS(BasicEffect, ShaderProgram);

		SHADER_VARIABLE_DECLARATION(WorldViewProjection);

	public:
		BasicEffect();
		BasicEffect(const BasicEffect&) = delete;
		BasicEffect& operator=(const BasicEffect&) = delete;
		BasicEffect(BasicEffect&&) = delete;
		BasicEffect& operator=(BasicEffect&&) = delete;
		~BasicEffect() = default;

		virtual void Initialize(GLuint vertexArrayObject) override;

	private:
		enum class VertexAttribute
		{
			Position = 0,
			Color = 1
		};
	};
}