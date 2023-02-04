/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs350_1
Author: Junwoo Seo, junwoo.seo, 0055213
End Header --------------------------------------------------------*/
#include "Time.h"
#include "GLFW/glfw3.h"
float Time::dt = 0.f;
float Time::m_LastTime = 0.f;

float Time::GetDelta()
{
	return dt;
}

void Time::Update()
{
	const float time = static_cast<float>(glfwGetTime());
	dt = time - m_LastTime;
	m_LastTime = time;
}
