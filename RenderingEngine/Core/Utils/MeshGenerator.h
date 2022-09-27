#pragma once
#include <string>

#include "Core/Data/Mesh.h"


class MeshGenerator
{
public:
	static Mesh GenerateSphere(float rad = 1.f, int segments = 30, int rings = 30);


	static std::vector<glm::vec3> GenerateFaceNormals(std::vector<glm::vec3>& loaded_points, std::vector<unsigned int>& loaded_index);
	static std::vector<glm::vec3> GenerateVertexNormals(std::vector<glm::vec3>& loaded_points, std::vector<glm::vec3>& face_normals, std::vector<unsigned int>& loaded_index);
};