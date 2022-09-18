#pragma once
#include<map>
#include<string>
#include<memory>
#include"Scene.h"
class SceneManager
{
public:
	template<typename T>
	void SetCurrentScene();
	std::shared_ptr<Scene> GetCurrentScene();


private:
	std::shared_ptr<Scene> m_Current;
};

template<typename T>
inline void SceneManager::SetCurrentScene()
{
	m_Current = std::make_shared<T>();
}
