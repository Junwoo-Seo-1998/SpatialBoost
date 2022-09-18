#include "SceneManager.h"

std::shared_ptr<Scene> SceneManager::GetCurrentScene()
{
	return m_Current;
}

