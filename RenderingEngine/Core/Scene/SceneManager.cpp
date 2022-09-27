#include "SceneManager.h"

void SceneManager::SetCurrentScene(std::shared_ptr<Scene> scene)
{
	m_Current = scene;
}

std::shared_ptr<Scene> SceneManager::GetCurrentScene()
{
	return m_Current;
}

