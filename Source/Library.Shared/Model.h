#pragma once
#include "Mesh.h"

struct aiNode;

namespace Library
{
	class ModelMaterial;

	struct ModelData final
	{
		std::vector<std::shared_ptr<Mesh>> Meshes;
		std::vector<std::shared_ptr<ModelMaterial>> Materials;
	};

	class Model final
	{
		friend class Mesh;
		
	public:
		Model() = default;
		explicit Model(const std::string& fileName);
		Model(const Model&) = default;
		Model(Model&&) = default;
		Model& operator=(const Model&) = default;
		Model& operator=(Model&&) = default;
		~Model() = default;

		inline bool HasMeshes() const { return mData.Meshes.size() > 0; }
		inline bool HasMaterials() const { return mData.Materials.size() > 0; }
		inline const std::vector<std::shared_ptr<Mesh>>& Meshes() const { return mData.Meshes; }
		inline const std::vector<std::shared_ptr<ModelMaterial>>& Materials() const { return mData.Materials; }

		ModelData& Data();
		void Save(const std::string& filename) const;
		void Save(std::ofstream& file) const;
	private:
		void Load(const std::string& filename);
		void Load(std::ifstream& file);
		ModelData mData;
	};
}

