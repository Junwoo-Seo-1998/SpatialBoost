/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_2
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------*/
#pragma once
#include "glm/glm.hpp"

enum class LightType
{
	PointLight=0,
	DirectionLight,
	SpotLight
};

struct Light
{
	LightType m_LightType{ LightType::PointLight };
	struct 
	{
		float inner = 30.f;
		float outer = 45.f;
	} m_Angle;

	glm::vec3 Ambient{0.f};
	glm::vec3 Diffuse{0.5f};
	glm::vec3 Specular{0.2f};
	float falloff{ 32.f };
};

struct Att
{
	float constant;
	float linear;
	float quadratic;
};
