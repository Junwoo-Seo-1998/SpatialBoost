#include "CameraComponent.h"
#include "Core/Utils/Math.h"


glm::mat4 CameraComponent::GetPerspective() const
{
	return Math::BuildPerspectiveProjectionMatrixFovy(glm::radians(fov_y), aspect_ratio, near, far);
}
