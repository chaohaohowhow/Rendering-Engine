#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <string>

namespace Library
{
	struct Vector2Helper final
	{
	public:
		Vector2Helper() = delete;
		Vector2Helper(const Vector2Helper&) = delete;
		Vector2Helper& operator=(const Vector2Helper&) = delete;
		Vector2Helper(Vector2Helper&&) = delete;
		Vector2Helper& operator=(Vector2Helper&&) = delete;
		~Vector2Helper() = default;

		inline static const glm::vec2 Zero = glm::vec2(0.0f, 0.0f);
		inline static const glm::vec2 One = glm::vec2(1.0f, 1.0f);

		static std::string ToString(const glm::vec2& vector);
	};

	struct Vector3Helper final
	{
	public:
		Vector3Helper() = delete;
		Vector3Helper(const Vector3Helper&) = delete;
		Vector3Helper& operator=(const Vector3Helper&) = delete;
		Vector3Helper(Vector3Helper&&) = delete;
		Vector3Helper& operator=(Vector3Helper&&) = delete;
		~Vector3Helper() = default;

		inline static const glm::vec3 Zero = glm::vec3(0.0f, 0.0f, 0.0f);
		inline static const glm::vec3 One = glm::vec3(1.0f, 1.0f, 1.0f);
		inline static const glm::vec3 Forward = glm::vec3(0.0f, 0.0f, -1.0f);
		inline static const glm::vec3 Backward = glm::vec3(0.0f, 0.0f, 1.0f);
		inline static const glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
		inline static const glm::vec3 Down = glm::vec3(0.0f, -1.0f, 0.0f);
		inline static const glm::vec3 Right = glm::vec3(1.0f, 0.0f, 0.0f);
		inline static const glm::vec3 Left = glm::vec3(-1.0f, 0.0f, 0.0f);

		static std::string ToString(const glm::vec3& vector);
	};

	struct Vector4Helper final
	{
	public:
		Vector4Helper() = delete;
		Vector4Helper(const Vector4Helper&) = delete;
		Vector4Helper& operator=(const Vector4Helper&) = delete;
		Vector4Helper(Vector4Helper&&) = delete;
		Vector4Helper& operator=(Vector4Helper&&) = delete;
		~Vector4Helper() = default;

		inline static const glm::vec4 Zero = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		inline static const glm::vec4 One = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		static std::string ToString(const glm::vec4& vector);
	};
}