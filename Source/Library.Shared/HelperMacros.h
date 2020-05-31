#include <gl/glew.h>
#include <iostream>

#define ASSERT(x) if(!(x)) __debugbreak();

#if defined(DEBUG) || defined(_DEBUG)
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__));
#else
#define GLCall(x) x;
#endif

static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (0x" << std::hex << error << std::dec << "):" << function << " " << file << ": " << line << std::endl;
		return false;
	}

	return true;
}