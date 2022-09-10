/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Window.cpp
Purpose: Abstricted Layer for Window.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------*/
#include "Window.h"
//for now glfw
#include "Core/Window/API/WindowGLFW.h"
bool Window::Init()
{
	m_StaticData.m_WindowAPI = std::make_shared<WindowGLFW>();
	return m_StaticData.m_WindowAPI->Init();
}

bool Window::ShouldClose()
{
	return m_StaticData.m_WindowAPI->ShouldClose();
}

void Window::Update()
{
	m_StaticData.m_WindowAPI->Update();
}

void Window::Close()
{
	m_StaticData.m_WindowAPI->Close();
}
