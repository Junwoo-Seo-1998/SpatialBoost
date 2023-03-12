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
#include "Log.h"
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/random.hpp>
#include <glm/gtx/string_cast.hpp>

#include "EPOS.h"
#undef far
#undef near

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

std::vector<glm::vec3> Math::RandomVec3(int how_many, const glm::vec3& min, const glm::vec3& max)
{
	std::vector<glm::vec3> result;
	for (int i=0; i<how_many; ++i)
	{
		result.push_back(glm::linearRand(min, max));
	}
	return result;
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
			return;

		float new_radius = (sphere.radius + found_dist) / 2.f;
		sphere.center += (new_radius - sphere.radius) * found_dir;
		sphere.radius = new_radius;
	}
}

AABB Math::CreateAABB(const std::vector<glm::vec3>& vertices)
{
	auto [min, max] = FindMinAndMax(vertices);
	auto center = (min + max) / 2.f;
	return { center, max - center };
}

Sphere Math::CreateRitterSphere(const std::vector<glm::vec3>& vertices)
{
	EngineLog::Info("Creating RitterSphere");
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
		for (int j = i+1; j < 6; ++j)
		{
			float len = glm::distance(vertices[candidates[j]], vertices[candidates[i]]);
			if (max_len < len)
			{
				max_len = len;
				center = vertices[candidates[j]] + vertices[candidates[i]];
			}
		}
	}
	Sphere sphere{ center / 2.f, max_len / 2.f };
	Math::ExtendSphere(sphere, vertices);
	return sphere;
}

Sphere Math::CreateLarssonSphere(const std::vector<glm::vec3>& vertices, EPOS epos)
{
	EngineLog::Info("Creating LarssonSphere");
	int to_compare = 0;
	switch (epos)
	{
	case EPOS::EPOS6:
		to_compare = 3;
		break;
	case EPOS::EPOS14:
		to_compare = 7;
		break;
	case EPOS::EPOS26:
		to_compare = 13;
		break;
	case EPOS::EPOS98:
		to_compare = 49;
		break;
	}
	std::vector<int> candidates;
	for (int i=0; i<to_compare; ++i)
	{
		auto [min, max] = ExtremePairAlongDirection(EPOS98[i], vertices);
		candidates.push_back(min);
		candidates.push_back(max);
	}
	int size = candidates.size();
	glm::vec3 center{ 0.f };
	float max_len = 0.f;
	for (int i = 0; i < size; ++i)
	{
		for (int j = i + 1; j < size; ++j)
		{
			float len = glm::distance(vertices[candidates[j]], vertices[candidates[i]]);
			if (max_len < len)
			{
				max_len = len;
				center = vertices[candidates[j]] + vertices[candidates[i]];
			}
		}
	}
	Sphere sphere{ center / 2.f, max_len / 2.f };
	Math::ExtendSphere(sphere, vertices);
	return sphere;
}

Sphere Math::CreatePCASphere(const std::vector<glm::vec3>& vertices)
{
	EngineLog::Info("Creating PCASphere");
	glm::vec3 mean = { 0.f, 0.f, 0.f };

	glm::vec3 minScale{ std::numeric_limits<float>().max() };
	glm::vec3 maxScale{ std::numeric_limits<float>().lowest() };

	float size = static_cast<float>(vertices.size());
	for (const auto& vertex : vertices)
	{
		mean += vertex;
		minScale.x = glm::min(minScale.x, vertex.x);
		minScale.y = glm::min(minScale.y, vertex.y);
		minScale.z = glm::min(minScale.z, vertex.z);

		maxScale.x = glm::max(maxScale.x, vertex.x);
		maxScale.y = glm::max(maxScale.y, vertex.y);
		maxScale.z = glm::max(maxScale.z, vertex.z);
	}

	mean /= size;

	float CovXX = 0.f;
	float CovXY = 0.f;
	float CovXZ = 0.f;
	float CovYY = 0.f;
	float CovYZ = 0.f;
	float CovZZ = 0.f;

	for (const auto& vertex : vertices)
	{
		CovXX += (vertex.x - mean.x) * (vertex.x - mean.x);
		CovXY += (vertex.x - mean.x) * (vertex.y - mean.y);
		CovXZ += (vertex.x - mean.x) * (vertex.z - mean.z);
		CovYY += (vertex.y - mean.y) * (vertex.y - mean.y);
		CovYZ += (vertex.y - mean.y) * (vertex.z - mean.z);
		CovZZ += (vertex.z - mean.z) * (vertex.z - mean.z);
	}
	//scale it to compute fast
	glm::vec3 scales = maxScale - minScale;
	CovXX /= scales.x * scales.x;
	CovXY /= scales.x * scales.y;
	CovXZ /= scales.x * scales.z;
	CovYY /= scales.y * scales.y;
	CovYZ /= scales.y * scales.z;
	CovZZ /= scales.z * scales.z;
	
	CovXX /= size;
	CovXY /= size;
	CovXZ /= size;
	CovYY /= size;
	CovYZ /= size;
	CovZZ /= size;
	
	glm::mat3 covMat{ CovXX, CovXY , CovXZ, CovXY , CovYY, CovYZ, CovXZ, CovYZ, CovZZ };
	glm::mat3 Q, X;
	X = glm::identity<glm::mat3>();

	bool convergence = true;
	float lastSum = 0.f;
	int p = 0, q = 0;

	while (convergence)
	{
		float maxElement = 0.f;
		float sum = 0.f;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (i != j)
				{
					const float abs_value = glm::abs(covMat[i][j]);
					sum += abs_value;
					if (maxElement < abs_value)
					{
						maxElement = abs_value;
						p = i;
						q = j;
					}
				}
			}
		}

		convergence = glm::abs(lastSum - sum) > std::numeric_limits<float>::epsilon();
		if (!convergence) break;
		lastSum = sum;

		float beta = (covMat[q][q] - covMat[p][p]) / (2.f * covMat[p][q]);
		float t = glm::sign(beta) / (glm::abs(beta) + glm::sqrt(beta * beta + 1.f));
		float costheta = 1.f / glm::sqrt(t * t + 1.f);
		float sintheta = costheta * t;
		Q = glm::identity<glm::mat3>();
		Q[p][p] = costheta;
		Q[p][q] = -sintheta;
		Q[q][p] = sintheta;
		Q[q][q] = costheta;
		X *= Q;
		covMat = glm::transpose(Q) * covMat * Q;
	}

	float maxElement = 0.f;
	int maxIndex = 0;
	for (int i = 0; i < 3; i++)
	{
		if (glm::abs(maxElement) < glm::abs(covMat[i][i]))
		{
			maxElement = covMat[i][i];
			maxIndex = i;
		}
	}
	auto [min, max] = ExtremePairAlongDirection(X[maxIndex], vertices);

	glm::vec3 center = (vertices[min] + vertices[max]) / 2.f;
	float radius = glm::distance(vertices[min], vertices[max]) / 2.f;
	int numOfVertices = static_cast<int>(vertices.size());
	for (int i = 0; i < numOfVertices; i++)
	{
		float newD = glm::distance(vertices[i], center);
		if (newD > radius)
		{
			float temp = radius;
			radius = (radius + newD) / 2.f;
			center += (radius - temp) * glm::normalize(vertices[i] - center);
		}
	}

	return { center , radius };
}

