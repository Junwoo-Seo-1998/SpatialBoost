#pragma once
#include <string>

#include "Core/Data/Mesh.h"
#include <memory>

class MeshGenerator
{
public:
	static std::vector<glm::vec3> GenerateSphere(float rad = 1.f, int segments = 30, int rings = 30);
	static std::shared_ptr<LineMesh> GenerateOrbit(float radius = 1.f, int numDivisions = 30);


	static std::shared_ptr<Mesh> GenerateFaceNormalMesh(const std::vector<glm::vec3>& loaded_points, const std::vector<unsigned int>& loaded_index,
		const std::vector<glm::vec3>& face_normals);
	static std::shared_ptr<LineMesh> GenerateFaceNormalLineMesh(const std::vector<glm::vec3>& loaded_points, const std::vector<unsigned int>& loaded_index,
		const std::vector<glm::vec3>& face_normals, float normal_len = 0.1f);

	static std::shared_ptr<Mesh> GenerateVertexNormalMesh(const std::vector<glm::vec3>& loaded_points, const std::vector<unsigned int>& loaded_index,
		const std::vector<glm::vec3>& vertex_normals);
	static std::shared_ptr<LineMesh> GenerateVertexNormalLineMesh(const std::vector<glm::vec3>& loaded_points, const std::vector<unsigned int>& loaded_index,
		const std::vector<glm::vec3>& vertex_normals, float normal_len = 0.1f);


	static std::vector<glm::vec3> GenerateFaceNormals(std::vector<glm::vec3>& loaded_points, std::vector<unsigned int>& loaded_index);
	static std::vector<glm::vec3> GenerateVertexNormals(std::vector<glm::vec3>& loaded_points, std::vector<glm::vec3>& face_normals, 
		std::vector<unsigned int>& loaded_index);
};