#include "pch.h"
#include "ShaderProgram.h"
#include "HelperMacros.h"
#include "Variable.h"

using namespace std;
using namespace glm;

namespace Library
{
	Variable::Variable(ShaderProgram& shaderProgram, const std::string& name) :
		mShaderProgram(shaderProgram), mName(name)
	{
		GLCall(mLocation = glGetUniformLocation(mShaderProgram.Program(), name.c_str()));
		if (mLocation == -1)
		{
			throw runtime_error("glGetUnifromLocation() did not find uniform location!");
		}
	}
	Variable& Variable::operator<<(const glm::mat4& value)
	{
		GLCall(glUniformMatrix4fv(mLocation, 1, GL_FALSE, value_ptr(value)));
		return *this;
	}
	Variable& Variable::operator<<(const glm::vec4& value)
	{
		GLCall(glUniform4fv(mLocation, 1, value_ptr(value)));
		return *this;
	}
	Variable& Variable::operator<<(const glm::vec3& value)
	{
		GLCall(glUniform3fv(mLocation, 1, value_ptr(value)));
		return *this;
	}

	Variable& Variable::operator<<(const glm::vec2& value)
	{
		GLCall(glUniform2fv(mLocation, 1, value_ptr(value)));
		return *this;
	}

	Variable& Variable::operator<<(float value)
	{
		GLCall(glUniform1f(mLocation, value));
		return *this;
	}

	Variable& Variable::operator<<(int value)
	{
		GLCall(glUniform1i(mLocation, value));
		return *this;
	}

}