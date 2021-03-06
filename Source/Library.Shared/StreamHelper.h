#pragma once

#include <iostream>

namespace Library
{
	class OutputStreamHelper final
	{
	public:
		OutputStreamHelper(std::ostream& stream);
		OutputStreamHelper(const OutputStreamHelper&) = delete;
		OutputStreamHelper& operator=(const OutputStreamHelper&) = delete;

		std::ostream& Stream();

		OutputStreamHelper& operator<<(int32_t value);
		OutputStreamHelper& operator<<(int64_t value);
		OutputStreamHelper& operator<<(uint32_t value);
		OutputStreamHelper& operator<<(uint64_t value);
		OutputStreamHelper& operator<<(float value);
		OutputStreamHelper& operator<<(const std::string& value);
		OutputStreamHelper& operator<<(const glm::mat4& value);
		OutputStreamHelper& operator<<(const glm::mat3& value);
		OutputStreamHelper& operator<<(const glm::vec3& value);
		OutputStreamHelper& operator<<(bool value);

	private:
		template <typename T>
		void WriteObject(std::ostream& stream, T value);

		std::ostream& mStream;
	};

	class InputStreamHelper final
	{
	public:
		InputStreamHelper(std::istream& stream);
		InputStreamHelper(const InputStreamHelper&) = delete;
		InputStreamHelper& operator=(const InputStreamHelper&) = delete;

		std::istream& Stream();

		InputStreamHelper& operator>>(int32_t& value);
		InputStreamHelper& operator>>(int64_t& value);
		InputStreamHelper& operator>>(uint32_t& value);
		InputStreamHelper& operator>>(uint64_t& value);
		InputStreamHelper& operator>>(float& value);
		InputStreamHelper& operator>>(std::string& value);
		InputStreamHelper& operator>>(glm::mat4& value);
		InputStreamHelper& operator>>(glm::mat3& value);
		InputStreamHelper& operator>>(glm::vec3& value);
		InputStreamHelper& operator>>(bool& value);

	private:
		template <typename T>
		void ReadObject(std::istream& stream, T& value);

		std::istream& mStream;
	};
}