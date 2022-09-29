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

