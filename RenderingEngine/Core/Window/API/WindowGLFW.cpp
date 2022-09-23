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
bool WindowGLFW::Init()
{
	if (glfwInit()!=GLFW_TRUE)
		return false;
	m_Window = glfwCreateWindow(800, 800, "CS300! - junwoo.seo", nullptr, nullptr);
	if (m_Window==NULL)
	{
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(m_Window);
	gladLoadGL();
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
	glClearColor(0.529f, 0.808f, 0.98f, 1.f);
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
