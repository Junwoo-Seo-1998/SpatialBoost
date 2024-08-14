#pragma once
/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Entity.h
Purpose: For Component Driven Design it defines Entities.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------*/
#include <entt/entt.hpp>
class Scene;
class Entity
{
public:
	Entity() = default;
	Entity(entt::entity entity_handle, Scene* scene);
	Entity(const Entity& other) = default;

	template <typename Comp_type, typename ... Args>
	Comp_type& AddComponent(Args&&... args)
	{
		Comp_type& comp = m_Scene->m_Registry.template  emplace<Comp_type>(m_EntityHandle, std::forward<Args>(args)...);
		return comp;
	}

	template <typename Comp_type>
	bool HasComponent() const
	{
		return m_Scene->m_Registry.template  any_of<Comp_type>(m_EntityHandle);
	}

	template <typename Comp_type>
	Comp_type& GetComponent()
	{
		return m_Scene->m_Registry.template  get<Comp_type>(m_EntityHandle);
	}

	template <typename Comp_type>
	void RemoveComponent()
	{
		m_Scene->m_Registry.template remove<Comp_type>(m_EntityHandle);
	}
private:
	entt::entity m_EntityHandle = entt::null;
	//for easy adding components
	Scene* m_Scene = nullptr;
};
