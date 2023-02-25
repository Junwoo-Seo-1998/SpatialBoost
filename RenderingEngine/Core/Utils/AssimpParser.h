#pragma once
#include "Parser.h"

struct aiScene;
struct aiNode;

class AssimpParser:public BasicParser
{
public:
	void ParseFile(const std::string& file_name, std::vector<glm::vec3>& loaded_points,
		std::vector<unsigned>& loaded_indices) override;
private:
	void ProcessNode(const aiScene* scene, aiNode* node, std::vector<glm::vec3>& loaded_points,
		std::vector<unsigned>& loaded_indices);
};
