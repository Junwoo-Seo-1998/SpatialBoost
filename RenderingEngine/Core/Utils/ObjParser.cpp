#include "ObjParser.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include<ctime>
#include"Core/Utils/Math.h"
#include<unordered_map>
#include<unordered_set>
#include<functional>


#include <chrono>
using namespace std::chrono;

inline bool is_not_dup(const std::vector<glm::vec3>& vec, const glm::vec3& to_comp)
{
	return std::find(vec.begin(), vec.end(), to_comp) == vec.end();
}

Mesh ObjParser::LoadFile(const std::string& file_name)
{
	auto start = high_resolution_clock::now();

	Mesh mesh;
	std::vector<glm::vec3>& vertices= mesh.GetVertices();
	std::vector<glm::vec3>& normals = mesh.GetNormals();
	std::vector<unsigned int>& indices = mesh.GetIndices();	

	std::ifstream file(file_name);
	std::stringstream stringstream;
	std::string line;
	std::string token;
	unsigned int index = 0;

	glm::vec3 vertex;

	glm::vec3 min{ std::numeric_limits<float>().max() };
	glm::vec3 max{ std::numeric_limits<float>().lowest() };
	
	while(std::getline(file, line))
	{	
		stringstream.clear();
		stringstream.str(line);
		stringstream >> token;

		if (token == "v")
		{
			stringstream >> vertex.x;
			stringstream >> vertex.y;
			stringstream >> vertex.z;

			min.x = std::min(min.x, vertex.x);
			min.y = std::min(min.y, vertex.y);
			min.z = std::min(min.z, vertex.z);

			max.x = std::max(max.x, vertex.x);
			max.y = std::max(max.y, vertex.y);
			max.z = std::max(max.z, vertex.z);

			vertices.push_back(vertex);
			//since i want to have normal per vertex
			normals.emplace_back();
		}
		else if (token == "f")
		{
			for (int i = 0; i < 3; ++i)
			{
				stringstream >> index;
				//since index starts from 0
				indices.push_back(index - 1);
			}
		}
	}

	//map to [-1,1] and center to origin.
	glm::vec3 center{ (max + min) / 2.f };
	const glm::vec3 scale = glm::vec3{ 2.f / (max - min) };
	for (auto& vert:vertices )
	{
		vert = (vert - center) * scale;
	}

	//process normal 
	int total_faces = indices.size() / 3;
	std::vector<glm::vec3> face_normals;
	face_normals.reserve(total_faces);
	std::unordered_map<unsigned int, std::vector<unsigned int>> vertex_faces_map;
	vertex_faces_map.reserve(vertices.size());

	for (int i = 0; i < total_faces; ++i)
	{
		const unsigned int offset = i * 3;
		const glm::vec3& v0 = vertices[indices[offset]];
		const glm::vec3& v1 = vertices[indices[offset + 1]];
		const glm::vec3& v2 = vertices[indices[offset + 2]];
		face_normals.emplace_back(Math::ComputeFaceNormal(v0, v1, v2));
		vertex_faces_map[indices[offset]].push_back(i);
		vertex_faces_map[indices[offset + 1]].push_back(i);
		vertex_faces_map[indices[offset + 2]].push_back(i);
	}

	
	std::vector<glm::vec3> prev_normals;
	int vertex_index;
	for (auto& pair : vertex_faces_map)
	{
		vertex_index = pair.first;
		prev_normals.clear();
		glm::vec3 acc_normal(0.f);
		std::vector<unsigned int>& face_indices=pair.second;
		for (unsigned int i: face_indices)
		{
			if (std::find(prev_normals.begin(), prev_normals.end(), face_normals[i]) == prev_normals.end()) //check the normal is unique.
			{
				acc_normal += face_normals[i];
				prev_normals.push_back(face_normals[i]);
			}
		}
		normals[vertex_index] = glm::normalize(acc_normal);
	}
	
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	std::cout << duration.count() << " ms - load  - ( " << file_name << " )" << std::endl;
	return mesh;
}

Mesh ObjParser::LoadFaceNormalLineMesh(const std::string& file_name, float line_length)
{
	std::vector<glm::vec3> loaded_points;
	std::vector<Face> loaded_faces;
	
	LoadFile(file_name, loaded_points, loaded_faces);
	//process face normal

	Mesh face_normal_mesh;
	face_normal_mesh.SetDrawType(DrawType::Lines);
	std::vector<glm::vec3>& mesh_vertex = face_normal_mesh.GetVertices();
	mesh_vertex.reserve(loaded_faces.size() * 2);
	for (const Face& face:loaded_faces)
	{
		const glm::vec3& v0 = loaded_points[face.indices[0]];
		const glm::vec3& v1 = loaded_points[face.indices[1]];
		const glm::vec3& v2 = loaded_points[face.indices[2]];
		const glm::vec3& face_normal = Math::ComputeFaceNormal(v0, v1, v2);
		const glm::vec3 face_center = (v0 + v1 + v2) / 3.f;
		mesh_vertex.push_back(face_center);
		mesh_vertex.push_back(face_center + line_length*face_normal);
	}
	return face_normal_mesh;
}

Mesh ObjParser::LoadVertexNormalLineMesh(const std::string& file_name, float line_length)
{
	std::vector<glm::vec3> loaded_points;
	std::vector<Face> loaded_faces;

	LoadFile(file_name, loaded_points, loaded_faces);
	//process face normal

	Mesh vertex_normal_mesh;
	vertex_normal_mesh.SetDrawType(DrawType::Lines);
	std::vector<glm::vec3>& mesh_vertex = vertex_normal_mesh.GetVertices();
	mesh_vertex.reserve(loaded_faces.size() * 2);
	for (const Face& face : loaded_faces)
	{
		const glm::vec3& v0 = loaded_points[face.indices[0]];
		const glm::vec3& v1 = loaded_points[face.indices[1]];
		const glm::vec3& v2 = loaded_points[face.indices[2]];
		const glm::vec3& face_normal = Math::ComputeFaceNormal(v0, v1, v2);
		const glm::vec3 face_center = (v0 + v1 + v2) / 3.f;
		mesh_vertex.push_back(face_center);
		mesh_vertex.push_back(face_center + line_length * face_normal);
	}
	return vertex_normal_mesh;
}

void ObjParser::LoadFile(const std::string& file_name, std::vector<glm::vec3>& loaded_points, std::vector<Face>& loaded_faces)
{
	std::ifstream file(file_name);
	std::stringstream stringstream;
	std::string line;
	std::string token;

	glm::vec3 vertex{};
	Face face{};

	glm::vec3 min{ std::numeric_limits<float>().max() };
	glm::vec3 max{ std::numeric_limits<float>().lowest() };

	while (std::getline(file, line))
	{
		stringstream.clear();
		stringstream.str(line);
		stringstream >> token;

		if (token == "v")
		{
			stringstream >> vertex.x;
			stringstream >> vertex.y;
			stringstream >> vertex.z;

			min.x = std::min(min.x, vertex.x);
			min.y = std::min(min.y, vertex.y);
			min.z = std::min(min.z, vertex.z);

			max.x = std::max(max.x, vertex.x);
			max.y = std::max(max.y, vertex.y);
			max.z = std::max(max.z, vertex.z);

			loaded_points.push_back(vertex);
		}
		else if (token == "f")
		{
			for (int i = 0; i < 3; ++i)
			{
				stringstream >> face.indices[i];
				//since index starts from 0
				--(face.indices[i]);
			}
			loaded_faces.push_back(face);
		}
	}

	//map to [-1,1] and center to origin.
	glm::vec3 center{ (max + min) / 2.f };
	const glm::vec3 scale = glm::vec3{ 2.f / (max - min) };
	for (auto& vert : loaded_points)
	{
		vert = (vert - center) * scale;
	}
}

void ObjParser::ParseFile(const std::string& file_name, std::vector<glm::vec3>& loaded_points,
	std::vector<unsigned>& loaded_indices)
{
	std::ifstream file(file_name);
	std::stringstream stringstream;
	std::string line;
	std::string token;

	glm::vec3 vertex{};
	unsigned int index = 0;
	glm::vec3 min{ std::numeric_limits<float>().max() };
	glm::vec3 max{ std::numeric_limits<float>().lowest() };
	
	while (std::getline(file, line))
	{
		stringstream.clear();
		stringstream.str(line);
		stringstream >> token;

		if (token == "v")
		{
			stringstream >> vertex.x;
			stringstream >> vertex.y;
			stringstream >> vertex.z;

			min.x = std::min(min.x, vertex.x);
			min.y = std::min(min.y, vertex.y);
			min.z = std::min(min.z, vertex.z);

			max.x = std::max(max.x, vertex.x);
			max.y = std::max(max.y, vertex.y);
			max.z = std::max(max.z, vertex.z);

			loaded_points.push_back(vertex);
		}
		else if (token == "f")
		{
			for (int i = 0; i < 3; ++i)
			{
				stringstream >> index;
				//since index starts from 0
				loaded_indices.push_back(index - 1);
			}
		}
	}

	//map to [-1,1] and center to origin.
	glm::vec3 center{ (max + min) / 2.f };
	const glm::vec3 scale = glm::vec3{ 2.f / (max - min) };
	for (auto& vert : loaded_points)
	{
		vert = (vert - center) * scale;
	}

}

