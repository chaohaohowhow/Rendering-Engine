#pragma once

namespace Library
{
	class GLWindow
	{
	public:
		GLWindow() = delete;
		~GLWindow();
		GLWindow(const GLWindow&) = delete;
		GLWindow(GLWindow&&) = delete;
		GLWindow& operator=(const GLWindow&) = delete;
		GLWindow& operator=(GLWindow&&) = delete;

		explicit GLWindow(GLint windowWidth, GLint windowHeight);

		int32_t Initialize();
		GLint GetBufferWidth();
		GLint GetBufferHeight();
		bool* GetKeys();
		GLfloat GetXChange();
		GLfloat GetYChange();
		bool GetShouldClose();

		void SwapBuffers();
	private:
		GLFWwindow* mMainWindow = nullptr;
		GLint mWidth = 800;
		GLint mHeight = 600;
		GLint mBufferWidth = 0;
		GLint mBufferHeight = 0;

		bool mKeys[1024] = { 0 };

		GLfloat mLastX = 0;
		GLfloat mLastY = 0;
		GLfloat mXChange = 0;
		GLfloat mYChange = 0;
		bool mMouseFirstMoved;

		static void HandleKeys(GLFWwindow* window, int key, int code, int action, int mode);
		static void HandleMouse(GLFWwindow* window, double xPos, double yPos);

		inline static const glm::vec4 sCornflowerBlue = glm::vec4(0.392f, 0.584f, 0.929f, 1.0f);
	};
}

#include "GLWindow.inl"