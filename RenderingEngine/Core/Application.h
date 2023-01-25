#pragma once
/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Application.h
Purpose: Abstraction for Application.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 10 2022
End Header --------------------------------------------------------*/
#include"Core/Window/Window.h"

class LayerManager;
class WindowResizeEvent;
class ImGuiRenderer;
class Scene;
class SceneManager;

class Application
{
public:
	Application();
	virtual ~Application();
	static Application& Get() { return *s_Instance; }
public:
	void OnEvent(Event& event);
	bool OnWindowResize(WindowResizeEvent& event);

	bool Init();
	virtual void UserDefinedInit()=0;
	void Update();

	void Close();

	void SetCurrentScene(std::shared_ptr<Scene> scene);

	std::shared_ptr<LayerManager> GetLayerManager();
	std::shared_ptr<SceneManager> GetSceneManager();

	std::tuple<int, int> GetWindowSize();
	//for user to access it 
protected:
	static Application* s_Instance;
	std::shared_ptr<Window> m_Window;
	std::shared_ptr<SceneManager> m_SceneManager;
	std::shared_ptr<ImGuiRenderer> m_ImGuiRenderer;
	std::shared_ptr<LayerManager> m_LayerManager;
};
