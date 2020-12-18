#include "pch.h"
#include "Plane.h"

#include "HelperMacros.h"
#include "VertexDeclarations.h"
#include "VectorHelper.h"
#include "TextureHelper.h"

using namespace glm;
using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(Plane);
	const VertexPositionTextureNormal Plane::sPlaneVertices[] =
	{
		VertexPositionTextureNormal(vec4(-1.0f, 0.0f, -1.0f, 1.0f), vec2(0.0f, 0.0f), Vector3Helper::Up),
		VertexPositionTextureNormal(vec4(-1.0f, 0.0f, +1.0f, 1.0f), vec2(0.0f, 1.0f), Vector3Helper::Up),
		VertexPositionTextureNormal(vec4(+1.0f, 0.0f, -1.0f, 1.0f), vec2(1.0f, 0.0f), Vector3Helper::Up),
		VertexPositionTextureNormal(vec4(+1.0f, 0.0f, +1.0f, 1.0f), vec2(1.0f, 1.0f), Vector3Helper::Up)
	};;

	const GLuint Plane::sPlaneIndicies[] =
	{
		1, 2, 0,
		2, 1, 3
	};
	Plane::Plane(Game& game, std::shared_ptr<Camera> camera, bool hasNormal) :
		DrawableGameComponent(game, move(camera)), mHasNormal(hasNormal)
	{
	}
	Plane::~Plane()
	{
		GLCall(glDeleteBuffers(1, &mVBO));
		GLCall(glDeleteBuffers(1, &mIBO));
		GLCall(glDeleteVertexArrays(1, &mVAO));
	}
	void Plane::Initialize()
	{
		// Vertex buffer
		GLCall(glGenBuffers(1, &mVBO));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, mVBO));
		GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(sPlaneVertices), sPlaneVertices, GL_STATIC_DRAW));

		// Index buffer
		GLCall(glGenBuffers(1, &mIBO));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * sIndexCount, sPlaneIndicies, GL_STATIC_DRAW));

		// Vertex array
		GLCall(glGenVertexArrays(1, &mVAO));
		GLCall(glBindVertexArray(mVAO));

		// Load texture
		mTexture = TextureHelper::LoadTexture("Content\\Textures\\Floor.jpg", GL_REPEAT, GL_LINEAR);

		// Attribute pointers
		GLCall(glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Position), 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormal), (void*)offsetof(VertexPositionTextureNormal, Position)));
		GLCall(glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Position)));
		GLCall(glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::TextureCoordinate), 2, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormal), (void*)offsetof(VertexPositionTextureNormal, TextureCoordinates)));
		GLCall(glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::TextureCoordinate)));
		if (mHasNormal)
		{
			GLCall(glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Normal), 3, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormal), (void*)offsetof(VertexPositionTextureNormal, Normal)));
			GLCall(glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Normal)));
		}

	}
	void Plane::Draw(const GameTime& /*gameTime*/)
	{
		GLCall(glBindVertexArray(mVAO));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, mVBO));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO));
		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, mTexture));
		GLCall(glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(sIndexCount), GL_UNSIGNED_INT, 0));
	}
}