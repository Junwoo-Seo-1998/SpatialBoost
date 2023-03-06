/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs350_1
Author: Junwoo Seo, junwoo.seo, 0055213
End Header --------------------------------------------------------*/
#pragma once
#include "glm/glm.hpp"

struct CameraComponent
{
	float fov_y = 45.f; //degree
	float aspect_ratio = 1.0f;
	float near = 1.0f;
	float far = 1000000.f;

	[[nodiscard]]glm::mat4 GetPerspective() const;
};