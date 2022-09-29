#pragma once
#include <string>
#include"Core/Data/Mesh.h"
class ObjParser
{
public:

	static void ParseFile(const std::string& file_name, std::vector<glm::vec3>& loaded_points, std::vector<unsigned int>& loaded_indices);

};

