#include "MeshGenerator.h"

#include <unordered_map>
#include <glm/ext/scalar_constants.hpp>

#include "Math.h"

Mesh MeshGenerator::GenerateSphere(float rad, int segments, int rings)
{
	//clamp
	segments = std::max(segments, 3);
	rings = std::max(rings, 3);
	
	Mesh mesh;
	float theta_step = glm::pi<float>() / static_cast<float>(rings);
	float theta = 0.f;
	
	for (int ring = 1; ring < rings; ++ring)
	{
		theta += theta_step;
		float ring_rad = rad * std::sin(theta);
		for (int segment = 0; segment < segments; ++segment)
		{
			
		}
	}


	return mesh;
}

std::vector<glm::vec3> MeshGenerator::GenerateFaceNormals(std::vector<glm::vec3>& loaded_points,
	std::vector<unsigned>& loaded_index)
{
	std::vector<glm::vec3> face_normals;
	const unsigned int total_faces = static_cast<unsigned int>(loaded_index.size()) / 3;
	face_normals.reserve(total_faces);
	for (unsigned int i = 0; i < total_faces; ++i)
	{
		const unsigned int offset = i * 3;
		const glm::vec3& v0 = loaded_points[loaded_index[offset]];
		const glm::vec3& v1 = loaded_points[loaded_index[offset + 1]];
		const glm::vec3& v2 = loaded_points[loaded_index[offset + 2]];
		face_normals.emplace_back(Math::ComputeFaceNormal(v0, v1, v2));
	}
	return face_normals;
}

std::vector<glm::vec3> MeshGenerator::GenerateVertexNormals(std::vector<glm::vec3>& loaded_points,
	std::vector<glm::vec3>& face_normals, std::vector<unsigned>& loaded_index)
{
	std::vector<glm::vec3> generated_normals{ loaded_points.size() };

	std::unordered_map<unsigned int, std::vector<unsigned int>> vertex_faces_map;
	vertex_faces_map.reserve(loaded_points.size());
	const unsigned int total_faces = static_cast<unsigned int>(loaded_index.size()) / 3;
	for (unsigned int i = 0; i < total_faces; ++i)
	{
		const unsigned int offset = i * 3;
		vertex_faces_map[loaded_index[offset]].push_back(i);
		vertex_faces_map[loaded_index[offset + 1]].push_back(i);
		vertex_faces_map[loaded_index[offset + 2]].push_back(i);
	}

	std::vector<glm::vec3> prev_normals;
	int vertex_index;
	for (auto& pair : vertex_faces_map)
	{
		vertex_index = pair.first;
		prev_normals.clear();
		glm::vec3 acc_normal(0.f);
		std::vector<unsigned int>& face_indices = pair.second;
		for (unsigned int i : face_indices)
		{
			if (std::find(prev_normals.begin(), prev_normals.end(), face_normals[i]) == prev_normals.end()) //check the normal is unique.
			{
				acc_normal += face_normals[i];
				prev_normals.push_back(face_normals[i]);
			}
		}
		generated_normals[vertex_index] = glm::normalize(acc_normal);
	}
	return generated_normals;
}


