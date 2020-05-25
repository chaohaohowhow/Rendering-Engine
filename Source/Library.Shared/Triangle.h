#pragma once
namespace Library
{
	class Triangle
	{
	public:
		Triangle() = default;
		Triangle(const Triangle&) = default;
		Triangle(Triangle&&) = default;
		Triangle& operator=(const Triangle&) = default;
		Triangle& operator=(Triangle&&) = default;
		~Triangle();

		void Initialize();
		void Render(GLuint uniformProjectionId, const glm::mat4& projectionMatrix, GLuint uniformViewId, const glm::mat4& viewMatrix) const;
		void Clear();
	private:
		inline static const GLfloat sVertices[] =
		{	//			position	 |		color
			-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};
		const GLsizei sVertexCount = 3;
		GLuint mVAO = 0;
		GLuint mVBO = 0;
	};
}

