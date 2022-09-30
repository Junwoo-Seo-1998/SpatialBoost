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

