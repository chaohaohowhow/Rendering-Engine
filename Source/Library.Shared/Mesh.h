#pragma once

struct aiMesh;

namespace Library
{
	class Model;
	class ModelMaterial;
	class OutputStreamHelper;
	class InputStreamHelper;

	struct MeshData final
	{
		std::shared_ptr<ModelMaterial> Material;
		std::string Name;
		std::vector<glm::vec3> Vertices;
		std::vector<glm::vec3> Normals;
		std::vector<glm::vec3> Tangents;
		std::vector<glm::vec3> BiNormals;
		std::vector<std::vector<glm::vec3>> TextureCoordinates;
		std::vector<std::vector<glm::vec4>> VertexColors;
		std::uint32_t FaceCount{ 0 };
		std::vector<std::uint32_t> Indices;
	};

	class Mesh final
	{
		friend class Model;

	public:
		Mesh(Library::Model& model, InputStreamHelper& streamHelper);
		Mesh(Library::Model& model, MeshData&& meshData);
		Mesh(const Mesh&) = default;
		Mesh(Mesh&&) = default;
		Mesh& operator=(const Mesh&) = default;
		Mesh& operator=(Mesh&&) = default;
		~Mesh() = default;

		// Getters
		inline Model& GetModel() { return *mModel; }
		inline std::shared_ptr<ModelMaterial> GetMaterial() const { return mData.Material; }
		inline const std::string& Name() const { return mData.Name; }
		inline const std::vector<glm::vec3>& Vertices() const { return mData.Vertices; }
		inline const std::vector<glm::vec3>& Normals() const { return mData.Normals; }
		inline const std::vector<glm::vec3>& Tangents() const { return mData.Tangents; }
		inline const std::vector<glm::vec3>& BiNormals() const { return mData.BiNormals; }
		inline const std::vector<std::vector<glm::vec3>>& TextureCoordinates() const { return mData.TextureCoordinates; }
		inline const std::vector<std::vector<glm::vec4>>& VertexColors() const { return mData.VertexColors; }
		inline std::uint32_t FaceCount() const { return mData.FaceCount; }
		inline const std::vector<std::uint32_t>& Indices() const { return mData.Indices; }

		void CreateIndexBuffer(GLuint& indexBuffer);
		void Save(OutputStreamHelper& streamHelper) const;
	private:
		Mesh(Model& model, aiMesh& mesh);
		void Load(InputStreamHelper& streamHelper);

		Model* mModel;
		MeshData mData;
	};
}

