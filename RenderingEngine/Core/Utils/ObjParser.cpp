#include "ObjParser.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include"Core/Utils/Math.h"
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
		if (token.find("#")!=std::string::npos)
		{
			continue;
		}

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
	for (Face& face : faces)
	{
		const Vertex& v0 = vertices[face.indices[0]];
		const Vertex& v1 = vertices[face.indices[1]];
		const Vertex& v2 = vertices[face.indices[2]];
		face_normals.push_back(Math::ComputeFaceNormal(v0.position, v1.position, v2.position));

	}

	int total_faces = faces.size();
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
			}
		}
		vert.normal = glm::normalize(acc_normal);
	}
	return mesh;
}
