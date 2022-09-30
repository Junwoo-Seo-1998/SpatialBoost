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
#include "Core/Scene/SceneManager.h"
#include "Event/ApplicationEvents/ApplicationEvents.h"
#include "ImGui/ImGuiRenderer.h"


Application::Application()
	:m_Window(std::make_shared<Window>()),m_SceneManager(std::make_shared<SceneManager>()), m_ImGuiRenderer(std::make_shared<ImGuiRenderer>())
{

}

Application::~Application()
{
}

void Application::OnEvent(Event& event)
{
	EventDispatcher  dispatcher(event);
	dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FUNCTION(OnWindowResize));
	m_SceneManager->GetCurrentScene()->OnEvent(event);
}

bool Application::OnWindowResize(WindowResizeEvent& event)
{
	auto [width, height] = event.GetWidthAndHeight();

	if (width == 0 || height == 0)
	{
		return false;
	}
	glViewport(0, 0, width, height);
	return false;
}

bool Application::Init()
{
	if (!m_Window->Init())
		return false;
	m_Window->SetEventCallbackFunction(BIND_EVENT_FUNCTION(OnEvent));
	return true;
}

void Application::Update()
{
	m_ImGuiRenderer->OnStart(m_Window->GetWindowHandle());
	m_SceneManager->GetCurrentScene()->Awake();
	m_SceneManager->GetCurrentScene()->Start();
	while (!m_Window->ShouldClose())
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_ImGuiRenderer->GuiBegin();
		m_SceneManager->GetCurrentScene()->Update();
		m_SceneManager->GetCurrentScene()->LateUpdate();
		m_ImGuiRenderer->GuiEnd();
		m_Window->Update();

	}
	
}

void Application::Close()
{
	m_ImGuiRenderer->OnDestroy();
	m_SceneManager->GetCurrentScene()->OnDestroy();
	m_Window->Close();
	
}

void Application::SetCurrentScene(std::shared_ptr<Scene> scene)
{
	m_SceneManager->SetCurrentScene(scene);
}

