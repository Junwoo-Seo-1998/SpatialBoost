#pragma once
/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Window.h
Purpose: Abstricted Layer for Window.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------*/
#include<memory>
#include <string>

#include "Core/Event/Event.h"
#include "GLFW/glfw3.h"

class WindowAPI
{
public:
	virtual bool Init() = 0;
	virtual bool ShouldClose() = 0;
	virtual void* GetWindowHandle() = 0;
	virtual void Update() = 0;
	virtual void Close() = 0;
public:
	using EventCallbackFunction = std::function<void(Event&)>;
	void SetEventCallbackFunction(const EventCallbackFunction& callback);
protected:

	struct WindowData
	{
		void* window = nullptr;
		std::string title = "window";
		unsigned int width = 800;
		unsigned int height = 800;

		EventCallbackFunction eventCallback = nullptr;
	};
	WindowData m_WindowData;
};

class Window
{
private:
	std::shared_ptr<WindowAPI> m_WindowAPI = nullptr;

public:
	using EventCallbackFunction = std::function<void(Event&)>;
	bool Init();
	bool ShouldClose();
	void* GetWindowHandle();
	void Update();
	void Close();
	void SetEventCallbackFunction(const EventCallbackFunction& callback);
};