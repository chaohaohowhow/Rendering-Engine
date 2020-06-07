#include "pch.h"
#include "VertexDeclarations.h"

#include "Mesh.h"
#include "HelperMacros.h"

using namespace std;
using namespace glm;

namespace Library
{
	template<typename T>
	void VertexDeclaration<T>::CreateVertexBuffer(const gsl::span<const T>& vertices, GLuint& vertexBuffer) {
		GLCall(glGenBuffers(1, &vertexBuffer));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));
		GLCall(glBufferData(GL_ARRAY_BUFFER, VertexSize() * vertices.size(), vertices.data(), GL_STATIC_DRAW));
	}

	void VertexPositionTexture::CreateVertexBuffer(const Mesh& mesh, GLuint& vertexBuffer)
	{
		const vector<vec3>& sourceVertices = mesh.Vertices();

		vector<VertexPositionTexture> vertices;
		vertices.reserve(sourceVertices.size());

		const auto& textureCoordinates = mesh.TextureCoordinates().at(0);
		assert(textureCoordinates.size() == sourceVertices.size());

		for (size_t i = 0; i < sourceVertices.size(); i++)
		{
			const vec3& position = sourceVertices.at(i);
			const vec2& uv = static_cast<vec2>(textureCoordinates.at(i));
			vertices.emplace_back(vec4(position.x, position.y, position.z, 1.0f), uv);
		}

		VertexDeclaration::CreateVertexBuffer(vertices, vertexBuffer);
	}
	void VertexPositionTextureNormal::CreateVertexBuffer(const Library::Mesh& mesh, GLuint& vertexBuffer)
	{
		const vector<vec3>& sourceVertices = mesh.Vertices();

		vector<VertexPositionTextureNormal> vertices;
		vertices.reserve(sourceVertices.size());

		const vector<vec3>& normals = mesh.Normals();
		assert(normals.size() == sourceVertices.size());

		for (size_t i = 0; i < sourceVertices.size(); i++)
		{
			const vec3& position = sourceVertices.at(i);
			const vec3& normal = normals.at(i);
			vertices.emplace_back(vec4(position.x, position.y, position.z, 1.0f), mesh.TextureCoordinates().empty() ? vec2(0.0f) : mesh.TextureCoordinates().at(0).at(i), normal);
		}

		VertexDeclaration::CreateVertexBuffer(vertices, vertexBuffer);
	}
}