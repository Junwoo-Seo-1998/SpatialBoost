#include"Entity.h"
Entity::Entity(entt::entity entity_handle, Scene* scene)
	:m_EntityHandle(entity_handle), m_Scene(scene)
{
}