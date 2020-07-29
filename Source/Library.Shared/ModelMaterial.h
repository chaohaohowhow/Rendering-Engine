#pragma once

struct aiMaterial;

namespace Library
{
	class Model;
	class OutputStreamHelper;
	class InputStreamHelper;

	enum class TextureType
	{
		Diffuse = 0,
		Specular,
		Ambient,
		Emissive,
		HeightMap,
		NormalMap,
		SpecularPowerMap,
		DisplacementMap,
		LightMap,

		Begin = Diffuse,
		End = LightMap
	};

	struct ModelMaterialData final
	{
		std::string Name;
		std::map<TextureType, std::vector<std::string>> Textures;
		glm::vec3 DiffuseColor;
		glm::vec3 AmbientColor;
		glm::vec3 SpecularColor;
	};

	class ModelMaterial final
	{
		friend class Model;

	public:
		explicit ModelMaterial(Model& model);
		ModelMaterial(Model& model, InputStreamHelper& streamHelper);
		ModelMaterial(Model& model, ModelMaterialData&& modelMaterialData);
		ModelMaterial(const ModelMaterial&) = default;
		ModelMaterial(ModelMaterial&&) = default;
		ModelMaterial& operator=(const ModelMaterial&) = default;
		ModelMaterial& operator=(ModelMaterial&&) = default;
		~ModelMaterial() = default;

		inline Model& GetModel() { return *mModel; }
		inline const std::string& Name() const { return mName; }
		inline const std::map<TextureType, std::vector<std::string>>& Textures() const { return mTextures; }
		inline const glm::vec3& DiffuseColor() const { return mDiffuseColor; }
		inline const glm::vec3& AmbientColor() const { return mAmbientColor; }
		inline const glm::vec3& SpecularColor() const { return mSpecularColor; }

		void Save(OutputStreamHelper& streamHelper) const;

	private:
		void Load(InputStreamHelper& streamHelper);

		const static std::map<TextureType, std::uint32_t> sTextureTypeMappings;

		ModelMaterial(Model& model, aiMaterial& material);

		Model* mModel;
		std::string mName;
		std::map<TextureType, std::vector<std::string>> mTextures;
		glm::vec3 mDiffuseColor;
		glm::vec3 mAmbientColor;
		glm::vec3 mSpecularColor;
	};
}

