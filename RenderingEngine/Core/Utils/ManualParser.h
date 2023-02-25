#pragma once
#include <string>
#include <vector>
#include <glm/vec3.hpp>

#include "Core/Utils/Parser.h"

class ManualParser: public BasicParser
{
public:
	void ParseFile(const std::string& file_name, std::vector<glm::vec3>& loaded_points,
		std::vector<unsigned>& loaded_indices) override;
};
