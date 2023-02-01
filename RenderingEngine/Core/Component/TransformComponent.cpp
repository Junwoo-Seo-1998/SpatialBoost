/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: TransformComponent.h
Purpose: TransformComponent for the engine.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------*/
#include "TransformComponent.h"
#include "glm/gtx/quaternion.hpp"

glm::mat4 TransformComponent::GetTransform() const
{
	glm::mat4 current = glm::translate(glm::mat4(1.0f), Position)
		* glm::toMat4(glm::quat(Rotation))
		* glm::scale(glm::mat4(1.0f), Scale);
	return current;
}

glm::vec3 TransformComponent::GetRight() const
{
	return glm::toMat4(glm::quat(Rotation)) * glm::vec4{ 1.f,0.f,0.f ,0.f };
}

glm::vec3 TransformComponent::GetUp() const
{
	return glm::toMat4(glm::quat(Rotation)) * glm::vec4{ 0.f,1.f,0.f ,0.f };
}


glm::vec3 TransformComponent::GetForward() const
{
	return glm::toMat4(glm::quat(Rotation)) * glm::vec4{ 0.f,0.f,-1.f ,0.f };
}
