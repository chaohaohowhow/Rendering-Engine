#pragma once
#include "RTTI.h"
#include "Variable.h"

namespace Library
{
	using ShaderDefinition = std::pair<GLenum, std::string>;

	class ShaderProgram : public RTTI
	{
		RTTI_DECLARATIONS(ShaderProgram, RTTI);

	public:
		// Special members
		ShaderProgram();
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram(ShaderProgram&&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;
		ShaderProgram& operator=(ShaderProgram&&) = delete;
		virtual ~ShaderProgram();

		// Getters
		inline GLuint Program() const { return mProgram; }
		inline const std::vector<Variable*>& Variables() const { return mVariables; }
		inline const std::map<std::string, Variable*>& VariablesByName() const { return mVariablesByName; }

		static GLuint CompileShaderFromFile(GLenum ShaderType, const std::string& fileName);

		void BuildProgram(const std::vector<ShaderDefinition>& shaderDefinitions);
		virtual void Initialize(GLuint vertexArrayObject);
		virtual void Use() const;

	protected:
		std::map<std::string, Variable*> mVariablesByName;
		std::vector<Variable*> mVariables;
		GLuint mProgram;
	};

// Shader variable macros
#define SHADER_VARIABLE_DECLARATION(VariableName)		\
public:													\
	Library::Variable& VariableName() const;			\
private:												\
	Library::Variable* m ## VariableName = nullptr

#define SHADER_VARIABLE_DEFINITION(ShaderProgram, VariableName)		\
	Library::Variable& ShaderProgram::VariablesByName() const		\
	{																\
		return *m ## VariableName;									\
	}

#define SHADER_VARIABLE_INSTANTIATE(VariableName)									\
	m ## VariableName = new Variable(*this, #VariableName);							\
	mVariables.push_back(m ## VariableName);										\
	mVariablesByName.insert(std::pair<std::string, Variable*>(m ## VariableName->Name(), m ## VariableName));
}

