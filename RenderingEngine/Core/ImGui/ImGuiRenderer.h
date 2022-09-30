#pragma once
/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: ImGuiRenderer.h
Purpose: For ImGui Rendering.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------*/
class ImGuiRenderer
{
public:
	ImGuiRenderer();
	~ImGuiRenderer() = default;
	void OnStart(void* window);
	void GuiBegin();
	void GuiEnd();
	void OnDestroy();
};