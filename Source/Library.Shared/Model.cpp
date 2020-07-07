#include "pch.h"
#include "Model.h"

#include "ModelMaterial.h"
#include "StreamHelper.h"

using namespace std;
using namespace gsl;

namespace Library
{
	Model::Model(const std::string& fileName)
	{
		Load(fileName);
	}

	ModelData& Model::Data()
	{
		return mData;
	}

	void Model::Save(const std::string& filename) const
	{
		ofstream file(filename.c_str(), ios::binary);
		if (!file.good())
		{
			throw exception("Could not open file.");
		}

		Save(file);
	}

	void Model::Save(std::ofstream& file) const
	{
		OutputStreamHelper streamHelper(file);

		// Serialize materials
		streamHelper << narrow_cast<uint32_t>(mData.Materials.size());
		for (const auto& material : mData.Materials)
		{
			material->Save(streamHelper);
		}

		// Serialize meshes
		streamHelper << narrow_cast<uint32_t>(mData.Meshes.size());
		for (auto& mesh : mData.Meshes)
		{
			mesh->Save(streamHelper);
		}
	}

	void Model::Load(const std::string& filename)
	{
		ifstream file(filename.c_str(), ios::binary);
		if (!file.good())
		{
			throw runtime_error("Could not open file.");
		}

		Load(file);
	}

	void Model::Load(std::ifstream& file)
	{
		InputStreamHelper streamHelper(file);

		// Deserialize materials
		uint32_t materialCount;
		streamHelper >> materialCount;
		mData.Materials.reserve(materialCount);
		for (uint32_t i = 0; i < materialCount; i++)
		{
			mData.Materials.emplace_back(make_shared<ModelMaterial>(*this, streamHelper));
		}

		// Deserialize meshes
		uint32_t meshCount;
		streamHelper >> meshCount;
		mData.Meshes.reserve(meshCount);
		for (uint32_t i = 0; i < meshCount; i++)
		{
			mData.Meshes.emplace_back(make_shared<Mesh>(*this, streamHelper));
		}
	}

}