#pragma once
namespace Library
{
	class ShaderProgram;

	class Variable final
	{
	public:
		Variable(ShaderProgram& shaderProgram, const std::string& name);
		Variable(const Variable&) = delete;
		Variable(Variable&&) = delete;
		Variable& operator=(const Variable&) = delete;
		Variable& operator=(Variable&&) = delete;
		~Variable() = default;

		inline ShaderProgram& GetShaderProgram() { return mShaderProgram; }
		inline const GLint& Location() const { return mLocation; }
		inline const std::string& Name() const { return mName; }

		Variable& operator<<(const glm::mat4& value);
		Variable& operator<<(const glm::vec4& value);
		Variable& operator<<(const glm::vec3& value);
		Variable& operator<<(const glm::vec2& value);
		Variable& operator<<(float value);
		Variable& operator<<(int value);
	private:
		ShaderProgram& mShaderProgram;
		GLint mLocation = -1;
		std::string mName;
	};
}

