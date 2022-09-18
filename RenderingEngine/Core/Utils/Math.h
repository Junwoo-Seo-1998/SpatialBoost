#pragma once
#include<glm/glm.hpp>
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
};

