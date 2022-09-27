#pragma once
#include<map>
#include<string>
#include<memory>
#include"Scene.h"
class SceneManager
{
public:
	void SetCurrentScene(std::shared_ptr<Scene> scene);
	std::shared_ptr<Scene> GetCurrentScene();


private:
	std::shared_ptr<Scene> m_Current;
};
