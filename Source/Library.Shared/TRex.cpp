#include "pch.h"
#include "TRex.h"

#include "HelperMacros.h"
#include "Model.h"
#include "VertexDeclarations.h"

using namespace std;
using namespace glm;

namespace Library
{
	RTTI_DEFINITIONS(TRex);

	TRex::TRex(Game& game, std::shared_ptr<Camera> camera) :
		DrawableGameComponent(game, move(camera))
	{
	}

	TRex::~TRex()
	{
		GLCall(glDeleteBuffers(1, &mVBO));
		GLCall(glDeleteVertexArrays(1, &mVAO));
	}

	void TRex::Initialize()
	{
		// Build Shader program
		vector<ShaderDefinition> shaders;
		shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content/Shader.vert"));
		shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content/Shader.frag"));
		mShaderProgram.BuildProgram(shaders);

		// Load the model
		Model model("Content/Models/T-Rex.3mf");

		// Create vertex and index buffer
		auto mesh = model.Meshes().at(0);
		VertexPositionTexture::CreateVertexBuffer(*mesh, mVBO);
		mesh->CreateIndexBuffer(mIBO);
		mIndexCount = mesh->Indices().size();

		// Get uniforms
		GLCall(mProjectionLocation = glGetUniformLocation(mShaderProgram.Program(), "Projection"));
		GLCall(mViewLocation = glGetUniformLocation(mShaderProgram.Program(), "View"));
		if (mProjectionLocation == -1 || mViewLocation == -1)
		{
			throw runtime_error("glGetUniformLocation() did not find uniform locations!");
		}

		// Create vertex array object
		GLCall(glGenVertexArrays(1, &mVAO));
		GLCall(glBindVertexArray(mVAO));

		// Attribute pointers
		GLCall(glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Position), 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTexture), (void*)offsetof(VertexPositionTexture, Position)));
		GLCall(glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Position)));

		GLCall(glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::TextureCoordinate), 2, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTexture), (void*)offsetof(VertexPositionTexture, TextureCoordinates)));
		GLCall(glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::TextureCoordinate)));

		GLCall(glBindVertexArray(0));
	}

	

}