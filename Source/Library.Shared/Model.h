#pragma once
#include "Mesh.h"

struct aiNode;

namespace Library
{
	class ModelMaterial;

	class Model final
	{
		friend class Mesh;
		
	public:
		explicit Model(const std::string& fileName, bool flipUVs = false);
		Model(const Model&) = default;
		Model(Model&&) = default;
		Model& operator=(const Model&) = default;
		Model& operator=(Model&&) = default;
		~Model() = default;

		inline bool HasMeshes() const { return mMeshes.size() > 0; }
		inline bool HasMaterials() const { return mMaterials.size() > 0; }
		inline const std::vector<std::shared_ptr<Mesh>>& Meshes() const { return mMeshes; }
		inline const std::vector<std::shared_ptr<ModelMaterial>>& Materials() const { return mMaterials; }

	private:
		std::vector<std::shared_ptr<Mesh>> mMeshes;
		std::vector<std::shared_ptr<ModelMaterial>> mMaterials;
	};
}

