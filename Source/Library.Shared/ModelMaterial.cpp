#include "pch.h"
#include "ModelMaterial.h"

#include "Enum.h"
#include "StreamHelper.h"

using namespace std;
using namespace gsl;

namespace Library
{
	const map<TextureType, uint32_t> ModelMaterial::sTextureTypeMappings =
	{
		{TextureType::Diffuse, aiTextureType_DIFFUSE},
		{TextureType::Specular, aiTextureType_SPECULAR},
		{TextureType::Ambient, aiTextureType_AMBIENT},
		{TextureType::Emissive, aiTextureType_EMISSIVE},
		{TextureType::HeightMap, aiTextureType_HEIGHT},
		{TextureType::NormalMap, aiTextureType_NORMALS},
		{TextureType::SpecularPowerMap, aiTextureType_SHININESS},
		{TextureType::DisplacementMap, aiTextureType_DISPLACEMENT},
		{TextureType::LightMap, aiTextureType_LIGHTMAP}
	};

	ModelMaterial::ModelMaterial(Model& model) :
		mModel(&model)
	{
	}
	ModelMaterial::ModelMaterial(Model& model, InputStreamHelper& streamHelper) :
		mModel(&model)
	{
		Load(streamHelper);
	}
	ModelMaterial::ModelMaterial(Model& model, ModelMaterialData&& modelMaterialData) :
		mModel(&model), mName(move(modelMaterialData.Name)), mTextures(move(modelMaterialData.Textures))
	{
	}
	void ModelMaterial::Save(OutputStreamHelper& streamHelper) const
	{
		streamHelper << mName;

		streamHelper << narrow_cast<uint32_t>(mTextures.size());
		for (const auto& texturePair : mTextures)
		{
			streamHelper << narrow_cast<int32_t>(texturePair.first);
			streamHelper << narrow_cast<uint32_t>(texturePair.second.size());
			for (const auto& texture : texturePair.second)
			{
				streamHelper << texture;
			}
		}
	}
	void ModelMaterial::Load(InputStreamHelper& streamHelper)
	{
		streamHelper >> mName;

		uint32_t texturesCount;
		streamHelper >> texturesCount;
		for (uint32_t i = 0; i < texturesCount; i++)
		{
			int32_t textureType;
			streamHelper >> textureType;

			vector<string> textures;
			uint32_t textureListCount;
			streamHelper >> textureListCount;
			textures.reserve(textureListCount);
			for (uint32_t j = 0; j < textureListCount; j++)
			{
				string texture;
				streamHelper >> texture;
				textures.push_back(move(texture));
			}

			mTextures.emplace(TextureType(textureType), move(textures));
		}
	}
	ModelMaterial::ModelMaterial(Model& model, aiMaterial& material) :
		mModel(&model)
	{
		aiString name;
		material.Get(AI_MATKEY_NAME, name);
		mName = name.C_Str();

		for (TextureType textureType : Enum<TextureType>())
		{
			aiTextureType mappedTextureType = static_cast<aiTextureType>(sTextureTypeMappings.at(textureType));

			uint32_t textureCount = material.GetTextureCount(mappedTextureType);
			if (textureCount > 0)
			{
				vector<string> textures;
				textures.reserve(textureCount);
				for (uint32_t textureIndex = 0; textureIndex < textureCount; ++textureIndex)
				{
					aiString path;
					if (material.GetTexture(mappedTextureType, textureIndex, &path) == AI_SUCCESS)
					{
						textures.push_back(path.C_Str());
					}
				}
				mTextures.emplace(textureType, move(textures));
			}
		}
	}
}