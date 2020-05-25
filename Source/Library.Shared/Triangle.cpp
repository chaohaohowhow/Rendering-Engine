#include "pch.h"
#include "Triangle.h"
#include "Utility.h"

namespace Library
{
	Triangle::~Triangle()
	{
		Clear();
	}
	void Triangle::Initialize()
	{
		GLCall(glGenVertexArrays(1, &mVAO));
		GLCall(glBindVertexArray(mVAO));

		GLCall(glGenBuffers(1, &mVBO));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, mVBO));
		GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(sVertices), sVertices, GL_STATIC_DRAW));

		GLCall(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, 0));
		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(sizeof(GLfloat) * 4)));
		GLCall(glEnableVertexAttribArray(1));

		// Unbinding
		GLCall(glBindVertexArray(0));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
	void Triangle::Render(GLuint uniformProjectionId, const glm::mat4& projectionMatrix, GLuint uniformViewId, const glm::mat4& viewMatrix) const
	{
		GLCall(glBindVertexArray(mVAO));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, mVBO));
		GLCall(glUniformMatrix4fv(uniformProjectionId, 1, GL_FALSE, glm::value_ptr(projectionMatrix)));
		GLCall(glUniformMatrix4fv(uniformViewId, 1, GL_FALSE, glm::value_ptr(viewMatrix)));
		GLCall(glDrawArrays(GL_TRIANGLES, 0, sVertexCount));
		GLCall(glBindVertexArray(0));
	}
	void Triangle::Clear()
	{
		if (mVAO != 0)
		{
			GLCall(glDeleteVertexArrays(1, &mVAO));
			mVAO = 0;
		}
		if (mVBO != 0)
		{
			GLCall(glDeleteBuffers(1, &mVBO));
			mVBO = 0;
		}
	}
}
