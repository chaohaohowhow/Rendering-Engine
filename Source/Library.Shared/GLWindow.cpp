#include "pch.h"
#include "GLWindow.h"

namespace Library
{
	GLWindow::GLWindow(GLint windowWidth, GLint windowHeight) :
		mWidth(windowWidth), mHeight(windowHeight)
	{
	}

	GLWindow::~GLWindow()
	{
		glfwDestroyWindow(mMainWindow);
		glfwTerminate();
	}

	int32_t GLWindow::Initialize()
	{
		if (!glfwInit())
		{
			std::cout << "GLFW initialization failed" << std::endl;
			glfwTerminate();
			return 1;
		}

		// Setup GLFW window properties
		// OpenGL version
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		// Core profile = No backwards compatibility
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		// Allow forward compatibility
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		mMainWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL Rendering Engine", nullptr, nullptr);
		if (!mMainWindow)
		{
			std::cout << "GLFW window creation failed" << std::endl;
			glfwTerminate();
			return 1;
		}

		// Get Buffer Size information
		glfwGetFramebufferSize(mMainWindow, &mBufferWidth, &mBufferHeight);

		// Set context for GLEW to use
		glfwMakeContextCurrent(mMainWindow);

		// Handle Key + mouse input
		glfwSetKeyCallback(mMainWindow, HandleKeys);
		glfwSetCursorPosCallback(mMainWindow, HandleMouse);
		// Uncomment the following line to disable cursor
		glfwSetInputMode(mMainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


		if (glewInit() != 0)
		{
			std::cout << "GLEW initialization failed" << std::endl;
			glfwDestroyWindow(mMainWindow);
			glfwTerminate();
			return 1;
		}

		glEnable(GL_DEPTH_TEST);

		// Setup viewport size
		glViewport(0, 0, mBufferWidth, mBufferHeight);

		// Makes this object the user of the window
		glfwSetWindowUserPointer(mMainWindow, this);
		return 0;
	}

	GLfloat GLWindow::GetXChange()
	{
		GLfloat change = mXChange;
		mXChange = 0.0f;
		return change;
	}

	GLfloat GLWindow::GetYChange()
	{
		GLfloat change = mYChange;
		mYChange = 0.0f;
		return change;
	}

	void GLWindow::HandleKeys(GLFWwindow* window, int key, int /*code*/, int action, int /*mode*/)
	{
		GLWindow* glWindow = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));

		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		if (key >= 0 && key < 1024)
		{
			if (action == GLFW_PRESS)
			{
				glWindow->mKeys[key] = true;
			}
			else if (action == GLFW_RELEASE)
			{
				glWindow->mKeys[key] = false;
			}
		}
	}
	void GLWindow::HandleMouse(GLFWwindow* window, double xPos, double yPos)
	{
		GLWindow* glWindow = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));
		if (glWindow->mMouseFirstMoved)
		{
			glWindow->mLastX = static_cast<GLfloat>(xPos);
			glWindow->mLastY = static_cast<GLfloat>(yPos);
			glWindow->mMouseFirstMoved = false;
		}

		glWindow->mXChange = static_cast<GLfloat>(xPos) - glWindow->mLastX;
		glWindow->mYChange = glWindow->mLastY - static_cast<GLfloat>(yPos);

		glWindow->mLastX = static_cast<GLfloat>(xPos);
		glWindow->mLastY = static_cast<GLfloat>(yPos);
	}
}