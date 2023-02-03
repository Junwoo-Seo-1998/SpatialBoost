/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: ObjParser.cpp
Purpose: for reading obj files.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------*/
#include "ObjParser.h"
#include <string>
#include <fstream>
#include <sstream>
#include"Core/Utils/Math.h"
#include"assimp/Importer.hpp"
void ObjParser::ParseFile(const std::string& file_name, std::vector<glm::vec3>& loaded_points,
	std::vector<unsigned>& loaded_indices)
{
	Assimp::Importer importer;
	std::ifstream file(file_name);
	std::stringstream stringstream;
	std::string line;
	std::string token;

	glm::vec3 vertex{};
	unsigned int index = 0;
	glm::vec3 min{ std::numeric_limits<float>().max() };
	glm::vec3 max{ std::numeric_limits<float>().lowest() };


	std::vector<unsigned int> indices;
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
			indices.clear();
			while (stringstream >> index)
			{
				indices.push_back(index - 1);
			}
			unsigned int size = static_cast<unsigned int>(indices.size());

			int first = 0;
			int prev = 1;
			for (unsigned int i = 2; i < size; ++i)
			{
				loaded_indices.push_back(indices[first]);
				loaded_indices.push_back(indices[prev]);
				loaded_indices.push_back(indices[i]);
				prev = i;
			}
		}
	}

	//map to [-1,1] and center to origin.
	glm::vec3 center{ (max + min) / 2.f };
	glm::vec3 size = max - min;
	size.x = size.x == 0.f ? 1.f : size.x;
	size.y = size.y == 0.f ? 1.f : size.y;
	size.z = size.z == 0.f ? 1.f : size.z;
	const glm::vec3 scale = glm::vec3{ 2.f / size };

	for (auto& vert : loaded_points)
	{
		vert = (vert - center) * scale;
	}

}

