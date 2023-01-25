#pragma once
/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Scene.h
Purpose: It defines Scene.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------*/
#include <entt/entt.hpp>

#include "Entity.h"
class Event;
class SceneManager;
class Application;

class Scene
{
public:
	Scene(Application& app);
	virtual void Awake() = 0;
	virtual void OnEnable() = 0;
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void PostUpdate() = 0;
	virtual void OnDisable() = 0;
	virtual void OnDestroy() = 0;
	virtual void OnEvent(Event& event) = 0;

	void SetMainCamera(Entity cam);
	Entity GetMainCamera() const;

	Entity CreateEntity();
	entt::registry& GetRegistry();


protected:
	Application& App;
	entt::registry m_Registry;
	Entity MainCamera;
	friend class Entity;
};

