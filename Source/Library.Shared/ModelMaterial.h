#pragma once

struct aiMaterial;

namespace Library
{
	class Model;
	enum class TextureType
	{
		Diffuse = 0,
		Specular,
		Ambient,
		Emissive,
		HeightMap,
		NormalMap,
		LightMap,

		Begin = Diffuse,
		End = LightMap
	};

	class ModelMaterial final
	{
		friend class Model;

	public:
		explicit ModelMaterial(Model& model);
		ModelMaterial(const ModelMaterial&) = default;
		ModelMaterial(ModelMaterial&&) = default;
		ModelMaterial& operator=(const ModelMaterial&) = default;
		ModelMaterial& operator=(ModelMaterial&&) = default;
		~ModelMaterial() = default;

		inline Model& GetModel() { return *mModel; }
		inline const std::string& Name() const { return mName; }
		inline const std::map<TextureType, std::vector<std::string>>& Textures() const { return mTextures; }

	private:
		const static std::map<TextureType, std::uint32_t> sTextureTypeMappings;

		ModelMaterial(Model& model, aiMaterial& material);

		Model* mModel;
		std::string mName;
		std::map<TextureType, std::vector<std::string>> mTextures;
	};
}

