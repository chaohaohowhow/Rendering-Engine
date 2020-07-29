#include "pch.h"
#include "ModelMaterialProcessor.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

using namespace std;
using namespace Library;
using namespace glm;

namespace ModelPipeline
{
	const map<TextureType, uint32_t> ModelMaterialProcessor::sTextureTypeMappings =
	{
		{ TextureType::Diffuse, aiTextureType_DIFFUSE },
		{ TextureType::Specular, aiTextureType_SPECULAR },
		{ TextureType::Ambient, aiTextureType_AMBIENT },
		{ TextureType::Emissive, aiTextureType_EMISSIVE },
		{ TextureType::HeightMap, aiTextureType_HEIGHT },
		{ TextureType::NormalMap, aiTextureType_NORMALS },
		{ TextureType::SpecularPowerMap, aiTextureType_SHININESS },
		{ TextureType::DisplacementMap, aiTextureType_DISPLACEMENT },
		{ TextureType::LightMap, aiTextureType_LIGHTMAP }
	};

	shared_ptr<ModelMaterial> ModelMaterialProcessor::LoadModelMaterial(Model& model, aiMaterial& material)
	{
		ModelMaterialData modelMaterialData;

		// Loading name
		aiString name;
		material.Get(AI_MATKEY_NAME, name);
		modelMaterialData.Name = name.C_Str();

		// Loading material colors
		aiColor3D color;
		material.Get(AI_MATKEY_COLOR_AMBIENT, color);
		modelMaterialData.AmbientColor = vec3(color.r, color.g, color.b);
		material.Get(AI_MATKEY_COLOR_DIFFUSE, color);
		modelMaterialData.DiffuseColor = vec3(color.r, color.g, color.b);
		material.Get(AI_MATKEY_COLOR_SPECULAR, color);
		modelMaterialData.SpecularColor = vec3(color.r, color.g, color.b);


		// Loading textures
		for (TextureType textureType = (TextureType)0; textureType < TextureType::End; textureType = (TextureType)(static_cast<int>(textureType) + 1))
		{
			aiTextureType mappedTextureType = static_cast<aiTextureType>(sTextureTypeMappings.at(textureType));

			uint32_t textureCount = material.GetTextureCount(mappedTextureType);
			if (textureCount > 0)
			{
				vector<string> textures;
				textures.reserve(textureCount);
				for (uint32_t textureIndex = 0; textureIndex < textureCount; textureIndex++)
				{
					aiString path;
					if (material.GetTexture(mappedTextureType, textureIndex, &path) == AI_SUCCESS)
					{
						textures.push_back(path.C_Str());
					}
				}
				modelMaterialData.Textures.emplace(textureType, move(textures));
			}
		}

		return make_shared<ModelMaterial>(model, move(modelMaterialData));
	}
}