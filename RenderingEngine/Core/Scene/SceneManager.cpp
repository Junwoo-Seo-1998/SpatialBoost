/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: SceneManager.h
Purpose: It manages Scenes.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------*/
#include "SceneManager.h"

void SceneManager::SetCurrentScene(std::shared_ptr<Scene> scene)
{
	m_Current = scene;
}

std::shared_ptr<Scene> SceneManager::GetCurrentScene()
{
	return m_Current;
}

