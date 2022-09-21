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

Mesh ObjParser::LoadFile(const std::string& file_name)
{
	using namespace std;
	Mesh mesh;
	std::vector<Vertex>& vertices=mesh.GetVertices();
	std::vector<Face>& faces = mesh.GetFaces();
	ifstream file(file_name);
	
	for (std::string str; std::getline(file, str);)
	{
		std::stringstream ss(str);
		std::string token;
		ss >> token;
		//if (token.find("#")!=std::string::npos)
		//{
		//	continue;
		//}

		if (token == "v")
		{
			glm::vec3 vertex;
			ss >> vertex.x;
			ss >> vertex.y;
			ss >> vertex.z;
			vertices.push_back({ vertex, });
			#ifdef DEBUG
			//std::cout << "(" << vertex.x << "," << vertex.y << "," << vertex.z << ")" << std::endl;
			#endif // DEBUG
		}
		else if (token == "f")
		{
			Face face;
			for (int i = 0; i < 3; ++i)
			{
				ss >> face.indices[i];
				//since index starts from 0
				--(face.indices[i]);
			}
			faces.push_back(face);
		}
		#ifdef DEBUG
		//std::cout << token << std::endl;
		//std::cout << str << std::endl;
		#endif
	}
	
	//process normal 
	std::vector<glm::vec3> face_normals;


	int total_faces = faces.size();
	for (int i=0; i<total_faces; ++i)
	{
		Face& face = faces[i];
		const Vertex& v0 = vertices[face.indices[0]];
		const Vertex& v1 = vertices[face.indices[1]];
		const Vertex& v2 = vertices[face.indices[2]];

		face_normals.push_back(Math::ComputeFaceNormal(v0.position, v1.position, v2.position));

	}


	for (Vertex& vert : vertices)
	{
		std::vector<glm::vec3> prev_normals;
		glm::vec3 acc_normal(0.f);
		for (int i=0; i<total_faces; ++i)
		{
			const glm::vec3& position = vert.position;
			const glm::vec3& v0 = vertices[faces[i].indices[0]].position;	
			const glm::vec3& v1 = vertices[faces[i].indices[1]].position;	
			const glm::vec3& v2 = vertices[faces[i].indices[2]].position;	
			//if there is a face that contain the vertex then add to normal
			if (v0 == position || v1 == position || v2 == position)
			{
				if (std::find(prev_normals.begin(), prev_normals.end(), face_normals[i]) == prev_normals.end())
				{
					acc_normal += face_normals[i];
					prev_normals.push_back(face_normals[i]);
				}
				else
				{
					std::cout << "same!";
				}
			}
		}
		vert.normal = glm::normalize(acc_normal);
	}
	return mesh;
}

inline bool is_not_dup(const std::vector<glm::vec3>& vec, const glm::vec3& to_comp)
{
	return std::find(vec.begin(), vec.end(), to_comp) == vec.end();
}

Mesh ObjParser::LoadFileFast(const std::string& file_name)
{
	

	Mesh mesh;
	std::vector<Vertex>& vertices= mesh.GetVertices();
	std::vector<Face>& faces=mesh.GetFaces();

	std::ifstream file(file_name);
	std::stringstream stringstream;
	std::string line;
	std::string token;
	

	glm::vec3 vertex;
	Face face;

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

			vertices.push_back({ vertex, });
		}
		else if (token == "f")
		{
			for (int i = 0; i < 3; ++i)
			{
				stringstream >> face.indices[i];
				//since index starts from 0
				--(face.indices[i]);
			}
			faces.push_back(face);
		}
	}

	//map to [-1,1] and center to origin.
	glm::vec3 center{ (max + min) / 2.f };

	const glm::vec3 scale = glm::vec3{ 2.f / (max - min) };
	for (auto& vert:vertices )
	{
		vert.position = (vert.position - center) * scale;
	}

	auto start = high_resolution_clock::now();


	int total_faces = faces.size();
	/*
	std::unordered_map<unsigned int, std::vector<glm::vec3>> vertex_normals_map;
	//std::unordered_set<glm::vec3> set;
	vertex_normals_map.reserve(vertices.size());
	for (int i = 0; i < total_faces; ++i)
	{
		Face& face = faces[i];
		const Vertex& v0 = vertices[face.indices[0]];
		const Vertex& v1 = vertices[face.indices[1]];
		const Vertex& v2 = vertices[face.indices[2]];
		const glm::vec3 normal{ Math::ComputeFaceNormal(v0.position, v1.position, v2.position) };
		
		if (is_not_dup(vertex_normals_map[face.indices[0]], normal))
		{
			vertex_normals_map[face.indices[0]].push_back(normal);
		}
		else
		{
			std::cout << "same!";
		}
		if (is_not_dup(vertex_normals_map[face.indices[1]], normal))
		{
			vertex_normals_map[face.indices[1]].push_back(normal);
		}
		else
		{
			std::cout << "same!";
		}
		if (is_not_dup(vertex_normals_map[face.indices[2]], normal))
		{
			vertex_normals_map[face.indices[2]].push_back(normal);
		}
		else
		{
			std::cout << "same!";
		}
	}

	for (auto& pair : vertex_normals_map)
	{
		glm::vec3 acc_normal(0.f);
		const std::vector<glm::vec3>& normals = pair.second;
		for (auto& element: normals)
		{
			acc_normal += element;
		}
		vertices[pair.first].normal = glm::normalize(acc_normal);
	}
	*/

	//process normal 
	std::vector<glm::vec3> face_normals;
	face_normals.reserve(faces.size());
	std::unordered_map<unsigned int, std::vector<unsigned int>> vertex_faces_map;
	vertex_faces_map.reserve(vertices.size());

	for (int i = 0; i < total_faces; ++i)
	{
		Face& face = faces[i];
		const Vertex& v0 = vertices[face.indices[0]];
		const Vertex& v1 = vertices[face.indices[1]];
		const Vertex& v2 = vertices[face.indices[2]];
		face_normals.emplace_back(Math::ComputeFaceNormal(v0.position, v1.position, v2.position));
		vertex_faces_map[face.indices[0]].push_back(i);
		vertex_faces_map[face.indices[1]].push_back(i);
		vertex_faces_map[face.indices[2]].push_back(i);
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
			if (std::find(prev_normals.begin(), prev_normals.end(), face_normals[i]) == prev_normals.end())
			{
				acc_normal += face_normals[i];
				prev_normals.push_back(face_normals[i]);
			}
			else
			{
				std::cout << "same!";
			}
		}
		vertices[vertex_index].normal = glm::normalize(acc_normal);
	}
	
	
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	std::cout << duration.count() << " ms - load " << std::endl;
	return mesh;
}
