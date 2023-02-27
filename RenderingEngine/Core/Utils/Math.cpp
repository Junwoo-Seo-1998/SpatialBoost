/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Math.cpp
Purpose: for maths.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------*/
#include "Math.h"
#include<glm/gtc/matrix_transform.hpp>



glm::mat4 Math::BuildCameraMatrix(glm::vec3 cam_position, glm::vec3 target, glm::vec3 world_up)
{
	//just studying purpose.. it uses my own matrix instead glm matrix on debug
#ifdef DEBUG
	
	glm::vec3 cam_foward = -glm::normalize(target-cam_position);
	glm::vec3 cam_right = glm::normalize(glm::cross(-cam_foward, world_up));
	glm::vec3 cam_up = glm::normalize(glm::cross(cam_right, -cam_foward));

	glm::mat4 inverse_scale_rotation =
	{
		{cam_right.x, cam_up.x, cam_foward.x, 0},
		{cam_right.y, cam_up.y, cam_foward.y, 0},
		{cam_right.z, cam_up.z, cam_foward.z, 0},
		{0, 0, 0, 1},
	};

	glm::mat4 inverse_transform =
	{
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{-cam_position.x, -cam_position.y, -cam_position.z, 1},
	};

	auto toret=inverse_scale_rotation* inverse_transform;
	auto glm = glm::lookAt(cam_position, target, world_up);

	return toret;
#else
	return glm::lookAt(cam_position, target, world_up);
#endif
}

glm::mat4 Math::BuildCameraMatrixWithDirection(const glm::vec3& cam_position, const glm::vec3& lookAt,
	const glm::vec3& world_up)
{
	if (lookAt == world_up)
		throw "shouldn't be same!";
	glm::vec3 cam_foward = -lookAt;
	glm::vec3 cam_right = glm::normalize(glm::cross(-cam_foward, world_up));
	glm::vec3 cam_up = glm::normalize(glm::cross(cam_right, -cam_foward));

	glm::mat4 inverse_scale_rotation =
	{
		{cam_right.x, cam_up.x, cam_foward.x, 0},
		{cam_right.y, cam_up.y, cam_foward.y, 0},
		{cam_right.z, cam_up.z, cam_foward.z, 0},
		{0, 0, 0, 1},
	};

	glm::mat4 inverse_transform =
	{
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{-cam_position.x, -cam_position.y, -cam_position.z, 1},
	};

	auto toret = inverse_scale_rotation * inverse_transform;

	return toret;
}

glm::mat4 Math::BuildPerspectiveProjectionMatrix(float width, float height, float near, float far)
{
	float projection_plane = near;
	glm::mat4 perspective =
	{
		{2 * projection_plane / width,0,0,0},
		{0,2 * projection_plane / height,0,0},
		{0,0,(near + far) / (near - far),-1},
		{0,0,2 * far * near / (near - far),0},
	};
	return perspective;
}

glm::mat4 Math::BuildPerspectiveProjectionMatrixFovx(float fov_x, float aspect_ratio, float near, float far)
{
	float cot = 1.f / glm::tan(fov_x / 2.f);
	glm::mat4 perspective =
	{
		{cot,0,0,0},
		{0,cot * aspect_ratio,0,0},
		{0,0,(near + far) / (near - far),-1},
		{0,0,2 * far * near / (near - far),0},
	};
	return perspective;
}

glm::mat4 Math::BuildPerspectiveProjectionMatrixFovy(float fov_y, float aspect_ratio, float near, float far)
{
	//just studying purpose.. it uses my own matrix instead glm matrix on debug
#ifdef DEBUG 
	float cot = 1.f / glm::tan(fov_y / 2.f);
	glm::mat4 perspective =
	{
		{cot / aspect_ratio,0,0,0},
		{0,cot ,0,0},
		{0,0,(near + far) / (near - far),-1},
		{0,0,2 * far * near / (near - far),0},
	};

	return perspective;
#else
	return glm::perspective(fov_y, aspect_ratio, near, far);
#endif

}

glm::vec3 Math::ComputeFaceNormal(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2)
{
	return glm::normalize(glm::cross(v1 - v0, v2 - v0));
}

glm::vec2 Math::ComputePlanarUV(const glm::vec3 TextureEntity, float min_x, float max_x, float min_y, float max_y)
{
	float u = (TextureEntity.x - min_x) / (max_x - min_x);
	float v = (TextureEntity.y - min_y) / (max_y - min_y);

	return { u, v };
}

glm::vec2 Math::ComputeCylindricalUV(const glm::vec3& TextureEntity, float min_y, float max_y)
{
	float theta = glm::degrees(glm::atan(TextureEntity.x, TextureEntity.z));
	//since atan will return [-180, 180] and i want to map this to [0,360]
	theta += 180.f;
	float y = TextureEntity.y;
	return { theta / 360.f, (y - min_y) / (max_y - min_y) };
}

glm::vec2 Math::ComputeSphericalUV(const glm::vec3& TextureEntity)
{
	float theta = glm::degrees(glm::atan(TextureEntity.x, TextureEntity.z));
	//since atan will return [-180, 180] and i want to map this to [0,360]
	theta += 180.f;

	float pi = 180.f - glm::degrees(glm::acos(TextureEntity.y / length(TextureEntity)));

	return { theta / 360.f, pi / 180.f };
}

glm::vec2 Math::ComputeCubeMapUV(const glm::vec3& TextureEntity)
{
	float u = 0.f;
	float v = 0.f;

	glm::vec3 Abs = glm::abs(TextureEntity);

	//-+x
	if (Abs.x >= Abs.y && Abs.x >= Abs.z)
	{
		//left or right
		u = (TextureEntity.x < 0.f) ? TextureEntity.z : -TextureEntity.z;
		u = u / Abs.x;
		v = TextureEntity.y / Abs.x;
	}

	//-+z
	if (Abs.z >= Abs.x && Abs.z >= Abs.y)
	{
		//back or front
		u = (TextureEntity.z < 0.f) ? -TextureEntity.x : TextureEntity.x;
		u = u / Abs.z;
		v = TextureEntity.y / Abs.z;
	}

	//-+y
	if (Abs.y >= Abs.x && Abs.y >= Abs.z)
	{
		u = TextureEntity.x / Abs.y;

		//bottom or top
		v = (TextureEntity.y < 0.f) ? TextureEntity.z : -TextureEntity.z;
		v = v / Abs.y;
	}

	//-1 to 1 to 0 to 1
	u = 0.5f * (u + 1.0f);
	v = 0.5f * (v + 1.0f);
	return { u, v };
}

std::shared_ptr<std::vector<glm::vec2>> Math::ComputePlanarUVs(const std::vector<glm::vec3> TextureEntities, const BoundingBox& box)
{
	std::shared_ptr<std::vector<glm::vec2>> UVs = std::make_shared<std::vector<glm::vec2>>();
	UVs->reserve(TextureEntities.size());
	glm::vec3 min = box.min - box.center;
	glm::vec3 max = box.max - box.center;
	for (const auto & entt : TextureEntities)
	{
		UVs->push_back(ComputePlanarUV(entt, min.x, max.x, min.y, max.y));
	}
	return UVs;
}

std::shared_ptr<std::vector<glm::vec2>> Math::ComputeCylindricalUVs(const std::vector<glm::vec3> TextureEntities, const BoundingBox& box)
{
	std::shared_ptr<std::vector<glm::vec2>> UVs = std::make_shared<std::vector<glm::vec2>>();
	UVs->reserve(TextureEntities.size());
	glm::vec3 min = box.min - box.center;
	glm::vec3 max = box.max - box.center;
	for (const auto& entt : TextureEntities)
	{
		UVs->push_back(ComputeCylindricalUV(entt, min.y, max.y));
	}
	return UVs;
}

std::shared_ptr<std::vector<glm::vec2>> Math::ComputeSphericalUVs(const std::vector<glm::vec3> TextureEntities)
{
	std::shared_ptr<std::vector<glm::vec2>> UVs = std::make_shared<std::vector<glm::vec2>>();
	UVs->reserve(TextureEntities.size());
	for (const auto& entt : TextureEntities)
	{
		UVs->push_back(ComputeSphericalUV(entt));
	}
	return UVs;
}

std::shared_ptr<std::vector<glm::vec2>> Math::ComputeCubeMapUVs(const std::vector<glm::vec3> TextureEntities)
{
	std::shared_ptr<std::vector<glm::vec2>> UVs = std::make_shared<std::vector<glm::vec2>>();
	UVs->reserve(TextureEntities.size());
	for (const auto& entt : TextureEntities)
	{
		UVs->push_back(ComputeCubeMapUV(entt));
	}
	return UVs;
}

std::tuple<glm::vec3, glm::vec3> Math::FindMinAndMax(const std::vector<glm::vec3>& vertices)
{
	glm::vec3 min{ std::numeric_limits<float>().max() };
	glm::vec3 max{ std::numeric_limits<float>().lowest() };

	for (const auto& vert : vertices)
	{
		min.x = glm::min(min.x, vert.x);
		min.y = glm::min(min.y, vert.y);
		min.z = glm::min(min.z, vert.z);

		max.x = glm::max(max.x, vert.x);
		max.y = glm::max(max.y, vert.y);
		max.z = glm::max(max.z, vert.z);
	}

	return { min, max };
}

std::tuple<int, int> Math::ExtremePairAlongDirection(glm::vec3 dir, const std::vector<glm::vec3>& vertices)
{
	float min = std::numeric_limits<float>().max();
	float max = std::numeric_limits<float>().lowest();
	int min_index = 0;
	int max_index = 0;
	int size = static_cast<int>(vertices.size());
	for (int i=0; i<size; ++i)
	{
		float dot = glm::dot(dir, vertices[i]);

		if(min>dot)
		{
			min = dot;
			min_index = i;
		}
		if(max<dot)
		{
			max = dot;
			max_index = i;
		}
	}

	return { min_index, max_index };
}

void Math::ExtendSphere(Sphere& sphere, const std::vector<glm::vec3>& vertices)
{
	int size = static_cast<int>(vertices.size());
	while (true)
	{
		bool containAll = true;
		glm::vec3 found_dir{ 0.f };
		float found_dist = 0.f;
		for (int i=0; i<size; ++i)
		{
			glm::vec3 dir = vertices[i] - sphere.center;
			float dist = glm::length(dir);
			dir /= dist;
			bool contain = dist <= sphere.radius;
			if(!contain)
			{
				if(found_dist<dist)
				{
					found_dist = dist;
					found_dir = dir;
				}
			}
			containAll &= contain;
		}

		if(containAll)
			break;

		float new_radius = (sphere.radius + found_dist) / 2.f;
		sphere.center += (new_radius - sphere.radius) * found_dir;
		sphere.radius = new_radius;
	}
}

AABB Math::CreateAABB(std::vector<glm::vec3>& vertices)
{
	auto [min, max] = FindMinAndMax(vertices);

	return { min, max };
}

Sphere Math::CreateRitterSphere(std::vector<glm::vec3>& vertices)
{
	//xyz
	auto [xmin, xmax] = ExtremePairAlongDirection({ 1,0,0 }, vertices);
	auto [ymin, ymax] = ExtremePairAlongDirection({ 0,1,0 }, vertices);
	auto [zmin, zmax] = ExtremePairAlongDirection({ 0,0,1 }, vertices);


	std::vector<int> candidates;
	candidates.push_back(xmin);
	candidates.push_back(ymin);
	candidates.push_back(zmin);

	candidates.push_back(xmax);
	candidates.push_back(ymax);
	candidates.push_back(zmax);

	glm::vec3 center{ 0.f };
	float max_len = 0.f;
	for (int i=0; i < 6; ++i)
	{
		for (int j = 0; j < 6; ++j)
		{
			if(i==j)
				continue;

			float len = glm::distance(vertices[j], vertices[i]);
			if (max_len < len)
			{
				max_len = len;
				center = vertices[j] + vertices[i];
			}
		}
	}
	Sphere sphere{ center / 2.f, max_len / 2.f };
	Math::ExtendSphere(sphere, vertices);
	return sphere;
}

Sphere Math::CreateLarssonSphere(std::vector<glm::vec3>& vertices)
{
	return {};
}

Sphere Math::CreatePCASphere(std::vector<glm::vec3>& vertices)
{
	return {};
}

