/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs350_1
Author: Junwoo Seo, junwoo.seo, 0055213
End Header --------------------------------------------------------*/
#include "CameraComponent.h"
#include "Core/Utils/Math.h"


glm::mat4 CameraComponent::GetPerspective() const
{
	return Math::BuildPerspectiveProjectionMatrixFovy(glm::radians(fov_y), aspect_ratio, near, far);
}
