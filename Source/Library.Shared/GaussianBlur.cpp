#include "pch.h"
#include "GaussianBlur.h"

#include "HelperMacros.h"
#include "VertexDeclarations.h"
#include "Game.h"

using namespace std;
using namespace glm;

namespace Library
{
	RTTI_DEFINITIONS(GaussianBlur);

	const VertexPositionTexture GaussianBlur::sScreenVertices[] =
	{
		VertexPositionTexture(vec4(-1.0f, -1.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),
		VertexPositionTexture(vec4(-1.0f, +1.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f)),
		VertexPositionTexture(vec4(+1.0f, -1.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f)),
		VertexPositionTexture(vec4(+1.0f, +1.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f))
	};;

	const uint32_t GaussianBlur::sScreenIndicies[] =
	{
		0, 2, 1,
		3, 1, 2
	}; 

	GaussianBlur::GaussianBlur(Game& game, std::shared_ptr<Camera> camera) :
		ScreenQuad(game, camera)
	{
	}
	void GaussianBlur::Initialize()
	{
		// Build shader programs
		vector<ShaderDefinition> shaders;
		shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content/GaussianBlur.vert"));
		shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content/GaussianBlur.frag"));
		mShaderProgram.BuildProgram(shaders);

		// Vertex Buffer
		GLCall(glGenBuffers(1, &mVBO));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, mVBO));
		GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(sScreenVertices), sScreenVertices, GL_STATIC_DRAW));

		// Index buffer
		GLCall(glGenBuffers(1, &mIBO));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * mIndexCount, sScreenIndicies, GL_STATIC_DRAW));

		// Vertex array
		GLCall(glGenVertexArrays(1, &mVAO));
		mShaderProgram.Initialize(mVAO);
		GLCall(glBindVertexArray(0));
	}
	void GaussianBlur::Draw(const GameTime& /*gameTime*/)
	{
		GLCall(glBindVertexArray(mVAO));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, mVBO));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO));

		mShaderProgram.Use();
		mFirstIteration = true;
		mHorizontal = true;

		for (size_t i = 0; i < mBlurAmount; ++i)
		{
			assert(mFrameBuffers != nullptr);
			GLCall(glBindFramebuffer(GL_FRAMEBUFFER, (*mFrameBuffers)[mHorizontal]));
			GLCall(glActiveTexture(GL_TEXTURE0));
			GLCall(glBindTexture(GL_TEXTURE_2D, mFirstIteration ? mTextureBuffer : (*mIntermediateColorBuffers)[!mHorizontal]));

			mShaderProgram.Horizontal() << mHorizontal;
			GLCall(glBindVertexArray(mVAO));
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, mVBO));
			GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO));
			assert(mIntermediateColorBuffers != nullptr);
			GLCall(glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndexCount), GL_UNSIGNED_INT, 0));
			GLCall(glBindVertexArray(0));
			mHorizontal = !mHorizontal;
			if (mFirstIteration)
			{
				mFirstIteration = false;
			}
		}
	}
}