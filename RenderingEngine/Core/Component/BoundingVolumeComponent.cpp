#include "BoundingVolumeComponent.h"

#include "Core/Utils/Math.h"

AABBComponent::AABBComponent(const AABB& aabb)
	:center(aabb.center), extend(aabb.extend)
{
}

AABBComponent::AABBComponent(const std::vector<glm::vec3>& points)
{
	auto aabb = Math::CreateAABB(points);
	center = aabb.center;
	extend = aabb.extend;
}

RitterSphereComponent::RitterSphereComponent(const std::vector<glm::vec3>& points)
{
	auto sphere = Math::CreateRitterSphere(points);
	center = sphere.center;
	radius = sphere.radius;
}

LarssonSphereComponent::LarssonSphereComponent(const std::vector<glm::vec3>& points)
{
	auto sphere = Math::CreateLarssonSphere(points);
	center = sphere.center;
	radius = sphere.radius;
}

PCASphereComponent::PCASphereComponent(const std::vector<glm::vec3>& points)
{
	auto sphere = Math::CreatePCASphere(points);
	center = sphere.center;
	radius = sphere.radius;
}
