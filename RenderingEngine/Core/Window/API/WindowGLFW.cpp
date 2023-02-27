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
#include "Core/Utils/Log.h"

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

void WindowGLFW::GLFWMousePositionCallback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	Input::SetMousePosition(xpos, ypos);
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

	EngineLog::Info("Vendor - {}", (const char*)(glGetString(GL_VENDOR)));
	EngineLog::Info("Renderer - {}", (const char*)(glGetString(GL_RENDERER)));
	EngineLog::Info("Version - {}", (const char*)(glGetString(GL_VERSION)));
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(OpenGLMessageCallback, nullptr);

	glClearColor(0.f,0.f,0.f, 1.f);

	//set glfw callback
		//so that we can use data of window in glfw call backs.
	glfwSetWindowUserPointer(static_cast<GLFWwindow*>(m_WindowData.window), &m_WindowData);
	//glfwSetInputMode(static_cast<GLFWwindow*>(m_WindowData.window), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(static_cast<GLFWwindow*>(m_WindowData.window), GLFWKeyCallback);
	glfwSetMouseButtonCallback(static_cast<GLFWwindow*>(m_WindowData.window), GLFWMouseCallback);
	glfwSetCursorPosCallback(static_cast<GLFWwindow*>(m_WindowData.window), GLFWMousePositionCallback);
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
