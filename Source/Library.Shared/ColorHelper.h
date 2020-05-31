#pragma once
namespace Library
{
	class ColorHelper final
	{
	public:
		ColorHelper() = delete;
		ColorHelper(const ColorHelper&) = delete;
		ColorHelper& operator=(const ColorHelper&) = delete;
		ColorHelper(ColorHelper&&) = delete;
		ColorHelper& operator=(ColorHelper&&) = delete;
		~ColorHelper() = default;

		inline static const glm::vec4 Black = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		inline static const glm::vec4 White = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		inline static const glm::vec4 Red = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		inline static const glm::vec4 Green = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		inline static const glm::vec4 Blue = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		inline static const glm::vec4 Yellow = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
		inline static const glm::vec4 BlueGreen = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
		inline static const glm::vec4 Purple = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
		inline static const glm::vec4 CornflowerBlue = glm::vec4(0.392f, 0.584f, 0.929f, 1.0f);
		inline static const glm::vec4 Wheat = glm::vec4(0.961f, 0.871f, 0.702f, 1.0f);
		inline static const glm::vec4 LightGray = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);

		static glm::vec4 RandomColor();

	private:
		static std::random_device sDevice;
		static std::default_random_engine sGenerator;
		static std::uniform_real_distribution<float> sDistribution;
	};
}