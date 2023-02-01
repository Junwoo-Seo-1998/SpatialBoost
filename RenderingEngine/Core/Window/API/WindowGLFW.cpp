/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: WindowGLFW.cpp
Purpose: Abstricted Layer for GLFW Window.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------*/
#include "WindowGLFW.h"
#include "GLFW/glfw3.h"
#include "glad.h"
#include <iostream>

#include "Core/Event/ApplicationEvents/ApplicationEvents.h"
#include "Core/Event/InputEvents/KeyBoardEvent.h"
#include "Core/Input/Input.h"

void GLAPIENTRY OpenGLMessageCallback(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	if (GL_DEBUG_TYPE_OTHER == type)
		return;
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}

void WindowGLFW::GLFWKeyCallback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/)
{
	WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

	switch (action)
	{
		case GLFW_PRESS:
		{
			Input::SetKey(static_cast<KeyCode>(key), true);
			KeyBoardPressedEvent event(static_cast<KeyCode>(key), 0);
			data.eventCallback(event);
			break;
		}
		case GLFW_RELEASE:
		{
			Input::SetKey(static_cast<KeyCode>(key), false);
			break;
		}
		case GLFW_REPEAT:
		{
			Input::SetKey(static_cast<KeyCode>(key), true);
			break;
		}
		default:
			break;
	}
}

void WindowGLFW::GLFWMouseCallback(GLFWwindow* window, int button, int action, int mods)
{
	switch (action)
	{
	case GLFW_PRESS:
	{
		Input::SetMouseButton(static_cast<Mouse>(button), true);
		break;
	}
	case GLFW_RELEASE:
	{
		Input::SetMouseButton(static_cast<Mouse>(button), false);
		break;
	}
	default:
		break;
	}
}

bool WindowGLFW::Init()
{
	if (glfwInit() != GLFW_TRUE)
		return false;
	m_Window = glfwCreateWindow(m_WindowData.width, m_WindowData.height, "CS350! - junwoo.seo", nullptr, nullptr);
	if (m_Window == NULL)
	{
		glfwTerminate();
		return false;
	}
	m_WindowData.window = m_Window;
	glfwMakeContextCurrent(m_Window);
	gladLoadGL();
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(OpenGLMessageCallback, nullptr);

	glClearColor(0.f,0.f,0.f, 1.f);

	//set glfw callback
		//so that we can use data of window in glfw call backs.
	glfwSetWindowUserPointer(static_cast<GLFWwindow*>(m_WindowData.window), &m_WindowData);
	glfwSetKeyCallback(static_cast<GLFWwindow*>(m_WindowData.window), GLFWKeyCallback);
	glfwSetMouseButtonCallback(static_cast<GLFWwindow*>(m_WindowData.window), GLFWMouseCallback);
	glfwSetWindowSizeCallback(static_cast<GLFWwindow*>(m_WindowData.window), [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.width = width;
			data.height = height;

			WindowResizeEvent event(width, height);
			data.eventCallback(event);
		});
	return true;
}

bool WindowGLFW::ShouldClose()
{
	return glfwWindowShouldClose(m_Window);
}

void WindowGLFW::Update()
{
	glfwSwapBuffers(m_Window);
	glfwPollEvents();
}

void WindowGLFW::Close()
{
	glfwTerminate();
}

void* WindowGLFW::GetWindowHandle()
{
	return m_Window;
}
