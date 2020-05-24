#pragma once
#include "Shader.h"

namespace Library
{
	inline void Shader::CreateFromFiles(const std::string& vertexPath, const std::string& fragmentPath)
	{
		CompileShader(ReadFile(vertexPath), ReadFile(fragmentPath));
	}
	inline void Shader::UseShader() const
	{
		GLCall(glUseProgram(mShaderId));
	}
}