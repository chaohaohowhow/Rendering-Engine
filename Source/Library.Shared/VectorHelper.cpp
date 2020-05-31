#include "pch.h"
#include "VectorHelper.h"

namespace Library
{
	std::string Vector2Helper::ToString(const glm::vec2& vector)
	{
		std::stringstream stream;

		stream << "{" << vector.x << ", " << vector.y << "}";

		return stream.str();
	}

	std::string Vector3Helper::ToString(const glm::vec3& vector)
	{
		std::stringstream stream;

		stream << "{" << vector.x << ", " << vector.y << ", " << vector.z << "}";

		return stream.str();
	}

	std::string Vector4Helper::ToString(const glm::vec4& vector)
	{
		std::stringstream stream;

		stream << "{" << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w << "}";

		return stream.str();
	}
}