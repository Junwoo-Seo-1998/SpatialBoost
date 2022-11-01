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
	LightType m_LightType;
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 data;

	struct 
	{
		float inner;
		float outer;
	} m_Angle;

	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;
};

struct Att
{
	float constant;
	float linear;
	float quadratic;
};
