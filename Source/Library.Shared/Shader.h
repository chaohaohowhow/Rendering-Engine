#pragma once

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
		GLuint GetUniformView();
		GLuint GetUniformProjection();
		void UseShader() const;
		void ClearShader();
	private:
		GLuint mShaderId = 0;
		GLuint mUniformProjection = 0;
		GLuint mUniformView = 0;
		void CompileShader(const std::string& vertexCode, const std::string& fragmentCode);
		void AddShader(GLuint shader, const std::string& shaderCode, GLenum shaderType);
		std::string ReadFile(const std::string& filePath);
	};
}
