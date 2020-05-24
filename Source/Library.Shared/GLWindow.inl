#pragma once
#include "GLWindow.h"
#include "Utility.h"

namespace Library
{
	inline GLint GLWindow::GetBufferWidth()
	{
		return mBufferWidth;
	}
	inline GLint Library::GLWindow::GetBufferHeight()
	{
		return mBufferHeight;
	}
	inline bool* GLWindow::GetKeys()
	{
		return mKeys;
	}
	inline bool Library::GLWindow::GetShouldClose()
	{
		return glfwWindowShouldClose(mMainWindow);
	}
	inline void Library::GLWindow::SwapBuffers()
	{
		GLCall(glClearBufferfv(GL_COLOR, 0, &sCornflowerBlue[0]));
		glfwSwapBuffers(mMainWindow);
	}
}