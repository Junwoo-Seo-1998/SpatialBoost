#include "TransformComponent.h"
#include "glm/gtx/quaternion.hpp"

glm::mat4 TransformComponent::GetTransform() const
{
	return glm::translate(glm::mat4(1.0f), Position)
		* glm::toMat4(glm::quat(Rotation))
		* glm::scale(glm::mat4(1.0f), Scale);
}