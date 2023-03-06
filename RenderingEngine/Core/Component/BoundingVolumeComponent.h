#pragma once
#include <vector>

#include "glm/glm.hpp"
#include "Core/BoundingVolume/AABB.h"
struct AABBComponent
{
	AABBComponent(const AABB& aabb);
	AABBComponent(const std::vector<glm::vec3>& points);
	glm::vec3 center;
	glm::vec3 extend;
};


struct RitterSphereComponent
{
	RitterSphereComponent(const std::vector<glm::vec3>& points);
	glm::vec3 center;
	float radius;
};

struct LarssonSphereComponent
{
	LarssonSphereComponent(const std::vector<glm::vec3>& points);
	glm::vec3 center;
	float radius;
};

struct PCASphereComponent
{
	PCASphereComponent(const std::vector<glm::vec3>& points);
	glm::vec3 center;
	float radius;
};
