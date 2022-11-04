#pragma once
/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Math.h
Purpose: for maths.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------*/
#include <memory>
#include <vector>
#include<glm/glm.hpp>

#include "Core/Data/BoundingBox.h"
//bit compute
#define BIT(x) (1)<<(x) 
class Math
{
public:
	//Matrix
	static glm::mat4 BuildCameraMatrix(glm::vec3 cam_position, glm::vec3 target, glm::vec3 world_up);
	static glm::mat4 BuildPerspectiveProjectionMatrix(float width, float height, float near, float far);
	static glm::mat4 BuildPerspectiveProjectionMatrixFovx(float fov_x, float aspect_ratio, float near, float far);
	static glm::mat4 BuildPerspectiveProjectionMatrixFovy(float fov_y, float aspect_ratio, float near, float far);
public:
	//Mesh
	static glm::vec3 ComputeFaceNormal(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);
public:
	static glm::vec2 ComputePlanarUV(const glm::vec3 TextureEntity, float min_x, float max_x, float min_y, float max_y);
	static glm::vec2 ComputeCylindricalUV(const glm::vec3& TextureEntity, float min_y, float max_y);
	static glm::vec2 ComputeSphericalUV(const glm::vec3& TextureEntity);
	static glm::vec2 ComputeCubeMapUV(const glm::vec3& TextureEntity);

	static std::shared_ptr<std::vector<glm::vec2>> ComputePlanarUVs(const std::vector<glm::vec3> TextureEntities, const BoundingBox& box);
	static std::shared_ptr<std::vector<glm::vec2>> ComputeCylindricalUVs(const std::vector<glm::vec3> TextureEntities, const BoundingBox& box);
	static std::shared_ptr<std::vector<glm::vec2>> ComputeSphericalUVs(const std::vector<glm::vec3> TextureEntities);
	static std::shared_ptr<std::vector<glm::vec2>> ComputeCubeMapUVs(const std::vector<glm::vec3> TextureEntities);
};

