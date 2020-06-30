#include "pch.h"
#include "ShaderProgram.h"

#include "HelperMacros.h"
#include "Utility.h"

using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(ShaderProgram);

	ShaderProgram::ShaderProgram() :
		mProgram(glCreateProgram())
	{
	}
	ShaderProgram::~ShaderProgram()
	{
		GLCall(glDeleteProgram(mProgram));

		for (Variable* variable : mVariables)
		{
			delete variable;
		}
	}
	GLuint ShaderProgram::CompileShaderFromFile(GLenum ShaderType, const std::string& fileName)
	{
		vector<char> shaderSource;
		Utility::LoadBinaryFile(fileName, shaderSource);
		GLchar* sourcePointer = &shaderSource[0];
		GLint length = static_cast<GLsizei>(shaderSource.size());

		GLCall(GLuint shader = glCreateShader(ShaderType));
		GLCall(glShaderSource(shader, 1, &sourcePointer, &length));
		GLCall(glCompileShader(shader));

		GLint compileStatus;
		GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus));
		if (!compileStatus)
		{
			GLint logLength;
			GLCall(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength));

			string log;
			log.reserve(logLength);

			GLCall(glGetShaderInfoLog(shader, logLength, nullptr, const_cast<GLchar*>(log.c_str())));

			stringstream errorMessage;
			errorMessage << "glCompileShader() failed!\n" << log.c_str();
			throw runtime_error(errorMessage.str().c_str());
		}

		return shader;
	}
	void ShaderProgram::BuildProgram(const std::vector<ShaderDefinition>& shaderDefinitions)
	{
		vector<GLuint> compiledShaders;
		compiledShaders.reserve(shaderDefinitions.size());

		for (ShaderDefinition definition : shaderDefinitions)
		{
			GLuint compiledShader = CompileShaderFromFile(definition.first, definition.second);
			GLCall(glAttachShader(mProgram, compiledShader));
			compiledShaders.push_back(compiledShader);
		}

		GLCall(glLinkProgram(mProgram));
		GLint linkStatus;
		GLCall(glGetProgramiv(mProgram, GL_LINK_STATUS, &linkStatus));
		if (!linkStatus)
		{
			GLint logLength;
			GLCall(glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &logLength));

			string log;
			log.reserve(logLength);

			GLCall(glGetProgramInfoLog(mProgram, logLength, nullptr, const_cast<GLchar*>(log.c_str())));

			stringstream errorMessage;
			errorMessage << "glLinkProgram() failed!\n" << log.c_str();

			throw runtime_error(errorMessage.str().c_str());
		}

		for (GLuint shader : compiledShaders)
		{
			GLCall(glDeleteShader(shader));
		}
	}
	void ShaderProgram::Initialize(GLuint vertexArrayObject)
	{
		GLCall(glBindVertexArray(vertexArrayObject));
	}
	void ShaderProgram::Use() const
	{
		GLCall(glUseProgram(mProgram));
	}
}