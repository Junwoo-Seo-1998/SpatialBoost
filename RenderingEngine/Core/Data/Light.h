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
	glm::vec3 position{};
	glm::vec3 direction{};
	struct 
	{
		float inner = 30.f;
		float outer = 45.f;
	} m_Angle;

	glm::vec3 Ambient{1.f};
	glm::vec3 Diffuse{1.f};
	glm::vec3 Specular{1.f};
	float falloff{ 32.f };
};

struct Att
{
	float constant;
	float linear;
	float quadratic;
};
