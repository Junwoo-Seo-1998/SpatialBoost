/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Application.cpp
Purpose: Abstraction for Application.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 10 2022
End Header --------------------------------------------------------*/
#include"Application.h"
#include "glad.h"
Application::Application()
	:m_Window(std::make_shared<Window>()),m_SceneManager(std::make_shared<SceneManager>())
{

}

Application::~Application()
{
}

bool Application::Init()
{
	if (!m_Window->Init())
		return false;
	return true;
}

void Application::Update()
{
	//TODO: remove this after testing.
	glViewport(0, 0, 800, 800);
	m_SceneManager->GetCurrentScene()->Start();
	while (!m_Window->ShouldClose())
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_SceneManager->GetCurrentScene()->Update();
		m_SceneManager->GetCurrentScene()->LateUpdate();
		m_Window->Update();
	}
}

void Application::Close()
{
	m_SceneManager->GetCurrentScene()->OnDestroy();
	m_Window->Close();
}
