#include "pch.h"
#include "RenderingGame.h"

using namespace Rendering;

#if defined(_WIN32)
int WINAPI WinMain(HINSTANCE /*instance*/, HINSTANCE /*previousInstance*/, LPSTR /*commandLine*/, int /*showCommand*/)
#else
int main()
#endif
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);	// Checking for memory leak
#endif
	RenderingGame game("OpenGL Rendering Engine");
	game.Run();

	return EXIT_SUCCESS;
}