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
class WindowAPI
{
public:
	virtual bool Init() = 0;
	virtual bool ShouldClose() = 0;
	virtual void Update() = 0;
	virtual void Close() = 0;
};
//struct WindowHandle;
class Window
{
private:
	struct StaticData
	{
		std::shared_ptr<WindowAPI> m_WindowAPI = nullptr;
	} m_StaticData;
	
public:
	bool Init();
	bool ShouldClose();
	void Update();
	void Close();
};

