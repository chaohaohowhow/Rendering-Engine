#include "pch.h"
#include "MeshProcessor.h"
#include "Model.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

using namespace std;
using namespace gsl;
using namespace glm;
using namespace Library;

namespace ModelPipeline
{
	shared_ptr<Library::Mesh> MeshProcessor::LoadMesh(Library::Model& model, aiMesh& mesh)
	{
		MeshData meshData;

		meshData.Material = model.Materials().at(mesh.mMaterialIndex);

		// Vertices
		{
			meshData.Vertices.reserve(mesh.mNumVertices);
			for (unsigned int i = 0; i < mesh.mNumVertices; i++)
			{
				const auto& vertex = mesh.mVertices[i];
				meshData.Vertices.emplace_back(vertex.x, vertex.y, vertex.z);
			}
		}

		// Normals
		if (mesh.HasNormals())
		{
			meshData.Normals.reserve(mesh.mNumVertices);
			for (unsigned int i = 0; i < mesh.mNumVertices; i++)
			{
				const auto& normal = mesh.mNormals[i];
				meshData.Normals.emplace_back(normal.x, normal.y, normal.z);
			}
		}

		// Tangents and Binormals
		if (mesh.HasTangentsAndBitangents())
		{
			meshData.Tangents.reserve(mesh.mNumVertices);
			meshData.BiNormals.reserve(mesh.mNumVertices);
			for (unsigned int i = 0; i < mesh.mNumVertices; i++)
			{
				const auto& tangents = mesh.mTangents[i];
				const auto& bitangents = mesh.mBitangents[i];
				meshData.Tangents.emplace_back(tangents.x, tangents.y, tangents.z);
				meshData.BiNormals.emplace_back(bitangents.x, bitangents.y, bitangents.z);
			}
		}

		// Texture Coordinates
		{
			unsigned int uvChannelCount = mesh.GetNumUVChannels();
			meshData.TextureCoordinates.reserve(uvChannelCount);
			for (unsigned int i = 0; i < uvChannelCount; i++)
			{
				vector<vec3> textureCoordinates;
				textureCoordinates.reserve(mesh.mNumVertices);
				aiVector3D* aiTextureCoordinates = mesh.mTextureCoords[i];
				for (unsigned int j = 0; j < mesh.mNumVertices; j++)
				{
					const auto& texCoord = aiTextureCoordinates[j];
					textureCoordinates.emplace_back(texCoord.x, texCoord.y, texCoord.z);
				}

				meshData.TextureCoordinates.push_back(move(textureCoordinates));
			}
		}

		// Vertex Colors
		{
			unsigned int colorChannelCount = mesh.GetNumColorChannels();
			meshData.VertexColors.reserve(colorChannelCount);
			for (unsigned int i = 0; i < colorChannelCount; i++)
			{
				vector<vec4> vertexColors;
				vertexColors.reserve(mesh.mNumVertices);
				aiColor4D* aiVertexColors = mesh.mColors[i];
				for (unsigned int j = 0; j < mesh.mNumVertices; j++)
				{
					const auto& color = aiVertexColors[j];
					vertexColors.emplace_back(color.r, color.g, color.b, color.a);
				}
				meshData.VertexColors.push_back(move(vertexColors));
			}
		}

		// Faces (note: could pre-reserve if we limit primitive types)
		if (mesh.HasFaces())
		{
			meshData.FaceCount = mesh.mNumFaces;
			for (uint32_t i = 0; i < meshData.FaceCount; i++)
			{
				aiFace* face = &mesh.mFaces[i];

				for (unsigned int j = 0; j < face->mNumIndices; j++)
				{
					meshData.Indices.push_back(face->mIndices[j]);
				}
			}
		}

		return make_shared<Library::Mesh>(model, move(meshData));
	}
}