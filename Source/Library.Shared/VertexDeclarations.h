#pragma once
#include "HelperMacros.h"

namespace Library
{
	template <typename T>
	struct VertexDeclaration
	{
		static constexpr uint32_t VertexSize() { return gsl::narrow_cast<uint32_t>(sizeof(T)); }
		static constexpr uint32_t VertexBufferByteWidth(size_t vertexCount) { return gsl::narrow_cast<uint32_t>(sizeof(T) * vertexCount); }
		inline static void CreateVertexBuffer(const gsl::span<const T>& vertices, GLuint& vertexBuffer)
		{
			GLCall(glGenBuffers(1, &vertexBuffer));
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));
			GLCall(glBufferData(GL_ARRAY_BUFFER, VertexSize() * vertices.size(), vertices.data(), GL_STATIC_DRAW));
		}
	};

	struct VertexPosition final : public VertexDeclaration<VertexPosition>
	{
		glm::vec4 Position{ 0 };

		VertexPosition() = default;
		VertexPosition(const glm::vec4& position) :
			Position(position)
		{
		}

		inline static void CreateVertexBuffer(const gsl::span<const VertexPosition>& vertices, GLuint& vertexBuffer)
		{
			VertexDeclaration::CreateVertexBuffer(vertices, vertexBuffer);
		}
	};

	struct VertexPositionColor final : public VertexDeclaration<VertexPositionColor>
	{
		glm::vec4 Position{ 0 };
		glm::vec4 Color{ 0 };

		VertexPositionColor() = default;
		
		VertexPositionColor(const glm::vec4& position, const glm::vec4& color) :
			Position(position), Color(color)
		{
		}

		inline static void CreateVertexBuffer(const gsl::span<const VertexPositionColor>& vertices, GLuint& vertexBuffer)
		{
			VertexDeclaration::CreateVertexBuffer(vertices, vertexBuffer);
		}
	};
}

