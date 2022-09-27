#pragma once
#include <glm/glm.hpp>
struct TransformComponent
{
public:
	TransformComponent() = default;
	TransformComponent(const TransformComponent&) = default;
	TransformComponent(const glm::vec3& translation)
		: Position(translation) {}

	glm::mat4 GetTransform() const;

	glm::vec3 Position{ 0.f };
	glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 Scale{ 1.f };

};