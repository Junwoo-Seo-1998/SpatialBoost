#include "ObjParser.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
Mesh ObjParser::LoadFile(const std::string& file_name)
{
	using namespace std;
	Mesh mesh;
	std::vector<Vertex>& vertices=mesh.GetVertices();
	std::vector<Face>& faces = mesh.GetFaces();
	ifstream file;
	file.open(file_name);
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
			std::cout << "(" << vertex.x << "," << vertex.y << "," << vertex.z << ")" << std::endl;
			#endif // DEBUG
		}
		else if (token == "f")
		{
			Face face;
			for (int i = 0; i < 3; ++i)
			{
				ss >> face.indices[i];
				//since index starts from 0
				--face.indices[i];
			}
			faces.push_back(face);
		}
		#ifdef DEBUG
		std::cout << token << std::endl;
		std::cout << str << std::endl;
		#endif
	}
	
	return mesh;
}
