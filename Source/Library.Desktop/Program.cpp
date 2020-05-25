#include "pch.h"

#include "Camera.h"
#include "GLWindow.h"
#include "Shader.h"
#include "Triangle.h"

using namespace Library;

#if defined(_WIN32)
int WINAPI WinMain(HINSTANCE /*instance*/, HINSTANCE /*previousInstance*/, LPSTR /*commandLine*/, int /*showCommand*/)
#else
int main()
#endif
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);	// Checking for memory leak
#endif

	GLWindow mainWindow(800, 600);
	mainWindow.Initialize();

	Triangle triangle;
	triangle.Initialize();

	Shader shader;
	shader.CreateFromFiles("Content\\Shader.vert", "Content\\Shader.frag");

	Camera camera;

	glm::mat4 projection = glm::perspective(90.0f, static_cast<GLfloat>(mainWindow.GetBufferWidth()) / static_cast<GLfloat>(mainWindow.GetBufferHeight()), 0.0f, 100.0f);
	while (!mainWindow.GetShouldClose())
	{
		mainWindow.SwapBuffers();
		shader.UseShader();
		triangle.Render(shader.GetUniformProjection(), projection, shader.GetUniformView(), camera.CalculateViewMatrix());

		camera.KeyControl(mainWindow.GetKeys());
		camera.MouseControl(mainWindow.GetXChange(), mainWindow.GetYChange());
		glfwPollEvents();
	}
}