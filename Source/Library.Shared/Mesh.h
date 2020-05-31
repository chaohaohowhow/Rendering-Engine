#pragma once

struct aiMesh;

namespace Library
{
	class Model;
	class Material;
	class ModelMaterial;

	class Mesh final
	{
		friend class Model;

	public:
		Mesh(const Mesh&) = default;
		Mesh(Mesh&&) = default;
		Mesh& operator=(const Mesh&) = default;
		Mesh& operator=(Mesh&&) = default;
		~Mesh() = default;

		// Getters
		inline Model& GetModel() { return *mModel; }
		inline std::shared_ptr<ModelMaterial> GetMaterial() const { return mMaterial; }
		inline const std::string& Name() const { return mName; }
		inline const std::vector<glm::vec3>& Vertices() const { return mVertices; }
		inline const std::vector<glm::vec3>& Normals() const { return mNormals; }
		inline const std::vector<glm::vec3>& Tangents() const { return mTangents; }
		inline const std::vector<glm::vec3>& BiNormals() const { return mBiNormals; }
		inline const std::vector<std::vector<glm::vec3>>& TextureCoordinates() const { return mTextureCoordinates; }
		inline const std::vector<std::vector<glm::vec4>>& VertexColors() const { return mVertexColors; }
		inline std::uint32_t FaceCount() const { return mFaceCount; }
		inline const std::vector<std::uint32_t>& Indices() const { return mIndices; }

		void CreateIndexBuffer(GLuint& indexBuffer);
	private:
		Mesh(Model& model, aiMesh& mesh);

		Model* mModel;
		std::shared_ptr<ModelMaterial> mMaterial;
		std::string mName;
		std::vector<glm::vec3> mVertices;
		std::vector<glm::vec3> mNormals;
		std::vector<glm::vec3> mTangents;
		std::vector<glm::vec3> mBiNormals;
		std::vector<std::vector<glm::vec3>> mTextureCoordinates;
		std::vector<std::vector<glm::vec4>> mVertexColors;
		std::uint32_t mFaceCount = 0;
		std::vector<std::uint32_t> mIndices;
	};
}

