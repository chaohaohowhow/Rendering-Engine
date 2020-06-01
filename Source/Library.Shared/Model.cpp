#include "pch.h"
#include "Model.h"

#include "ModelMaterial.h"

using namespace std;

namespace Library
{
	Model::Model(const std::string& fileName, bool flipUVs)
	{
		Assimp::Importer importer;

		uint32_t flags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType;
		if (flipUVs)
		{
			flags |= aiProcess_FlipUVs;
		}

		const aiScene* scene = importer.ReadFile(fileName, flags);
		if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			throw runtime_error(importer.GetErrorString());
		}

		if (scene->HasMaterials())
		{
			for (uint32_t i = 0; i < scene->mNumMaterials; ++i)
			{
				mMaterials.push_back(shared_ptr<ModelMaterial>(new ModelMaterial(*this, *scene->mMaterials[i])));
			}
		}

		if (scene->HasMeshes())
		{
			for (uint32_t i = 0; i < scene->mNumMeshes; ++i)
			{
				mMeshes.push_back(shared_ptr<Mesh>(new Mesh(*this, *scene->mMeshes[i])));
			}
		}
	}
}