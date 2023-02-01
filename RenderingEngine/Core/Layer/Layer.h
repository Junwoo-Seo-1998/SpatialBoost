#pragma once
/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Layer.h
Purpose: Layer class that can help to seperate update loop.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 10 2022
End Header --------------------------------------------------------*/
class Event;
class Layer
{
public:
	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void OnUpdate() {}
	virtual void OnPreRender() {}
	virtual void OnRender() {}
	virtual void OnPostRender() {}
	virtual void OnGuiRender() {}
	virtual void OnEvent([[maybe_unused]] Event& event) {}
};

