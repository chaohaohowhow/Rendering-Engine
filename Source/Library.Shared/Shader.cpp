#include "pch.h"
#include "Shader.h"

namespace Library
{
	Shader::~Shader()
	{
		ClearShader();
	}

	void Shader::ClearShader()
	{
		if (mShaderId != 0)
		{
			GLCall(glDeleteProgram(mShaderId));
			mShaderId = 0;
		}
	}

	void Shader::CompileShader(const std::string& vertexCode, const std::string& fragmentCode)
	{
		GLCall(mShaderId = glCreateProgram());
		if (!mShaderId)
		{
			throw std::exception("Shader program creation failed!");
		}
		AddShader(mShaderId, vertexCode, GL_VERTEX_SHADER);
		AddShader(mShaderId, fragmentCode, GL_FRAGMENT_SHADER);

		GLint result = 0;
		GLchar eLog[1024] = { 0 };

		GLCall(glLinkProgram(mShaderId));
		GLCall(glGetProgramiv(mShaderId, GL_LINK_STATUS, &result));
		if (!result)
		{
			GLCall(glGetProgramInfoLog(mShaderId, sizeof(eLog), NULL, eLog));
			std::cout << "Error linking program: " << eLog << std::endl;
			throw std::exception("Program linking failed!");
		}

		GLCall(glValidateProgram(mShaderId));
		GLCall(glGetShaderiv(mShaderId, GL_VALIDATE_STATUS, &result));
		if (!result)
		{
			GLCall(glGetProgramInfoLog(mShaderId, sizeof(eLog), NULL, eLog));
			std::cout << "Error validating program: " << eLog << std::endl;
			throw std::exception("Program validation failed!");
		}
	}

	void Shader::AddShader(GLuint program, const std::string& shaderCode, GLenum shaderType)
	{
		GLCall(GLuint shader = glCreateShader(shaderType));

		const GLchar* code = shaderCode.c_str();
		GLint codeLength = static_cast<GLint>(shaderCode.size());

		GLCall(glShaderSource(shader, 1, &code, &codeLength));
		GLCall(glCompileShader(shader));

		GLint result = 0;
		GLchar eLog[1024] = { 0 };
		GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &result));

		if (!result)
		{
			GLCall(glGetShaderInfoLog(shader, sizeof(eLog), NULL, eLog));
			std::cout << "Error compiling shader: " << eLog << std::endl;
			throw std::exception("Shader compilation failed");
		}

		GLCall(glAttachShader(program, shader));
	}
	std::string Shader::ReadFile(const std::string& fileLocation)
	{
		std::ifstream fileStream(fileLocation);
		return std::string((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
	}
}