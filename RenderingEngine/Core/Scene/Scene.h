#pragma once
#include <memory>
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
	virtual void LateUpdate() = 0;
	virtual void OnDisable() = 0;
	virtual void OnDestroy() = 0;
	virtual void OnEvent(Event& event) = 0;

	Entity CreateEntity();
	entt::registry& GetRegistry();
protected:
	Application& App;
	entt::registry m_Registry;
	friend class Entity;
};

