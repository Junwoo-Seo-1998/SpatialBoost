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
#include "Time.h"
#include "Core/Scene/SceneManager.h"
#include "Event/ApplicationEvents/ApplicationEvents.h"
#include "ImGui/ImGuiRenderer.h"
#include "Core/Layer/LayerManager.h"
#include "Core/Layer/Layer.h"
#include "Graphics/DebugRenderer.h"
#include "Input/Input.h"
Application* Application::s_Instance;
Application::Application()
	:m_Window(std::make_shared<Window>()),m_SceneManager(std::make_shared<SceneManager>()), m_ImGuiRenderer(std::make_shared<ImGuiRenderer>())
,m_LayerManager(std::make_shared<LayerManager>())
{
	s_Instance = this;
}

Application::~Application()
{
}

void Application::OnEvent(Event& event)
{
	EventDispatcher  dispatcher(event);
	dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FUNCTION(OnWindowResize));

	auto& overlay = m_LayerManager->GetOverLays();
	for (auto it = overlay.rbegin(); it != overlay.rend(); ++it)
	{
		if (event.m_Handled)
			break;
		(*it)->OnEvent(event);
	}

	auto& layers = m_LayerManager->GetLayers();
	for (auto it = layers.rbegin(); it != layers.rend(); ++it)
	{
		if (event.m_Handled)
			break;
		(*it)->OnEvent(event);
	}

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

	DebugRenderer::Init();
	return true;
}

void Application::Update()
{
	m_ImGuiRenderer->OnStart(m_Window->GetWindowHandle());
	while (!m_Window->ShouldClose())
	{
		
		Time::Update();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_ImGuiRenderer->GuiBegin();
		m_SceneManager->GetCurrentScene()->Update();
		m_SceneManager->GetCurrentScene()->PostUpdate();
		for (auto layer : m_LayerManager->GetLayers())
			layer->OnUpdate();
		for (auto layer:m_LayerManager->GetOverLays())
			layer->OnUpdate();
		for (auto layer : m_LayerManager->GetLayers())
			layer->OnPreRender();
		for (auto layer : m_LayerManager->GetOverLays())
			layer->OnPreRender();
		for (auto layer : m_LayerManager->GetLayers())
			layer->OnRender();
		for (auto layer : m_LayerManager->GetOverLays())
			layer->OnRender();
		for (auto layer : m_LayerManager->GetLayers())
			layer->OnPostRender();
		for (auto layer : m_LayerManager->GetOverLays())
			layer->OnPostRender();
		for (auto layer : m_LayerManager->GetLayers())
			layer->OnGuiRender();
		for (auto layer : m_LayerManager->GetOverLays())
			layer->OnGuiRender();
		m_ImGuiRenderer->GuiEnd();
		m_LayerManager->ClearDeleteQueue();
		Input::Reset();
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

std::shared_ptr<LayerManager> Application::GetLayerManager()
{
	return m_LayerManager;
}

std::shared_ptr<SceneManager> Application::GetSceneManager()
{
	return m_SceneManager;
}

std::tuple<int, int> Application::GetWindowSize()
{
	//int width, height;
	//glfwGetWindowSize(static_cast<GLFWwindow*>(m_Window->GetWindowHandle()), &width, &height);
	return m_Window->GetWindowSize();
}

