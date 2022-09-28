#pragma once
#include <string>
#include "Core/Data/Mesh.h"
#include <memory>

class Shader;

class AssetManager
{
public:
	static std::shared_ptr<Mesh> LoadMeshFromFile(const std::string& file_name);


	static std::shared_ptr<Shader> LoadShaderFromFile(const std::string& vert_file, const std::string& frag_file);
private:
	
};
