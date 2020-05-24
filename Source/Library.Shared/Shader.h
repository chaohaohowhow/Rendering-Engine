#pragma once
#include "Utility.h"

namespace Library
{
	class Shader
	{
	public:
		Shader() = default;
		~Shader();
		Shader(const Shader&) = default;
		Shader(Shader&&) = default;
		Shader& operator=(const Shader&) = default;
		Shader& operator=(Shader&&) = default;
		void CreateFromFiles(const std::string& vertexPath, const std::string& fragmentPath);

		void UseShader() const;
		void ClearShader();
	private:
		GLuint mShaderId = 0;
		void CompileShader(const std::string& vertexCode, const std::string& fragmentCode);
		void AddShader(GLuint shader, const std::string& shaderCode, GLenum shaderType);
		std::string ReadFile(const std::string& filePath);
	};
}

#include "Shader.inl"
