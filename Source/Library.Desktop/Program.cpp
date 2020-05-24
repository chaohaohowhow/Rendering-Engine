#include "pch.h"

#include "GLWindow.h"

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

	while (!mainWindow.GetShouldClose())
	{
		glfwPollEvents();

		mainWindow.SwapBuffers();
	}
}