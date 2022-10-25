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
	union
	{
		glm::vec3 position;
		glm::vec3 direction;
	}m_LightData;

	struct 
	{
		float inner;
		float outer;
	} m_Angle;

	float constant;
	float linear;
	float quadratic;

	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;
};