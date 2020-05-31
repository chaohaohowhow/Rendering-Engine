#include "pch.h"
#include "ModelMaterial.h"
#include "Enum.h"

using namespace std;

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
		{TextureType::LightMap, aiTextureType_LIGHTMAP}
	};

	ModelMaterial::ModelMaterial(Model& model) :
		mModel(&model)
	{
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