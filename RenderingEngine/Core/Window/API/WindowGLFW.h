#pragma once
/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: WindowGLFW.h
Purpose: Abstricted Layer for GLFW Window.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------*/
#include"Core/Window/Window.h"
struct GLFWwindow;
class WindowGLFW :
    public WindowAPI
{
private:
    GLFWwindow* m_Window = nullptr;
public:
    virtual bool Init() override;
    virtual bool ShouldClose() override;
    virtual void Update() override;
    virtual void Close() override;
};

