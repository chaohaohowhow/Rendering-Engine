#include "pch.h"
#include "Mesh.h"

#include "HelperMacros.h"
#include "Model.h"
#include "ModelMaterial.h"
#include "StreamHelper.h"

using namespace std;
using namespace glm;
using namespace gsl;

namespace Library
{
	void Mesh::CreateIndexBuffer(GLuint& indexBuffer)
	{
		GLCall(glGenBuffers(1, &indexBuffer));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * mData.Indices.size(), &mData.Indices[0], GL_STATIC_DRAW));
	}
	Mesh::Mesh(Library::Model& model, InputStreamHelper& streamHelper) :
		mModel(&model)
	{
		Load(streamHelper);
	}
	Mesh::Mesh(Library::Model& model, MeshData&& meshData) :
		mModel(&model), mData(move(meshData))
	{
	}

	Mesh::Mesh(Model& model, aiMesh& mesh) :
		mModel(&model)
	{
		mData.Name = mesh.mName.C_Str();
		mData.Material = mModel->Materials().at(mesh.mMaterialIndex);

		// Vertices
		mData.Vertices.reserve(mesh.mNumVertices);
		for (uint32_t i = 0; i < mesh.mNumVertices; i++)
		{
			const auto& vertex = mesh.mVertices[i];
			mData.Vertices.emplace_back(vertex.x, vertex.y, vertex.z);
		}

		// Normals
		if (mesh.HasNormals())
		{
			mData.Normals.reserve(mesh.mNumVertices);
			for (uint32_t i = 0; i < mesh.mNumVertices; i++)
			{
				const auto& normal = mesh.mNormals[i];
				mData.Normals.emplace_back(normal.x, normal.y, normal.z);
			}
		}

		// Tangents and Binormals
		if (mesh.HasTangentsAndBitangents())
		{
			mData.Tangents.reserve(mesh.mNumVertices);
			mData.BiNormals.reserve(mesh.mNumVertices);
			for (uint32_t i = 0; i < mesh.mNumVertices; i++)
			{
				const auto& tangent = mesh.mTangents[i];
				mData.Tangents.emplace_back(tangent.x, tangent.y, tangent.z);

				const auto& binormal = mesh.mBitangents[i];
				mData.BiNormals.emplace_back(binormal.x, binormal.y, binormal.z);
			}
		}

		// Texture Coordinates
		uint32_t uvChannelCount = mesh.GetNumUVChannels();
		for (uint32_t i = 0; i < uvChannelCount; i++)
		{
			vector<vec3> textureCoordinates;
			textureCoordinates.reserve(mesh.mNumVertices);
			aiVector3D* aiTextureCoordinates = mesh.mTextureCoords[i];
			for (uint32_t j = 0; j < mesh.mNumVertices; j++)
			{
				const auto& uv = aiTextureCoordinates[j];
				textureCoordinates.emplace_back(uv.x, uv.y, uv.z);
			}

			mData.TextureCoordinates.push_back(move(textureCoordinates));
		}

		// Vertex Colors
		uint32_t colorChannelCount = mesh.GetNumColorChannels();
		for (uint32_t i = 0; i < colorChannelCount; i++)
		{
			vector<vec4> vertexColors;
			vertexColors.reserve(mesh.mNumVertices);
			aiColor4D* aiVertexColors = mesh.mColors[i];
			for (uint32_t j = 0; j < mesh.mNumVertices; j++)
			{
				const auto& c = aiVertexColors[j];
				vertexColors.emplace_back(c.r, c.g, c.b, c.a);
			}
			mData.VertexColors.push_back(move(vertexColors));
		}

		// Faces (note: could pre-reserve if we limit primitive types)
		if (mesh.HasFaces())
		{
			mData.FaceCount = mesh.mNumFaces;
			for (uint32_t i = 0; i < mData.FaceCount; i++)
			{
				aiFace* face = &mesh.mFaces[i];

				for (uint32_t j = 0; j < face->mNumIndices; j++)
				{
					mData.Indices.push_back(face->mIndices[j]);
				}
			}
		}
	}

	void Mesh::Load(InputStreamHelper& streamHelper)
	{
		// Deserialize material reference
		{
			string materialName;
			streamHelper >> materialName;

			for (auto material : mModel->Materials())
			{
				if (material->Name() == materialName)
				{
					mData.Material = material;
					break;
				}
			}
		}

		// Deserialize name
		streamHelper >> mData.Name;

		// Deserialize vertices
		{
			uint32_t vertexCount;
			streamHelper >> vertexCount;
			mData.Vertices.reserve(vertexCount);
			for (uint32_t i = 0; i < vertexCount; i++)
			{
				vec3 vertex;
				streamHelper >> vertex.x >> vertex.y >> vertex.z;
				mData.Vertices.push_back(vertex);
			}
		}

		// Deserialize normals
		{
			uint32_t normalCount;
			streamHelper >> normalCount;
			mData.Normals.reserve(normalCount);
			for (uint32_t i = 0; i < normalCount; i++)
			{
				vec3 normal;
				streamHelper >> normal.x >> normal.y >> normal.z;
				mData.Normals.push_back(normal);
			}
		}

		// Deserialize tangents
		{
			uint32_t tangentCount;
			streamHelper >> tangentCount;
			mData.Tangents.reserve(tangentCount);
			for (uint32_t i = 0; i < tangentCount; i++)
			{
				vec3 tangent;
				streamHelper >> tangent.x >> tangent.y >> tangent.z;
				mData.Tangents.push_back(tangent);
			}
		}

		// Deserialize binormals
		{
			uint32_t binormalCount;
			streamHelper >> binormalCount;
			mData.BiNormals.reserve(binormalCount);
			for (uint32_t i = 0; i < binormalCount; i++)
			{
				vec3 binormal;
				streamHelper >> binormal.x >> binormal.y >> binormal.z;
				mData.BiNormals.push_back(binormal);
			}
		}

		// Deserialize texture coordinates
		{
			uint32_t textureCoordinateCount;
			streamHelper >> textureCoordinateCount;
			mData.TextureCoordinates.reserve(textureCoordinateCount);
			for (uint32_t i = 0; i < textureCoordinateCount; i++)
			{
				uint32_t uvListCount;
				streamHelper >> uvListCount;
				if (uvListCount > 0)
				{
					std::vector<vec3> uvs;
					uvs.reserve(uvListCount);
					for (uint32_t j = 0; j < uvListCount; j++)
					{
						vec3 uv;
						streamHelper >> uv.x >> uv.y >> uv.z;
						uvs.push_back(uv);
					}

					mData.TextureCoordinates.push_back(move(uvs));
				}
			}
		}

		// Deserialize vertex colors
		{
			uint32_t vertexColorCount;
			streamHelper >> vertexColorCount;
			mData.VertexColors.reserve(vertexColorCount);
			for (uint32_t i = 0; i < vertexColorCount; i++)
			{
				uint32_t vertexColorListCount;
				streamHelper >> vertexColorListCount;
				if (vertexColorListCount > 0)
				{
					std::vector<vec4> vertexColors;
					vertexColors.reserve(vertexColorListCount);
					for (uint32_t j = 0; j < vertexColorListCount; j++)
					{
						vec4 vertexColor;
						streamHelper >> vertexColor.x >> vertexColor.y >> vertexColor.z >> vertexColor.w;
						vertexColors.push_back(vertexColor);
					}

					mData.VertexColors.push_back(move(vertexColors));
				}
			}
		}

		// Deserialize indexes	
		{
			streamHelper >> mData.FaceCount;
			uint32_t indexCount;
			streamHelper >> indexCount;
			mData.Indices.reserve(indexCount);
			for (uint32_t i = 0; i < indexCount; i++)
			{
				uint32_t index;
				streamHelper >> index;
				mData.Indices.push_back(index);
			}
		}
	}

	void Mesh::Save(OutputStreamHelper& streamHelper) const
	{
		string materialName = (mData.Material != nullptr ? mData.Material->Name() : "");
		streamHelper << materialName;

		// Serialize name
		streamHelper << mData.Name;

		// Serialize vertices
		streamHelper << narrow_cast<uint32_t>(mData.Vertices.size());
		for (const vec3& vertex : mData.Vertices)
		{
			streamHelper << vertex.x << vertex.y << vertex.z;
		}

		// Serialize normals
		streamHelper << narrow_cast<uint32_t>(mData.Normals.size());
		for (const vec3& normal : mData.Normals)
		{
			streamHelper << normal.x << normal.y << normal.z;
		}

		// Serialize tangents
		streamHelper << narrow_cast<uint32_t>(mData.Tangents.size());
		for (const vec3& tangents : mData.Tangents)
		{
			streamHelper << tangents.x << tangents.y << tangents.z;
		}

		// Serialize binormals
		streamHelper << narrow_cast<uint32_t>(mData.BiNormals.size());
		for (const vec3& binormal : mData.BiNormals)
		{
			streamHelper << binormal.x << binormal.y << binormal.z;
		}

		// Serialize texture coordinates
		streamHelper << narrow_cast<uint32_t>(mData.TextureCoordinates.size());
		for (const auto& uvList : mData.TextureCoordinates)
		{
			streamHelper << narrow_cast<uint32_t>(uvList.size());
			for (const vec3& uv : uvList)
			{
				streamHelper << uv.x << uv.y << uv.z;
			}
		}

		// Serialize vertex colors
		streamHelper << narrow_cast<uint32_t>(mData.VertexColors.size());
		for (const auto& vertexColorList : mData.VertexColors)
		{
			streamHelper << vertexColorList.size();
			for (const vec4& vertexColor : vertexColorList)
			{
				streamHelper << vertexColor.x << vertexColor.y << vertexColor.z << vertexColor.w;
			}
		}

		// Serialize indices
		streamHelper << mData.FaceCount;
		streamHelper << narrow_cast<uint32_t>(mData.Indices.size());
		for (const uint32_t& index : mData.Indices)
		{
			streamHelper << index;
		}
	}
}