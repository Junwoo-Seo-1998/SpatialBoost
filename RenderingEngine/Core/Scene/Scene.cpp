#include "Scene.h"

#include "Core/Component/TransformComponent.h"

Scene::Scene(Application& app)
	:App(app)
{
	
}

Entity Scene::CreateEntity()
{
	Entity entity{ m_Registry.create(),this };
	entity.AddComponent<TransformComponent>();
	return entity;
}

entt::registry& Scene::GetRegistry()
{
	return m_Registry;
}
