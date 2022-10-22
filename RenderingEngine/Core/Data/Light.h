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
};